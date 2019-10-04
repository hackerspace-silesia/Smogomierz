#pragma once

extern bool DEVICENAME_AUTO;
extern char DEVICENAME[255];
extern bool DISPLAY_PM1;
extern char LANGUAGE[255];
extern int SELECTED_LANGUAGE;

extern char THP_MODEL[255];
extern char DUST_MODEL[255];
extern bool FREQUENTMEASUREMENT;

extern int DUST_TIME;
extern int NUMBEROFMEASUREMENTS;

extern bool LUFTDATEN_ON;
extern bool SMOGLIST_ON;

extern bool AIRMONITOR_ON;
extern bool AIRMONITOR_GRAPH_ON;
extern char LATITUDE[255];
extern char LONGITUDE[255];
extern int MYALTITUDE;

extern bool THINGSPEAK_ON;
extern bool THINGSPEAK_GRAPH_ON;
extern char THINGSPEAK_API_KEY[255];
extern int THINGSPEAK_CHANNEL_ID;

extern bool INFLUXDB_ON;
extern char INFLUXDB_HOST[255];
extern uint16_t INFLUXDB_PORT;
extern char INFLUXDB_DATABASE[255];
extern char DB_USER[255];
extern char DB_PASSWORD[255];

extern bool MQTT_ON;
extern char MQTT_HOST[255];
extern uint16_t MQTT_PORT;
extern char MQTT_USER[255];
extern char MQTT_PASSWORD[255];

extern int SENDING_FREQUENCY;
extern int SENDING_DB_FREQUENCY;
extern bool DEEPSLEEP_ON;

extern bool DEBUG;
extern bool AUTOUPDATE_ON;

extern bool CONFIG_AUTH;
extern char CONFIG_USERNAME[512];
extern char CONFIG_PASSWORD[512];

extern char MODEL[255];

const char PMSENSORVERSION[] = "PMS";
extern char SERVERSOFTWAREVERSION[255];
extern char CURRENTSOFTWAREVERSION[255];
#if defined(ARDUINO_ARCH_ESP8266)
const char SOFTWAREVERSION[] = "2.7.2 build " __TIME__ " " __DATE__;
const char HARDWAREVERSION[] = "1.0 - ESP8266";
#elif defined(ARDUINO_ARCH_ESP32)
const char SOFTWAREVERSION[40] PROGMEM = "2.7.2 build " __TIME__ " " __DATE__;
const char HARDWAREVERSION[40] PROGMEM = "2.0 - ESP32";
#endif