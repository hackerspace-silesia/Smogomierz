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
extern char INFLUXDB_HOST[128];
extern uint16_t INFLUXDB_PORT;
extern char INFLUXDB_DATABASE[64];
extern char DB_USER[64];
extern char DB_PASSWORD[64];

extern bool MQTT_ON;
extern char MQTT_HOST[128];
extern uint16_t MQTT_PORT;
extern char MQTT_USER[64];
extern char MQTT_PASSWORD[64];

extern bool  MQTT_IP_IN_TOPIC;

extern char MQTT_SENSOR_PREFIX[32];

extern char MQTT_VALNAME_TEMP[32];
extern char MQTT_VALNAME_HUMI[32];
extern char MQTT_VALNAME_PRESS[32];
extern char MQTT_VALNAME_PM1[32];
extern char MQTT_VALNAME_PM25[32];
extern char MQTT_VALNAME_PM10[32];
extern char MQTT_VALNAME_AIRQUALITY[32];

extern char MQTT_TSKNAME_TEMP[32];
extern char MQTT_TSKNAME_HUMI[32];
extern char MQTT_TSKNAME_PRESS[32];
extern char MQTT_TSKNAME_PM1[32];
extern char MQTT_TSKNAME_PM25[32];
extern char MQTT_TSKNAME_PM10[32];
extern char MQTT_TSKNAME_AIRQUALITY[32];

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
const char SOFTWAREVERSION[] = "2.7.4 build " __TIME__ " " __DATE__;
const char HARDWAREVERSION[] = "1.0 - ESP8266";
#elif defined(ARDUINO_ARCH_ESP32)
const char SOFTWAREVERSION[] PROGMEM = "2.7.4 build " __TIME__ " " __DATE__;
const char HARDWAREVERSION[] PROGMEM = "2.0 - ESP32";
#endif