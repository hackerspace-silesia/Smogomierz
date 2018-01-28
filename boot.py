# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import gc
#import webrepl
#webrepl.start()
gc.collect()


def do_connect():
    from utime import sleep
    import network
    import esp

    sta_if = network.WLAN(network.STA_IF)
    ap_if = network.WLAN(network.AP_IF)
    # todo - get jumper to one PIN
    ap_if.active(False)
    if not sta_if.isconnected():
        print('connecting to network...')
        sta_if.active(True)
        sta_if.connect('essid', 'passwd')
        while not sta_if.isconnected():
            pass
        print('network config:', sta_if.ifconfig())

do_connect()

