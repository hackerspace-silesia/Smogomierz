/*
 *  
 *  default translation file - english translation
 *  
 */

#pragma once

extern char TEXT_LANG[16] = "en";

extern char TEXT_INDEX_PAGE[16] = "Measurements";
extern char TEXT_CONFIG_PAGE[16] = "Configuration";
extern char TEXT_UPDATE_PAGE[16] = "Update";

extern char TEXT_WEATHER[16] = "Weather";
extern char TEXT_TEMPERATURE[16] = "Temperature";
extern char TEXT_HUMIDITY[16] = "Humidity";
extern char TEXT_PRESSURE[16] = "Pressure";
extern char TEXT_DEWPOINT[16] = "Dewpoint";

extern char TEXT_AIRPOLLUTION[32] = "Air pollution";
extern char TEXT_SAVED[16] = "SAVED!";

extern char TEXT_POSTCONFIG_INFO[64] = "everything looks OK, in a moment the Smogomierz will restart";
extern char TEXT_INSTRUCIONSLINK[80] = "All instructions and descriptions[in polish] are available {GITHUB_LINK}.";
extern char TEXT_DEVICENAME[32] = "Device Name";
extern char TEXT_DEVICENAMEAUTO[32] = "Automatic name generation";
extern char TEXT_SELECTEDLANGUAGE[16] = "Language";
extern char TEXT_TEMPHUMIPRESSSENSOR[64] = "Temp/Humi/Press Sensor";
extern char TEXT_PMSENSOR[32] = "PM2.5/PM10 Sensor";

extern char TEXT_FREQUENTMEASUREMENTONOFF[32] = "Continuous measurements";
extern char TEXT_FREQUENTMEASUREMENTINFO[96] = "Continuous measurements significantly shorten the life span of the PM sensor.";
extern char TEXT_MEASUREMENTFREQUENCY[32] = "Make PM measurements every";
extern char TEXT_AVERAGELASTRESULT[32] = "Average result from last";
extern char TEXT_PMMEASUREMENTS[32] = "PM measurements";
extern char TEXT_SENDINGINTERVAL[64] = "Sending data to external services every";
extern char TEXT_SERVICESSENDINGINTERVAL[64] = "Sending data to AirMonitor/Luftdaten/Smoglist every";
extern char TEXT_DBSENDINGINTERVAL[64] = "Sending data to ThingSpeak/InfluxDB/MQTT every";
extern char TEXT_SECONDS[16] = "seconds";
extern char TEXT_MINUTES[16] = "minutes";

extern char TEXT_DEEPSLEEPINFO[544] = "DeepSleep put the device into deep sleep between a series of measurements - significantly lower energy consumption (the possibility of working on the battery for several weeks), but lack of continuous access to the web interface. The web interface will only be available for about {INTERFACEWWWONTIME} seconds every {SENDING_FREQUENCY} minutes (and right after the device reboots). Sleep time will be the same as the period for sending measurements to external services. DeepSleep requires connection of D0 and RST pins on ESP8266!";
extern char TEXT_DEEPSLEEPINFO_ESP32[544] = "DeepSleep put the device into deep sleep between a series of measurements - significantly lower energy consumption (the possibility of working on the battery for several weeks), but lack of continuous access to the web interface. The web interface will only be available for about {INTERFACEWWWONTIME} seconds every {SENDING_FREQUENCY} minutes (and right after the device reboots). Sleep time will be the same as the period for sending measurements to external services.";

extern char TEXT_DISPLAYPM1[32] = "Display of PM1 measurements";
extern char TEXT_ALTITUDEINFO[128] = "Altitude above sea level (required for correct pressure measurements. You can check it {WSPOLRZEDNE_GPS_LINK})";

extern char TEXT_SECURECONFIGUPDATEPAGE[96] = "Secure the Configuration and Update pages(default: admin/password)";
extern char TEXT_SECURELOGIN[16] = "Login";
extern char TEXT_SECUREPASSWD[16] = "Password";
extern char TEXT_SECURELOGOUTINFO[64] = "Restart your web browser to log out!";

extern char TEXT_LUFTDATENSENDING[96] = "Sending data to the {LUFTDATEN_LINK} service(requires registration at {LUFTDATENFORM_LINK})"; 

extern char TEXT_SMOGLISTSENDING[64] = "Sending data to the {SMOGLIST_LINK} service";
extern char TEXT_SMOGLISTINFO[256] = "Smoglist is a service that collects Smogomierz usage statistics(we do not collect logins and passwords). It will allow us to improve the most commonly used functions in the future and generate our own charts.";

extern char TEXT_AIRMONITORSENDING[128] = "Sending data to the {AIRMONITOR_LINK} service(requires filling out {AIRMONITORFORM_LINK}; Sensor: e.g. {PMSENSORMODEL})";
extern char TEXT_AIRMONITORCHARTS[64] = "Displaying charts from the AirMonitor site";
extern char TEXT_AIRMONITORCOORDINATESINFO[80] = "Geographical coordinates(you can check it {LATLONG_LINK})";
extern char TEXT_AIRMONITORLATITUDE[32] = "Latitude";
extern char TEXT_AIRMONITORLONGITUDE[32] = "Longitude";

extern char TEXT_THINGSPEAKSENDING[64] = "Sending data to the {THINGSPEAK_LINK} service:";
extern char TEXT_THINGSPEAKCHARTS[64] = "Displaying charts from the ThingSpeak site";
extern char TEXT_THINGSPEAKAPIKEY[32] = "ThingSpeak API_KEY";
extern char TEXT_THINGSPEAKCHANNELID[32] = "ThingSpeak Channel ID";

extern char TEXT_INFLUXDBSENDING[64] = "Sending data to the InfluxDB";
extern char TEXT_INFLUXDBSERVER[64] = "InfluxDB database address";
extern char TEXT_INFLUXDBPORT[32] = "InfluxDB port";
extern char TEXT_INFLUXDBNAME[32] = "Name of the database";
extern char TEXT_INFLUXDBUSER[32] = "Database user";
extern char TEXT_INFLUXDBPASSWD[32] = "Database password";

extern char TEXT_MQTTSENDING[64] = "Sending data to the MQTT server";
extern char TEXT_MQTTSERVER[32] = "MQTT server address";
extern char TEXT_MQTTPORT[32] = "MQTT port";
extern char TEXT_MQTTUSER[32] = "MQTT user";
extern char TEXT_MQTTPASSWD[32] = "MQTT password";

extern char TEXT_CALIBMETHOD[32] = "Calibration method";
extern char TEXT_CALIB1[32] = "calib1";
extern char TEXT_CALIB2[32] = "calib2";
extern char TEXT_SOFTWATEVERSION[32] = "Software version";

extern char TEXT_ERASEWIFICONFIG[32] = "Erase WiFi Config";
extern char TEXT_RESTORESETTINGS[32] = "Restore default settings";
extern char TEXT_SAVE[16] = "Save";
extern char TEXT_YES[16] = "Yes";
extern char TEXT_NO[16] = "No";
extern char TEXT_WITHOUTSENSOR[32] = "Without sensor";
extern char TEXT_WITHOUTCALIBRATION[32] = "Without calibration";
extern char TEXT_AUTOMATICCALIBRATION[32] = "Automatic calibration";

extern char TEXT_INTL_EN[16] = "english";
extern char TEXT_INTL_PL[16] = "polish";

extern char TEXT_FWUPDATEAVALIBLE[64] = "Firmware update available!";
extern char TEXT_AUTOUPDATEON[32] = "Autoupdate";
extern char TEXT_MANUALUPDATEBUTTON[32] = "Manual Upgrade";
extern char TEXT_FWUPDATEBUTTON[32] = "Firmware Upgrade";
extern char TEXT_AUTOUPDATEONBUTTON[64] = "Autoupdate On";
extern char TEXT_AUTOUPDATEWARNING[672] = "Automatic updates are performed via an unencrypted connection (HTTP, not HTTPS). It can be dangerous and allow interception of packets and hacking the device. You can use the Autoupdate once by clicking the \"{TEXT_FWUPDATEBUTTON}\" button or turn on Autoupdate permanently. You can change your mind at any time and disable Autoupdate in the Configuration. After selecting \"{TEXT_FWUPDATEBUTTON}\" or turning on Autoupdate, the device will save the current configuration and restart. It may take several seconds.";
extern char TEXT_UPDATEPAGEAUTOUPDATEWARNING[192] = "Automatic updates are performed via an unencrypted connection (HTTP, not HTTPS). It can be dangerous and allow interception of packets and hacking the device.";

extern char TEXT_SELECTUPDATEFILE[32] = "Select .bin file";
extern char TEXT_SUBMITUPDATE[16] = "Update!";
extern char TEXT_CURRENTSOFTVERSION[64] = "Currently used version of the firmware";
extern char TEXT_SERVERSOFTWAREVERSION[64] = "The latest available firmware version";
extern char TEXT_LATESTAVAILABLESOFT[96] = "Latest version of the software available {SMOGOMIERZRELEASES_LINK}.";

extern char TEXT_HERE[16] = "here";
extern char TEXT_THEFORM[16] = "the form";

extern char TEXT_CONNECTEDWIFI[32] = "Connected to a WiFi(SSID)";
extern char TEXT_WIFIRSSI[32] = "WiFi Signal Strength(RSSI)";
extern char TEXT_WIFIQUALITY[32] = "WiFi Signal Quality";

#ifdef ARDUINO_ARCH_ESP8266
static char GITHUB_LINK[] = "<a title='Instructions' href='https://github.com/hackerspace-silesia/Smogomierz#instrukcje' target='_blank'>{TEXT_HERE}</a>";
static char WSPOLRZEDNE_GPS_LINK[] = "<a title='wspolrzedne-gps.pl' href='https://www.wspolrzedne-gps.pl' target='_blank'>{TEXT_HERE}</a>";
static char LUFTDATEN_LINK[] = "<a title='LuftDaten.info' href='https://luftdaten.info/en/home-en/' target='_blank'>LuftDaten.info</a>";
static char LUFTDATENFORM_LINK[] = "<a title='meine.luftdaten.info' href='https://meine.luftdaten.info' target='_blank'>meine.luftdaten.info</a>";
static char SMOGLIST_LINK[] = "<a title='smoglist.pl' href='http://smoglist.pl' target='_blank'>Smoglist</a>";
static char AIRMONITOR_LINK[] = "<a title='AirMonitor' href='http://mapa.airmonitor.pl' target='_blank'>AirMonitor</a>";
static char AIRMONITORFORM_LINK[] = "<a title='AirMonitor Form' href='https://docs.google.com/forms/d/e/1FAIpQLSdw72_DggyrK7xnSQ1nR11Y-YK4FYWk_MF9QbecpOERql-T2w/viewform' target='_blank'>{TEXT_THEFORM}</a>";
static char LATLONG_LINK[] PROGMEM = "<a title='latlong.net' href='https://www.latlong.net' target='_blank'>{TEXT_HERE}</a>";
static char THINGSPEAK_LINK[] = "<a title='ThingSpeak' href='https://thingspeak.com' target='_blank'>ThingSpeak</a>";
static char SMOGOMIERZRELEASES_LINK[] = "<b><a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/releases'>{TEXT_HERE}</a></b>";
static char MADAVI_LINK[] = "<a title='madavi.de' href='https://www.madavi.de/sensor/graph.php' target='_blank'>{TEXT_HERE}</a>";
#elif defined ARDUINO_ARCH_ESP32
static char GITHUB_LINK[] PROGMEM = "<a title='Instructions' href='https://github.com/hackerspace-silesia/Smogomierz#instrukcje' target='_blank'>{TEXT_HERE}</a>";
static char WSPOLRZEDNE_GPS_LINK[] PROGMEM = "<a title='wspolrzedne-gps.pl' href='https://www.wspolrzedne-gps.pl' target='_blank'>{TEXT_HERE}</a>";
static char LUFTDATEN_LINK[] PROGMEM = "<a title='LuftDaten.info' href='https://luftdaten.info/en/home-en/' target='_blank'>LuftDaten.info</a>";
static char LUFTDATENFORM_LINK[] PROGMEM = "<a title='meine.luftdaten.info' href='https://meine.luftdaten.info' target='_blank'>meine.luftdaten.info</a>";
static char SMOGLIST_LINK[] PROGMEM = "<a title='smoglist.pl' href='http://smoglist.pl' target='_blank'>Smoglist</a>";
static char AIRMONITOR_LINK[] PROGMEM = "<a title='AirMonitor' href='http://mapa.airmonitor.pl' target='_blank'>AirMonitor</a>";
static char AIRMONITORFORM_LINK[] PROGMEM = "<a title='AirMonitor Form' href='https://docs.google.com/forms/d/e/1FAIpQLSdw72_DggyrK7xnSQ1nR11Y-YK4FYWk_MF9QbecpOERql-T2w/viewform' target='_blank'>{TEXT_THEFORM}</a>";
static char LATLONG_LINK[] PROGMEM = "<a title='latlong.net' href='https://www.latlong.net' target='_blank'>{TEXT_HERE}</a>";
static char THINGSPEAK_LINK[] PROGMEM = "<a title='ThingSpeak' href='https://thingspeak.com' target='_blank'>ThingSpeak</a>";
static char SMOGOMIERZRELEASES_LINK[] PROGMEM = "<b><a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/releases'>{TEXT_HERE}</a></b>";
static char MADAVI_LINK[] PROGMEM = "<a title='madavi.de' href='https://www.madavi.de/sensor/graph.php' target='_blank'>{TEXT_HERE}</a>";
#endif