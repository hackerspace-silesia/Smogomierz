/*
 * 
 *  translation file - polish translations
 * 
 */

#pragma once

char TEXT_LANG[6] PROGMEM = "pl";

char TEXT_INDEX_PAGE[16] PROGMEM = "Pomiary";
char TEXT_CONFIG_PAGE[14] PROGMEM = "Ustawienia";

char TEXT_CONFIG_DEVICE_PAGE[24] PROGMEM = "Ustawienia Urządzenia";
char TEXT_CONFIG_INFO_DEVICE_PAGE[144] PROGMEM = "Wybór czujników podłączonych do Smogly, częstotliwość wykonywania pomiarów, czy język interfejsu można zmienić w sekcji";
char TEXT_CONFIG_SERVICES_PAGE[24] PROGMEM = "Ustawienia Serwisów";
char TEXT_CONFIG_INFO_SERVICES_PAGE[144] PROGMEM = "Ustawienia takich serwisów jak Luftdaten, aqi.eco, Smoglist, AirMonitor, ThingSpeak czy usług InfluxDB lub MQTT można zmienić w sekcji";

char TEXT_UPDATE_PAGE[14] PROGMEM = "Aktualizacje";

char TEXT_WEATHER[10] PROGMEM = "Pogoda";
char TEXT_TEMPERATURE[14] PROGMEM = "Temperatura";
char TEXT_HUMIDITY[14] PROGMEM = "Wilgotność";
char TEXT_PRESSURE[14] PROGMEM = "Ciśnienie";
char TEXT_DEWPOINT[14] PROGMEM = "Punkt rosy";

char TEXT_AIRPOLLUTION[26] PROGMEM = "Pomiary zanieczyszczeń";
char TEXT_SAVED[10] PROGMEM = "ZAPISANO";

char TEXT_POSTCONFIG_INFO[60] PROGMEM = "wszystko wygląda OK, za chwilę nastąpi restart Smogly";
char TEXT_INSTRUCIONSLINK[76] PROGMEM = "Wszystkie instrukcje i opisy dostępne są {GITHUB_LINK}.";
char TEXT_DEVICENAME[20] PROGMEM = "Nazwa urządzenia";
char TEXT_DEVICENAMEAUTO[32] PROGMEM = "Automatyczne generowanie nazwy";
char TEXT_SELECTEDLANGUAGE[10] PROGMEM = "Język";
char TEXT_TEMPHUMIPRESSSENSOR[40] PROGMEM = "Miernik Temp/Wilgotności/Ciśnienia";
char TEXT_PMSENSOR[20] PROGMEM = "Miernik PM2.5/PM10";

char TEXT_FIRST_THP_SDA_SCL[40] PROGMEM = "Piny miernika Temp/Wilgotności";
char TEXT_FIRST_THP_SDA[8] PROGMEM = "Pin SDA";
char TEXT_FIRST_THP_SCL[8] PROGMEM = "Pin SCL";

char TEXT_SECOND_THP[40] PROGMEM = "Włacz drugi miernik Temp/Wilgotności";

char TEXT_SECOND_THP_SDA_SCL[48] PROGMEM = "Piny drugiego miernika Temp/Wilgotności";
char TEXT_SECOND_THP_SDA[8] PROGMEM = "Pin SDA";
char TEXT_SECOND_THP_SCL[8] PROGMEM = "Pin SCL";

char TEXT_DUST_TX_RX[32] PROGMEM = "Piny miernika PM2.5/PM10";
char TEXT_DUST_TX[8] PROGMEM = "Pin TX";
char TEXT_DUST_RX[8] PROGMEM = "Pin RX";

char TEXT_FREQUENTMEASUREMENTONOFF[26] PROGMEM = "Ciągłe pomiary PM";
char TEXT_FREQUENTMEASUREMENTINFO[80] PROGMEM = "Ciągłe pomiary znacznie skracają żywotność miernika PM.";
char TEXT_MEASUREMENTFREQUENCY[30] PROGMEM = "Wykonywanie pomiarów PM co";
char TEXT_AVERAGELASTRESULT[30] PROGMEM = "Uśredniaj wynik z ostatnich";
char TEXT_PMMEASUREMENTS[18] PROGMEM = "pomiarów PM";
char TEXT_SENDINGINTERVAL[50] PROGMEM = "Wysyłanie danych do serwisów zewnętrznych co";
char TEXT_SERVICESSENDINGINTERVAL[64] PROGMEM = "Wysyłanie danych do AirMonitor/Luftdaten/Smoglist/aqi.eco co";
char TEXT_DBSENDINGINTERVAL[50] PROGMEM = "Wysyłanie danych do ThingSpeak/InfluxDB/MQTT co";
char TEXT_SECONDS[10] PROGMEM = "sekund";
char TEXT_MINUTES[10] PROGMEM = "minut";

char TEXT_DEEPSLEEPINFO[540] PROGMEM = "DeepSleep wprowadza urządzenie w głębokie uśpienie pomiędzy seriami pomiarów - znacznie niższe zużycie energii(możliwość pracy na baterii przez kilka tygodni), ale brak ciągłego dostępu do interfejsu WWW. Interfejs WWW będzie dostępny tylko przez około {INTERFACEWWWONTIME} sekund co {SENDING_FREQUENCY} minut(oraz zaraz po restarcie urządzenia). Czas uśpienia będzie taki sam jak okres wysyłania pomiarów do serwisów zewnętrznych. Wymaga połączenia pinów D0 oraz RST na ESP8266!";
char TEXT_DEEPSLEEPINFO_ESP32[476] PROGMEM = "DeepSleep wprowadza urządzenie w głębokie uśpienie pomiędzy seriami pomiarów - znacznie niższe zużycie energii(możliwość pracy na baterii przez kilka tygodni), ale brak ciągłego dostępu do interfejsu WWW. Interfejs WWW będzie dostępny tylko przez około {INTERFACEWWWONTIME} sekund co {SENDING_FREQUENCY} minut(oraz zaraz po restarcie urządzenia). Czas uśpienia będzie taki sam jak okres wysyłania pomiarów do serwisów zewnętrznych.";

char TEXT_DISPLAYPM1[30] PROGMEM = "Wyświetlanie pomiarów PM1";
char TEXT_ALTITUDEINFO[116] PROGMEM = "Wysokość(wymagana do poprawnych pomiarów ciśnienia. Można sprawdzić {WSPOLRZEDNE_GPS_LINK})";

char TEXT_SECURECONFIGUPDATEPAGE[84] PROGMEM = "Wymagaj hasła dla strony Konfiguracyjnej oraz Update(domyślne: admin/password)";
char TEXT_SECURELOGIN[8] PROGMEM = "Login";
char TEXT_SECUREPASSWD[10] PROGMEM = "Hasło";
char TEXT_SECURELOGOUTINFO[48] PROGMEM = "Zrestartuj przeglądarkę w celu wylogowania!";

char TEXT_LUFTDATENSENDING[96] PROGMEM = "Wysyłanie danych do serwisu {LUFTDATEN_LINK} (wymaga rejestracji {LUFTDATENFORM_LINK})";

char TEXT_SMOGLISTSENDING[48] PROGMEM = "Wysyłanie danych do serwisu {SMOGLIST_LINK}";
char TEXT_SMOGLISTINFO[242] PROGMEM = "Smoglist to serwis zbierający statystyki użytkowania Smogly(nie zbieramy loginów i haseł). Smoglist pozwoli nam w przyszłości ulepszyć najcześciej używane funkcje i generować własne wykresy.";

char TEXT_AIRMONITORSENDING[128] PROGMEM = "Wysyłanie danych do serwisu {AIRMONITOR_LINK} (wymaga wypełnienia {AIRMONITORFORM_LINK}; Sensor: np. {PMSENSORMODEL})";
char TEXT_AIRMONITORCHARTS[48] PROGMEM = "Wyświetlanie wykresów z serwisu AirMonitor";
char TEXT_AIRMONITORCOORDINATESINFO[74] PROGMEM = "Współrzędne geograficzne miernika(można sprawdzić {LATLONG_LINK})";
char TEXT_AIRMONITOR_API_KEY[32] PROGMEM = "AirMonitor API Key";
char TEXT_AIRMONITORLATITUDE[24] PROGMEM = "Szerokość(latitude)";
char TEXT_AIRMONITORLONGITUDE[24] PROGMEM = "Długość(longitude)";

char TEXT_THINGSPEAKSENDING[50] PROGMEM = "Wysyłanie danych do serwisu {THINGSPEAK_LINK}";
char TEXT_THINGSPEAKCHARTS[48] PROGMEM = "Wyświetlanie wykresów z serwisu ThingSpeak";
char TEXT_THINGSPEAK_WRITE_API_KEY[28] PROGMEM = "ThingSpeak Write API Key";
char TEXT_THINGSPEAKCHANNELID[24] PROGMEM = "ThingSpeak Channel ID";
char TEXT_THINGSPEAK_READ_API_KEY[28] PROGMEM = "ThingSpeak Read API Key";

char TEXT_INFLUXDBSENDING[32] PROGMEM = "Wysyłanie danych do InfluxDB";
char TEXT_INFLUXDBVERSION[18] PROGMEM = "Wersja InfluxDB";
char TEXT_INFLUXDBSERVER[30] PROGMEM = "Adres bazy danych InfluxDB";
char TEXT_INFLUXDBPORT[16] PROGMEM = "Port InfluxDB";
char TEXT_INFLUXDBNAME[24] PROGMEM = "Nazwa bazy danych";
char TEXT_INFLUXDBUSER[28] PROGMEM = "Użytkownik bazy danych";
char TEXT_INFLUXDBPASSWD[24] PROGMEM = "Hasło do bazy danych";
char TEXT_INFLUXDBORG[16] PROGMEM = "Organizacja";
char TEXT_INFLUXDBBUCKET[10] PROGMEM = "Bucket";
char TEXT_INFLUXDBTOKEN[8] PROGMEM = "Token";

char TEXT_CONFIG_ADV_MQTT[22] PROGMEM = "Ustawienia MQTT";
char TEXT_MQTTSENDING[34] PROGMEM = "Wysyłanie danych poprzez MQTT";
char TEXT_MQTTSERVER[22] PROGMEM = "Adres serwera MQTT";
char TEXT_MQTTPORT[12] PROGMEM = "Port MQTT";
char TEXT_MQTTUSER[20] PROGMEM = "Użytkownik MQTT";
char TEXT_MQTTPASSWD[16] PROGMEM = "Hasło MQTT";

char TEXT_MQTT_IP_IN_TOPIC[22] PROGMEM = "Dodać Adres IP";
char TEXT_MQTT_DEVICENAME_IN_TOPIC[30] PROGMEM = "Dodać Nazwę urządzenia";
char TEXT_MQTT_TOPIC_INFO[256] PROGMEM = "Poniżej możesz sformatować sposób w jaki będą wysyłane dane MQTT. Masz możliwość dodania 'Adresu IP' oraz 'Nazwy urządzenia' na początku. Środkową część możesz zdefiniować samemu dla każdej z wartości mierzonych przez urządzenie.";
char TEXT_MQTT_SLASH_AT_THE_BEGINNING[40] PROGMEM = "Dodać '/' na początku";
char TEXT_MQTT_SLASH_AT_THE_END[36] PROGMEM = "Dodać '/' na końcu";

char TEXT_AQIECOSENDING[100] PROGMEM = "Wysyłanie danych do <a title='aqi.eco' href='https://aqi.eco' target='_blank'>aqi.eco</a>";
char TEXT_AQIECOSERVER[26] PROGMEM = "Adres serwera aqi.eco";
char TEXT_AQIECOPATH[20] PROGMEM = "Ścieżka aqi.eco";

char TEXT_CALIBMETHOD[22] PROGMEM = "Sposób kalibracji";
char TEXT_CALIB1[30] PROGMEM = "Zmienna kalibracyjna calib1";
char TEXT_CALIB2[30] PROGMEM = "Zmienna kalibracyjna calib2";
char TEXT_SOFTWATEVERSION[24] PROGMEM = "Wersja oprogramowania";

char TEXT_ERASEWIFICONFIG[22] PROGMEM = "Zapomnij sieć WiFi";
char TEXT_RESTORESETTINGS[32] PROGMEM = "Przywróć ustawienia domyślne";
char TEXT_SAVE[8] PROGMEM = "Zapisz";
char TEXT_YES[6] PROGMEM = "Tak";
char TEXT_NO[6] PROGMEM = "Nie";
char TEXT_WITHOUTSENSOR[16] PROGMEM = "Bez miernika";
char TEXT_WITHOUTCALIBRATION[22] PROGMEM = "Bez kalibracji";
char TEXT_AUTOMATICCALIBRATION[26] PROGMEM = "Automatyczna kalibracja";

char TEXT_INTL_EN[12] PROGMEM = "angielski";
char TEXT_INTL_PL[8] PROGMEM = "polski";

char TEXT_FWUPDATEAVALIBLE[42] PROGMEM = "Dostępna aktualizacja oprogramowania!";
char TEXT_AUTOUPDATEON[28] PROGMEM = "Automatyczne Aktualizacje";
char TEXT_MANUALUPDATEBUTTON[22] PROGMEM = "Ręczna Aktualizacja";
char TEXT_FWUPDATEBUTTON[32] PROGMEM = "Automatycznie - tylko tym razem";
char TEXT_AUTOUPDATEONBUTTON[36] PROGMEM = "Automatycznie - zapamiętaj wybór";
char TEXT_AUTOUPDATEWARNING[650] PROGMEM = "Automatyczne aktualizacje wykonywane są przez nieszyfrowane połaczenie(HTTP, a nie HTTPS). Może to być niebezpieczne i umożliwić przechwycenie pakietów oraz włamanie na urządzenie. Możesz skorzystać z Automatycznej aktualizacji jednorazowo, klikając przycisk \"{TEXT_FWUPDATEBUTTON}\", lub włączyć Automatyczne aktualizacje na stałe. W każdej chwili możesz zmienić swoje zdanie i wyłączyć Automatyczne Aktualizacje w Konfiguracji. Po wybraniu \"{TEXT_FWUPDATEBUTTON}\" lub włączeniu automatycznych aktualizacji urządzenie zapisze bieżącą konfigurację i uruchomi się ponownie. Może to potrwać kilkanaście sekund.";
char TEXT_UPDATEPAGEAUTOUPDATEWARNING[192] PROGMEM = "Automatyczne aktualizacje wykonywane są przez nieszyfrowane połaczenie(HTTP, a nie HTTPS). Może to być niebezpieczne i umożliwić przechwycenie pakietów oraz włamanie na urządzenie.";

char TEXT_SELECTUPDATEFILE[20] PROGMEM = "Wybierz plik .bin";
char TEXT_SUBMITUPDATE[10] PROGMEM = "Update!";
char TEXT_CURRENTSOFTVERSION[40] PROGMEM = "Używana wersja oprogramowania";
char TEXT_SERVERSOFTWAREVERSION[46] PROGMEM = "Najnowsza dostępna wersja oprogramowania";
char TEXT_LATESTAVAILABLESOFT[82] PROGMEM = "Najnowszą wersję oprogramowania znajdziesz zawsze {SMOGOMIERZRELEASES_LINK}.";

char TEXT_HERE[8] PROGMEM = "tutaj";
char TEXT_THEFORM[12] PROGMEM = "formularz";

char TEXT_CONNECTEDWIFI[28] PROGMEM = "Połączony z WiFi(SSID)";
char TEXT_WIFIRSSI[28] PROGMEM = "Siła sygnału WiFi(RSSI)";
char TEXT_WIFIQUALITY[26] PROGMEM = "Jakość sygnału WiFi";

char TEXT_HOMEKIT_SUPPORT[26] PROGMEM = "Apple HomeKit";
char TEXT_HOMEKIT_IS_PAIRED[94] = "Aby podłączyć nowe urządzenie iOS/macOS, musisz skasować aktualnie sparowane";
char TEXT_PAGE_HOMEKIT_RESET[30] = "Skasuj sparowane urządzenie";

char TEXT_AIRQUALITY_TOPIC[12] PROGMEM = "Airquality";
