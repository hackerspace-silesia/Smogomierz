/*
 *  
 *  translation file - english translation
 *  
 */

#pragma once
#ifdef ARDUINO_ARCH_ESP8266
#include <avr/pgmspace.h>
#endif

char EN_INTL_LANG[6] PROGMEM = "en";

char EN_INTL_INDEX_PAGE[16] PROGMEM = "Measurements";
char EN_INTL_CONFIG_PAGE[14] PROGMEM = "Settings";

char EN_INTL_CONFIG_DEVICE_PAGE[24] PROGMEM = "Device Settings";
char EN_INTL_CONFIG_INFO_DEVICE_PAGE[144] PROGMEM = "The choice of sensors connected to Smogly, the frequency of measurements, or the language of the web interface can be changed in the section";
char EN_INTL_CONFIG_SERVICES_PAGE[24] PROGMEM = "Services Settings";
char EN_INTL_CONFIG_INFO_SERVICES_PAGE[144] PROGMEM = "Settings of such services as Sensor.Community, aqi.eco, ThingSpeak, AirMonitor or InfluxDB or MQTT services can be changed in the section";

char EN_INTL_UPDATE_PAGE[14] PROGMEM = "Update";

char EN_INTL_WEATHER[10] PROGMEM = "Weather";
char EN_INTL_TEMPERATURE[14] PROGMEM = "Temperature";
char EN_INTL_HUMIDITY[14] PROGMEM = "Humidity";
char EN_INTL_PRESSURE[14] PROGMEM = "Pressure";
char EN_INTL_DEWPOINT[14] PROGMEM = "Dewpoint";

char EN_INTL_AIRPOLLUTION[26] PROGMEM = "Air pollution";
char EN_INTL_SAVED[10] PROGMEM = "SAVED!";

char EN_INTL_POSTCONFIG_INFO[60] PROGMEM = "everything looks OK, in a moment the Smogly will restart";
char EN_INTL_INSTRUCIONSLINK[76] PROGMEM = "All instructions and descriptions[in polish] are available {GITHUB_LINK}.";
char EN_INTL_DEVICENAME[20] PROGMEM = "Device Name";
char EN_INTL_DEVICENAMEAUTO[32] PROGMEM = "Automatic name generation";
char EN_INTL_SELECTEDLANGUAGE[10] PROGMEM = "Language";
char EN_INTL_TEMPHUMIPRESSSENSOR[40] PROGMEM = "Temp/Humi/Press Sensor";
char EN_INTL_PMSENSOR[20] PROGMEM = "PM2.5/PM10 Sensor";

char EN_INTL_FIRST_THP_SDA_SCL[40] PROGMEM = "First Temp/Humi Sensor SDA/SCL config";
char EN_INTL_FIRST_THP_SDA[8] PROGMEM = "Pin SDA";
char EN_INTL_FIRST_THP_SCL[8] PROGMEM = "Pin SCL";

char EN_INTL_SECOND_THP[40] PROGMEM = "Enable Second Temp/Humi Sensor";

char EN_INTL_SECOND_THP_SDA_SCL[48] PROGMEM = "Second Temp/Humi Sensor SDA/SCL config";
char EN_INTL_SECOND_THP_SDA[8] PROGMEM = "Pin SDA";
char EN_INTL_SECOND_THP_SCL[8] PROGMEM = "Pin SCL";

char EN_INTL_DUST_TX_RX[32] PROGMEM = "PM2.5/PM10 Sensor TX/RX config";
char EN_INTL_DUST_TX[8] PROGMEM = "Pin TX";
char EN_INTL_DUST_RX[8] PROGMEM = "Pin RX";

char EN_INTL_FREQUENTMEASUREMENTONOFF[26] PROGMEM = "Continuous measurements";
char EN_INTL_FREQUENTMEASUREMENTINFO[80] PROGMEM = "Continuous measurements significantly shorten the life span of the PM sensor.";
char EN_INTL_MEASUREMENTFREQUENCY[30] PROGMEM = "Make PM measurements every";
char EN_INTL_AVERAGELASTRESULT[30] PROGMEM = "Average result from last";
char EN_INTL_PMMEASUREMENTS[18] PROGMEM = "PM measurements";
char EN_INTL_SENDINGINTERVAL[50] PROGMEM = "Sending data to external services every";
char EN_INTL_SERVICESSENDINGINTERVAL[64] PROGMEM = "Sending data to Sensor.Community/aqi.eco/AirMonitor every";
char EN_INTL_DBSENDINGINTERVAL[50] PROGMEM = "Sending data to ThingSpeak/InfluxDB/MQTT every";
char EN_INTL_SECONDS[10] PROGMEM = "seconds";
char EN_INTL_MINUTES[10] PROGMEM = "minutes";

char EN_INTL_DEEPSLEEPINFO[540] PROGMEM = "DeepSleep put the device into deep sleep between a series of measurements - significantly lower energy consumption (the possibility of working on the battery for several weeks), but lack of continuous access to the web interface. The web interface will only be available for about {INTERFACEWWWONTIME} seconds every {SENDING_FREQUENCY} minutes (and right after the device reboots). Sleep time will be the same as the period for sending measurements to external services. DeepSleep requires connection of D0 and RST pins on ESP8266!";
char EN_INTL_DEEPSLEEPINFO_ESP32[476] PROGMEM = "DeepSleep put the device into deep sleep between a series of measurements - significantly lower energy consumption (the possibility of working on the battery for several weeks), but lack of continuous access to the web interface. The web interface will only be available for about {INTERFACEWWWONTIME} seconds every {SENDING_FREQUENCY} minutes (and right after the device reboots). Sleep time will be the same as the period for sending measurements to external services.";

char EN_INTL_DISPLAYPM1[30] PROGMEM = "Display of PM1 measurements";
char EN_INTL_ALTITUDEINFO[116] PROGMEM = "Altitude above sea level (required for correct pressure measurements. You can check it {WSPOLRZEDNE_GPS_LINK})";

char EN_INTL_SECURECONFIGUPDATEPAGE[84] PROGMEM = "Secure the Configuration and Update pages(default: admin/password)";
char EN_INTL_SECURELOGIN[8] PROGMEM = "Login";
char EN_INTL_SECUREPASSWD[10] PROGMEM = "Password";
char EN_INTL_SECURELOGOUTINFO[48] PROGMEM = "Restart your web browser to log out!";

char EN_INTL_LUFTDATENSENDING[96] PROGMEM = "Sending data to the {LUFTDATEN_LINK} service(requires registration at {LUFTDATENFORM_LINK})"; 
char EN_INTL_LUFTDATEN_GRAPH_ON[64] = "Displaying charts from the Sensor.Community";

char EN_INTL_SMOGLISTSENDING[48] PROGMEM = "Sending data to the {SMOGLIST_LINK} service";
char EN_INTL_SMOGLISTINFO[242] = "Smoglist is a service that collects Smogly usage statistics(we do not collect logins and passwords). It will allow us to improve the most commonly used functions in the future and generate our own charts.";

char EN_INTL_AIRMONITORSENDING[128] PROGMEM = "Sending data to the {AIRMONITOR_LINK} service(requires filling out {AIRMONITORFORM_LINK}; Sensor: e.g. {PMSENSORMODEL})";
char EN_INTL_AIRMONITORCHARTS[48] PROGMEM = "Displaying charts from the AirMonitor site";

char EN_INTL_MY_COORDINATESINFO[74] PROGMEM = "Geographical coordinates(you can check it {LATLONG_LINK})";
char EN_INTL_MY_LATITUDE[24] PROGMEM = "Latitude";
char EN_INTL_MY_LONGITUDE[24] PROGMEM = "Longitude";

char EN_INTL_THINGSPEAKSENDING[50] PROGMEM = "Sending data to the {THINGSPEAK_LINK} service:";
char EN_INTL_THINGSPEAKCHARTS[48] PROGMEM = "Displaying charts from the ThingSpeak site";
char EN_INTL_THINGSPEAK_WRITE_API_KEY[28] PROGMEM = "ThingSpeak Write API Key";
char EN_INTL_THINGSPEAKCHANNELID[24] PROGMEM = "ThingSpeak Channel ID";
char EN_INTL_THINGSPEAK_READ_API_KEY[28] PROGMEM = "ThingSpeak Read API Key";

char EN_INTL_INFLUXDBSENDING[32] PROGMEM = "Sending data to the InfluxDB";
char EN_INTL_INFLUXDBVERSION[18] PROGMEM = "InfluxDB version";
char EN_INTL_INFLUXDBSERVER[30] PROGMEM = "InfluxDB database address";
char EN_INTL_INFLUXDBPORT[16] PROGMEM = "InfluxDB port";
char EN_INTL_INFLUXDBNAME[24] PROGMEM = "Name of the database";
char EN_INTL_INFLUXDBUSER[28] PROGMEM = "Database user";
char EN_INTL_INFLUXDBPASSWD[24] PROGMEM = "Database password";
char EN_INTL_INFLUXDBORG[16] PROGMEM = "Organization";
char EN_INTL_INFLUXDBBUCKET[10] PROGMEM = "Bucket";
char EN_INTL_INFLUXDBTOKEN[8] PROGMEM = "Token";

char EN_INTL_CONFIG_ADV_MQTT[22] PROGMEM = "MQTT Configuration";
char EN_INTL_MQTTSENDING[34] PROGMEM = "Sending data to the MQTT server";
char EN_INTL_MQTTSERVER[22] PROGMEM = "MQTT server address";
char EN_INTL_MQTTPORT[12] PROGMEM = "MQTT port";
char EN_INTL_MQTTUSER[20] PROGMEM = "MQTT user";
char EN_INTL_MQTTPASSWD[16] PROGMEM = "MQTT password";

char EN_INTL_MQTT_IP_IN_TOPIC[22] PROGMEM = "IP address in topic";
char EN_INTL_MQTT_DEVICENAME_IN_TOPIC[30] PROGMEM = "Device name in topic";
char EN_INTL_MQTT_TOPIC_INFO[256] PROGMEM = "Below you can format the way you want to send MQTT data. You can add the 'IP address' and 'Device name' at the beginning. You can specify the middle part yourself for each of the values measured by the device.";
char EN_INTL_MQTT_SLASH_AT_THE_BEGINNING[40] PROGMEM = "Add '/' at the beginning of the topic";
char EN_INTL_MQTT_SLASH_AT_THE_END[36] PROGMEM = "Add '/' at the end of the topic";

char EN_INTL_AQIECOSENDING[100] PROGMEM = "Sending data to the <a title='aqi.eco' href='https://aqi.eco' target='_blank'>aqi.eco</a> service";
char EN_INTL_AQIECOSERVER[26] PROGMEM = "aqi.eco server";
char EN_INTL_AQIECOPATH[20] PROGMEM = "aqi.eco path";

char EN_INTL_CALIBMETHOD[22] PROGMEM = "Calibration method";
char EN_INTL_CALIB1[30] PROGMEM = "calib1";
char EN_INTL_CALIB2[30] PROGMEM = "calib2";
char EN_INTL_SOFTWATEVERSION[24] PROGMEM = "Software version";

char EN_INTL_ERASEWIFICONFIG[22] PROGMEM = "Erase WiFi Config";
char EN_INTL_RESTORESETTINGS[32] PROGMEM = "Restore default settings";
char EN_INTL_SAVE[8] PROGMEM = "Save";
char EN_INTL_YES[6] PROGMEM = "Yes";
char EN_INTL_NO[6] PROGMEM = "No";
char EN_INTL_WITHOUTSENSOR[16] PROGMEM = "Without sensor";
char EN_INTL_WITHOUTCALIBRATION[22] PROGMEM = "Without calibration";
char EN_INTL_AUTOMATICCALIBRATION[26] PROGMEM = "Automatic calibration";

char EN_INTL_INTL_EN[12] PROGMEM = "english";
char EN_INTL_INTL_PL[8] PROGMEM = "polish";

char EN_INTL_FWUPDATEAVALIBLE[42] PROGMEM = "Firmware update available!";
char EN_INTL_AUTOUPDATEON[28] PROGMEM = "Autoupdate";
char EN_INTL_MANUALUPDATEBUTTON[22] PROGMEM = "Manual Upgrade";
char EN_INTL_FWUPDATEBUTTON[32] PROGMEM = "Firmware Upgrade";
char EN_INTL_AUTOUPDATEONBUTTON[36] PROGMEM = "Autoupdate On";
char EN_INTL_AUTOUPDATEWARNING[650] PROGMEM = "Automatic updates are performed via an unencrypted connection (HTTP, not HTTPS). It can be dangerous and allow interception of packets and hacking the device. You can use the Autoupdate once by clicking the \"{TEXT_FWUPDATEBUTTON}\" button or turn on Autoupdate permanently. You can change your mind at any time and disable Autoupdate in the Configuration. After selecting \"{TEXT_FWUPDATEBUTTON}\" or turning on Autoupdate, the device will save the current configuration and restart. It may take several seconds.";
char EN_INTL_UPDATEPAGEAUTOUPDATEWARNING[192] PROGMEM = "Automatic updates are performed via an unencrypted connection (HTTP, not HTTPS). It can be dangerous and allow interception of packets and hacking the device.";

char EN_INTL_SELECTUPDATEFILE[20] PROGMEM = "Select .bin file";
char EN_INTL_SUBMITUPDATE[10] PROGMEM = "Update!";
char EN_INTL_CURRENTSOFTVERSION[40] PROGMEM = "Currently used version of the firmware";
char EN_INTL_SERVERSOFTWAREVERSION[46] PROGMEM = "The latest available firmware version";
char EN_INTL_LATESTAVAILABLESOFT[82] PROGMEM = "Latest version of the software available {SMOGOMIERZRELEASES_LINK}.";

char EN_INTL_HERE[8] PROGMEM = "here";
char EN_INTL_THEFORM[12] PROGMEM = "the form";

char EN_INTL_CONNECTEDWIFI[28] PROGMEM = "Connected to a WiFi(SSID)";
char EN_INTL_WIFIRSSI[28] PROGMEM = "WiFi Signal Strength(RSSI)";
char EN_INTL_WIFIQUALITY[26] PROGMEM = "WiFi Signal Quality";

char EN_INTL_HOMEKIT_SUPPORT[26] PROGMEM = "Apple HomeKit Support";
char EN_INTL_HOMEKIT_IS_PAIRED[94] = "If you want to connect a new iOS/macOS device, you need to erase the current paired devices";
char EN_INTL_PAGE_HOMEKIT_RESET[30] = "Delete the paired device";