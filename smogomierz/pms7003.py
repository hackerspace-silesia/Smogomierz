#!/usr/bin/env python

# http://docs.micropython.org/en/v1.9.3/wipy/library/machine.UART.html

from machine import UART
import time, sys, json, ubinascii


class PMS7003:
    def __init__(self):
        self.serial = UART(0, 9600)
        self.serial.init(9600, bits=8, parity=None, stop=1, timeout=10)

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
        for i in range(255):
            b = self.serial.read(1)
            if b is None:
                break
            if b != b'\x42':
                continue
            data = self.serial.read(31)
            if data is None:
                break
            if data[0] != b'\x4d':
                continue
            self.data = bytearray(b'\x42' + data)
            return self._PMdata()
        return float('nan'), float('nan'), float('nan')

    def _PMdata(self):
        pm1 = int(self.data[4] * 256 + self.data[5])
        pm25 = int(self.data[6] * 256 + self.data[7])
        pm10 = int(self.data[8] * 256 + self.data[9])
        # When results are crazy then fill results with zeros
        if (pm25 == 0 and pm10 > 5000) or (pm25 > 5000 and pm10 == 0):
            pm1 = 0
            pm25 = 0
            pm10 = 0
        return pm1, pm25, pm10

