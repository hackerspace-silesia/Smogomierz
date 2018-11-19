# Smogomierz

Smogomierz to niekomercyjny projekt łatwego w budowie i taniego miernika zanieczyszczeń powietrza. Projekt jest w pełni otwarty i łatwy w modyfikowaniu. 

Miernik wysyła dane na stronę - [mapa.airmonitor.pl](http://mapa.airmonitor.pl) oraz ma możliwość przesyłania pomiarów do serwisu [ThingSpeak](https://thingspeak.com) i baz danych [InfluxDB](https://www.influxdata.com/time-series-platform/influxdb/).

Smogomierz po pierwszym uruchomieniu tworzy własną sieć WiFi o nazwie "Smogomierz-deviceName". "deviceName" generowane jest na podstawie unikalnego numeru Chip ID płytki ESP8266. Na początku pliku defaultConfig.h, możemy zmienić wartość "DEVICENAME_AUTO" z "true" na "false". Wówczas zamiast unikalnego numery Chip ID używana będzie nazwa z paramatru "DEVICENAME" z pliku defaultConfig.h. 

Smogomierz obsługuje Zeroconf(Bonjour). Pozwala on na połączenie się z miernikiem będącym w naszej sieci, bez znajomości jego adresu IP. Po połączeniu Smogomierza z naszą siecią WiFi, możemy otworzyć przeglądarkę internetową i wpisać adres: "Smogomierz-deviceName.local". Zeroconf do działania potrzebuje doinstalowania na komputerze z Windowsem aplikacji [Bonjour](https://support.apple.com/kb/DL999?locale=pl_PL), a pod Linuksem – Avahi. System macOS(oraz iOS) nie wydama instalacji dodatkowego oprogramowania. 

### Aktualizacje oprogramowania

Zakładka Update w panelu Smogomierza pozwala na wgrywanie plików .bin z nową wersją oprgramowania dla miernika. Plik bin można stworzyć samemu z kodu dostępnego w tym repozytorium. Do zrobienia tego potrzebne jest ArduinoIDE – [Instrukcja instalacji wymaganego oprogramowania](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software.md). W "Plik > Preferencje" lub "Arduino > Preferences…" zaznaczamy przy "Pokaż szczegółowe informacje podczas:" opcję "Kompilacji". W czasie kompilowania naszego kodu, na dole ekranu ArduinoIDE pojawią się adresy(coś w stylu "/var/folders/k9/tbv_kmpn1dj5fl5spjk209480000gn/T/arduino_build_795090/Smogomierz.ino.bin"), gdzie zostanie zapisany plik Smogomierz.ino.bin. Plik ten to właśnie paczka z naszym oprogramowaniem, które możemy wgrać przez kartę Update w panelu Smogomierza.

### Obsługa HomeKit dla systemu iOS

Jeśli jesteś użytkownikiem urządzenia z systemem iOS, możesz użyć wtyczki dla [Homebridge](https://github.com/nfarina/homebridge), aby mieć dostęp do danych pomiarowych w aplikacji Dom na swoim iPhonie, iPadzie lub na Macu. Wtyczka Homebridge-Smogomierz dostępna jest [tutaj](https://github.com/bfaliszek/homebridge-smogomierz).

## Instrukcje

1. [Instrukcja instalacji wymaganego oprogramowania](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software.md)

2. [Rejestracja i tworzenie nowego kanału w serwisie ThingSpeak](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/thingspeak.md)

3. [Instrukcja podłączenia elektroniki](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/hardware.md)


## Parts / Potrzebne części

https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/components.md

![Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/schemat.png)


### Tested on:

ArdinoIDE 1.8.5 – https://www.arduino.cc/en/main/software

### TODO

- [ ] Nowy design
- [ ] Kropki a nie przecinki przy zmianie współrzędnych w /config
- [ ] Automatyczne aktualizacje oprogramowania mierników
- [ ] Przygotowanie aplikacji Smogomierz-firmware-flasher bazującej na [airrohr-firmware-flasher](https://github.com/hackerspace-silesia/airrohr-firmware-flasher)


