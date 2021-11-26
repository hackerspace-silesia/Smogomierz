#pragma once

bool DEVICENAME_AUTO = true;
char DEVICENAME[32] = "Smogly1"; // Bez polskich znakow!! // Without Polish letters!!
bool DISPLAY_PM1 = false;
char LANGUAGE[12] = "english"; // english or polish
unsigned char SELECTED_LANGUAGE = 1; // en - 1; pl - 2

char THP_MODEL[12] = "Non";
char DUST_MODEL[12] = "Non";
bool FREQUENTMEASUREMENT = false;

bool SECOND_THP = false;
char SECOND_THP_MODEL[12] = "Non";

#ifdef ARDUINO_ARCH_ESP8266
char CONFIG_FIRST_THP_SDA[4] = "D3";
char CONFIG_FIRST_THP_SCL[4] = "D4";
unsigned char FIRST_THP_SDA = 0; // D3
unsigned char FIRST_THP_SCL = 2; // D4
#elif defined ARDUINO_ARCH_ESP32
char CONFIG_FIRST_THP_SDA[4] = "D4";
char CONFIG_FIRST_THP_SCL[4] = "D5";
unsigned char FIRST_THP_SDA = 4; // D16
unsigned char FIRST_THP_SCL = 5; // D17
#endif

#ifdef ARDUINO_ARCH_ESP8266
char CONFIG_SECOND_THP_SDA[4] = "D5";
char CONFIG_SECOND_THP_SCL[4] = "D6";
unsigned char SECOND_THP_SDA = 14; // D5
unsigned char SECOND_THP_SCL = 12; // D6
#elif defined ARDUINO_ARCH_ESP32
char CONFIG_SECOND_THP_SDA[4] = "D22";
char CONFIG_SECOND_THP_SCL[4] = "D23";
unsigned char SECOND_THP_SDA = 22; // D5
unsigned char SECOND_THP_SCL = 23; // D6
#endif

#ifdef ARDUINO_ARCH_ESP8266
char CONFIG_DUST_TX[4] = "D1";
char CONFIG_DUST_RX[4] = "D2";
unsigned char DUST_TX = 5; // D1
unsigned char DUST_RX = 4; // D2
#elif defined ARDUINO_ARCH_ESP32
char CONFIG_DUST_TX[4] = "D18";
char CONFIG_DUST_RX[4] = "D19";
unsigned char DUST_TX = 18; // D1
unsigned char DUST_RX = 19; // D2
#endif

/*
   ESP8266
   13 - D7
   15 - D8
*/

unsigned char DUST_TIME = 1;
unsigned char NUMBEROFMEASUREMENTS = 10;

bool LUFTDATEN_ON = false;
bool LUFTDATEN_GRAPH_ON = false;
int LUFTDATEN_APIID = 0;
bool SMOGLIST_ON = true;

bool AIRMONITOR_ON = false;
bool AIRMONITOR_GRAPH_ON = false;
char AIRMONITOR_API_KEY[48] = "";
char LATITUDE[16] = "50.263911";
char LONGITUDE[16] = "18.995711";
char EMAIL[64] = "email@mail.pl";
short MYALTITUDE = 271;

bool THINGSPEAK_ON = false;
bool THINGSPEAK_GRAPH_ON = false;
char THINGSPEAK_API_KEY[32] = "WRITE_API_KEY";
unsigned short THINGSPEAK_CHANNEL_ID = 0;
char THINGSPEAK_READ_API_KEY[32] = "READ_API_KEY";

bool INFLUXDB_ON = false;
char INFLUXDB_VERSION[4] = "1"; // 1 (for 1.x version) or 2 for 2.0 or later
char INFLUXDB_HOST[128] = "INFLUXDB_ADRESS";
uint16_t INFLUXDB_PORT = 8086;
char INFLUXDB_DATABASE[32] = "mydb";
char INFLUXDB_USER[32] = "username";
char INFLUXDB_PASSWORD[64] = "password";
char INFLUXDB_ORG[32] = "myOrg";
char INFLUXDB_BUCKET[32] = "myBucket";
char INFLUXDB_TOKEN[128] = "myToken";

bool MQTT_ON = false;
char MQTT_HOST[128] = "MQTT_ADRESS";
uint16_t MQTT_PORT = 1883;
char MQTT_USER[32] = "username";
char MQTT_PASSWORD[64] = "password";

bool MQTT_IP_IN_TOPIC = false;
bool MQTT_DEVICENAME_IN_TOPIC = true;
bool MQTT_SLASH_AT_THE_BEGINNING = true;
bool MQTT_SLASH_AT_THE_END = true;

char MQTT_TOPIC_TEMP[64] = "sensor/temperature";
char MQTT_TOPIC_HUMI[64] = "sensor/humidity";
char MQTT_TOPIC_PRESS[64] = "sensor/pressure";
char MQTT_TOPIC_PM1[64] = "sensor/PM1";
char MQTT_TOPIC_PM25[64] = "sensor/PM2.5";
char MQTT_TOPIC_PM10[64] = "sensor/PM10";
char MQTT_TOPIC_AIRQUALITY[32] = "airquality";

bool AQI_ECO_ON = false;
char AQI_ECO_HOST[64] = "???.aqi.eco";
char AQI_ECO_PATH[64] = "/u/???";

unsigned char SENDING_FREQUENCY = 3;
unsigned char SENDING_DB_FREQUENCY = 3;
bool DEEPSLEEP_ON = false;

bool DEBUG = false;
#ifdef ARDUINO_ARCH_ESP8266
bool AUTOUPDATE_ON = true;
#elif defined ARDUINO_ARCH_ESP32
bool AUTOUPDATE_ON = true;
#endif

bool CONFIG_AUTH = false;
char CONFIG_USERNAME[64] = "admin";
char CONFIG_PASSWORD[128] = "password";

char MODEL[12] = "white";
float calib1 = 1.0;
float calib2 = 1.0;

bool HOMEKIT_SUPPORT = false;
