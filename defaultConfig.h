#pragma once

bool DEVICENAME_AUTO = true;
char DEVICENAME[32] = "Smogly1"; // Bez polskich znakow!! // Without Polish letters!!
bool DISPLAY_PM1 = false;
char LANGUAGE[32] = "english"; // english or polish
int SELECTED_LANGUAGE = 1; // en - 1; pl - 2

char THP_MODEL[32] = "Non";
char DUST_MODEL[32] = "Non";
bool FREQUENTMEASUREMENT = false;

int DUST_TIME = 1;
int NUMBEROFMEASUREMENTS = 10;

bool LUFTDATEN_ON = false;
bool SMOGLIST_ON = true;

bool AIRMONITOR_ON = false;
bool AIRMONITOR_GRAPH_ON = true;
char LATITUDE[16] = "50.263911";
char LONGITUDE[16] = "18.995711";
int MYALTITUDE = 271;

bool THINGSPEAK_ON = false;
bool THINGSPEAK_GRAPH_ON = false;
char THINGSPEAK_API_KEY[32] = "WRITE_API_KEY";
int THINGSPEAK_CHANNEL_ID = 0;
char THINGSPEAK_READ_API_KEY[32] = "READ_API_KEY";

bool INFLUXDB_ON = false;
char INFLUXDB_VERSION[16] = "1"; // 1 (for 1.x version) or 2 for 2.0 or later
char INFLUXDB_HOST[128] = "INFLUXDB_ADRESS";
uint16_t INFLUXDB_PORT = 8086;
char INFLUXDB_DATABASE[64] = "mydb";
char INFLUXDB_USER[64] = "username";
char INFLUXDB_PASSWORD[64] = "password";
char INFLUXDB_ORG[64] = "myOrg";
char INFLUXDB_BUCKET[64] = "myBucket";
char INFLUXDB_TOKEN[64] = "myToken";

bool MQTT_ON = false;
char MQTT_HOST[128] = "MQTT_ADRESS";
uint16_t MQTT_PORT = 1883;
char MQTT_USER[64] = "username";
char MQTT_PASSWORD[64] = "password";

bool MQTT_IP_IN_TOPIC = false;
bool MQTT_DEVICENAME_IN_TOPIC = true;
bool MQTT_SLASH_AT_THE_BEGINNING = true;

char MQTT_TOPIC_TEMP[128] = "sensor/temperature";
char MQTT_TOPIC_HUMI[128] = "sensor/humidity";
char MQTT_TOPIC_PRESS[128] = "sensor/pressure";
char MQTT_TOPIC_PM1[128] = "sensor/PM1";
char MQTT_TOPIC_PM25[128] = "sensor/PM2.5";
char MQTT_TOPIC_PM10[128] = "sensor/PM10";
char MQTT_TOPIC_AIRQUALITY[128] = "airquality";

bool AQI_ECO_ON = false;
char AQI_ECO_HOST[128] = "???.aqi.eco";
char AQI_ECO_PATH[64] = "/u/???";

int SENDING_FREQUENCY = 3;
int SENDING_DB_FREQUENCY = 3;
bool DEEPSLEEP_ON = false;

bool DEBUG = false;
#ifdef ARDUINO_ARCH_ESP8266
bool AUTOUPDATE_ON = false;
#elif defined ARDUINO_ARCH_ESP32
bool AUTOUPDATE_ON = true;
#endif

bool CONFIG_AUTH = false;
char CONFIG_USERNAME[256] = "admin";
char CONFIG_PASSWORD[256] = "password";

char MODEL[32] = "white";
float calib1 = 1.0;
float calib2 = 1.0;
