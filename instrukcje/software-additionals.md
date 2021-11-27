# Smogomierz - dodatki

Smogomierz pozwala na obsługę różnych serwisów internetowych, do których możesz wysłać swoje dane. Poniżej znajdziesz opis jak korzystać z kilku z nich.

## Nazwa urządzenia oraz Bonjour/Zeroconf

Smogomierz domyślnie tworzy sieć WiFi o nazwie np. "Smogomierz-328017". Ostatnie 6 znaków to Chip ID płytki ESP. Jest ono unikatowe dla każdego ESP32/ESP8266. Nazwę naszego urządzenia możemy zmienić w zakładce Konfiguracja w panelu Smogomierza. Musimy w tym celu wyłączyć automatyczne generowanie nazwy i wpisać swoją własną. 

Smogomierz domyślnie obsługuje protokół Bonjour/Zeroconf. Pozwala on na połączenie się z miernikiem będącym w naszej sieci, bez znajomości jego adresu IP. Po połączeniu Smogomierza z naszą siecią WiFi, możemy otworzyć przeglądarkę internetową i wpisać adres np. "Smogomierz-328017.local"(jeśli ustawimy sobie własną nazwę urządzenia będzie to NaszaNazwa.local). Zeroconf do działania potrzebuje doinstalowania na komputerze z Windowsem aplikacji [Bonjour](https://support.apple.com/kb/DL999?locale=pl_PL), a pod Linuksem – Avahi. System macOS(oraz iOS) nie wymaga instalacji dodatkowego oprogramowania.

## Sensor.Community

[Sensor.Community](https://maps.sensor.community/#7/52.000/19.000) to największa sieć społecznych czujników jakości powietrza na świecie. Powstała na bazie projektu Luftdaten. Serwis ten umożliwia tworzenie wykresów z pomiarami zanieczyszczeń powietrza i wyświetlanie ich bezpośrednio w panelu Smogly. Dodatkowo serwis udostępnia dane do dziesiątek aplikacji i serwisów zewnętrznych, dzięki temu nasze dane pomiarowe będą mogły znaleźć się w wielu aplikacjach mobilnych czy na mapach zanieczyszczeń powietrza.

Jeśli chcesz wysyłać swoje dane do serwisu Sensor.Community i wyświetlać swoje wykresy, to na początku musisz sprawdzić współrzędne geograficzne miejsca, gdzie znajduje się Twój miernik. Może to zrobić na stronie https://www.wspolrzedne-gps.pl. Wystarczy tam wpisać adres, a uzyskamy szerokość oraz długość geograficzną. 

Sensor.Community wymaga rejestracji na stronie [devices.sensor.community](https://devices.sensor.community), gdzie podajemy adres naszego urządzenia(współrzędne geograficzne, warto zwrócić uwagę, aby były takie same jak w panelu Smogly). Dodatkowo podajemy ID sensora(ChipID z panelu Smogly), oraz wybieramy jakie mierniki ma nasze urządzenie. Jeśli Współrzędne oraz miernik naszego urządzenia są identyczne na stronie devices.sensor.community jak w panelu ustawień Smogly, możemy właczyć wykresy, które powstają na podstawie przesłanych do serwisu danych.

## aqi.eco

[aqi.eco](https://aqi.eco) to serwis, który pozwala na gromadzenie danych pomiarowych z różnych czujników oraz tworzenie wykresów do zgromadzonych danych. Wykresy oraz pomiary mogą być prywatne lub publicznie dostępne. Serwis wymaga rejestracji. Ma darmowe konta ograniczone. aqi.eco pozwala na dostęp do danych pomiarowych z dowolnego miejsca. Jeśli chcesz mieć dostęp do informacji ze swojego Smogomierza w każdym miejscu, to aqi.eco powinien w tym pomóc. Jest to wygodne rozwiązanie dla osób, które nie chcą tworzyć własnej bazy danych, a zależy im głównie na dostępnie do informacjach z własnego miernika.

## ThingSpeak

[ThingSpeak](https://thingspeak.com) to serwis, który pozwala na gromadzenie danych pomiarowych z różnych czujników oraz tworzenie wykresów do zgromadzonych danych. Wykresy oraz pomiary mogą być prywatne lub publicznie dostępne. Serwis wymaga rejestracji. Ma darmowe konta ograniczone do 3 mln pomiarów na rok. ThingSpeak pozwala na dostęp do danych pomiarowych z dowolnego miejsca. Wystarczy tylko login i hasło. Jeśli chcesz mieć dostęp do informacji ze swojego Smogomierza w każdym miejscu, to ThingSpeak powinien w tym pomóc. Dodatkowo serwis posiada aplikacje dostępne na smartfony. Jest to wygodne rozwiązanie dla osób, które nie chcą tworzyć własnej bazy danych, a zależy im głównie na dostępnie do informacjach z własnego miernika. Opis rejestracji w serwisie ThingSpeak oraz konfiguracji profilu znajdziesz w instrukcji ["ThingSpeak"](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/thingspeak.md).

## AirMonitor - ESP32

Serwis [AirMonitor](http://mapa.airmonitor.pl) pozwala na umieszczenie naszych danych pomiarowych na mapie. Dodatkowo umożliwia nam generowanie wykresów. Działa tylko z ESP32.

Jeśli chcesz wysyłać swoje dane do serwisu AirMonitor i wyświetlać swoje wykresy, to na początku musisz sprawdzić współrzędne geograficzne miejsca, gdzie znajduje się Twój Smogomierz. Może to zrobić na stronie https://www.wspolrzedne-gps.pl. Wystarczy tam wpisać adres, a uzyskamy szerokość oraz długość geograficzną. 

Musimy również wypełnić [formularz](https://airmonitor.pl/prod/station_registration), aby punkt z naszymi współrzędnymi pojawił się na mapie(Model sensora np. PMS7003). W formularzu długość i szerokość geograficzną wpisujemy w formacie xx.xxxx(np. 50.1234; używamy kropek, a nie przecinków).

Na koniec zostało nam tylko włączenie możliwości wysyłania danych pomiarowych w naszym mierniku. Możemy to zrobić oczywiście w zakładce Konfiguracja w panelu Smogomierza. Musimy tutaj również wpisać nasze współrzędne geograficzne(tutaj używamy formatu xx,xxx – np. 50,1234; używamy przecinków a nie kropek). Dodatkowo podanie wysokości umożliwi nam dokładniejsze pomiary ciśnienia atmosferycznego.

## InfluxDB

Smogomierz pozwala na wysyłanie pomiarów do własnej bazy danych [InfluxDB](https://www.influxdata.com/time-series-platform/influxdb/). Jeśli chcesz to zrobić, to w zakładce Konfiguracja w panelu Smogomierza, musisz włączyć wysyłanie danych do InfluxDB oraz wpisać adres swojej bazy danych, podać port na którym działa, nazwę bazy danych oraz użytkownika i hasło. InfluxDB jest świetnym źródłem danych dla Grafany, czyli darmowego środowiska do wizualizacji danych. [Grafana](https://grafana.com) pozwala na łatwe tworzenie wykresów ze zgromadzonych danych. 

## HomeKit

Jeśli jesteś użytkownikiem sprzętu Apple i Twoje urządzenie używa ESP32, to w ustawieniach możesz włączyć obsługę HomeKit. Dzięki temu będizesz mógł dodać swój miernik do aplikacji Dom na iPhonie/iPadzie/Macu. Domyślny kod dodawania nowych urządzeń to 111-11-111.

## Homebridge

Jeśli jesteś użytkownikiem sprzętu Apple, to możesz również skorzystać z wtyczki dla [Homebridge](https://github.com/nfarina/homebridge), aby mieć dostęp do danych pomiarowych w aplikacji Dom na swoim iPhonie, iPadzie lub na Macu. Wtyczka Homebridge-Smogomierz dostępna jest [tutaj](https://github.com/bfaliszek/homebridge-smogomierz).

## API

Każdy Smogomierz wyświetla pod adresem IP_Smogomierza/api aktualne dane pomiarowe w formacie JSON. Możesz ich użyć w dowolny sposób w swoich własnych projektach.
