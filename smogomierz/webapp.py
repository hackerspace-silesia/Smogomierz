from ustructk import unpack

import picoweb
import ujson
import gc

webapp = picoweb.WebApp(None)

db = []

TABLE_ROWS = (
    'pm1', 'pm25', 'pm10', 'temperature', 'pressure', 'humidity', 'date',
)

DATE_FMT = '{year}-{month:02}-{day:02} {hour:02}:{minute:02}:{second:02}'


def _get_date(time):
    year, month, day, hour, minute, second, *_ = utime.localtime(time)
    del now
    del _

    return DATE_FMT.format(
        year=year,
        month=month,
        day=day,
        hour=hour,
        minute=minute,
        second=second,
    )


def read_data(filename):
    for obj in db:
        data = unpack('iffffff', obj)
        time, pm1, pm25, pm10, temperature, pressure, humidity = data
        yield {
            'pm1': pm1,
            'pm25': pm25,
            'pm10': pm10,
            'temperature': temperature,
            'pressure': pressure,
            'humidity': humidity,
            'date': _get_date(time),
        }


@webapp.route('/')
async def index(req, resp):
    await picoweb.start_response(resp)
    w = resp.awrite
    
    await w('<title>Smogomierz by Hackerspace</title>')
    await w('<a href="/config">CONFIG!</a>')

    await w('<table>')
    await w('<thead><tr>')
    await w('<th>DATE</th>')
    await w('<th>PM1.0 <small>[&micro;g/m&sup3;]</small></th>')
    await w('<th>PM2.5 <small>[&micro;g/m&sup3;]</small></th>')
    await w('<th>PM10.0 <small>[&micro;g/m&sup3;]</small></th>')
    await w('<th>TEMPERATURE <small>[&deg;C]</small></th>')
    await w('<th>PRESSURE <small>[hPa]</small></th>')
    await w('<th>HUMIDITY <small>[%]</small></th>')
    await w('</tr></thead>')
    await w('<tbody>')

    for obj in read_lines('data'):
        await w('<tr>')
        for row in TABLE_ROWS:
            await w('<td>%s</td>' % obj.get(row))
        await w('</tr>')

    await w('</tbody>')
    await w('</table>')


def striped_str(value):
    return value.strip()[:128]


def get_input(name, label, value):
    tmp = ( 
        '<div class="group">'
        '<label for="{name}">{label}</label>'
        '<input type="text" id="{name}" name="{name}" value="{value}"/>'
        '</div>'
    )

    value = str(value).replace("'", "\\'")
    return tmp.format(
        name=name,
        label=label,
        value=value,
    )


def get_cord_input(name, label, value):
    tmp = ( 
        '<div class="group">'
        '<label for="{name}">{label}</label>'
        '<input type="number" step="0.0001" id="{name}" name="{name}" value="{value}"/>'
        '</div>'
    )

    value = str(value).replace("'", "\\'")
    return tmp.format(
        name=name,
        label=label,
        value=value,
    )


@webapp.route('/config')
async def config(req, resp):
    try:
        with open('config') as f:
            config = ujson.load(f)
    except OSError:
        config = {}

    if req.method == 'POST':
        gc.collect()
        await req.read_form_data()

        def update_element(key, cls=striped_str):
            try:
                value = cls(req.form[key][0])
            except (IndexError, KeyError, ValueError):
                pass
            else:
                config[key] = value

        update_element('wifi_essid')
        update_element('wifi_password')
        update_element('airmonitor_lat', float)
        update_element('airmonitor_long', float)

        req.form = None  # remove useless memory
        gc.collect()
        with open('config', 'w') as f:
            f.write(ujson.dumps(config))

        gc.collect()
        updated = True
    else:
        updated = None

    w = resp.awrite

    await picoweb.start_response(resp)
    await w('<title>Smogomierz by Hackerspace</title>')
    if updated is True:
        await w('<strong>Config updated!</strong>')
    await w('<form method="POST" action="/config">')
    await w(get_input('wifi_essid', 'ESSID', config.get('wifi_essid', '-')))
    await w(get_input('wifi_password', 'PASSWORD', config.get('wifi_password', '-')))
    await w(get_cord_input('airmonitor_lat', 'LATITUDE', config.get('airmonitor_lat', '0.0000')))
    await w(get_cord_input('airmonitor_long', 'LONGITUDE', config.get('airmonitor_long', '0.0000')))
    await w('<button type="submit"> UPDATE </button>')
    await w('</form>')
