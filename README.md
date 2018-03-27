### TODO

- [X] OTA Update – [Dokumentacja](http://esp8266.github.io/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html#arduino-ide)
- [ ] Przeniesienie webserver ze Smogomierz.ino do webserver.h i webserver.cpp 
- [ ] Sprawdzenie czy plik config.h istnieje
- [ ] Przegranie zawartości config.h do pliku w ESP - przykład https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/examples/FSBrowser/FSBrowser.ino
- [ ] Możliwość edycji pliku Config w pamięci ESP poprzez przeglądarkę(zakładka Config w panelu miernika)
- [X] Kolory dla wartości pomiarów PM10 i PM2.5 w zależności od przekroczenia normy zanieczyszczeń 
- [ ] Uśrednianie wyników pomiarów - np. wyświetlenie i wysyłanie wyników z 10 pomiarów, a nie bierzącego 
- [ ] Wysyłanie danych do InfluxDB 
- [X] Podstrona /api z bierzącymi danymi pomiarowymi jako JSON
- [X] Obsługa Homebridge 

# Smogomierz

Smogomierz to projekt łatwego w budowie i taniego miernika zanieczyszczeń powietrza. Projekt jest w pełni otwarty i łatwy w modyfikowaniu. 

Miernik wysyła dane na stronę - [mapa.airmonitor.pl](http://mapa.airmonitor.pl) oraz ma możliwość przesyłania pomiarów do serwisu [ThingSpeak](https://thingspeak.com).

Smogomierz po pierwszym uruchomieniu tworzy własną sieć WiFi o nazwie "Smogomierz-deviceName". "deviceName" generowane jest na podstawie unikalnego numeru Chip ID płytki ESP8266. Na początku pliku Config.h, możemy zmienić wartość "DEVICENAME_AUTO" z "true" na "false". Wówczas zamiast unikalnego numery Chip ID używana będzie nazwa z paramatru "DEVICENAME" w pliku Config.h. 

Smogomierz obsługuje Zeroconf(Bonjour). Pozwala on na połączenie się z miernikiem będącym w naszej sieci, bez znajomości jego adresu IP. Po połączeniu miernika smogu z naszą siecią WiFi, możemy otworzyć przeglądarkę internetową i wpisać adres: "Smogomierz-deviceName.local". Zeroconf do działania potrzebuje doinstalowania na komputerze z Windowsem aplikacji [Bonjour](https://support.apple.com/kb/DL999?locale=pl_PL), a pod Linuksem – Avahi. 

Zakładka Update w panelu Smogomierza pozwala na wgrywanie plików .bin z nową wersją oprgramowania dla miernika. Plik bin można stworzyć samemu z kodu dostępnego w tym repozytorium. Do zrobienia tego potrzebne jest ArduinoIDE – [Instrukcja instalacji wymaganego oprogramowania](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software.md). W "Plik > Preferencje" lub "Arduino > Preferences…" zaznaczamy przy "Pokaż szczegółowe informacje podczas:" opcję "Kompilacji". W czasie kompilowania naszego kodu, na dole ekranu ArduinoIDE pojawią się adresy(coś w stylu "/var/folders/k9/tbv_kmpn1dj5fl5spjk209480000gn/T/arduino_build_795090/Smogomierz.ino.bin"), gdzie zostanie zapisany plik Smogomierz.ino.bin. Plik ten to właśnie paczka z naszym oprogramowaniem, które możemy wgrać przez kartę Update w panelu Smogomierza.

Jeśli jesteś użytkownikiem urządzenia z systemem iOS, możesz użyć wtyczki dla [Homebridge](https://github.com/nfarina/homebridge), aby mieć dostęp do danych pomiarowych w aplikacji Dom na swoim iPhonie lub iPadzie. Wtyczka Homebridge-Smogomierz dostępna jest [tutaj](https://github.com/bfaliszek/homebridge-smogomierz).

## Instrukcje

1. [Instrukcja instalacji wymaganego oprogramowania](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software.md)

2. [Rejestracja i tworzenie nowego kanału w serwisie ThingSpeak](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/thingspeak.md)

3. [Instrukcja podłączenia elektroniki](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/hardware.md)


## Parts / Potrzebne części

https://github.com/hackerspace-silesia/Smogomierz/blob/master/components.md

![Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/schemat.png)


### Tested on:

ArdinoIDE 1.8.5 – https://www.arduino.cc/en/main/software

nodemcu-master-16-modules-2018-01-14-09-39-20-float - https://www.dropbox.com/s/rxtoz876kscjvb2/nodemcu-master-16-modules-2018-01-14-09-39-20-float.bin

NodeMCU custom builds – https://nodemcu-build.com


### Dodatkowe informacje

"Projekt finansowany ze środków programu ramowego Unii Europejskiej w zakresie badań naukowych i innowacji „Horyzont 2020” na podstawie umowy o udzielenie dotacji nr 709443."
