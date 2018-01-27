from sensor_manager import SensorManager

import uasyncio


if __name__ == "__main__":
    sensor_manager = SensorManager()
    sensor_manager.setup()

    while True:
        loop = uasyncio.get_event_loop()

        loop.create_task(sensor_manager.execute())
        # todo: loop.create_task(webapp.run())

        loop.run_forever()
        loop.close()
