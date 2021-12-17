/*
 * 
 *  translation file - polish translations
 * 
 */

#pragma once
#ifdef ARDUINO_ARCH_ESP8266
#include <avr/pgmspace.h>
#endif

char PL_INTL_LANG[6] PROGMEM = "pl";

char PL_INTL_INDEX_PAGE[16] PROGMEM = "Pomiary";
char PL_INTL_CONFIG_PAGE[14] PROGMEM = "Ustawienia";

char PL_INTL_CONFIG_DEVICE_PAGE[24] PROGMEM = "Ustawienia Urządzenia";
char PL_INTL_CONFIG_INFO_DEVICE_PAGE[144] PROGMEM = "Wybór czujników podłączonych do Smogly, częstotliwość wykonywania pomiarów, czy język interfejsu można zmienić w sekcji";
char PL_INTL_CONFIG_SERVICES_PAGE[24] PROGMEM = "Ustawienia Serwisów";
char PL_INTL_CONFIG_INFO_SERVICES_PAGE[144] PROGMEM = "Ustawienia takich serwisów jak Sensor.Community, aqi.eco, ThingSpeak, AirMonitor czy usług InfluxDB lub MQTT można zmienić w sekcji";

char PL_INTL_UPDATE_PAGE[14] PROGMEM = "Aktualizacje";

char PL_INTL_WEATHER[10] PROGMEM = "Pogoda";
char PL_INTL_TEMPERATURE[14] PROGMEM = "Temperatura";
char PL_INTL_HUMIDITY[14] PROGMEM = "Wilgotność";
char PL_INTL_PRESSURE[14] PROGMEM = "Ciśnienie";
char PL_INTL_DEWPOINT[14] PROGMEM = "Punkt rosy";

char PL_INTL_AIRPOLLUTION[26] PROGMEM = "Pomiary zanieczyszczeń";
char PL_INTL_SAVED[10] PROGMEM = "ZAPISANO";

char PL_INTL_POSTCONFIG_INFO[60] PROGMEM = "wszystko wygląda OK, za chwilę nastąpi restart Smogly";
char PL_INTL_INSTRUCIONSLINK[76] PROGMEM = "Wszystkie instrukcje i opisy dostępne są {GITHUB_LINK}.";
char PL_INTL_DEVICENAME[20] PROGMEM = "Nazwa urządzenia";
char PL_INTL_DEVICENAMEAUTO[32] PROGMEM = "Automatyczne generowanie nazwy";
char PL_INTL_SELECTEDLANGUAGE[10] PROGMEM = "Język";
char PL_INTL_TEMPHUMIPRESSSENSOR[40] PROGMEM = "Miernik Temp/Wilgotności/Ciśnienia";
char PL_INTL_PMSENSOR[20] PROGMEM = "Miernik PM2.5/PM10";

char PL_INTL_FIRST_THP_SDA_SCL[40] PROGMEM = "Piny miernika Temp/Wilgotności";
char PL_INTL_FIRST_THP_SDA[8] PROGMEM = "Pin SDA";
char PL_INTL_FIRST_THP_SCL[8] PROGMEM = "Pin SCL";

char PL_INTL_SECOND_THP[40] PROGMEM = "Włacz drugi miernik Temp/Wilgotności";

char PL_INTL_SECOND_THP_SDA_SCL[48] PROGMEM = "Piny drugiego miernika Temp/Wilgotności";
char PL_INTL_SECOND_THP_SDA[8] PROGMEM = "Pin SDA";
char PL_INTL_SECOND_THP_SCL[8] PROGMEM = "Pin SCL";

char PL_INTL_DUST_TX_RX[32] PROGMEM = "Piny miernika PM2.5/PM10";
char PL_INTL_DUST_TX[8] PROGMEM = "Pin TX";
char PL_INTL_DUST_RX[8] PROGMEM = "Pin RX";

char PL_INTL_FREQUENTMEASUREMENTONOFF[26] PROGMEM = "Ciągłe pomiary PM";
char PL_INTL_FREQUENTMEASUREMENTINFO[80] PROGMEM = "Ciągłe pomiary znacznie skracają żywotność miernika PM.";
char PL_INTL_MEASUREMENTFREQUENCY[30] PROGMEM = "Wykonywanie pomiarów PM co";
char PL_INTL_AVERAGELASTRESULT[30] PROGMEM = "Uśredniaj wynik z ostatnich";
char PL_INTL_PMMEASUREMENTS[18] PROGMEM = "pomiarów PM";
char PL_INTL_SENDINGINTERVAL[50] PROGMEM = "Wysyłanie danych do serwisów zewnętrznych co";
char PL_INTL_SERVICESSENDINGINTERVAL[64] PROGMEM = "Wysyłanie danych do Sensor.Community/aqi.eco/AirMonitor co";
char PL_INTL_DBSENDINGINTERVAL[50] PROGMEM = "Wysyłanie danych do ThingSpeak/InfluxDB/MQTT co";
char PL_INTL_SECONDS[10] PROGMEM = "sekund";
char PL_INTL_MINUTES[10] PROGMEM = "minut";

char PL_INTL_DEEPSLEEPINFO[540] PROGMEM = "DeepSleep wprowadza urządzenie w głębokie uśpienie pomiędzy seriami pomiarów - znacznie niższe zużycie energii(możliwość pracy na baterii przez kilka tygodni), ale brak ciągłego dostępu do interfejsu WWW. Interfejs WWW będzie dostępny tylko przez około {INTERFACEWWWONTIME} sekund co {SENDING_FREQUENCY} minut(oraz zaraz po restarcie urządzenia). Czas uśpienia będzie taki sam jak okres wysyłania pomiarów do serwisów zewnętrznych. Wymaga połączenia pinów D0 oraz RST na ESP8266!";
char PL_INTL_DEEPSLEEPINFO_ESP32[476] PROGMEM = "DeepSleep wprowadza urządzenie w głębokie uśpienie pomiędzy seriami pomiarów - znacznie niższe zużycie energii(możliwość pracy na baterii przez kilka tygodni), ale brak ciągłego dostępu do interfejsu WWW. Interfejs WWW będzie dostępny tylko przez około {INTERFACEWWWONTIME} sekund co {SENDING_FREQUENCY} minut(oraz zaraz po restarcie urządzenia). Czas uśpienia będzie taki sam jak okres wysyłania pomiarów do serwisów zewnętrznych.";

char PL_INTL_DISPLAYPM1[30] PROGMEM = "Wyświetlanie pomiarów PM1";
char PL_INTL_ALTITUDEINFO[116] PROGMEM = "Wysokość(wymagana do poprawnych pomiarów ciśnienia. Można sprawdzić {WSPOLRZEDNE_GPS_LINK})";

char PL_INTL_SECURECONFIGUPDATEPAGE[84] PROGMEM = "Wymagaj hasła dla strony Konfiguracyjnej oraz Update(domyślne: admin/password)";
char PL_INTL_SECURELOGIN[8] PROGMEM = "Login";
char PL_INTL_SECUREPASSWD[10] PROGMEM = "Hasło";
char PL_INTL_SECURELOGOUTINFO[48] PROGMEM = "Zrestartuj przeglądarkę w celu wylogowania!";

char PL_INTL_LUFTDATENSENDING[96] PROGMEM = "Wysyłanie danych do serwisu {LUFTDATEN_LINK} (wymaga rejestracji {LUFTDATENFORM_LINK})";
char PL_INTL_LUFTDATEN_GRAPH_ON[64] = "Wyświetlanie wykresów z serwisu Sensor.Community";

char PL_INTL_SMOGLISTSENDING[48] PROGMEM = "Wysyłanie danych do serwisu {SMOGLIST_LINK}";
char PL_INTL_SMOGLISTINFO[242] PROGMEM = "Smoglist to serwis zbierający statystyki użytkowania Smogly(nie zbieramy loginów i haseł). Smoglist pozwoli nam w przyszłości ulepszyć najcześciej używane funkcje i generować własne wykresy.";

char PL_INTL_AIRMONITORSENDING[128] PROGMEM = "Wysyłanie danych do serwisu {AIRMONITOR_LINK} (wymaga wypełnienia {AIRMONITORFORM_LINK}; Sensor: np. {PMSENSORMODEL})";
char PL_INTL_AIRMONITORCHARTS[48] PROGMEM = "Wyświetlanie wykresów z serwisu AirMonitor";

char PL_INTL_MY_COORDINATESINFO[74] PROGMEM = "Współrzędne geograficzne miernika(można sprawdzić {LATLONG_LINK})";
char PL_INTL_MY_LATITUDE[24] PROGMEM = "Szerokość(latitude)";
char PL_INTL_MY_LONGITUDE[24] PROGMEM = "Długość(longitude)";

char PL_INTL_THINGSPEAKSENDING[50] PROGMEM = "Wysyłanie danych do serwisu {THINGSPEAK_LINK}";
char PL_INTL_THINGSPEAKCHARTS[48] PROGMEM = "Wyświetlanie wykresów z serwisu ThingSpeak";
char PL_INTL_THINGSPEAK_WRITE_API_KEY[28] PROGMEM = "ThingSpeak Write API Key";
char PL_INTL_THINGSPEAKCHANNELID[24] PROGMEM = "ThingSpeak Channel ID";
char PL_INTL_THINGSPEAK_READ_API_KEY[28] PROGMEM = "ThingSpeak Read API Key";

char PL_INTL_INFLUXDBSENDING[32] PROGMEM = "Wysyłanie danych do InfluxDB";
char PL_INTL_INFLUXDBVERSION[18] PROGMEM = "Wersja InfluxDB";
char PL_INTL_INFLUXDBSERVER[30] PROGMEM = "Adres bazy danych InfluxDB";
char PL_INTL_INFLUXDBPORT[16] PROGMEM = "Port InfluxDB";
char PL_INTL_INFLUXDBNAME[24] PROGMEM = "Nazwa bazy danych";
char PL_INTL_INFLUXDBUSER[28] PROGMEM = "Użytkownik bazy danych";
char PL_INTL_INFLUXDBPASSWD[24] PROGMEM = "Hasło do bazy danych";
char PL_INTL_INFLUXDBORG[16] PROGMEM = "Organizacja";
char PL_INTL_INFLUXDBBUCKET[10] PROGMEM = "Bucket";
char PL_INTL_INFLUXDBTOKEN[8] PROGMEM = "Token";

char PL_INTL_CONFIG_ADV_MQTT[22] PROGMEM = "Ustawienia MQTT";
char PL_INTL_MQTTSENDING[34] PROGMEM = "Wysyłanie danych poprzez MQTT";
char PL_INTL_MQTTSERVER[22] PROGMEM = "Adres serwera MQTT";
char PL_INTL_MQTTPORT[12] PROGMEM = "Port MQTT";
char PL_INTL_MQTTUSER[20] PROGMEM = "Użytkownik MQTT";
char PL_INTL_MQTTPASSWD[16] PROGMEM = "Hasło MQTT";

char PL_INTL_MQTT_IP_IN_TOPIC[22] PROGMEM = "Dodać Adres IP";
char PL_INTL_MQTT_DEVICENAME_IN_TOPIC[30] PROGMEM = "Dodać Nazwę urządzenia";
char PL_INTL_MQTT_TOPIC_INFO[256] PROGMEM = "Poniżej możesz sformatować sposób w jaki będą wysyłane dane MQTT. Masz możliwość dodania 'Adresu IP' oraz 'Nazwy urządzenia' na początku. Środkową część możesz zdefiniować samemu dla każdej z wartości mierzonych przez urządzenie.";
char PL_INTL_MQTT_SLASH_AT_THE_BEGINNING[40] PROGMEM = "Dodać '/' na początku";
char PL_INTL_MQTT_SLASH_AT_THE_END[36] PROGMEM = "Dodać '/' na końcu";

char PL_INTL_AQIECOSENDING[100] PROGMEM = "Wysyłanie danych do <a title='aqi.eco' href='https://aqi.eco' target='_blank'>aqi.eco</a>";
char PL_INTL_AQIECOSERVER[26] PROGMEM = "Adres serwera aqi.eco";
char PL_INTL_AQIECOPATH[20] PROGMEM = "Ścieżka aqi.eco";

char PL_INTL_CALIBMETHOD[22] PROGMEM = "Sposób kalibracji";
char PL_INTL_CALIB1[30] PROGMEM = "Zmienna kalibracyjna calib1";
char PL_INTL_CALIB2[30] PROGMEM = "Zmienna kalibracyjna calib2";
char PL_INTL_SOFTWATEVERSION[24] PROGMEM = "Wersja oprogramowania";

char PL_INTL_ERASEWIFICONFIG[22] PROGMEM = "Zapomnij sieć WiFi";
char PL_INTL_RESTORESETTINGS[32] PROGMEM = "Przywróć ustawienia domyślne";
char PL_INTL_SAVE[8] PROGMEM = "Zapisz";
char PL_INTL_YES[6] PROGMEM = "Tak";
char PL_INTL_NO[6] PROGMEM = "Nie";
char PL_INTL_WITHOUTSENSOR[16] PROGMEM = "Bez miernika";
char PL_INTL_WITHOUTCALIBRATION[22] PROGMEM = "Bez kalibracji";
char PL_INTL_AUTOMATICCALIBRATION[26] PROGMEM = "Automatyczna kalibracja";

char PL_INTL_INTL_EN[12] PROGMEM = "angielski";
char PL_INTL_INTL_PL[8] PROGMEM = "polski";

char PL_INTL_FWUPDATEAVALIBLE[42] PROGMEM = "Dostępna aktualizacja oprogramowania!";
char PL_INTL_AUTOUPDATEON[28] PROGMEM = "Automatyczne Aktualizacje";
char PL_INTL_MANUALUPDATEBUTTON[22] PROGMEM = "Ręczna Aktualizacja";
char PL_INTL_FWUPDATEBUTTON[32] PROGMEM = "Automatycznie - tylko tym razem";
char PL_INTL_AUTOUPDATEONBUTTON[36] PROGMEM = "Automatycznie - zapamiętaj wybór";
char PL_INTL_AUTOUPDATEWARNING[650] PROGMEM = "Automatyczne aktualizacje wykonywane są przez nieszyfrowane połaczenie(HTTP, a nie HTTPS). Może to być niebezpieczne i umożliwić przechwycenie pakietów oraz włamanie na urządzenie. Możesz skorzystać z Automatycznej aktualizacji jednorazowo, klikając przycisk \"{TEXT_FWUPDATEBUTTON}\", lub włączyć Automatyczne aktualizacje na stałe. W każdej chwili możesz zmienić swoje zdanie i wyłączyć Automatyczne Aktualizacje w Konfiguracji. Po wybraniu \"{TEXT_FWUPDATEBUTTON}\" lub włączeniu automatycznych aktualizacji urządzenie zapisze bieżącą konfigurację i uruchomi się ponownie. Może to potrwać kilkanaście sekund.";
char PL_INTL_UPDATEPAGEAUTOUPDATEWARNING[192] PROGMEM = "Automatyczne aktualizacje wykonywane są przez nieszyfrowane połaczenie(HTTP, a nie HTTPS). Może to być niebezpieczne i umożliwić przechwycenie pakietów oraz włamanie na urządzenie.";

char PL_INTL_SELECTUPDATEFILE[20] PROGMEM = "Wybierz plik .bin";
char PL_INTL_SUBMITUPDATE[10] PROGMEM = "Update!";
char PL_INTL_CURRENTSOFTVERSION[40] PROGMEM = "Używana wersja oprogramowania";
char PL_INTL_SERVERSOFTWAREVERSION[46] PROGMEM = "Najnowsza dostępna wersja oprogramowania";
char PL_INTL_LATESTAVAILABLESOFT[82] PROGMEM = "Najnowszą wersję oprogramowania znajdziesz zawsze {SMOGOMIERZRELEASES_LINK}.";

char PL_INTL_HERE[8] PROGMEM = "tutaj";
char PL_INTL_THEFORM[12] PROGMEM = "formularz";

char PL_INTL_CONNECTEDWIFI[28] PROGMEM = "Połączony z WiFi(SSID)";
char PL_INTL_WIFIRSSI[28] PROGMEM = "Siła sygnału WiFi(RSSI)";
char PL_INTL_WIFIQUALITY[26] PROGMEM = "Jakość sygnału WiFi";

char PL_INTL_HOMEKIT_SUPPORT[26] PROGMEM = "Apple HomeKit";
char PL_INTL_HOMEKIT_IS_PAIRED[94] = "Aby podłączyć nowe urządzenie iOS/macOS, musisz skasować aktualnie sparowane";
char PL_INTL_PAGE_HOMEKIT_RESET[30] = "Skasuj sparowane urządzenie";