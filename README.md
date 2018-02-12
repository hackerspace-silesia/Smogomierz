# Smogomierz

Smogomierz to projekt łatwego w budowie i taniego miernika zanieczyszczeń powietrza. Projekt jest w pełni otwarty i łatwy w modyfikowaniu. 

## Instrukcje

Instrukcja instalacji wymaganego oprogramowania: https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software.md


Instrukcja podłączenia elektroniki: https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/hardware.md


## Parts / Potrzebne części

https://github.com/hackerspace-silesia/Smogomierz/blob/master/components.md

## Connection / Podłączenie

**BME280** – VIN - 3V; GND - G; SCL - D4; SDA - D3

**PMS7003** – White/Biały/5V - VIN/5V; Black/Czarny/GND - G; Green/Zielony/TX - D1; Blue/Niebieski/RX - D2

![Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/schemat.png)

### Tested on:

ArdinoIDE 1.8.5 – https://www.arduino.cc/en/main/software

nodemcu-master-16-modules-2018-01-14-09-39-20-float - https://www.dropbox.com/s/rxtoz876kscjvb2/nodemcu-master-16-modules-2018-01-14-09-39-20-float.bin

NodeMCU custom builds – https://nodemcu-build.com

### TODO

- [ ] OTA Update – http://esp8266.github.io/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html#arduino-ide


### Dodatkowe informacje

"Projekt finansowany ze środków programu ramowego Unii Europejskiej w zakresie badań naukowych i innowacji „Horyzont 2020” na podstawie umowy o udzielenie dotacji nr 709443."