# Smogomierz - podłączenie elektroniki

Poniższy opis krok po kroku, przeprowadzi Cię przez procedurę podłączenia wszystkich czujników obsługiwanych przez nasz miernik smogu. Listę części, wraz z linkami do przykładowych sklepów, gdzie można je kupić znajdziesz pod adresem: https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/components.md

Jeśli masz już wszystkie części, to pamiętaj, że kilka z nich wymaga przylutowania pinów. Opis wszystkiego co trzeba zlutować wraz ze zdjęciami znajdziesz w instrukcji ["Lutowanie"](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/soldering.md).

Całościowy schemat podłączenia:

![Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/schemat.png)

## ESP8266 - NodeMCU V3

Główny element miernika. To właśnie na tę płytkę wgrywamy oprogramowanie. Ma ona wbudowany moduł WiFi, który pozwala na tworzenie sieci bezprzewodowej i wysyłanie danych z pozostałych czujników. Wszystkie piny GND na płytce są ze sobą połączone i można z nich korzystać zamiennie. Podobnie z pinami 3V.

![ESP8266 NodeMCU V3 Pinout](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/nodemcu_pins.png)

## BME280 - miernik temperatury, wilgotności i ciśnienia powietrza

BME280 to niewielki moduł pozwalający na bardzo precyzyjne pomiary temperatury powietrza, ciśnienia oraz wilgotności powietrza. 

BME280 można podłączyć na dwa sposoby. W naszym przypadku skorzystamy z I2C. Oznacza to, że poza zasilaniem, będziemy musieli jeszcze podłączyć dwa piny: SCL oraz SDA. 

BME280 występuje również w wersji **Adafruit**. Charakteryzuje się ona [niebieską płytką](https://www.adafruit.com/product/2652). W przypadku tej wersji BME280 musimy podłączyć SCK tam gdzie pin SCL(do D4) i SDI tam gdzie SDA(do D3) oraz SDO do dowolnego pinu GND na naszym ESP8266.

BME280 zasilamy z pinu 3V na ESP8266. Podpinamy go do VIN na BME280. GND łączymy z dowolnym G(GND) na ESP8266. SCL podpinamy do pinu D4, SDA do D3 na ESP8266.

![BME280 Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/BME280_schemat.png)
![BME280](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/BME280.jpg)

## SHT1x - miernik temperatury i wilgotności powietrza

SHT1x to niewielki moduł pozwalający na bardzo precyzyjne pomiary temperatury oraz wilgotności powietrza. 

SHT1x zasilamy z pinu 3V na ESP8266. Podpinamy go do VCC na SHT1x. GND łączymy z dowolnym G(GND) na ESP8266. CLOCK/SCL podpinamy do pinu D6, DATA/SDA do D5 na ESP8266. Pomiędzy DATA/SDA a VCC musimy umieścić jeszcze rezystor 10k (pull-up).

## HTU21D - miernik temperatury i wilgotności powietrza

Jeśli zamiast **BME280** chcesz użyć HTU21D. Podłącz go w następujący sposób: 
VIN z HTU21D do pinu 3V na ESP8266. GND łączymy z dowolnym G(GND) na ESP8266. SCL podpinamy do pinu D5, SDA do D6 na ESP8266.

## DHT22 - miernik temperatury i wilgotności powietrza

Jeśli zamiast **BME280** chcesz użyć DHT22. Podłącz go w następujący sposób: 
VIN z HTU21D do pinu 3V na ESP8266. GND łączymy z dowolnym G(GND) na ESP8266. Pin z danymi pomiarowymi podepnij do D7 na płytce ESP8266.

## PMS7003 - laserowy miernik pyłu

Plantower PMS7003 to laserowy miernik pyłu zawieszonego w powietrzu, który podane pomiary PM1, PM2.5 oraz PM10. Posiada wbudowany wiatraczek. Urządzenia zasilane jest 5V i wyniki podaje poprzez UART. W naszym projekcie korzystamy z dwóch pinów RX oraz TX do pobierania wyników pomiarów oraz z pinu 5V i GND do zasilania PMS7003. Pozostałe dostępne w mierniku PMS7003 nie będą przez nas wykorzystywane.

PMS7003 zasilamy bezpośrednio z pinu 5V wyprowadzonego z portu microUSB w obudowie urządzenia. Piny GND podpinamy do dowolnego pinu G(GND) na płytce ESP8266. Pin TX(Pin 9 w PMS7003) podpinamy do pinu D1 na płytce ESP8266. Pin RX(Pin 7 w PMS7003) podpinamy do pinu D2 na ESP8266.

![PMS7003 Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/PMS7003_schemat.png)

## HPMA115S0

Honeywell HPMA115S0 to laserowy miernik pyłu zawieszonego w powietrzu, który podane pomiary PM1, PM2.5 oraz PM10. Posiada wbudowany wiatraczek. Urządzenia zasilane jest 5V i wyniki podaje poprzez UART. W naszym projekcie korzystamy z dwóch pinów RX oraz TX do pobierania wyników pomiarów oraz z pinu 5V i GND do zasilania HPMA115S0. Pozostałe dostępne w mierniku HPMA115S0 nie będą przez nas wykorzystywane.

## SDS011

SDS011 to laserowy miernik pyłu zawieszonego w powietrzu, który podane pomiary PM2.5 oraz PM10. Posiada wbudowany wiatraczek. Urządzenia zasilane jest 5V i wyniki podaje poprzez UART. W naszym projekcie korzystamy z dwóch pinów RX oraz TX do pobierania wyników pomiarów oraz z pinu 5V i GND do zasilania SDS011. Pozostałe dostępne w mierniku SDS011 nie będą przez nas wykorzystywane.

## Obudowa

Na obudowę Smogomierza wybraliśmy puszkę IP67 o wymiarach 118 x 78 x 55 mm. Oferuje ona dobre zabezpieczenie przeciwko niekorzystnym warunkom atmosferycznym. W obudowie Smogomierza istotne są dwie rzeczy. Ochrona przed deszczem oraz swobodny przepływ powietrza. Jeśli chcesz, możesz zastosować jakieś inne rozwiązanie, które spełni te dwa cele.

W przypadku wybranej przez nas obudowy. Musimy wykonać w niej 3 otwory. Dwa większe, które umożliwią przepływ powietrza, oraz jeden mniejszy w celu podłączenia zasilania. Zalecamy wykonanie otworów wentylacyjnych na dłuższym boku obudowy. Otwór na zasilanie możemy umieścić z dowolnej strony. Otwory wentylacyjne najwygodniej wykonać przy użyciu wiertarki i wiertła 10 lub większego. Wykonujemy dwa odwierty obok siebie i następnie wycinany je nożem do tapet w kształt kwadratu. Jeśli chodzi o zasilanie, to wystarczy jeden odwiert wiertłem 10. 

![Obudowa_wyciecie](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Obudowa_wyciecie.jpg)

Na otwory wentylacyjne wklejamy klejem na gorąco [kratki](https://github.com/hackerspace-silesia/Smogomierz/blob/master/80x80x40mm.stl) wydrukowane na drukarce 3D. Pozwalają one na lepszy przepływ powietrza w obudowie. Natomiast wewnątrz obudowy w miejscu otworu na zasilanie wklejamy wtyczkę microUSB(z wlutowanymi wcześniej pinami).

![Obudowa_sklejona](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Obudowa_sklejona.jpg)
![Obudowa_sklejona_srodek](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Obudowa_sklejona_srodek.jpg)

## Zasilanie

Miernik smogu zasilany jest poprzez kable microUSB. Układ z wtyczką microUSB wkleiliśmy klejem na gorąco w obudowę. Z układu wychodzą dwa potrzebne nam piny: 5V oraz GND. Służą do zasilania całego Smogomierza.

ESP8266 NodeMCu oraz miernik pyłu PM2.5 i PM10 zasilamy poprzez 5V. Źródłem jest pin 5V wyprowadzony z portu microUSB w obudowie urządzenia. 5V musimy doprowadzić do pinu VIN w ESP8266 i 5V w mierniku pyłu(VCC). W tym celu rozdzielamy zasilanie z jednego pinu, na dwa. Mirnik temperatury i wilgotności zasilamy z pinu 3V na płytce ESP8266. Na koniec nasz rozdzielacz zasilania 5V najlepiej zabezpieczyć kawałkiem taśmy izolacyjnej.

![5V_Tasma](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/5V_Tasma.jpg)

Podobnie robimy z GND. Jednak w tym wypadku nie musimy ręcznie rozdzielać masy. Wystarczy ją podłączyć do pinu G na ESP8266. GND z miernika pyłu podpinamy do G na ESP8266. GND z miernika temperatury i wilgotności również łączymy z G na płytce ESP8266. Wszystkie masy(G/GND) są ze sobą połączone. Tylko od ciebie zależy, do którego pinu G na ESP8266 podłączysz pin G z miernika pyłu czy miernika temperatury.

![Zasilanie Schemat](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/Zasilanie_Schemat.png)

## Połączenie całości

Zgodnie ze schematem na początku instrukcji, łączymy wszystkie elementy ze sobą. Z **BME280** pin VIN podpinamy do 3V na ESP8266; GND do G; SCL do D4; SDA do D3. W przypadku **PMS7003** kable 5V z płytką z 3 pinami(pozostałe podpinamy do VIN na ESP8266 oraz VBUS w obudowie miernika); GND do G na ESP8266; TX do D1; RX do D2. Na koniec pin G z obudowy miernika wpinamy do G na ESP8266.

![ESP8266_polaczenia](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/ESP8266_polaczenia.jpg)

## Umiejscowienie podzespołów w obudowie

Jeśli postanowiliśmy użyć jako miernika pyłu modelu SDS011 to obudowa może wydawać się dość mała, ale wszystkie elementy zmieszczą się w niej bez problemu. Jeśli korzystamy z PMS7003 lub HPMA115S0 mamy nadmiar miejsca. Musimy pamiętać, aby wloty z miernika pyłu umiejscowić jak najbliżej otworów wentylacyjnych w obudowie. Pozwoli to na lepszy przepływ powietrza i dokładniejsze pomiary. Polecam ustawienie mierników w obudowie w taki sposób, aby wiatraczkiem z miernika pyłu znajdował się obok otworu wentylacyjnego. 

![Calosc_PMS5003](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Calosc_PMS5003.jpg)
![Calosc_SDS](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/photos/Calosc_SDS.jpg) 
