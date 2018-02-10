# Smogomierz - podłączenie elektroniki

Poniższy opis krok po kroku, przeprowadzi Was przez procedurę podłączenia wszystkich czujników obsługiwanych przez nasz miernik smogu. Listę części, wraz z linkami do przykładowych sklepów, gdzie można je kupić znajdziecie pod adresem: https://github.com/hackerspace-silesia/Smogomierz/blob/Arduino/components.md

Całościowy schemat podłączenia:

![Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/schemat.png)

## ESP8266 - NodeMCU V3

Główny element miernika. To właśnie na tę płytkę wgrywamy oprogramowanie. Ma ona wbudowany moduł WiFi, który pozwala na tworzenie sieci bezprzewodowej i wysyłanie danych z pozostałych czujników.

OPIS ESP8266

## BME280 - miernik temperatury, wilgotności i ciśnienia powietrza

BME280 to niewielki moduł pozwalający na bardzo precyzyjne pomiary temperatury powietrza, ciśnienia oraz wilgotności powietrza. 

OPIS BME280

PODŁĄCZENIE BME280

## PMS7003 - laserowy miernik pyłu

Plantower PMS7003 to laserowy miernik pyłu zawieszonego w powietrzu, który podane pomiary PM1, PM2.5 oraz PM10. Posiada wbudowany wiatraczek. Urządzenia zasilane jest 5V i wyniki podaje poprzez UART. W naszym projekcie korzystamy z dwóch pinów Rx oraz Tx. Pozostałe dostępne w mierniku PMS7003 nie są wykorzystywane.

SCHEMAT PODŁĄCZENIA PMS7003

## Zasilanie

Miernik smogu zasilany jest poprzez kable microUSB. Układ z wtyczką microUSB umieściliśmy w jednej z gumowych zatyczek w obudowie. Przykleiliśmy go na stałe klejem na gorąco. Z układu wychodzą dwa piny: 5V oraz GND. Służą do zasilania urządzenia.

SCHEMAT ZASILANIA

## Obudowa

Obudowę miernika smogu stanowi puszka natynkowa IP55. Daje ona dobre zabezpieczenie przeciwko niekorzystnym warunkom atmosferycznym. Puszka ma wymiary 86x86mm i posiada 6 gumowych zatyczek. W miejsce dwóch z nich wkleiliśmy klejem na gorąco elementy wydrukowane na drukarce 3D. Pozwalają one na lepszy przepływ powietrza w obudowie. Można dodać do obudowy niewielki wiatraczek zasilany przez 5V.

ZDJĘCIA OBUDOWY
