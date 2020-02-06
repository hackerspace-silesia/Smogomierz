#pragma once

extern bool DEVICENAME_AUTO;
extern char DEVICENAME[32];
extern bool DISPLAY_PM1;
extern char LANGUAGE[32];
extern int SELECTED_LANGUAGE;

extern char THP_MODEL[32];
extern char DUST_MODEL[32];
extern bool FREQUENTMEASUREMENT;

extern int DUST_TIME;
extern int NUMBEROFMEASUREMENTS;

extern bool LUFTDATEN_ON;
extern bool SMOGLIST_ON;

extern bool AIRMONITOR_ON;
extern bool AIRMONITOR_GRAPH_ON;
extern char LATITUDE[16];
extern char LONGITUDE[16];
extern int MYALTITUDE;

extern bool THINGSPEAK_ON;
extern bool THINGSPEAK_GRAPH_ON;
extern char THINGSPEAK_API_KEY[32];
extern int THINGSPEAK_CHANNEL_ID;
extern char THINGSPEAK_READ_API_KEY[32];

extern bool INFLUXDB_ON;
extern char INFLUXDB_VERSION[16];
extern char INFLUXDB_HOST[128];
extern uint16_t INFLUXDB_PORT;
extern char INFLUXDB_DATABASE[64];
extern char INFLUXDB_USER[64];
extern char INFLUXDB_PASSWORD[64];
extern char INFLUXDB_ORG[64];
extern char INFLUXDB_BUCKET[64];
extern char INFLUXDB_TOKEN[64];

extern bool MQTT_ON;
extern char MQTT_HOST[128];
extern uint16_t MQTT_PORT;
extern char MQTT_USER[64];
extern char MQTT_PASSWORD[64];

extern bool  MQTT_IP_IN_TOPIC;
extern bool  MQTT_DEVICENAME_IN_TOPIC;

extern char MQTT_TOPIC_TEMP[128];
extern char MQTT_TOPIC_HUMI[128];
extern char MQTT_TOPIC_PRESS[128];
extern char MQTT_TOPIC_PM1[128];
extern char MQTT_TOPIC_PM25[128];
extern char MQTT_TOPIC_PM10[128];
extern char MQTT_TOPIC_AIRQUALITY[128];

extern bool AQI_ECO_ON;
extern char AQI_ECO_HOST[128];
extern char AQI_ECO_PATH[64];

extern int SENDING_FREQUENCY;
extern int SENDING_DB_FREQUENCY;
extern bool DEEPSLEEP_ON;

extern bool DEBUG;
extern bool AUTOUPDATE_ON;

extern bool CONFIG_AUTH;
extern char CONFIG_USERNAME[256];
extern char CONFIG_PASSWORD[256];

extern char MODEL[32];

#ifdef DUSTSENSOR_PMS5003_7003_BME280_0x76 or DUSTSENSOR_PMS5003_7003_BME280_0x77
const char PMSENSORVERSION[] = "PMS";
#elif defined DUSTSENSOR_SDS011_21
const char PMSENSORVERSION[] = "SDS";
#elif defined DUSTSENSOR_HPMA115S0
const char PMSENSORVERSION[] = "HPMA115S0";
#elif defined DUSTSENSOR_SPS30
const char PMSENSORVERSION[] = "SPS30";
#else // If no dust sensor has been defined - use DUSTSENSOR_PMS5003_7003_BME280_0x76
const char PMSENSORVERSION[] = "PMS";
#endif

extern char SERVERSOFTWAREVERSION[32];
extern char CURRENTSOFTWAREVERSION[32];
#if defined(ARDUINO_ARCH_ESP8266)
const char SOFTWAREVERSION[] = "2.7.5 build " __TIME__ " " __DATE__;
const char HARDWAREVERSION[] = "1.0 - ESP8266";
#elif defined(ARDUINO_ARCH_ESP32)
const char SOFTWAREVERSION[] = "2.7.5 build " __TIME__ " " __DATE__;
const char HARDWAREVERSION[] = "2.0 - ESP32";
#endif