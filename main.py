from sensor_manager import SensorManager

import uasyncio


if __name__ == "__main__":
    sensor_manager = SensorManager()
    sensor_manager.setup()

    while True:
        loop = uasyncio.get_event_loop()

        loop.create_task(app.get_and_send_data())

        loop.run_forever()
        loop.close()
