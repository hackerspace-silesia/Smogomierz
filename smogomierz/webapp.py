from ustruct import unpack

import ujson
import gc
import sys

from utime import localtime, time

db = []

TABLE_ROWS = (
    'date', 'pm1', 'pm25', 'pm10',
    'temperature', 'pressure', 'humidity', 
)

DEFAULT_STYLE = (
    '<style>body{'
    'font-family:monospace;color:#FFF;'
    'font-size: 12pt;'
    'background-color:#339ACC;'
    '}</style>'
)


async def serve(reader, writer):
    try:
        await _serve(reader, writer)
    except KeyboardInterrupt:
        raise
    except MemoryError:
        raise
    except Exception as e:
        sys.print_exception(e)
    finally:
        try:
            await writer.aclose()
        except:
            pass
        

async def _serve(reader, writer):
    gc.collect()
    line = await reader.readline()
    method, path, _ = line.split(b' ', 2)
    size = 0

    while line and line != b'\r\n':
        line = await reader.readline()
        if line.startswith(b'Content-Length:'):
            size = min(512, int(line[16:]))

    method = method.decode('utf-8')
    if method == 'POST' and size:
        raw_data = await reader.read(size)
        raw_data = raw_data.decode('utf-8')
    else: 
        raw_data = None

    await writer.awrite('HTTP/1.0 200 OK\r\n')
    await writer.awrite('Content-type: text/html\r\n\r\n')
    gc.collect()
    if path.strip().lower() == b'/config':
        await config(writer, method, raw_data)
    else:
        await index(writer)


def _get_date(dt):
    numbers = localtime(dt)
    del dt

    return '%d-%02d-%02d %02d:%02d:%02d' % numbers[0:6]


def read_data():
    for obj in db:
        data = unpack('iffffff', obj)
        dt, pm1, pm25, pm10, temperature, pressure, humidity = data
        yield {
            'pm1': pm1,
            'pm25': pm25,
            'pm10': pm10,
            'temperature': temperature,
            'pressure': pressure,
            'humidity': humidity,
            'date': _get_date(dt),
        }


async def index(writer):
    w = writer.awrite
    
    await w('<title>Smogomierz by Hackerspace</title>')
    await w(DEFAULT_STYLE)
    await w('<style>table{border-collapse: collapse} table, th, td {border: 1px solid #FFF}</style>')
    await w('<a href="/config">CONFIG!</a><br/>')
    await w('time: <strong>' + _get_date(time()) + '</strong><br/>')
    await w('memory left: <strong>' + str(gc.mem_free()) + '</strong><br/>')
    await w('memory allocated: <strong>' + str(gc.mem_alloc()) + '</strong><br/>')

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

    for obj in read_data():
        await w('<tr>')
        for row in TABLE_ROWS:
            await w('<td>%s</td>' % obj.get(row))
        await w('</tr>')

    await w('</tbody>')
    await w('</table>')


def striped_str(value):
    return value.strip()[:128]


async def get_input(awrite, name, label, value):
    value = str(value).replace("'", "\\'")
    await awrite('<div class="group">')
    await awrite('<label for="' + name + '">' + label + '</label>')
    await awrite('<input type="text" id="' + name)
    await awrite('" name="' + name + '" value="' + value + '"/>')
    await awrite('</div>')


async def get_cord_input(awrite, name, label, value):
    value = str(value).replace("'", "\\'")
    await awrite('<div class="group">')
    await awrite('<label for="' + name + '">' + label + '</label>')
    await awrite('<input type="number" step="0.0001" id="' + name)
    await awrite('" name="' + name + '" value="' + value + '"/>')
    await awrite('</div>')


async def config(writer, method, raw_data):
    try:
        with open('config') as f:
            config = ujson.load(f)
    except OSError:
        config = {}

    if method == 'POST':
        gc.collect()

        data = dict(obj.split('=', 1) for obj in raw_data.split('&'))
        del raw_data
        def update_element(key, cls=striped_str):
            try:
                value = cls(data.pop(key))
            except (IndexError, KeyError, ValueError):
                pass
            else:
                config[key] = value

        update_element('wifi_essid')
        update_element('wifi_password')
        update_element('airmonitor_lat', float)
        update_element('airmonitor_long', float)

        del data

        gc.collect()
        with open('config', 'w') as f:
            f.write(ujson.dumps(config))

        gc.collect()
        updated = True
    else:
        updated = None

    
    w = writer.awrite
    await w('<title>Smogomierz by Hackerspace</title>')
    await w(DEFAULT_STYLE)
    await w('<style>label{width:100px;float:left;}input{font-size:12pt}</style>')
    if updated is True:
        await w('<strong>Config updated!</strong>')
    await w('<form method="POST" action="/config">')
    await get_input(w, 'wifi_essid', 'ESSID', config.get('wifi_essid', '-'))
    await get_input(w, 'wifi_password', 'PASSWORD', config.get('wifi_password', '-'))
    await get_cord_input(w, 'airmonitor_lat', 'LATITUDE', config.get('airmonitor_lat', '0.0000'))
    await get_cord_input(w, 'airmonitor_long', 'LONGITUDE', config.get('airmonitor_long', '0.0000'))
    await w('<button type="submit"> UPDATE </button>')
    await w('</form>')
