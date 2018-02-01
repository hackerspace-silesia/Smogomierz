from sensor_manager import SensorManager
from webapp import serve, db
from ntptime import settime
from machine import reset

import uasyncio
import network
import utime
import sys


def do_webapp():
    return uasyncio.start_server(serve, '0.0.0.0', 80)


async def loop_set_time():
    while True:
        settime()
        await uasyncio.sleep(3600)


if __name__ == "__main__":
    ap_if = network.WLAN(network.AP_IF)
    sensor_manager = SensorManager(db, send_to_airmonitor=True)

    gc.collect()
    try:
        loop = uasyncio.get_event_loop()
        if not debug_mode:
            settime()
            sensor_manager.setup()

        if not debug_mode:
            loop.create_task(sensor_manager.execute(loop))
            loop.create_task(loop_set_time())
        loop.create_task(do_webapp())
        
        loop.run_forever()
    except KeyboardInterrupt:
        loop.close() 
    except Exception as e:
        with open('error', 'w') as f:
            if debug_mode:
                sys.print_exception(e)
            else:
                sys.print_exception(e, f)

        utime.sleep(1)
        reset()

