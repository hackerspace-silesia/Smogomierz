# Smogomierz - podłączenie elektroniki

Poniższy opis krok po kroku, przeprowadzi Was przez procedurę podłączenia wszystkich czujników obsługiwanych przez nasz miernik smogu. Listę części, wraz z linkami do przykładowych sklepów, gdzie można je kupić znajdziecie pod adresem: https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/components.md

Całościowy schemat podłączenia:

![Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/schemat.png)

## ESP8266 - NodeMCU V3

Główny element miernika. To właśnie na tę płytkę wgrywamy oprogramowanie. Ma ona wbudowany moduł WiFi, który pozwala na tworzenie sieci bezprzewodowej i wysyłanie danych z pozostałych czujników.

![ESP8266 NodeMCU V3 Pinout](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/nodemcu_pins.png)
![ESP8266 calosc](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/ESP8266_Calosc.jpg)

## BME280 - miernik temperatury, wilgotności i ciśnienia powietrza

BME280 to niewielki moduł pozwalający na bardzo precyzyjne pomiary temperatury powietrza, ciśnienia oraz wilgotności powietrza. 

BME280 można podłączyć na dwa sposoby. W naszym przypadku skorzystamy z I2C. Oznacza to, że poza zasilaniem, będziemy musili jeszcze podłączyć dwa piny: SCL oraz SDA. 

BME280 zasilamy z pinu 3V na ESP8266. Podpinamy go do VIN na BME280. GND łączymy z dowolnym G na ESP8266. SCL podpinamy do pinu D4, SDA do D3 na ESP8266.

![BME280 Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/BME280_schemat.png)
![BME280](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/BME280.jpg)

## PMS7003 - laserowy miernik pyłu

Plantower PMS7003 to laserowy miernik pyłu zawieszonego w powietrzu, który podane pomiary PM1, PM2.5 oraz PM10. Posiada wbudowany wiatraczek. Urządzenia zasilane jest 5V i wyniki podaje poprzez UART. W naszym projekcie korzystamy z dwóch pinów RX oraz TX. Pozostałe dostępne w mierniku PMS7003 nie są wykorzystywane.

PMS7003 zasilamy bezpośrednio z pinu 5V wyprowadzonego z portu microUSB w obudowie urządzenia. Piny GND podpinamy do dowolnego pinu G na płytce ESP8266. Pin TX(Pin 9 w PMS7003; zielony kabelek) podpinamy do pinu D1 na płytce ESP8266. Pin RX(Pin 7 w PMS7003; niebieski kabelek) podpinamy do pinu D2 na ESP8266.

![PMS7003 Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/PMS7003_schemat.png)
![PMS7003](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/PMS7003_1.jpg)
![PMS7003](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/PMS7003_2.jpg)

## Zasilanie

Miernik smogu zasilany jest poprzez kable microUSB. Układ z wtyczką microUSB umieściliśmy w jednej z gumowych zatyczek w obudowie. Przykleiliśmy go na stałe klejem na gorąco. Z układu wychodzą dwa piny: 5V oraz GND. Służą do zasilania urządzenia.

Wszystkie elementy urządzenia zasilamy poprzez 5V. Źródłem jest pin 5V wyprowadzony z portu microUSB w obudowie urządzenia. 5V musimy doprowadzić do pinu VIN w ESP8266 i 5V w PMS7003(biały kabelek). W tym celu rozdzielamy zasilanie z jednego pinu, na dwa. BME2866 zasilamy z pinu 3V na płytce ESP8266.

Podobnie robimy z GND. Jednak w tym wypadku nie musimy ręcznie rozdzielać masy. Wystarczy ją podłączyć do pinu G na ESP8266. GND z miernika PMS7003(czarny kabelek) podpinamy do G na ESP8266. GND z BME280 również łączymy z G na płytce ESP8266.

![Zasilanie Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/Zasilanie_Schemat.png)

## Obudowa

Obudowę miernika smogu stanowi puszka natynkowa IP55. Daje ona dobre zabezpieczenie przeciwko niekorzystnym warunkom atmosferycznym. Puszka ma wymiary 86x86mm i posiada 6 gumowych zatyczek. W miejsce dwóch z nich wkleiliśmy klejem na gorąco elementy wydrukowane na drukarce 3D. Pozwalają one na lepszy przepływ powietrza w obudowie. Można dodać do obudowy niewielki wiatraczek zasilany przez 5V.

![Obudowa](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Obudowa2.jpg)
![Zasilanie_obudowa1](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Zasilanie_obudowa1.jpg)

## Połączenie całości

Zgodnie ze schmeatem na początku instrukcji, łączymy wszystkie elementy ze sobą. Z **BME280** pin VIN podpinamy do 3V na ESP8266; GND do G; SCL do D4; SDA do D3. W przypadku **PMS7003** kable 5V/Biały łączymy z płytką z 3 pinami(pozostałe podpinamy do VIN na ESP8266 oraz VBUS w obudowie miernika); Czarny/GND do G na ESP8266; Zielony/TX do D1; Niebieski/RX do D2. Na koniec pin G z obudowy miernika wpinamy do G na ESP8266.

![ESP8266_polaczenia](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/ESP8266_polaczenia.jpg)
![Calosc2](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Calosc2.jpg)

## Umiejscowienie podzespołów w obudowie

Obudowa jest dosyć mała, ale wszystkie elementy zmieszczą się w niej bez problemu. Musimy pamiętać, aby wyloty z mietnika PMS7003 umiejscowić jak najbliżej otworów wentylacyjnych w obudowie. Pozwoli to na lepszy przepływ powietrza i dokładniejsze pomiary.

![Wsordku1](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Wsordku1.jpg)
![Wsordku2](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Wsordku2.jpg)
![Wsordku3](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Wsordku3.jpg)
