# This file is executed on every boot (including wake-boot from deepsleep)
import esp
esp.osdebug(None)
import gc
gc.collect()

debug_mode = False


def do_connect():
    global debug_mode
    from utime import sleep
    import ujson
    import network
    import machine
    import ustruct

    sta_if = network.WLAN(network.STA_IF)
    ap_if = network.WLAN(network.AP_IF)

    pin = machine.Pin(14, machine.Pin.IN, machine.Pin.PULL_UP)
    sleep(0.1)
    if not pin.value():
        machine_id = ustruct.unpack("<L", machine.unique_id())[0]
        sta_if.active(False)
        ap_if.active(True)
        sleep(1)
        ap_if.config(essid='Smogomierz %d' % machine_id)
        print (ap_if.ifconfig())
        print ('DEBUG MODE')
        debug_mode = True
        sleep(3)
        return

    ap_if.active(False)

    try:
        with open('config') as f:
            config = ujson.load(f)
    except OSError:
        print('config failed.')
        return

    if not sta_if.isconnected():
        print('connecting to network %s...' % config['wifi_essid'])
        sta_if.active(True)
        sta_if.connect(config['wifi_essid'], config['wifi_password'])
        while not sta_if.isconnected():
            pass
        print('network config:', sta_if.ifconfig())
        sleep(2)

do_connect()

