# Smogomierz - Components / Podzespoły

Wszystkie potrzebne części możesz zamówić w sklepie internetowym **Botland**. Oferuje on gotowe zestawy – https://botland.com.pl/pl/czujniki-czystosci-powietrza/13434-zestaw-diy-do-budowy-miernika-smogu-czujnik-czystosci-powietrza-pm25-i-pm10.html

Jeśli jednak wolisz kupić wszystkie części samodzielnie, to poniżej znajdziesz listę potrzebnych elementów. Podane linki to przykładowe miejsca, w których można kupić dany produkt. Każdą z wymienionych części można kupić w dowolnym sklepie. Zastosowane elementy to łatwo dostępne podzespoły, które znajdziesz w większości sklepów z elektroniką. 

## Układ główny
Główny układ sterujący Smogomierzem to ESP8266 w wersji ESP-12. Zalecamy użycie **ESP8266 – NodeMCu V3**, jednak bez problemu oprogramowanie będzie działać np. na WeMos D1.

+ **ESP8266 – NodeMCu V3** – https://www.aliexpress.com/item/V3-Wireless-module-NodeMcu-4M-bytes-Lua-WIFI-Internet-of-Things-development-board-based-ESP8266-esp/32647542733.html

## Mierniki PM2.5 i PM10

Oprogramowanie Smogomierza wspiera obecnie kilka różnych modeli mierników pyłu zawieszonego w powietrzu. Zalecamy użycie miernika **PMS7003**. Lista obsługiwanych modeli znajduje się poniżej:

+ **PMS5003/PMS7003** – Laserowy miernik pyłu PM1, PM2.5 i PM10. Polecam zamówić wersję kablem, bo bezpośrednio do PMS7003 praktycznie nie da się przylutować/podłaczyć przewodów. Przykładowy model z kablem – https://www.aliexpress.com/item/1-set-Laser-PM2-5-PMS7003-G7-High-precision-laser-dust-concentration-sensor-digital-dust-particles/32826370928.html 

+ **Honeywell HPMA115S0** – Laserowy miernik pyłu PM1, PM2.5 i PM10 – https://www.tme.eu/pl/details/hpma115s0-xxx/czujniki-gazu/honeywell/

+ **SDS011** – Laserowy miernik pyłu PM2.5 i PM10 – https://www.aliexpress.com/item/Nova-PM-sensor-SDS011-High-precision-laser-pm2-5-air-quality-detection-sensor-module-Super-dust/32606349048.html

## Mierniki temperatury, wilgotności i ciśnienia

Smogomierza wspiera obecnie kilka różnych modeli mierników temperatury, wilgotności i ciśnienia powietrza. Najistotniejszym elementem jest tutaj pomiar wilgotności powietrza. Wilgotność wpływa na pomiary pyłu PM2.5 oraz PM10(przy wilgotności >60% mierniki zaczynają wykrywać parę wodną jako zanieczyszczenia powietrza). Smogomierz pozwala na automatyczną kalibrację pomiarów pyłu zawieszonego, jednak potrzebuje do tego pomiarów wilgotności powietrza. Zalecamy użycie miernika **BME280**. Lista obsługiwanych modeli znajduje się poniżej:

+ **BME280** – Miernik temperatury, wilgotności i ciśnienia powietrza – https://www.aliexpress.com/item/High-Accuracy-BME280-Digital-Sensor-Temperature-Humidity-Barometric-Pressure-Sensor-Module-GY-BME280-I2C-SPI-1/32672210336.html 

+ **HTU21D** – Miernik temperatury i wilgotności powietrza – https://www.aliexpress.com/item/New-HTU21D-Temperature-and-Humidity-Sensor-Module/32476325114.html

+ **DHT22** – Miernik temperatury i wilgotności powietrza – https://www.aliexpress.com/item/DHT22-single-bus-digital-temperature-and-humidity-sensor-module-2302-electronic-building-blocks/32753887461.html

## Obudowa

Do wyboru mamy kilka możliwości. Polecamy zastosowanie modelu **IP55**. Obudowa chroni elektronikę przed bezpośrednim kontaktem z wodą. Musi umożliwiać swobodny przepływ powietrza, ale jednocześnie chronić przez deszczem. Jeśli chcesz możesz użyć jakiejś zupełnie innej obudowy, pamiętaj jednak do przepływie powietrza oraz wymiarach. Na wszystkie elementy Smogomierza potrzebne jest około 120 x 80 x 50 mm. Zwłaszcza jeśli postanowiłeś użyć miernika SDS011. 

+ **Obudowa IP55**, 118 x 78 x 55 mm – https://botland.com.pl/pl/obudowy/6235-obudowa-plastikowa-kradex-z57j-118x78x55mm-jasna.html?search_query=z57j&results=2

+ **Obudowa IP67**, 118 x 78 x 55 mm – https://botland.com.pl/pl/obudowy/9051-obudowa-plastikowa-kradex-z57js-abs-z-uszczelka-i-mosieznymi-tulejkami-ip67-118x78x55mm-jasna.html?search_query=Z57J&results=2

+ **Obudowa IP56**, 120 x 80 x 50 mm – http://allegro.pl/puszka-hermetyczna-ip56-120x80x50mm-przezroczysta-i6661288346.html

	Opcjonalne, choć bardzo mocno zalecane jest użycie dwóch elementów chroniących otwory umożliwiające przepływ powietrza przez obudowę. Elementy te można wydrukować na drukarce 3D / Two 3D printed parts – https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/80x80x40mm.stl

## Pozostałe części

Elementy potrzebne do podłączenia całości. 

+ **Wtyczka** – microUSB – https://www.aliexpress.com/item/1pcs-MICRO-USB-to-DIP-Adapter-5pin-female-connector-B-type-pcb-converter-pinboard-2-54/32666589270.html 

+ **Przewody** do połączenia(żeńskie/female) wszystkich elementów – https://www.aliexpress.com/item/Free-Shipping-80pcs-dupont-cable-jumper-wire-dupont-line-female-to-female-dupont-line-20cm-1P/701588771.html 

+ **Goldpiny** – http://allegro.pl/wtyk-szpilkowy-goldpin-arduino-avr-pic-80szt-fv-i6846674238.html 

## Narzędzia

Lutownica(wraz z cyną) oraz pistolet do kleju na gorąco, wiertarka oraz nóż do tapet i śrubokręt do skręcenia obudowy. 

Jeśli masz już wszystkie części, to pamiętaj, że kilka z nich wymaga przylutowania pinów. Opis wszystkiego co trzeba zlutować wraz ze zdjęciami znajdziesz w instrukcji ["Lutowanie"](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/soldering.md). 
