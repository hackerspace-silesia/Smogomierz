# Smogomierz - Arduino version

## Parts / Potrzebne części


## Connection / Podłączenia

**BME280** – VIN - 3V; GND - G; SCL - D4; SDA - D3

**PMS7003** – White/Biały/5V - VIN/5V; Black/Czarny/GND - G; Green/Zielony/TX - D1; Blue/Niebieski/RX - D2

![Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/schemat.png)

Tested on:

ArdinoIDE 1.8.5 – https://www.arduino.cc/en/main/software

nodemcu-master-16-modules-2018-01-14-09-39-20-float - https://www.dropbox.com/s/rxtoz876kscjvb2/nodemcu-master-16-modules-2018-01-14-09-39-20-float.bin

NodeMCU custom builds – https://nodemcu-build.com

### TODO

- [x] WiFi config
- [x] PMS7003 support
- [x] BME280 support
- [x] Display current data on Webbapp on ESP8266
- [ ] Send data from PMS7003 and BME280 to http://mapa.airmonitor.pl
- [ ] Get time from NTP server
- [ ] Storage data
- [ ] Display old data on Webbapp on ESP8266
- [ ] Change Latitude and Longitude on Webbapp on ESP8266
- [ ] OTA Update – http://esp8266.github.io/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html#arduino-ide