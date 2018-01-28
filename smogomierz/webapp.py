import picoweb
import ujson

webapp = picoweb.WebApp(None)

TABLE_ROWS = (
    'date', 'pm1', 'pm25', 'pm10', 'temperature', 'pressure', 'humidity'
)


def read_lines(filename):
    try:
        with open(filename) as f:
            for line in f:
                yield ujson.loads(line)
    except FileNotFoundError:
        pass


@webapp.route('/')
async def index(req, resp):
    await picoweb.start_response(resp)
    await resp.awrite('<table>')
    await resp.awrite('<thead><tr>')
    await resp.awrite('<th>DATE</th>')
    await resp.awrite('<th>PM1</th>')
    await resp.awrite('<th>PM25</th>')
    await resp.awrite('<th>PM10</th>')
    await resp.awrite('<th>TEMPERATURE</th>')
    await resp.awrite('<th>PRESSURE</th>')
    await resp.awrite('<th>HUMIDITY</th>')
    await resp.awrite('</tr></thead>')
    await resp.awrite('<tbody>')

    for obj in read_lines('data'):
        await resp.awrite('<tr>')
        for row in TABLE_ROWS:
            await resp.awrite('<td>%s</td>' % obj.get(row))
        await resp.awrite('</tr>')

    await resp.awrite('</tbody>')
    await resp.awrite('</table>')


def striped_str(value):
    return value.strip()[:128]


@webapp.route('/config')
async def config(req, resp):
    with open('config') as f:
        config = ujson.load(f)

    if req.method == 'POST':
        await req.read_form_data()

        def update_element(key, cls=striped_str):
            try:
                value = cls(req.form[key])
            except (KeyError, ValueError):
                pass
            else:
                config[key] = value

        update_element('wifi_essid')
        update_element('wifi_password')
        update_element('airmonitor_lat', float)
        update_element('airmonitor_long', float)
        update_element('save_time_minutes', int)

        with open('config', 'w') as f:
            f.write(ujson.dump(config))

        updated = True
    else:
        updated = None

    args = (config, updated)

    await picoweb.start_response(resp)
    await webapp.render_template(resp, "config.tpl", args)
