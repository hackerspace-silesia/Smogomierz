# Smogomierz - lutowanie

W przypadku kilku części używanych w Smogomierzy konieczne użycie lutownicy. Lutowanie jest tutaj ograniczone do minimum i w 90% polega na przylutowaniu pinów do mierników. Dla przykładu miernik temperatury, wilgotności i ciśnienia powietrza BME280 najcześciej dostaje się w woreczku razem z pinami do przylotowania. Podobnie z wtyczką microUSB, której użyjemy jako źródło zasilania Smogomierza. Będziemy musili również zlutować ze sobą trzy goldpiny, aby stworzyć rozdzielacz zasilania 5V.

![lutowanie1](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/lutowanie1.jpg)

## BME280 oraz wtyczka microUSB

W przypadku obu tych elementów sprawa jest dosyć prosta. Musimy w otwory w miernikach wlutować goldpiny.

![Lutowanie_BME280](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Lutowanie_BME280.jpg)
![Lutowanie_microUSB](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Lutowanie_microUSB.jpg)

## Rozdzielacz 5V

W celu jednoczesnego zasilania ESP8266 oraz miernika zanieczyszczeń powietrza, musimy przygotować rozdzielacz napięcia 5V. Nie jest to nic skomplikowanego. Można go zrobić na wiele sposobów. Postanowiliśmy użyć do tego trzech goldpinów i je ze sobą zlutować.

![Lutowanie_Rozdzielacz5V](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Lutowanie_Rozdzielacz5V.jpg)