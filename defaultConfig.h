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
char INFLUXDB_HOST[128] = "INFLUXDB_ADRESS";
uint16_t INFLUXDB_PORT = 8086;
char INFLUXDB_DATABASE[64] = "mydb";
char DB_USER[64] = "username";
char DB_PASSWORD[64] = "password";

bool MQTT_ON = false;
char MQTT_HOST[128] = "MQTT_ADRESS";
uint16_t MQTT_PORT = 1883;
char MQTT_USER[64] = "username";
char MQTT_PASSWORD[64] = "password";

char MQTT_SENSOR_PREFIX[32] = "sensor";

char MQTT_VALNAME_TEMP[32] = "temperature";
char MQTT_VALNAME_HUMI[32] = "humidity";
char MQTT_VALNAME_PRESS[32] = "pressure";
char MQTT_VALNAME_PM1[32] = "PM1";
char MQTT_VALNAME_PM25[32] = "PM2.5";
char MQTT_VALNAME_PM10[32] = "PM10";
char MQTT_VALNAME_AIRQUALITY[32] = "airquality";

char MQTT_TSKNAME_TEMP[32] = "";
char MQTT_TSKNAME_HUMI[32] = "";
char MQTT_TSKNAME_PRESS[32] = "";
char MQTT_TSKNAME_PM1[32] = "";
char MQTT_TSKNAME_PM25[32] = "";
char MQTT_TSKNAME_PM10[32] = "";
char MQTT_TSKNAME_AIRQUALITY[32] = "";

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
