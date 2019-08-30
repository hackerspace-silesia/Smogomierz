/*
 * 
 *  translation file - polish translations
 * 
 */

#pragma once

char PL_INTL_LANG[16] PROGMEM = "pl";

char PL_INTL_INDEX_PAGE[16] PROGMEM = "Pomiary";
char PL_INTL_CONFIG_PAGE[16] PROGMEM = "Konfiguracja";
char PL_INTL_UPDATE_PAGE[16] PROGMEM = "Update";

char PL_INTL_WEATHER[16] PROGMEM = "Pogoda";
char PL_INTL_TEMPERATURE[16] PROGMEM = "Temperatura";
char PL_INTL_HUMIDITY[16] PROGMEM = "Wilgotność";
char PL_INTL_PRESSURE[16] PROGMEM = "Ciśnienie";
char PL_INTL_DEWPOINT[16] PROGMEM = "Punkt rosy";

char PL_INTL_AIRPOLLUTION[32] PROGMEM = "Pomiary zanieczyszczeń";
char PL_INTL_SAVED[16] PROGMEM = "ZAPISANO";

char PL_INTL_POSTCONFIG_INFO[64] PROGMEM = "wszystko wygląda OK, za chwilę nastąpi restart Smogomierza";
char PL_INTL_INSTRUCIONSLINK[80] PROGMEM = "Wszystkie instrukcje i opisy dostępne są {GITHUB_LINK}.";
char PL_INTL_DEVICENAME[32] PROGMEM = "Nazwa urządzenia";
char PL_INTL_DEVICENAMEAUTO[32] PROGMEM = "Automatyczne generowanie nazwy";
char PL_INTL_SELECTEDLANGUAGE[16] PROGMEM = "Język";
char PL_INTL_TEMPHUMIPRESSSENSOR[64] PROGMEM = "Miernik Temp/Wilgotności/Ciśnienia";
char PL_INTL_PMSENSOR[32] PROGMEM = "Miernik PM2.5/PM10";

char PL_INTL_FREQUENTMEASUREMENTONOFF[32] PROGMEM = "Ciągłe pomiary PM";
char PL_INTL_FREQUENTMEASUREMENTINFO[96] PROGMEM = "Ciągłe pomiary znacznie skracają żywotność miernika PM.";
char PL_INTL_MEASUREMENTFREQUENCY[32] PROGMEM = "Wykonywanie pomiarów PM co";
char PL_INTL_AVERAGELASTRESULT[32] PROGMEM = "Uśredniaj wynik z ostatnich";
char PL_INTL_PMMEASUREMENTS[32] PROGMEM = "pomiarów PM";
char PL_INTL_SENDINGINTERVAL[64] PROGMEM = "Wysyłanie danych do serwisów zewnętrznych co";
char PL_INTL_SERVICESSENDINGINTERVAL[64] PROGMEM = "Wysyłanie danych do AirMonitor/Luftdaten/Smoglist co";
char PL_INTL_DBSENDINGINTERVAL[64] PROGMEM = "Wysyłanie danych do ThingSpeak/InfluxDB/MQTT co";
char PL_INTL_SECONDS[16] PROGMEM = "sekund";
char PL_INTL_MINUTES[16] PROGMEM = "minut";

char PL_INTL_DEEPSLEEPINFO[544] PROGMEM = "DeepSleep wprowadza urządzenie w głębokie uśpienie pomiędzy seriami pomiarów - znacznie niższe zużycie energii(możliwość pracy na baterii przez kilka tygodni), ale brak ciągłego dostępu do interfejsu WWW. Interfejs WWW będzie dostępny tylko przez około {INTERFACEWWWONTIME} sekund co {SENDING_FREQUENCY} minut(oraz zaraz po restarcie urządzenia). Czas uśpienia będzie taki sam jak okres wysyłania pomiarów do serwisów zewnętrznych. Wymaga połączenia pinów D0 oraz RST na ESP8266!";
char PL_INTL_DEEPSLEEPINFO_ESP32[544] PROGMEM = "DeepSleep wprowadza urządzenie w głębokie uśpienie pomiędzy seriami pomiarów - znacznie niższe zużycie energii(możliwość pracy na baterii przez kilka tygodni), ale brak ciągłego dostępu do interfejsu WWW. Interfejs WWW będzie dostępny tylko przez około {INTERFACEWWWONTIME} sekund co {SENDING_FREQUENCY} minut(oraz zaraz po restarcie urządzenia). Czas uśpienia będzie taki sam jak okres wysyłania pomiarów do serwisów zewnętrznych.";

char PL_INTL_DISPLAYPM1[32] PROGMEM = "Wyświetlanie pomiarów PM1";
char PL_INTL_ALTITUDEINFO[128] PROGMEM = "Wysokość(wymagana do poprawnych pomiarów ciśnienia. Można sprawdzić {WSPOLRZEDNE_GPS_LINK})";

char PL_INTL_SECURECONFIGUPDATEPAGE[96] PROGMEM = "Wymagaj hasła dla strony Konfiguracyjnej oraz Update(domyślne: admin/password)";
char PL_INTL_SECURELOGIN[16] PROGMEM = "Login";
char PL_INTL_SECUREPASSWD[16] PROGMEM = "Hasło";
char PL_INTL_SECURELOGOUTINFO[64] PROGMEM = "Zrestartuj przeglądarkę w celu wylogowania!";

char PL_INTL_LUFTDATENSENDING[96] PROGMEM = "Wysyłanie danych do serwisu {LUFTDATEN_LINK} (wymaga rejestracji {LUFTDATENFORM_LINK})";

char PL_INTL_SMOGLISTSENDING[64] PROGMEM = "Wysyłanie danych do serwisu {SMOGLIST_LINK}";
char PL_INTL_SMOGLISTINFO[256] = "Smoglist to serwis zbierający statystyki użytkowania Smogomierzy(nie zbieramy loginów i haseł). Smoglist pozwoli nam w przyszłości ulepszyć najcześciej używane funkcje i generować własne wykresy.";

char PL_INTL_AIRMONITORSENDING[128] PROGMEM = "Wysyłanie danych do serwisu {AIRMONITOR_LINK} (wymaga wypełnienia {AIRMONITORFORM_LINK}; Sensor: np. {PMSENSORMODEL})";
char PL_INTL_AIRMONITORCHARTS[64] PROGMEM = "Wyświetlanie wykresów z serwisu AirMonitor";
char PL_INTL_AIRMONITORCOORDINATESINFO[80] PROGMEM = "Współrzędne geograficzne miernika(można sprawdzić {LATLONG_LINK})";
char PL_INTL_AIRMONITORLATITUDE[32] PROGMEM = "Szerokość(latitude)";
char PL_INTL_AIRMONITORLONGITUDE[32] PROGMEM = "Długość(longitude)";

char PL_INTL_THINGSPEAKSENDING[64] PROGMEM = "Wysyłanie danych do serwisu {THINGSPEAK_LINK}";
char PL_INTL_THINGSPEAKCHARTS[64] PROGMEM = "Wyświetlanie wykresów z serwisu ThingSpeak";
char PL_INTL_THINGSPEAKAPIKEY[32] PROGMEM = "ThingSpeak API_KEY";
char PL_INTL_THINGSPEAKCHANNELID[32] PROGMEM = "ThingSpeak Channel ID";

char PL_INTL_INFLUXDBSENDING[64] PROGMEM = "Wysyłanie danych do InfluxDB";
char PL_INTL_INFLUXDBSERVER[64] PROGMEM = "Adres bazy danych InfluxDB";
char PL_INTL_INFLUXDBPORT[32] PROGMEM = "Port InfluxDB";
char PL_INTL_INFLUXDBNAME[32] PROGMEM = "Nazwa bazy danych";
char PL_INTL_INFLUXDBUSER[32] PROGMEM = "Użytkownik bazy danych";
char PL_INTL_INFLUXDBPASSWD[32] PROGMEM = "Hasło do bazy danych";

char PL_INTL_MQTTSENDING[64] PROGMEM = "Wysyłanie danych poprzez MQTT";
char PL_INTL_MQTTSERVER[32] PROGMEM = "Adres serwera MQTT";
char PL_INTL_MQTTPORT[32] PROGMEM = "Port MQTT";
char PL_INTL_MQTTUSER[32] PROGMEM = "Użytkownik MQTT";
char PL_INTL_MQTTPASSWD[32] PROGMEM = "Hasło MQTT";

char PL_INTL_CALIBMETHOD[32] PROGMEM = "Sposób kalibracji";
char PL_INTL_CALIB1[32] PROGMEM = "Zmienna kalibracyjna calib1";
char PL_INTL_CALIB2[32] PROGMEM = "Zmienna kalibracyjna calib2";
char PL_INTL_SOFTWATEVERSION[32] PROGMEM = "Wersja oprogramowania";

char PL_INTL_ERASEWIFICONFIG[32] PROGMEM = "Zapomnij sieć WiFi";
char PL_INTL_RESTORESETTINGS[32] PROGMEM = "Przywróć ustawienia domyślne";
char PL_INTL_SAVE[16] PROGMEM = "Zapisz";
char PL_INTL_YES[16] PROGMEM = "Tak";
char PL_INTL_NO[16] PROGMEM = "Nie";
char PL_INTL_WITHOUTSENSOR[32] PROGMEM = "Bez miernika";
char PL_INTL_WITHOUTCALIBRATION[32] PROGMEM = "Bez kalibracji";
char PL_INTL_AUTOMATICCALIBRATION[32] PROGMEM = "Automatyczna kalibracja";

char PL_INTL_INTL_EN[16] PROGMEM = "angielski";
char PL_INTL_INTL_PL[16] PROGMEM = "polski";

char PL_INTL_FWUPDATEAVALIBLE[64] PROGMEM = "Dostępna aktualizacja oprogramowania!";
char PL_INTL_AUTOUPDATEON[32] PROGMEM = "Automatyczne Aktualizacje";
char PL_INTL_MANUALUPDATEBUTTON[32] PROGMEM = "Ręczna Aktualizacja";
char PL_INTL_FWUPDATEBUTTON[32] PROGMEM = "Automatycznie - tylko tym razem";
char PL_INTL_AUTOUPDATEONBUTTON[64] PROGMEM = "Automatycznie - zapamiętaj wybór";
char PL_INTL_AUTOUPDATEWARNING[672] PROGMEM = "Automatyczne aktualizacje wykonywane są przez nieszyfrowane połaczenie(HTTP, a nie HTTPS). Może to być niebezpieczne i umożliwić przechwycenie pakietów oraz włamanie na urządzenie. Możesz skorzystać z Automatycznej aktualizacji jednorazowo, klikając przycisk \"{TEXT_FWUPDATEBUTTON}\", lub włączyć Automatyczne aktualizacje na stałe. W każdej chwili możesz zmienić swoje zdanie i wyłączyć Automatyczne Aktualizacje w Konfiguracji. Po wybraniu \"{TEXT_FWUPDATEBUTTON}\" lub włączeniu automatycznych aktualizacji urządzenie zapisze bieżącą konfigurację i uruchomi się ponownie. Może to potrwać kilkanaście sekund.";
char PL_INTL_UPDATEPAGEAUTOUPDATEWARNING[192] PROGMEM = "Automatyczne aktualizacje wykonywane są przez nieszyfrowane połaczenie(HTTP, a nie HTTPS). Może to być niebezpieczne i umożliwić przechwycenie pakietów oraz włamanie na urządzenie.";

char PL_INTL_SELECTUPDATEFILE[32] PROGMEM = "Wybierz plik .bin";
char PL_INTL_SUBMITUPDATE[16] PROGMEM = "Update!";
char PL_INTL_CURRENTSOFTVERSION[64] PROGMEM = "Używana wersja oprogramowania";
char PL_INTL_SERVERSOFTWAREVERSION[64] PROGMEM = "Najnowsza dostępna wersja oprogramowania";
char PL_INTL_LATESTAVAILABLESOFT[96] PROGMEM = "Najnowszą wersję oprogramowania znajdziesz zawsze {SMOGOMIERZRELEASES_LINK}.";

char PL_INTL_HERE[16] PROGMEM = "tutaj";
char PL_INTL_THEFORM[16] PROGMEM = "formularz";

char PL_INTL_CONNECTEDWIFI[32] PROGMEM = "Połączony z WiFi(SSID)";
char PL_INTL_WIFIRSSI[32] PROGMEM = "Siła sygnału WiFi(RSSI)";
char PL_INTL_WIFIQUALITY[32] PROGMEM = "Jakość sygnału WiFi";