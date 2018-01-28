all:

lib: requirements.txt
	micropython -m upip install -p lib -r requirements.txt

build:
	mpy_cross_all.py lib/picoweb -o mpy/lib/picoweb
	mpy-cross lib/pkg_resources.py -o mpy/lib/pkg_resources.mpy
	mpy_cross_all.py smogomierz -o mpy

deploy:
	ampy -p /dev/ttyUSB0 rmdir lib
	ampy -p /dev/ttyUSB0 put mpy/lib /lib
	ampy -p /dev/ttyUSB0 put mpy/bme280.mpy 
	ampy -p /dev/ttyUSB0 put mpy/pms7003.mpy 
	ampy -p /dev/ttyUSB0 put mpy/webapp.mpy 
	ampy -p /dev/ttyUSB0 put mpy/sensor_manager.mpy 
