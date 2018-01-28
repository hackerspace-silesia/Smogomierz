#!/usr/bin/env python

# http://docs.micropython.org/en/v1.9.3/wipy/library/machine.UART.html

from machine import UART
import time, sys, json, ubinascii
import machine
#import bme280

#i2c = machine.I2C(scl=machine.Pin(5), sda=machine.Pin(4))
#bme = bme280.BME280(i2c=i2c)


class PMS7003():
    def __init__(self):
        self.serial = UART(0, 9600)
        self.serial(9600, bits=8, parity=None, stop=1)

    def __exit__(self, exc_type, exc_value, traceback):
        self.serial.deinit()

    def setIdel(self):
        idelcmd = b'\x42\x4d\xe4\x00\x00\x01\x73'
        ary = bytearray(idelcmd)
        self.serial.write(ary)

    def setNormal(self):
        normalcmd = b'\x42\x4d\xe4\x00\x01\x01\x74'
        ary = bytearray(normalcmd)
        self.serial.write(ary)

    def vertify_data(self):
        if not self.data:
            return False
        return True

    def read_data(self):
        while True:
            b = self.serial.read(1)
            if b == b'\x42':
                data = self.serial.read(31)
                if data[0] == b'\x4d':
                    self.data = bytearray(b'\x42' + data)
                    if self.vertify_data():
                        return self._PMdata()

    def _PMdata(self):
        #global PM1, PM25, PM10
        PM1 = int(self.data[4] * 256 + self.data[5])
        PM25 = int(self.data[6] * 256 + self.data[7])
        PM10 = int(self.data[8] * 256 + self.data[9])
        #Czasami wyniki moga byc szalone - sprawdzenie czy odczyt nie pokazuje czegos dziwnego. Jesli tak to zerowanie pomiaru
        if PM25 == 0 and PM10 > 5000:
            PM1 = 0
            PM25 = 0
            PM10 = 0
        elif PM25 > 5000 and PM10 == 0:
            PM1 = 0
            PM25 = 0
            PM10 = 0
        return PM1, PM25, PM10


if __name__ == "__main__":
    print ("Uruchamianie...")

    while True:
        #print(bme.values)
        read_data()
        print('PM1: %d ug/m3' % round(PM1))
        print('PM2.5: %d ug/m3' % round(PM25))
        print('PM10: %d ug/m3' % round(PM10))
        time.sleep(5)
