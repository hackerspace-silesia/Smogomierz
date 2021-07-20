/*
 *
 *  default translation file - english translation
 *
 */

#pragma once

char TEXT_LANG[6] PROGMEM = "en";

char TEXT_INDEX_PAGE[16] PROGMEM = "Measurements";
char TEXT_CONFIG_PAGE[14] PROGMEM = "Settings";

char TEXT_CONFIG_DEVICE_PAGE[24] PROGMEM = "Device Settings";
char TEXT_CONFIG_INFO_DEVICE_PAGE[144] PROGMEM = "The choice of sensors connected to Smogly, the frequency of measurements, or the language of the web interface can be changed in the section";
char TEXT_CONFIG_SERVICES_PAGE[24] PROGMEM = "Services Settings";
char TEXT_CONFIG_INFO_SERVICES_PAGE[144] PROGMEM = "Settings of such services as Luftdaten, aqi.eco, Smoglist, AirMonitor, ThingSpeak or InfluxDB or MQTT services can be changed in the section";

char TEXT_UPDATE_PAGE[14] PROGMEM = "Update";

char TEXT_WEATHER[10] PROGMEM = "Weather";
char TEXT_TEMPERATURE[14] PROGMEM = "Temperature";
char TEXT_HUMIDITY[14] PROGMEM = "Humidity";
char TEXT_PRESSURE[14] PROGMEM = "Pressure";
char TEXT_DEWPOINT[14] PROGMEM = "Dewpoint";

char TEXT_AIRPOLLUTION[26] PROGMEM = "Air pollution";
char TEXT_SAVED[10] PROGMEM = "SAVED!";

char TEXT_POSTCONFIG_INFO[60] PROGMEM = "everything looks OK, in a moment the Smogly will restart";
char TEXT_INSTRUCIONSLINK[76] PROGMEM = "All instructions and descriptions[in polish] are available {GITHUB_LINK}.";
char TEXT_DEVICENAME[20] PROGMEM = "Device Name";
char TEXT_DEVICENAMEAUTO[32] PROGMEM = "Automatic name generation";
char TEXT_SELECTEDLANGUAGE[10] PROGMEM = "Language";
char TEXT_TEMPHUMIPRESSSENSOR[40] PROGMEM = "Temp/Humi/Press Sensor";
char TEXT_PMSENSOR[20] PROGMEM = "PM2.5/PM10 Sensor";

char TEXT_FIRST_THP_SDA_SCL[40] PROGMEM = "First Temp/Humi Sensor SDA/SCL config";
char TEXT_FIRST_THP_SDA[8] PROGMEM = "Pin SDA";
char TEXT_FIRST_THP_SCL[8] PROGMEM = "Pin SCL";

char TEXT_SECOND_THP[40] PROGMEM = "Enable Second Temp/Humi Sensor";

char TEXT_SECOND_THP_SDA_SCL[48] PROGMEM = "Second Temp/Humi Sensor SDA/SCL config";
char TEXT_SECOND_THP_SDA[8] PROGMEM = "Pin SDA";
char TEXT_SECOND_THP_SCL[8] PROGMEM = "Pin SCL";

char TEXT_DUST_TX_RX[32] PROGMEM = "PM2.5/PM10 Sensor TX/RX config";
char TEXT_DUST_TX[8] PROGMEM = "Pin TX";
char TEXT_DUST_RX[8] PROGMEM = "Pin RX";

char TEXT_FREQUENTMEASUREMENTONOFF[26] PROGMEM = "Continuous measurements";
char TEXT_FREQUENTMEASUREMENTINFO[80] PROGMEM = "Continuous measurements significantly shorten the life span of the PM sensor.";
char TEXT_MEASUREMENTFREQUENCY[30] PROGMEM = "Make PM measurements every";
char TEXT_AVERAGELASTRESULT[30] PROGMEM = "Average result from last";
char TEXT_PMMEASUREMENTS[18] PROGMEM = "PM measurements";
char TEXT_SENDINGINTERVAL[50] PROGMEM = "Sending data to external services every";
char TEXT_SERVICESSENDINGINTERVAL[64] PROGMEM = "Sending data to AirMonitor/Luftdaten/Smoglist/aqi.eco every";
char TEXT_DBSENDINGINTERVAL[50] PROGMEM = "Sending data to ThingSpeak/InfluxDB/MQTT every";
char TEXT_SECONDS[10] PROGMEM = "seconds";
char TEXT_MINUTES[10] PROGMEM = "minutes";

char TEXT_DEEPSLEEPINFO[540] PROGMEM = "DeepSleep put the device into deep sleep between a series of measurements - significantly lower energy consumption (the possibility of working on the battery for several weeks), but lack of continuous access to the web interface. The web interface will only be available for about {INTERFACEWWWONTIME} seconds every {SENDING_FREQUENCY} minutes (and right after the device reboots). Sleep time will be the same as the period for sending measurements to external services. DeepSleep requires connection of D0 and RST pins on ESP8266!";
char TEXT_DEEPSLEEPINFO_ESP32[476] PROGMEM = "DeepSleep put the device into deep sleep between a series of measurements - significantly lower energy consumption (the possibility of working on the battery for several weeks), but lack of continuous access to the web interface. The web interface will only be available for about {INTERFACEWWWONTIME} seconds every {SENDING_FREQUENCY} minutes (and right after the device reboots). Sleep time will be the same as the period for sending measurements to external services.";

char TEXT_DISPLAYPM1[30] PROGMEM = "Display of PM1 measurements";
char TEXT_ALTITUDEINFO[116] PROGMEM = "Altitude above sea level (required for correct pressure measurements. You can check it {WSPOLRZEDNE_GPS_LINK})";

char TEXT_SECURECONFIGUPDATEPAGE[84] PROGMEM = "Secure the Configuration and Update pages(default: admin/password)";
char TEXT_SECURELOGIN[8] PROGMEM = "Login";
char TEXT_SECUREPASSWD[10] PROGMEM = "Password";
char TEXT_SECURELOGOUTINFO[48] PROGMEM = "Restart your web browser to log out!";

char TEXT_LUFTDATENSENDING[96] PROGMEM = "Sending data to the {LUFTDATEN_LINK} service(requires registration at {LUFTDATENFORM_LINK})";

char TEXT_SMOGLISTSENDING[48] PROGMEM = "Sending data to the {SMOGLIST_LINK} service";
char TEXT_SMOGLISTINFO[242] PROGMEM = "Smoglist is a service that collects Smogly usage statistics(we do not collect logins and passwords). It will allow us to improve the most commonly used functions in the future and generate our own charts.";

char TEXT_AIRMONITORSENDING[128] PROGMEM = "Sending data to the {AIRMONITOR_LINK} service(requires filling out {AIRMONITORFORM_LINK}; Sensor: e.g. {PMSENSORMODEL})";
char TEXT_AIRMONITORCHARTS[48] PROGMEM = "Displaying charts from the AirMonitor site";
char TEXT_AIRMONITOR_API_KEY[32] PROGMEM = "AirMonitor API Key";
char TEXT_AIRMONITORCOORDINATESINFO[74] PROGMEM = "Geographical coordinates(you can check it {LATLONG_LINK})";
char TEXT_AIRMONITORLATITUDE[24] PROGMEM = "Latitude";
char TEXT_AIRMONITORLONGITUDE[24] PROGMEM = "Longitude";

char TEXT_THINGSPEAKSENDING[50] PROGMEM = "Sending data to the {THINGSPEAK_LINK} service:";
char TEXT_THINGSPEAKCHARTS[48] PROGMEM = "Displaying charts from the ThingSpeak site";
char TEXT_THINGSPEAK_WRITE_API_KEY[28] PROGMEM = "ThingSpeak Write API Key";
char TEXT_THINGSPEAKCHANNELID[24] PROGMEM = "ThingSpeak Channel ID";
char TEXT_THINGSPEAK_READ_API_KEY[28] PROGMEM = "ThingSpeak Read API Key";

char TEXT_INFLUXDBSENDING[32] PROGMEM = "Sending data to the InfluxDB";
char TEXT_INFLUXDBVERSION[18] PROGMEM = "InfluxDB version";
char TEXT_INFLUXDBSERVER[30] PROGMEM = "InfluxDB database address";
char TEXT_INFLUXDBPORT[16] PROGMEM = "InfluxDB port";
char TEXT_INFLUXDBNAME[24] PROGMEM = "Name of the database";
char TEXT_INFLUXDBUSER[28] PROGMEM = "Database user";
char TEXT_INFLUXDBPASSWD[24] PROGMEM = "Database password";
char TEXT_INFLUXDBORG[16] PROGMEM = "Organization";
char TEXT_INFLUXDBBUCKET[10] PROGMEM = "Bucket";
char TEXT_INFLUXDBTOKEN[8] PROGMEM = "Token";

char TEXT_CONFIG_ADV_MQTT[22] PROGMEM = "MQTT Configuration";
char TEXT_MQTTSENDING[34] PROGMEM = "Sending data to the MQTT server";
char TEXT_MQTTSERVER[22] PROGMEM = "MQTT server address";
char TEXT_MQTTPORT[12] PROGMEM = "MQTT port";
char TEXT_MQTTUSER[20] PROGMEM = "MQTT user";
char TEXT_MQTTPASSWD[16] PROGMEM = "MQTT password";

char TEXT_MQTT_IP_IN_TOPIC[22] PROGMEM = "IP address in topic";
char TEXT_MQTT_DEVICENAME_IN_TOPIC[30] PROGMEM = "Device name in topic";
char TEXT_MQTT_TOPIC_INFO[256] PROGMEM = "Below you can format the way you want to send MQTT data. You can add the 'IP address' and 'Device name' at the beginning. You can specify the middle part yourself for each of the values measured by the device.";
char TEXT_MQTT_SLASH_AT_THE_BEGINNING[40] PROGMEM = "Add '/' at the beginning of the topic";
char TEXT_MQTT_SLASH_AT_THE_END[36] PROGMEM = "Add '/' at the end of the topic";

char TEXT_AQIECOSENDING[100] PROGMEM = "Sending data to the <a title='aqi.eco' href='https://aqi.eco' target='_blank'>aqi.eco</a> service";
char TEXT_AQIECOSERVER[26] PROGMEM = "aqi.eco server";
char TEXT_AQIECOPATH[20] PROGMEM = "aqi.eco path";

char TEXT_CALIBMETHOD[22] PROGMEM = "Calibration method";
char TEXT_CALIB1[30] PROGMEM = "calib1";
char TEXT_CALIB2[30] PROGMEM = "calib2";
char TEXT_SOFTWATEVERSION[24] PROGMEM = "Software version";

char TEXT_ERASEWIFICONFIG[22] PROGMEM = "Erase WiFi Config";
char TEXT_RESTORESETTINGS[32] PROGMEM = "Restore default settings";
char TEXT_SAVE[8] PROGMEM = "Save";
char TEXT_YES[6] PROGMEM = "Yes";
char TEXT_NO[6] PROGMEM = "No";
char TEXT_WITHOUTSENSOR[16] PROGMEM = "Without sensor";
char TEXT_WITHOUTCALIBRATION[22] PROGMEM = "Without calibration";
char TEXT_AUTOMATICCALIBRATION[26] PROGMEM = "Automatic calibration";

char TEXT_INTL_EN[12] PROGMEM = "english";
char TEXT_INTL_PL[8] PROGMEM = "polish";

char TEXT_FWUPDATEAVALIBLE[42] PROGMEM = "Firmware update available!";
char TEXT_AUTOUPDATEON[28] PROGMEM = "Autoupdate";
char TEXT_MANUALUPDATEBUTTON[22] PROGMEM = "Manual Upgrade";
char TEXT_FWUPDATEBUTTON[32] PROGMEM = "Firmware Upgrade";
char TEXT_AUTOUPDATEONBUTTON[36] PROGMEM = "Autoupdate On";
char TEXT_AUTOUPDATEWARNING[650] PROGMEM = "Automatic updates are performed via an unencrypted connection (HTTP, not HTTPS). It can be dangerous and allow interception of packets and hacking the device. You can use the Autoupdate once by clicking the \"{TEXT_FWUPDATEBUTTON}\" button or turn on Autoupdate permanently. You can change your mind at any time and disable Autoupdate in the Configuration. After selecting \"{TEXT_FWUPDATEBUTTON}\" or turning on Autoupdate, the device will save the current configuration and restart. It may take several seconds.";
char TEXT_UPDATEPAGEAUTOUPDATEWARNING[192] PROGMEM = "Automatic updates are performed via an unencrypted connection (HTTP, not HTTPS). It can be dangerous and allow interception of packets and hacking the device.";

char TEXT_SELECTUPDATEFILE[20] PROGMEM = "Select .bin file";
char TEXT_SUBMITUPDATE[10] PROGMEM = "Update!";
char TEXT_CURRENTSOFTVERSION[40] PROGMEM = "Currently used version of the firmware";
char TEXT_SERVERSOFTWAREVERSION[46] PROGMEM = "The latest available firmware version";
char TEXT_LATESTAVAILABLESOFT[82] PROGMEM = "Latest version of the software available {SMOGOMIERZRELEASES_LINK}.";

char TEXT_HERE[8] PROGMEM = "here";
char TEXT_THEFORM[12] PROGMEM = "the form";

char TEXT_CONNECTEDWIFI[28] PROGMEM = "Connected to a WiFi(SSID)";
char TEXT_WIFIRSSI[28] PROGMEM = "WiFi Signal Strength(RSSI)";
char TEXT_WIFIQUALITY[26] PROGMEM = "WiFi Signal Quality";

char TEXT_HOMEKIT_SUPPORT[26] PROGMEM = "Apple HomeKit Support";
char TEXT_HOMEKIT_IS_PAIRED[94] PROGMEM = "If you want to connect a new iOS/macOS device, you need to erase the current paired devices";
char TEXT_PAGE_HOMEKIT_RESET[30] PROGMEM = "Delete the paired device";

char TEXT_AIRQUALITY_TOPIC[12] PROGMEM = "Airquality";
