/*
 * 
 *  translation file - polish translations
 * 
 */

#pragma once

char PL_INTL_LANG[] PROGMEM = "pl";

char PL_INTL_INDEX_PAGE[] PROGMEM = "Pomiary";
char PL_INTL_CONFIG_PAGE[] PROGMEM = "Konfiguracja";
char PL_INTL_UPDATE_PAGE[] PROGMEM = "Update";

char PL_INTL_WEATHER[] PROGMEM = "Pogoda";
char PL_INTL_TEMPERATURE[] PROGMEM = "Temperatura";
char PL_INTL_HUMIDITY[] PROGMEM = "Wilgotność";
char PL_INTL_PRESSURE[] PROGMEM = "Ciśnienie";
char PL_INTL_DEWPOINT[] PROGMEM = "Punkt rosy";

char PL_INTL_AIRPOLLUTION[] PROGMEM = "Pomiary zanieczyszczeń";
char PL_INTL_SAVED[] PROGMEM = "ZAPISANO";

char PL_INTL_POSTCONFIG_INFO[] PROGMEM = "wszystko wygląda OK, za chwilę nastąpi restart Smogomierza";
char PL_INTL_INSTRUCIONSLINK[] PROGMEM = "Wszystkie instrukcje i opisy dostępne są {GITHUB_LINK}.";
char PL_INTL_DEVICENAME[] PROGMEM = "Nazwa urządzenia";
char PL_INTL_DEVICENAMEAUTO[] PROGMEM = "Automatyczne generowanie nazwy";
char PL_INTL_SELECTEDLANGUAGE[] PROGMEM = "Język";
char PL_INTL_TEMPHUMIPRESSSENSOR[] PROGMEM = "Miernik Temp/Wilgotności/Ciśnienia";
char PL_INTL_PMSENSOR[] PROGMEM = "Miernik PM2.5/PM10";

char PL_INTL_FREQUENTMEASUREMENTONOFF[] PROGMEM = "Częste pomiary PM";
char PL_INTL_FREQUENTMEASUREMENTINFO[] PROGMEM = "Częste pomiary – co kilka-kilkanaście sekund, skracają żywotność miernika PM.";
char PL_INTL_MEASUREMENTFREQUENCY[] PROGMEM = "Wykonywanie pomiarów PM co";
char PL_INTL_AVERAGELASTRESULT[] PROGMEM = "Uśredniaj wynik z ostatnich";
char PL_INTL_PMMEASUREMENTS[] PROGMEM = "pomiarów PM";
char PL_INTL_SENDINGINTERVAL[] PROGMEM = "Wysyłanie pomiarów do serwisów zewnętrznych co";
char PL_INTL_SECONDS[] PROGMEM = "sekund";
char PL_INTL_MINUTES[] PROGMEM = "minut";

char PL_INTL_DEEPSLEEPINFO[] PROGMEM = "DeepSleep wprowadza urządzenie w głębokie uśpienie pomiędzy seriami pomiarów - znacznie niższe zużycie energii(możliwość pracy na baterii przez kilka tygodni), ale brak ciągłego dostępu do interfejsu WWW. Interfejs WWW będzie dostępny tylko przez około {INTERFACEWWWONTIME} sekund co {SENDING_FREQUENCY} minut(oraz zaraz po restarcie urządzenia). Czas uśpienia będzie taki sam jak okres wysyłania pomiarów do serwisów zewnętrznych. Wymaga połączenia pinów D0 oraz RST na ESP8266!";

char PL_INTL_DISPLAYPM1[] PROGMEM = "Wyświetlanie pomiarów PM1";
char PL_INTL_ALTITUDEINFO[] PROGMEM = "Wysokość(wymagana do poprawnych pomiarów ciśnienia. Można sprawdzić {WSPOLRZEDNE_GPS_LINK})";

char PL_INTL_SECURECONFIGUPDATEPAGE[] PROGMEM = "Wymagaj hasła dla strony Konfiguracyjnej oraz Update(domyślne: admin/password)";
char PL_INTL_SECURELOGIN[] PROGMEM = "Login";
char PL_INTL_SECUREPASSWD[] PROGMEM = "Hasło";
char PL_INTL_SECURELOGOUTINFO[] PROGMEM = "Zrestartuj przeglądarkę w celu wylogowania!";

char PL_INTL_LUFTDATENSENDING[] PROGMEM = "Wysyłanie danych do serwisu {LUFTDATEN_LINK} (wymaga wypełnienia {LUFTDATENFORM_LINK})";

char PL_INTL_AIRMONITORSENDING[] PROGMEM = "Wysyłanie danych do serwisu {AIRMONITOR_LINK} (wymaga wypełnienia {AIRMONITORFORM_LINK}; Sensor: np. PMS7003)";
char PL_INTL_AIRMONITORCHARTS[] PROGMEM = "Wyświetlanie wykresów z serwisu AirMonitor";
char PL_INTL_AIRMONITORCOORDINATESINFO[] PROGMEM = "Współrzędne geograficzne miernika(można sprawdzić {LATLONG_LINK}";
char PL_INTL_AIRMONITORLATITUDE[] PROGMEM = "Szerokość(latitude)";
char PL_INTL_AIRMONITORLONGITUDE[] PROGMEM = "Długość(longitude)";

char PL_INTL_THINGSPEAKSENDING[] PROGMEM = "Wysyłanie danych do serwisu {THINGSPEAK_LINK}";
char PL_INTL_THINGSPEAKCHARTS[] PROGMEM = "Wyświetlanie wykresów z serwisu ThingSpeak";
char PL_INTL_THINGSPEAKAPIKEY[] PROGMEM = "ThingSpeak API_KEY";
char PL_INTL_THINGSPEAKCHANNELID[] PROGMEM = "ThingSpeak Channel ID";

char PL_INTL_INFLUXDBSENDING[] PROGMEM = "Wysyłanie danych do InfluxDB";
char PL_INTL_INFLUXDBSERVER[] PROGMEM = "Adres bazy danych InfluxDB";
char PL_INTL_INFLUXDBPORT[] PROGMEM = "Port InfluxDB";
char PL_INTL_INFLUXDBNAME[] PROGMEM = "Nazwa bazy danych";
char PL_INTL_INFLUXDBUSER[] PROGMEM = "Użytkownik bazy danych";
char PL_INTL_INFLUXDBPASSWD[] PROGMEM = "Hasło do bazy danych";

char PL_INTL_MQTTSENDING[] PROGMEM = "Wysyłanie danych poprzez MQTT";
char PL_INTL_MQTTSERVER[] PROGMEM = "Adres serwera MQTT";
char PL_INTL_MQTTPORT[] PROGMEM = "Port MQTT";
char PL_INTL_MQTTUSER[] PROGMEM = "Użytkownik MQTT";
char PL_INTL_MQTTPASSWD[] PROGMEM = "Hasło MQTT";

char PL_INTL_CALIBMETHOD[] PROGMEM = "Sposób kalibracji";
char PL_INTL_CALIB1[] PROGMEM = "Zmienna kalibracyjna calib1";
char PL_INTL_CALIB2[] PROGMEM = "Zmienna kalibracyjna calib2";
char PL_INTL_SOFTWATEVERSION[] PROGMEM = "Wersja oprogramowania";

char PL_INTL_ERASEWIFICONFIG[] PROGMEM = "Zapomnij sieć WiFi";
char PL_INTL_RESTORESETTINGS[] PROGMEM = "Przywróć ustawienia domyślne";
char PL_INTL_SAVE[] PROGMEM = "Zapisz";
char PL_INTL_YES[] PROGMEM = "Tak";
char PL_INTL_NO[] PROGMEM = "Nie";
char PL_INTL_WITHOUTSENSOR[] PROGMEM = "Bez miernika";
char PL_INTL_WITHOUTCALIBRATION[] PROGMEM = "Bez kalibracji";
char PL_INTL_AUTOMATICCALIBRATION[] PROGMEM = "Automatyczna kalibracja";

char PL_INTL_INTL_EN[] PROGMEM = "angielski";
char PL_INTL_INTL_PL[] PROGMEM = "polski";

char PL_INTL_FWUPDATEAVALIBLE[] PROGMEM = "Dostępna aktualizacja oprogramowania!";
char PL_INTL_AUTOUPDATEON[] PROGMEM = "Automatyczne Aktualizacje";
char PL_INTL_MANUALUPDATEBUTTON[] PROGMEM = "Ręczna Aktualizacja";
char PL_INTL_FWUPDATEBUTTON[] PROGMEM = "Automatycznie - tylko tym razem";
char PL_INTL_AUTOUPDATEONBUTTON[] PROGMEM = "Automatycznie - zapamiętaj wybór";
char PL_INTL_AUTOUPDATEWARNING[] PROGMEM = "Automatyczne aktualizacje wykonywane są przez nieszyfrowane połaczenie(HTTP, a nie HTTPS). Może to być niebezpieczne i umożliwić przechwycenie pakietów oraz włamanie na urządzenie. Możesz skorzystać z Automatycznej aktualizacji jednorazowo, klikając przycisk \"{TEXT_FWUPDATEBUTTON}\", lub włączyć Automatyczne aktualizacje na stałe. W każdej chwili możesz zmienić swoje zdanie i wyłączyć Automatyczne Aktualizacje w Konfiguracji. Po wybraniu \"{TEXT_FWUPDATEBUTTON}\" lub włączeniu automatycznych aktualizacji urządzenie zapisze bieżącą konfigurację i uruchomi się ponownie. Może to potrwać kilkanaście sekund.";
char PL_INTL_UPDATEPAGEAUTOUPDATEWARNING[] PROGMEM = "Automatyczne aktualizacje wykonywane są przez nieszyfrowane połaczenie(HTTP, a nie HTTPS). Może to być niebezpieczne i umożliwić przechwycenie pakietów oraz włamanie na urządzenie.";

char PL_INTL_SELECTUPDATEFILE[] PROGMEM = "Wybierz plik .bin";
char PL_INTL_SUBMITUPDATE[] PROGMEM = "Update!";
char PL_INTL_CURRENTSOFTVERSION[] PROGMEM = "Aktualna wersja oprogramowania";
char PL_INTL_SERVERSOFTWAREVERSION[] PROGMEM = "Najnowsza dostępna wersja oprogramowania";
char PL_INTL_LATESTAVAILABLESOFT[] PROGMEM = "Najnowszą wersję oprogramowania znajdziesz zawsze {SMOGOMIERZRELEASES_LINK}.";

char PL_INTL_HERE[] PROGMEM = "tutaj";
char PL_INTL_THEFORM[] PROGMEM = "formularz";