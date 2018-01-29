from sensor_manager import SensorManager
from webapp import webapp, db
from ntptime import settime
from machine import reset

import uasyncio
import picoweb
import network


def do_webapp():
    webapp.debug = False
    webapp.init()
    for mount in webapp.mounts:
        mount.init()
    return uasyncio.start_server(webapp._handle, '0.0.0.0', 80)


async def loop_set_time():
    while True:
        settime()
        await uasyncio.sleep(3600)


if __name__ == "__main__":
    ap_if = network.WLAN(network.AP_IF)
    if not ap_if.active():
        settime()

    sensor_manager = SensorManager(db, fake_data=True, send_to_airmonitor=False)
    #sensor_manager.setup()

    try:
        gc.collect()
        loop = uasyncio.get_event_loop()

        loop.create_task(sensor_manager.execute(loop))
        if not ap_if.active():
            loop.create_task(loop_set_time())
        loop.create_task(do_webapp())
        
        loop.run_forever()
    except KeyboardInterrupt:
        loop.close() 
    except Exception as e:
        reset()

