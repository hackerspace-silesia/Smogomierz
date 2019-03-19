/*
 *  
 *  default translation file - english translation
 *  
 */

#pragma once

String TEXT_LANG = "en";

String TEXT_INDEX_PAGE = "Measurements";
String TEXT_CONFIG_PAGE = "Configuration";
String TEXT_UPDATE_PAGE = "Update";

String TEXT_WEATHER = "Weather";
String TEXT_TEMPERATURE = "Temperature";
String TEXT_HUMIDITY = "Humidity";
String TEXT_PRESSURE = "Pressure";
String TEXT_DEWPOINT = "Dewpoint";

String TEXT_AIRPOLLUTION = "Air pollution";
String TEXT_SAVED = "SAVED!";

String TEXT_POSTCONFIG_INFO = "everything looks OK, in a moment the Smogomierz will restart";
String TEXT_INSTRUCIONSLINK = "All instructions and descriptions[in polish] are available {GITHUB_LINK}.";
String TEXT_DEVICENAME = "Device Name";
String TEXT_DEVICENAMEAUTO = "Automatic name generation";
String TEXT_SELECTEDLANGUAGE = "Language";
String TEXT_TEMPHUMIPRESSSENSOR = "Temp/Humi/Press Sensor";
String TEXT_PMSENSOR = "PM2.5/PM10 Sensor";

String TEXT_FREQUENTMEASUREMENTONOFF = "Frequent measurement";
String TEXT_FREQUENTMEASUREMENTINFO = "frequent measurements - every few seconds, shorten the life span of the PM sensor.";
String TEXT_MEASUREMENTFREQUENCY = "Make PM measurements every";
String TEXT_AVERAGELASTRESULT = "Average result from last";
String TEXT_PMMEASUREMENTS = "PM measurements";
String TEXT_SENDINGINTERVAL = "Sending measurements to external services every";
String TEXT_SECONDS = "seconds";
String TEXT_MINUTES = "minutes";
String TEXT_DISPLAYPM1 = "Display of PM1 measurements";
String TEXT_ALTITUDEINFO = "Altitude above sea level (required for correct pressure measurements. You can check it {WSPOLRZEDNE_GPS_LINK})";

String TEXT_SECURECONFIGUPDATEPAGE = "Secure the Configuration and Update pages(default: admin/password)";
String TEXT_SECURELOGIN = "Login";
String TEXT_SECUREPASSWD = "Password";
String TEXT_SECURELOGOUTINFO = "Restart your web browser to log out!";

String TEXT_LUFTDATENSENDING = "Sending data to the {LUFTDATEN_LINK} service(requires filling out {LUFTDATENFORM_LINK})"; 

String TEXT_AIRMONITORSENDING = "Sending data to the {AIRMONITOR_LINK} service(requires filling out {AIRMONITORFORM_LINK}; Sensor: e.g. PMS7003)";
String TEXT_AIRMONITORCHARTS = "Displaying charts from the AirMonitor site";
String TEXT_AIRMONITORCOORDINATESINFO = "Geographical coordinates(you can check it {LATLONG_LINK}";
String TEXT_AIRMONITORLATITUDE = "Latitude";
String TEXT_AIRMONITORLONGITUDE = "Longitude";

String TEXT_THINGSPEAKSENDING = "Sending data to the {THINGSPEAK_LINK} service:";
String TEXT_THINGSPEAKCHARTS = "Displaying charts from the ThingSpeak site";
String TEXT_THINGSPEAKAPIKEY = "ThingSpeak API_KEY";
String TEXT_THINGSPEAKCHANNELID = "ThingSpeak Channel ID";

String TEXT_INFLUXDBSENDING = "Sending data to the InfluxDB";
String TEXT_INFLUXDBSERVER = "InfluxDB database address";
String TEXT_INFLUXDBPORT = "InfluxDB port";
String TEXT_INFLUXDBNAME = "Name of the database";
String TEXT_INFLUXDBUSER = "Database user";
String TEXT_INFLUXDBPASSWD = "Database password";

String TEXT_MQTTSENDING = "Sending data to the MQTT server";
String TEXT_MQTTSERVER = "MQTT server address";
String TEXT_MQTTPORT = "MQTT port";
String TEXT_MQTTUSER = "MQTT user";
String TEXT_MQTTPASSWD = "MQTT password";

String TEXT_CALIBMETHOD = "Calibration method";
String TEXT_CALIB1 = "calib1";
String TEXT_CALIB2 = "calib2";
String TEXT_SOFTWATEVERSION = "Software version";

String TEXT_ERASEWIFICONFIG = "Erase WiFi Config";
String TEXT_RESTORESETTINGS = "Restore default settings";
String TEXT_SAVE = "Save";
String TEXT_YES = "yes";
String TEXT_NO = "no";
String TEXT_WITHOUTSENSOR = "Without sensor";
String TEXT_WITHOUTCALIBRATION = "Without calibration";
String TEXT_AUTOMATICCALIBRATION = "Automatic calibration";

String TEXT_INTL_EN = "english";
String TEXT_INTL_PL = "polish";
// future translations gose here!

String TEXT_SELECTUPDATEFILE = "Select .bin file";
String TEXT_SUBMITUPDATE = "Update!";
String TEXT_CURRENTSOFTVERSION = "The currently used version of the software";
String TEXT_LATESTAVAILABLESOFT = "Latest version of the software available {SMOGOMIERZRELEASES_LINK}.";

String TEXT_HERE = "here";
String TEXT_THEFORM = "the form";

String GITHUB_LINK = "<a title='Instructions' href='https://github.com/hackerspace-silesia/Smogomierz#instrukcje' target='_blank'>{TEXT_HERE}</a>";
String WSPOLRZEDNE_GPS_LINK = "<a title='wspolrzedne-gps.pl' href='https://www.wspolrzedne-gps.pl' target='_blank'>{TEXT_HERE}</a>";
String LUFTDATEN_LINK = "<a title='LuftDaten.info' href='https://luftdaten.info/en/home-en/' target='_blank'>LuftDaten.info</a>";
String LUFTDATENFORM_LINK = "<a title='LuftDaten.info Form' href='https://luftdaten.info/en/construction-manual/#feinstaubsensor-konfiguration' target='_blank'>{TEXT_THEFORM}</a>";
String AIRMONITOR_LINK = "<a title='AirMonitor' href='http://mapa.airmonitor.pl' target='_blank'>AirMonitor</a>";
String AIRMONITORFORM_LINK = "<a title='AirMonitor Form' href='https://docs.google.com/forms/d/e/1FAIpQLSdw72_DggyrK7xnSQ1nR11Y-YK4FYWk_MF9QbecpOERql-T2w/viewform' target='_blank'>{TEXT_THEFORM}</a>";
String LATLONG_LINK = "<a title='latlong.net' href='https://www.latlong.net' target='_blank'>{TEXT_HERE}</a>";
String THINGSPEAK_LINK = "<a title='ThingSpeak' href='https://thingspeak.com' target='_blank'>ThingSpeak</a>";
String SMOGOMIERZRELEASES_LINK = "<b><a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/releases'>{TEXT_HERE}</a></b>";
