# aqi.eco - opis

aqi.eco to serwis, dzięki któremu w atrakcyjny sposób wyświetlisz wyniki pomiarów czystości powietrza, zmierzone urządzeniami Smogomierz, Nettigo Air Monitor lub Luftdaten.

## Rejestracja

1.  Rejestrujemy się na [aqi.eco](https://aqi.eco/register).

2.  Po rejestracji i zalogowaniu, klikamy na [Urządzenia](https://aqi.eco/device) i znaczek [plusa](https://aqi.eco/device/create).

3.  Jako ESP8266 ID należy wpisać część numeryczną nazwy "ChipID" ze strony konfiguracyjnej czujnika (np. dla czujnika `ChipID: smogomierz-14958720` musimy wpisać 14958720).

4.  Po dodaniu czujnika otworzy się strona konfiguracyjna. Kopiujemy z niej pola "Adres serwera" i "Ścieżka".

5.  Otwieramy stronę Smogomierza i zaznaczymy:

*   Wysyłanie danych do aqi.eco: *Tak*
*   Adres serwera aqi.eco: *skopiowane z aqi.eco*
*   Ścieżka aqi.eco: *skopiowane z aqi.eco*

6.  Po zapisaniu ustawień Smogomierz zacznie wysyłać dane do aqi.eco.