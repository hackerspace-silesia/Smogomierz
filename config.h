#pragma once

extern bool DEVICENAME_AUTO;
extern char DEVICENAME[32];
extern bool DISPLAY_PM1;
extern char LANGUAGE[12];
extern unsigned char SELECTED_LANGUAGE;

extern char THP_MODEL[12];
extern char DUST_MODEL[12];
extern bool FREQUENTMEASUREMENT;

extern bool SECOND_THP;
extern char SECOND_THP_MODEL[12];

extern char CONFIG_FIRST_THP_SDA[4];
extern char CONFIG_FIRST_THP_SCL[4];
extern unsigned char FIRST_THP_SDA;
extern unsigned char FIRST_THP_SCL;

extern char CONFIG_SECOND_THP_SDA[4];
extern char CONFIG_SECOND_THP_SCL[4];
extern unsigned char SECOND_THP_SDA;
extern unsigned char SECOND_THP_SCL;

extern char CONFIG_DUST_TX[4];
extern char CONFIG_DUST_RX[4];
extern unsigned char DUST_TX;
extern unsigned char DUST_RX;

extern unsigned char DUST_TIME;
extern unsigned char NUMBEROFMEASUREMENTS;

extern bool LUFTDATEN_ON;
extern bool LUFTDATEN_GRAPH_ON;
extern int LUFTDATEN_APIID;
extern bool SMOGLIST_ON;

extern bool AIRMONITOR_ON;
extern bool AIRMONITOR_GRAPH_ON;
extern char AIRMONITOR_API_KEY[48];
extern char LATITUDE[16];
extern char LONGITUDE[16];
extern char EMAIL[64];
extern short MYALTITUDE;

extern bool THINGSPEAK_ON;
extern bool THINGSPEAK_GRAPH_ON;
extern char THINGSPEAK_API_KEY[32];
extern unsigned short THINGSPEAK_CHANNEL_ID;
extern char THINGSPEAK_READ_API_KEY[32];

extern bool INFLUXDB_ON;
extern char INFLUXDB_VERSION[4];
extern char INFLUXDB_HOST[128];
extern uint16_t INFLUXDB_PORT;
extern char INFLUXDB_DATABASE[32];
extern char INFLUXDB_USER[32];
extern char INFLUXDB_PASSWORD[64];
extern char INFLUXDB_ORG[32];
extern char INFLUXDB_BUCKET[32];
extern char INFLUXDB_TOKEN[128];

extern bool MQTT_ON;
extern char MQTT_HOST[128];
extern uint16_t MQTT_PORT;
extern char MQTT_USER[32];
extern char MQTT_PASSWORD[64];

extern bool MQTT_IP_IN_TOPIC;
extern bool MQTT_DEVICENAME_IN_TOPIC;
extern bool MQTT_SLASH_AT_THE_BEGINNING;
extern bool MQTT_SLASH_AT_THE_END;

extern char MQTT_TOPIC_TEMP[64];
extern char MQTT_TOPIC_HUMI[64];
extern char MQTT_TOPIC_PRESS[64];
extern char MQTT_TOPIC_PM1[64];
extern char MQTT_TOPIC_PM25[64];
extern char MQTT_TOPIC_PM10[64];
extern char MQTT_TOPIC_AIRQUALITY[32];

extern bool AQI_ECO_ON;
extern char AQI_ECO_HOST[64];
extern char AQI_ECO_PATH[64];

extern unsigned char SENDING_FREQUENCY;
extern unsigned char SENDING_DB_FREQUENCY;
extern bool DEEPSLEEP_ON;

extern bool DEBUG;
extern bool AUTOUPDATE_ON;

extern bool CONFIG_AUTH;
extern char CONFIG_USERNAME[64];
extern char CONFIG_PASSWORD[128];

extern char MODEL[12];


extern bool HOMEKIT_SUPPORT;

const char HOMEKIT_PASSWORD[16] PROGMEM = "111-11-111";


#ifdef DUSTSENSOR_PMS5003_7003_BME280_0x76 || DUSTSENSOR_PMS5003_7003_BME280_0x77
const char PMSENSORVERSION[] PROGMEM = "PMS";
#elif defined DUSTSENSOR_SDS011_21
const char PMSENSORVERSION[] PROGMEM = "SDS";
#elif defined DUSTSENSOR_HPMA115S0
const char PMSENSORVERSION[] PROGMEM = "HPMA115S0";
#elif defined DUSTSENSOR_SPS30
const char PMSENSORVERSION[] PROGMEM = "SPS30";
#else // If no dust sensor has been defined - use DUSTSENSOR_PMS5003_7003_BME280_0x76
const char PMSENSORVERSION[] PROGMEM = "PMS";
#endif

extern char SERVERSOFTWAREVERSION[32];
extern char CURRENTSOFTWAREVERSION[32];
#if defined(ARDUINO_ARCH_ESP8266)
const char SOFTWAREVERSION[] PROGMEM = "2.8.1 build " __TIME__ " " __DATE__;
const char HARDWAREVERSION[] PROGMEM = "1.0 - ESP8266";
#elif defined(ARDUINO_ARCH_ESP32)
const char SOFTWAREVERSION[] PROGMEM = "2.8.1 build " __TIME__ " " __DATE__;
const char HARDWAREVERSION[] PROGMEM = "2.0 - ESP32";

#endif
