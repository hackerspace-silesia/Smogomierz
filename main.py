from sensor_manager import SensorManager
from webapp import webapp
from ntptime import settime, loop_set_time

import uasyncio
import picoweb
import network


def do_webapp():
    webapp.debug = False
    webapp.init()
    for mount in webapp.mounts:
        mount.init()
    return asyncio.start_server(webapp._handle, '0.0.0.0', 80)


if __name__ == "__main__":
    ap_if = network.WLAN(network.AP_IF)
    if not ap_if.active():
        settime()

    sensor_manager = SensorManager(fake_data=True, send_to_airmonitor=False)
    #sensor_manager.setup()

    while True:
        loop = uasyncio.get_event_loop()

        loop.create_task(sensor_manager.execute(loop))
        if not ap_if.active():
            loop.create_task(loop_set_time())
        loop.create_task(do_webapp())
        
        loop.run_forever()
        loop.close()

