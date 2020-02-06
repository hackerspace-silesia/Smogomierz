# Smogly [![Build Status](https://travis-ci.org/hackerspace-silesia/Smogomierz.svg?branch=master)](https://travis-ci.org/hackerspace-silesia/Smogomierz)

![Smogly1](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Smogomierz1.jpg)

Smogly to niekomercyjny projekt łatwego w budowie i taniego miernika zanieczyszczeń powietrza. Projekt jest w pełni otwarty i łatwy w modyfikowaniu. 

Smogly nie wysyła Twoich danych do żadnego serwisu, jeśli nie wyrazisz na to zgody. Dane pomiarowe z Twojego miernika należą do Ciebie i to od Ciebie zależy co z nimi zrobisz i komu je udostępnisz. Jeśli tylko Ty chcesz mieć dostęp do swoich danych, to masz do tego prawo. Jeśli jednak chcesz się nimi podzielić, to w karcie Konfiguracji panelu Smogomierza masz możliwość wyboru kilku serwisów, do których możesz wysłać dane. Wśród nich znajdziesz między innymi serwis [AirMonitor](http://mapa.airmonitor.pl), [ThingSpeak](https://thingspeak.com), [aqi.eco](https://aqi.eco/), czy bazę danych [InfluxDB](https://www.influxdata.com/time-series-platform/influxdb/). Smogomierz posiada również [wtyczkę Homebridge](https://github.com/bfaliszek/homebridge-smogomierz).

Smogly po pierwszym uruchomieniu tworzy własną sieć WiFi o nazwie "Smogly-deviceName"(np. "Smogly-328017"). Po podłączeniu się z nią, będziesz miał możliwość skonfigurowania połączenia WiFi z własną siecią domową. Z listy dostępnych sieci wybierz swoją własną i podaj do niej hasło. Smogly zapisze dane w swojej pamięci wewnętrznej i ponownie się uruchomienie. Dalszą konfigurację wykonasz już przez przeglądarkę internetową wchodząc pod adres Smogly-deviceName.local lub IP_Smogly(do sprawdzenia na routerze). 

Stworzyliśmy również aplikację dla systemów Windows, macOS oraz Linux), która umożliwia w łatwy sposób wgranie oprogramowania oraz znajdowanie adresu IP Twojego miernika! Możesz ją pobrać ze strony [github.com/hackerspace-silesia/Smogomierz-Flashing-Tool](https://github.com/hackerspace-silesia/Smogomierz-Flashing-Tool/releases).

## Darowizny

Projekt Smogly jest rozwijany od 2017 roku. Wszystko w ramach projektu Smogly robione jest za darmo, w ramach wolontariatu. Jeśli chcesz wesprzeć rozwój projektu, to możesz przekazać darowiznę na Stowarzyszenie Hackerspace Silesia. Najprościej można to zrobić przez specjalny formularz znajdujący się na stronie: [hs-silesia.pl/smogomierz](https://hs-silesia.pl/smogomierz/).

<a href="https://hs-silesia.pl/smogomierz/"><img align="left" width="1204" height="336" src="https://smogomierz.hs-silesia.pl/WsparcieSmogomierzeHS.jpg"></a>

## Instrukcje

1. [Instrukcja zlutowania wymaganych elementów](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/soldering.md)

2. [Instrukcja przygotowania obudowy oraz podłączenia elektroniki](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/hardware.md)

3. [Instrukcja instalacji/aktualizacji oprogramowania z pliku .bin](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software-bin.md)

4. [Konfiguracja i ustawienia obsługi zewnętrznych serwisów(AirMonitor, InfluxDB itd.)](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software-additionals.md)

## Parts / Potrzebne części

Do zbudowania własnego Smogly wymagana jest lutownica(wraz z cyną) oraz pistolet do kleju na gorąco, wiertarka, nóż do tapet i śrubokręt do skręcenia obudowy oraz części z poniższej listy.

[Lista potrzebnych części](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/components.md)

Gotowe zestawy części potrzebnych do zbudowania własnego Smogly, dostępne są również w sklepie Botland: [Zestaw DIY do budowy miernika smogu](https://botland.com.pl/pl/czujniki-czystosci-powietrza/13434-zestaw-diy-do-budowy-miernika-smogu-czujnik-czystosci-powietrza-pm25-i-pm10.html)

## Zaawansowane

[Instrukcja instalacji oprogramowania wymaganego do kompilacji oprogramowania z repozytorium](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software.md)

