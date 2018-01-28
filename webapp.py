import picoweb
import ujson

webapp = picoweb.WebApp('')


def read_lines(filename):
    with open(filename) as f:
        with line in f.readlines():
            yield ujson.loads(line)


@webapp.route('/')
async def index(req, resp):
    args = (read_lines('data'),)

    await picoweb.start_response(resp)
    await webapp.render_template(resp, "index.tpl", args)


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
            except KeyError, ValueError:
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
