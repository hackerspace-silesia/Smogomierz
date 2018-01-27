from bme280 import BME280
from pms7003 import PMS7003
from machine import Pin, I2C
from datetime import datetime

from uasyncio import sleep, gather

import ujson
import urequests


URL_API = 'http://api.airmonitor.pl:5000/api'
SENSOR_MODEL = 'PMS7003'


class SensorManager:
    debug = False
    bme280 = None  # type: BME280
    pms7003 = None  # type: PMS7003
    
    def __init__(self, debug=False):
        self.debug = debug

    def setup(self):
        """
            Setup sensors BME280 nad PMS7003
            WARNING: pms7003 need UART,
            Communication python shell <-> UART will be turned off!
        """
        i2c = I2C(scl=Pin(5), sda=Pin(4)) 
        self.bme280 = BME280(i2c=i2c)
        self.pms7003 = PMS7003()

    def get_data(self):
        """
            returns a dict with data
            :return: dict with keys::
                
                pm1 - PM1.0 from PMS7003 [μg/m³]
                pm25 - PM2.5 from PMS7003 [μg/m³]
                pm10 - PM10 from PMS7003 [μg/m³]
                temperature - [°C]
                pressure - [hPa]
                humidity - [%]
                date - YYYY-MM-DD hh:mm:ss
        """
        pm1, pm25, pm10 = self.pms7003.read_data()
        temp, press, hum = self.bmp280.read_compensated_data()
        return {
            'pm1': pm1,
            'pm25': pm25,
            'pm10': pm10,
            'temperature': temp / 100.0,
            'pressure': (press // 256) / 100.0,
            'humidity': hum / 1024.0,
            'date': datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
        }

    async def execute(self):
        while True:
            gather(
                self.get_and_send_data,
                sleep(60),
            )

    async def get_and_send_data(self):
        data = self.get_data()

        gather(
            save_data_to_file(data),
            upload_to_airmonitor(data),
        )


    async def save_data_to_file(self, data):
        with open('data', 'a') as f:
            f.write(ujson.dumps(data) + '\n')

    async def upload_to_airmonitor(self, data):
        # todo: get lat & long from config
        config_lat = 50.2639
        config_long = 18.9957

        air_data = {
            'lat': lat,
            'long': long,
            'pressure': data['pressure'],
            'temperature': data['temperature'],
            'humidity': data['humidity'],
            'pm1': data['pm1'],
            'pm25': data['pm25'],
            'pm10': data['pm10'],
            'sensor': SENSOR_MODEL,
        }

        resp = urequests.post(
            URL_API,
            timeout=10,
            data=data,
            headers={"Content-Type": "application/json"},
        )
