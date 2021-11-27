/*
 *
 *  default translation file - english translation
 *
 */

#pragma once

extern char TEXT_LANG[6] = "en";

extern char TEXT_INDEX_PAGE[16] = "Measurements";
extern char TEXT_CONFIG_PAGE[14] = "Settings";

extern char TEXT_CONFIG_DEVICE_PAGE[24] = "Device Settings";
extern char TEXT_CONFIG_INFO_DEVICE_PAGE[144] = "The choice of sensors connected to Smogly, the frequency of measurements, or the language of the web interface can be changed in the section";
extern char TEXT_CONFIG_SERVICES_PAGE[24] = "Services Settings";
extern char TEXT_CONFIG_INFO_SERVICES_PAGE[144] = "Settings of such services as Sensor.Community, aqi.eco, ThingSpeak, AirMonitor or InfluxDB or MQTT services can be changed in the section";

extern char TEXT_UPDATE_PAGE[14] = "Update";

extern char TEXT_WEATHER[10] = "Weather";
extern char TEXT_TEMPERATURE[14] = "Temperature";
extern char TEXT_HUMIDITY[14] = "Humidity";
extern char TEXT_PRESSURE[14] = "Pressure";
extern char TEXT_DEWPOINT[14] = "Dewpoint";

extern char TEXT_AIRPOLLUTION[26] = "Air pollution";
extern char TEXT_SAVED[10] = "SAVED!";

extern char TEXT_POSTCONFIG_INFO[60] = "everything looks OK, in a moment the Smogly will restart";
extern char TEXT_INSTRUCIONSLINK[76] = "All instructions and descriptions[in polish] are available {GITHUB_LINK}.";
extern char TEXT_DEVICENAME[20] = "Device Name";
extern char TEXT_DEVICENAMEAUTO[32] = "Automatic name generation";
extern char TEXT_SELECTEDLANGUAGE[10] = "Language";
extern char TEXT_TEMPHUMIPRESSSENSOR[40] = "Temp/Humi/Press Sensor";
extern char TEXT_PMSENSOR[20] = "PM2.5/PM10 Sensor";

extern char TEXT_FIRST_THP_SDA_SCL[40] = "First Temp/Humi Sensor SDA/SCL config";
extern char TEXT_FIRST_THP_SDA[8] = "Pin SDA";
extern char TEXT_FIRST_THP_SCL[8] = "Pin SCL";

extern char TEXT_SECOND_THP[40] = "Enable Second Temp/Humi Sensor";

extern char TEXT_SECOND_THP_SDA_SCL[48] = "Second Temp/Humi Sensor SDA/SCL config";
extern char TEXT_SECOND_THP_SDA[8] = "Pin SDA";
extern char TEXT_SECOND_THP_SCL[8] = "Pin SCL";

extern char TEXT_DUST_TX_RX[32] = "PM2.5/PM10 Sensor TX/RX config";
extern char TEXT_DUST_TX[8] = "Pin TX";
extern char TEXT_DUST_RX[8] = "Pin RX";

extern char TEXT_FREQUENTMEASUREMENTONOFF[26] = "Continuous measurements";
extern char TEXT_FREQUENTMEASUREMENTINFO[80] = "Continuous measurements significantly shorten the life span of the PM sensor.";
extern char TEXT_MEASUREMENTFREQUENCY[30] = "Make PM measurements every";
extern char TEXT_AVERAGELASTRESULT[30] = "Average result from last";
extern char TEXT_PMMEASUREMENTS[18] = "PM measurements";
extern char TEXT_SENDINGINTERVAL[50] = "Sending data to external services every";
extern char TEXT_SERVICESSENDINGINTERVAL[64] = "Sending data to Sensor.Community/aqi.eco/AirMonitor every";
extern char TEXT_DBSENDINGINTERVAL[50] = "Sending data to ThingSpeak/InfluxDB/MQTT every";
extern char TEXT_SECONDS[10] = "seconds";
extern char TEXT_MINUTES[10] = "minutes";

extern char TEXT_DEEPSLEEPINFO[540] = "DeepSleep put the device into deep sleep between a series of measurements - significantly lower energy consumption (the possibility of working on the battery for several weeks), but lack of continuous access to the web interface. The web interface will only be available for about {INTERFACEWWWONTIME} seconds every {SENDING_FREQUENCY} minutes (and right after the device reboots). Sleep time will be the same as the period for sending measurements to external services. DeepSleep requires connection of D0 and RST pins on ESP8266!";
extern char TEXT_DEEPSLEEPINFO_ESP32[476] = "DeepSleep put the device into deep sleep between a series of measurements - significantly lower energy consumption (the possibility of working on the battery for several weeks), but lack of continuous access to the web interface. The web interface will only be available for about {INTERFACEWWWONTIME} seconds every {SENDING_FREQUENCY} minutes (and right after the device reboots). Sleep time will be the same as the period for sending measurements to external services.";

extern char TEXT_DISPLAYPM1[30] = "Display of PM1 measurements";
extern char TEXT_ALTITUDEINFO[116] = "Altitude above sea level (required for correct pressure measurements. You can check it {WSPOLRZEDNE_GPS_LINK})";

extern char TEXT_SECURECONFIGUPDATEPAGE[84] = "Secure the Configuration and Update pages(default: admin/password)";
extern char TEXT_SECURELOGIN[8] = "Login";
extern char TEXT_SECUREPASSWD[10] = "Password";
extern char TEXT_SECURELOGOUTINFO[48] = "Restart your web browser to log out!";

extern char TEXT_LUFTDATENSENDING[96] = "Sending data to the {LUFTDATEN_LINK} service(requires registration at {LUFTDATENFORM_LINK})";
extern char TEXT_LUFTDATEN_GRAPH_ON[64] = "Displaying charts from the Sensor.Community";

extern char TEXT_SMOGLISTSENDING[48] = "Sending data to the {SMOGLIST_LINK} service";
extern char TEXT_SMOGLISTINFO[242] = "Smoglist is a service that collects Smogly usage statistics(we do not collect logins and passwords). It will allow us to improve the most commonly used functions in the future and generate our own charts.";

extern char TEXT_AIRMONITORSENDING[128] = "Sending data to the {AIRMONITOR_LINK} service(requires filling out {AIRMONITORFORM_LINK}; Sensor: e.g. {PMSENSORMODEL})";
extern char TEXT_AIRMONITORCHARTS[48] = "Displaying charts from the AirMonitor site";
extern char TEXT_AIRMONITOR_API_KEY[32] = "AirMonitor API Key";

extern char TEXT_MY_COORDINATESINFO[74] = "Geographical coordinates(you can check it {LATLONG_LINK})";
extern char TEXT_MY_LATITUDE[24] = "Latitude";
extern char TEXT_MY_LONGITUDE[24] = "Longitude";

extern char TEXT_THINGSPEAKSENDING[50] = "Sending data to the {THINGSPEAK_LINK} service:";
extern char TEXT_THINGSPEAKCHARTS[48] = "Displaying charts from the ThingSpeak site";
extern char TEXT_THINGSPEAK_WRITE_API_KEY[28] = "ThingSpeak Write API Key";
extern char TEXT_THINGSPEAKCHANNELID[24] = "ThingSpeak Channel ID";
extern char TEXT_THINGSPEAK_READ_API_KEY[28] = "ThingSpeak Read API Key";

extern char TEXT_INFLUXDBSENDING[32] = "Sending data to the InfluxDB";
extern char TEXT_INFLUXDBVERSION[18] = "InfluxDB version";
extern char TEXT_INFLUXDBSERVER[30] = "InfluxDB database address";
extern char TEXT_INFLUXDBPORT[16] = "InfluxDB port";
extern char TEXT_INFLUXDBNAME[24] = "Name of the database";
extern char TEXT_INFLUXDBUSER[28] = "Database user";
extern char TEXT_INFLUXDBPASSWD[24] = "Database password";
extern char TEXT_INFLUXDBORG[16] = "Organization";
extern char TEXT_INFLUXDBBUCKET[10] = "Bucket";
extern char TEXT_INFLUXDBTOKEN[8] = "Token";

extern char TEXT_CONFIG_ADV_MQTT[22] = "MQTT Configuration";
extern char TEXT_MQTTSENDING[34] = "Sending data to the MQTT server";
extern char TEXT_MQTTSERVER[22] = "MQTT server address";
extern char TEXT_MQTTPORT[12] = "MQTT port";
extern char TEXT_MQTTUSER[20] = "MQTT user";
extern char TEXT_MQTTPASSWD[16] = "MQTT password";

extern char TEXT_MQTT_IP_IN_TOPIC[22] = "IP address in topic";
extern char TEXT_MQTT_DEVICENAME_IN_TOPIC[30] = "Device name in topic";
extern char TEXT_MQTT_TOPIC_INFO[256] = "Below you can format the way you want to send MQTT data. You can add the 'IP address' and 'Device name' at the beginning. You can specify the middle part yourself for each of the values measured by the device.";
extern char TEXT_MQTT_SLASH_AT_THE_BEGINNING[40] = "Add '/' at the beginning of the topic";
extern char TEXT_MQTT_SLASH_AT_THE_END[36] = "Add '/' at the end of the topic";

extern char TEXT_AQIECOSENDING[100] = "Sending data to the <a title='aqi.eco' href='https://aqi.eco' target='_blank'>aqi.eco</a> service";
extern char TEXT_AQIECOSERVER[26] = "aqi.eco server";
extern char TEXT_AQIECOPATH[20] = "aqi.eco path";

extern char TEXT_CALIBMETHOD[22] = "Calibration method";
extern char TEXT_CALIB1[30] = "calib1";
extern char TEXT_CALIB2[30] = "calib2";
extern char TEXT_SOFTWATEVERSION[24] = "Software version";

extern char TEXT_ERASEWIFICONFIG[22] = "Erase WiFi Config";
extern char TEXT_RESTORESETTINGS[32] = "Restore default settings";
extern char TEXT_SAVE[8] = "Save";
extern char TEXT_YES[6] = "Yes";
extern char TEXT_NO[6] = "No";
extern char TEXT_WITHOUTSENSOR[16] = "Without sensor";
extern char TEXT_WITHOUTCALIBRATION[22] = "Without calibration";
extern char TEXT_AUTOMATICCALIBRATION[26] = "Automatic calibration";

extern char TEXT_INTL_EN[12] = "english";
extern char TEXT_INTL_PL[8] = "polish";

extern char TEXT_FWUPDATEAVALIBLE[42] = "Firmware update available!";
extern char TEXT_AUTOUPDATEON[28] = "Autoupdate";
extern char TEXT_MANUALUPDATEBUTTON[22] = "Manual Upgrade";
extern char TEXT_FWUPDATEBUTTON[32] = "Firmware Upgrade";
extern char TEXT_AUTOUPDATEONBUTTON[36] = "Autoupdate On";
extern char TEXT_AUTOUPDATEWARNING[650] = "Automatic updates are performed via an unencrypted connection (HTTP, not HTTPS). It can be dangerous and allow interception of packets and hacking the device. You can use the Autoupdate once by clicking the \"{TEXT_FWUPDATEBUTTON}\" button or turn on Autoupdate permanently. You can change your mind at any time and disable Autoupdate in the Configuration. After selecting \"{TEXT_FWUPDATEBUTTON}\" or turning on Autoupdate, the device will save the current configuration and restart. It may take several seconds.";
extern char TEXT_UPDATEPAGEAUTOUPDATEWARNING[192] = "Automatic updates are performed via an unencrypted connection (HTTP, not HTTPS). It can be dangerous and allow interception of packets and hacking the device.";

extern char TEXT_SELECTUPDATEFILE[20] = "Select .bin file";
extern char TEXT_SUBMITUPDATE[10] = "Update!";
extern char TEXT_CURRENTSOFTVERSION[40] = "Currently used version of the firmware";
extern char TEXT_SERVERSOFTWAREVERSION[46] = "The latest available firmware version";
extern char TEXT_LATESTAVAILABLESOFT[82] = "Latest version of the software available {SMOGOMIERZRELEASES_LINK}.";

extern char TEXT_HERE[8] = "here";
extern char TEXT_THEFORM[12] = "the form";

extern char TEXT_CONNECTEDWIFI[28] = "Connected to a WiFi(SSID)";
extern char TEXT_WIFIRSSI[28] = "WiFi Signal Strength(RSSI)";
extern char TEXT_WIFIQUALITY[26] = "WiFi Signal Quality";

extern char TEXT_HOMEKIT_SUPPORT[26] = "Apple HomeKit Support";
extern char TEXT_HOMEKIT_IS_PAIRED[94] = "If you want to connect a new iOS/macOS device, you need to erase the current paired devices";
extern char TEXT_PAGE_HOMEKIT_RESET[30] = "Delete the paired device";

extern char TEXT_AIRQUALITY_TOPIC[12] = "Airquality";
