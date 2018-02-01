from bme280 import BME280
from pms7003 import PMS7003
from machine import Pin, I2C, RTC

from uasyncio import sleep
from ustruct import pack

import ujson
import urequests
import utime
import uasyncio


URL_API = 'http://api.airmonitor.pl:5000/api'
SENSOR_MODEL = 'PMS7003'


class SensorManager:
    fake_data = False
    send_to_airmonitor = False

    bme280 = None  # type: BME280
    pms7003 = None  # type: PMS7003
    db = None  # type: list
    
    def __init__(self, db, fake_data=False, send_to_airmonitor=True):
        self.db = db
        self.fake_data = fake_data
        self.send_to_airmonitor = send_to_airmonitor

    def setup(self):
        """
            Setup sensors BME280 nad PMS7003
            WARNING: pms7003 need UART,
            Communication python shell <-> UART will be turned off!
        """
        i2c = I2C(scl=Pin(5), sda=Pin(4)) 
        self.bme280 = BME280(i2c=i2c)
        self.pms7003 = PMS7003()

    @staticmethod
    def _avg(a, b):
        return (a + b) / 2

    async def get_data(self):
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
        avg = self._avg

        # get two results and compute avg
        _1pm1, _1pm25, _1pm10 = self.pms7003.read_data()
        await sleep(0.05)
        _2pm1, _2pm25, _2pm10 = self.pms7003.read_data()

        pm1 = avg(_1pm1, _2pm1)
        pm25 = avg(_1pm25, _2pm25)
        pm10 = avg(_1pm10, _2pm10)
        
        _1temp, _1press, _1hum = self.bme280.read_compensated_data()
        await sleep(0.05)
        _2temp, _2press, _2hum = self.bme280.read_compensated_data()

        temp = avg(_1temp, _2temp)
        press = avg(_1press, _2press)
        hum = avg(_1hum, _2hum)

        return {
            'pm1': pm1,
            'pm25': pm25,
            'pm10': pm10,
            'temperature': temp / 100.0,
            'pressure': (press // 256) / 100.0,
            'humidity': hum / 1024.0,
            'date': utime.time(),
        }

    async def get_fake_data(self):
        return {
            'pm1': 5,
            'pm25': 5,
            'pm10': 5,
            'temperature': 20,
            'pressure': 20,
            'humidity': 100,
            'date': utime.time(),
        }

    async def execute(self, loop):
        while True:
            loop.create_task(self.get_and_send_data(loop))
            await sleep(15)

    async def get_and_send_data(self, loop):
        if self.fake_data:
            data = await self.get_fake_data()
        else:
            data = await self.get_data()

        loop.create_task(self.save_data(data))
        loop.create_task(self.upload_to_airmonitor(data))

    async def save_data(self, data):
        obj = pack(
            'iffffff',
            data['date'],
            data['pm1'],
            data['pm25'],
            data['pm10'],
            data['temperature'],
            data['pressure'],
            data['humidity'],
        )
        self.db.insert(0, obj)
        if len(self.db) > 48:
            self.db.pop()

    async def upload_to_airmonitor(self, data):
        if not self.send_to_airmonitor:
            return

        try:
            with open('config') as f:
                config = ujson.load(f)
        except OSError:
            return

        air_data = ''
        air_data += '{'
        air_data += '"lat":%0.4f,' % config.pop('airmonitor_lat')
        air_data += '"long":%0.4f,' % config.pop('airmonitor_long')
        del config
        air_data += '"pressure":%0.2f,' % data['pressure']
        air_data += '"temperature":%0.2f,' % data['temperature']
        air_data += '"humidity":%0.2f,' % data['humidity']
        air_data += '"pm1":%0.2f,' % data['pm1']
        air_data += '"pm25":%0.2f,' % data['pm25']
        air_data += '"pm10":%0.2f,' % data['pm10']
        air_data += '"sensor":"%s"' % SENSOR_MODEL
        air_data += '}'

        resp = urequests.post(
            URL_API,
            data=air_data,
            headers={"Content-Type": "application/json"},
        )

        resp.close()
