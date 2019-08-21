# Smogomierz - wgrywanie pliku .bin z oprogramowaniem

Poniższy opis krok po kroku, przeprowadzi Cię przez procedurę wgrywania oprogramowania z pliku .bin na Twój mierniki smogu. Poniższy opis jest pomocny jeśli nie chcesz zmieniać przygotowanego przez nas oprogramowania dla Smogomierza. Jeśli planujesz dopisać własny kod, lub chcesz samemu skompilować plik .bin z oprogramowaniem dla Miernika zajrzyj do opisu ["Instalacja oprogramowania"](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software.md)

Pliki .bin można wgrać na płytkę ESP8266 na kilka sposobów. Jeśli masz już działający Smogomierz, wystarczy, że wejdziesz przez przeglądarkę internetową w kartę Update i wybierzesz tam plik .bin. Oprogramowanie zrobi resztę. Przeinstaluje się na nowsza wersję i uruchomi ponownie miernik smogu. Jeśli jednak wgrywasz oprogramowanie po raz pierwszy będziesz musiał skorzystać z połączenia przewodowego i podłączyć NodeMCu do portu USB w komputerze.

Najnowszą wersję oprogramowania dla Smogomierza znajdziesz zawsze [tutaj](https://github.com/hackerspace-silesia/Smogomierz/releases).

## Sterowniki

Jeśli nigdy wcześniej nie korzystaliśmy z Arduino ani ESP8266 lub podobnych rozwiązań, będziemy musieli zainstalować sterowniki, aby nasz system widział podłączoną do USB płytkę ESP8266.

NodeMCu V3 korzysta najczęściej z układu **CH340**. Sterowniki do niego można pobrać [tutaj](https://sparks.gogo.co.nz/ch340.html)(W przypadku systemu macOS, zalecamy instalację wersji 1.4 dostępnej [tutaj](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver)). 
Istnieje jednak możliwość, że Twoja płytka ma układ **CP2102**. Sterowniki do niego dostępnej są [tutaj](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers). 
Jeśli nie jesteś pewien jaki układ znajduje się na Twojej płytce NodeMCu, zobacz informacje na spodzie płytki. Powinno być tam to napisane. 
Po pobraniu i instalacji odpowiednich sterowników, powinniśmy uruchomić ponownie komputer. 


## Flasher

Nazwa brzmi trochę strasznie, ale to oprogramowanie służące do wgrywania plików .bin na płytkę ESP8266. Istnieją różne programy służące do wgrywania plików .bin. Polecamy użycie **NodeMCU PyFlasher**. To darmowy program z graficznym interfejsem użytkownika. Działa zarówno pod systemem Windows jak i macOS. Można go pobrać [tutaj](https://github.com/marcelstoer/nodemcu-pyflasher/releases). 
W przypadku Linuksa(działa również na macOS) zalecamy instalację narzędzia konsolowego: [esptool](https://github.com/espressif/esptool). 
![PyFlasher1](https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/instrukcje/screens/PyFlasher1.jpg)

W przypadku aktualizacji oprogramowania pamiętaj o tym, aby zmienić opcję "Erase flash" na "no". W przeciwnym wypadku wszystkie Twoje ustawienia zostaną skasowane. W przypadku problemów z uruchomieniem się Smogomierza po wgraniu oprogramowania, warto wgrać je jeszcze raz z włączonym wymazaniem pamięci.

## Aktualizacja oprogramowania

W celu aktualizacji oprogramowania najlepiej wejść w zakładkę Update w panelu Smogomierza. Pozwala ona na wgrywanie plików .bin z nową wersją oprogramowania dla miernika. 

Najnowszą wersję oprogramowania dla Smogomierza znajdziesz zawsze [tutaj](https://github.com/hackerspace-silesia/Smogomierz/releases).

Plik .bin można stworzyć samemu z kodu dostępnego w tym repozytorium. Do zrobienia tego potrzebne jest ArduinoIDE – [Instrukcja instalacji wymaganego oprogramowania](https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software.md). W "Plik > Preferencje" lub "Arduino > Preferences…" zaznaczamy przy "Pokaż szczegółowe informacje podczas:" opcję "Kompilacji". W czasie kompilowania naszego kodu, na dole ekranu ArduinoIDE pojawią się adresy(coś w stylu "/var/folders/k9/tbv_kmpn1dj5fl5spjk209480000gn/T/arduino_build_795090/Smogomierz.ino.bin"), gdzie zostanie zapisany plik Smogomierz.ino.bin. Plik ten to właśnie paczka z naszym oprogramowaniem, które możemy wgrać przez kartę Update w panelu Smogomierza. 
