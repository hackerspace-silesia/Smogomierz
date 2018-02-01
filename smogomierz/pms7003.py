#!/usr/bin/env python

# http://docs.micropython.org/en/v1.9.3/wipy/library/machine.UART.html

from machine import UART
import time, sys, json, ubinascii


class PMS7003:
    def __init__(self):
        self.serial = UART(0, 9600)
        self.serial.init(9600, bits=8, parity=None, stop=1)

    def read_data(self):
        for i in range(2048):
            b = self.serial.read(1)
            if b != b'B':
                continue

            data = self.serial.read(31)
            # I HATE YOU PYTHON3 SO MUCH: data[0] is an integer, not string
            if not data or data[0:1] != b'M':
                continue

            self.data = bytearray(b'\x42' + data)
            return self._PMdata()

        # timeout
        return 0, 0, 0

    def _PMdata(self):
        data = self.data
        pm1 = int(data[4] * 256 + data[5])
        pm25 = int(data[6] * 256 + data[7])
        pm10 = int(data[8] * 256 + data[9])
        # When results are crazy then fill results with zeros
        if (pm25 == 0 and pm10 > 5000) or (pm25 > 5000 and pm10 == 0):
            pm1 = 0
            pm25 = 0
            pm10 = 0
        return pm1, pm25, pm10

