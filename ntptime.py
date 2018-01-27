# below is from https://gist.github.com/drrk/4a17c4394f93d0f9123560af056f6f30 + pyb->machine and other fixes --bb

# borrowed from https://github.com/micropython/micropython/blob/master/esp8266/scripts/ntptime.py
import socket
import machine
import network
import utime

# (date(2000, 1, 1) - date(1900, 1, 1)).days * 24*60*60
NTP_DELTA = 3155673600

host = "pool.ntp.org"

def getntptime():
    NTP_QUERY = bytearray(48)
    NTP_QUERY[0] = 0x1b
    addr = socket.getaddrinfo(host, 123)[0][-1]
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    res = s.sendto(NTP_QUERY, addr)
    msg = s.recv(48)
    s.close()
    import ustruct
    val = ustruct.unpack("!I", msg[40:44])[0]
    return val - NTP_DELTA

def settime():
    import time
    from machine import RTC
    t = getntptime()
    tm = time.localtime(t)
    tm = tm[0:3] + (0,) + tm[3:6] + (0,)
    rtc = RTC()
    #rtc.init()
    rtc.datetime(tm)

#nic = network.CC3100()
#nic.connect("emfcamp-insecure")

#print("Get NTP Time")
# set the RTC using time from ntp
settime()
#print("Display RTC Time")
# print out RTC datetime
#print(pyb.RTC().datetime())
#print("Set NIC Time")
#nic.settime(utime.localtime())

