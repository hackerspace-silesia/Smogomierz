# Smogomierz - instalacja orpgoramowania

Poniższy opis krok po kroku, przeprowadzi Was przez procedurę instalacji oprogramowania wymaganego do wgrania nowego systemu na mierniki smogu. Całe oprogramowanie jest w pełni darmowe, a jego pobranie nie wymaga żadnej rejestracji i podawania swoich danych.

## Arduino - instalacja

Na początku musimy pobrać program ArduinoIDE. Pozwala on na programowanie płytek z rodziny Arduino. W późniejszym kroku, dodamy do niego obsługę płytki ESP8266. To właśnie na nich oparte są nasze mierniki smogu.

1. Pobieramy oprogramowanie ze strony https://www.arduino.cc/en/Main/Software. W tym celu musimy wybrać jaki system operacyjny mamy zainstalowany na komputerze, a następnie kliknąć na napis "Download the Arduino IDE".
![ArduinoIDE1](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/screens/ArduinoIDE1.jpg)

2. Klikamy napis "JUST DOWNLOAD". W tym momencie rozpocznie się pobieranie instalatora ArduinoIDE.
![ArduinoIDE2](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/screens/ArduinoIDE2.jpg)

3. Instalujemy pobrany program. W przypadku Windowsa klikamy na niego 2 razy i instalujemy. W systemie macOS przenosimy do foldery Aplikacje. Na Linuksie uruchamiamy skrypt install.sh.

## Arduino - konfiguracja

Po zainstalowaniu ArduinoIDE, musimy dodać w nim obsługę płytek ESP8266.

1. Uruchamiamy aplikację ArduinoIDE i wybieramy "Plik > Ustawienia" lub "Arduino > Preferences…" w przypadku systemu macOS.
![ArduinoIDE3_1](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/screens/ArduinoIDE3_1.jpg)

2. W okienku „Dodatkowe adresy URL do menedżera płytek:” wpisujemy adres: http://arduino.esp8266.com/stable/package_esp8266com_index.json
![ArduinoIDE3_2](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/screens/ArduinoIDE3_2.jpg)

3. Po dodaniu adresu, skąd program może pobrać informacje o nowych płytkach, musimy je jeszcze pobrać. W tym celu klikamy "Narzędzia > Płytka > Menedżer płytek…".
![ArduinoIDE3_3](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/screens/ArduinoIDE3_3.jpg)

4. W wyszukiwarce wpisujemy frazę: "esp8266" i instalujemy nowe płytki.
![ArduinoIDE4](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/screens/ArduinoIDE4.jpg)

5. Teraz pozostaje nam tylko ustawić w ArduinoIDE, że nasz kod ma być dostosowany do płytek ESP8266, a nie standardowego Arduino. W tym celu musimy wybrać "Narzędzia > Płytka > NodeMCU 1.0 (ESP-12E Module)". 
![ArduinoIDE5](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/screens/ArduinoIDE5.jpg)

6. Na koniec pozostaje nam wybranie portu szeregowego, go którego podłączona jest nasza płytka. W tym celu podpinamy ESP8266 do portu USB w komputerze i wybieramy "Narzędzia > Port > COM3". W przypadku systemu innego niż Windows, zamiast "COM3" będziemy mieć inną opcję do wyboru np. "/dev/cu.wchusbserial1410". 
![ArduinoIDE6](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/screens/ArduinoIDE6.jpg)

7. Teraz wszystko jest już gotowe do obsługi naszych mierników smogu. Możemy przejść do pobrania oprogramowania i sprawdzenia czy działa.

## Smogomierz - pobranie i wgranie oprogramowania na ESP8266

1. Wchodzimy na stronę projektu Smogomierz w serwisie github i pobieramy pliki: https://github.com/hackerspace-silesia/Smogomierz/tree/Arduino
![Github1](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/screens/Github1.jpg)

2. Rozpakowujemy pobraną paczkę i otwieramy plik Smogomierz.ino.

3. Otworzy się nam okienko programu ArduinoIDE z kodem naszej aplikacji Smogomierza.  
![ArduinoIDE7](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/Arduino/screens/ArduinoIDE7.jpg)

1 – Sprawdzenie poprawności oprogramowania

2 – Wgranie oprogramowania na płytkę

3 – Postęp weryfikacji/wgrywania oprogramowania i ewentualne błędy

4 – Płytka oraz port, które mamy ustawione
