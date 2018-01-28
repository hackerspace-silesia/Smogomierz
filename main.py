from sensor_manager import SensorManager
from webapp import webapp
from ntptime import settime, loop_set_time

import uasyncio
import picoweb


def do_webapp():
    webapp.debug = False
    webapp.init()
    for mount in webapp.mounts:
        mount.init()
    return asyncio.start_server(webapp._handle, '0.0.0.0', 80)


if __name__ == "__main__":
    sensor_manager = SensorManager()
    sensor_manager.setup()

    settime()

    while True:
        loop = uasyncio.get_event_loop()

        loop.create_task(sensor_manager.execute())
        loop.create_task(loop_set_time())
        loop.create_task(do_webapp(routes))
        
        loop.run_forever()
        loop.close()

