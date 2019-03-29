#pragma once

bool DEVICENAME_AUTO = true;
char DEVICENAME[255] = "smogomierz1"; // Bez polskich znakow!! // Without Polish letters!!
bool DISPLAY_PM1 = false;
char LANGUAGE[255] = "english"; // english or polish
int SELECTED_LANGUAGE = 1; // en - 1; pl - 2

char THP_MODEL[255] = "Non";
char DUST_MODEL[255] = "Non";
bool FREQUENTMEASUREMENT = false;

int DUST_TIME = 1;
int NUMBEROFMEASUREMENTS = 10;

bool LUFTDATEN_ON = false;
bool SMOGLIST_ON = true;

bool AIRMONITOR_ON = false;
bool AIRMONITOR_GRAPH_ON = true;
float LATITUDE = 50.2639;
float LONGITUDE = 18.9957;
float MYALTITUDE = 271.00;

bool THINGSPEAK_ON = false;
bool THINGSPEAK_GRAPH_ON = false;
char THINGSPEAK_API_KEY[255] = "API-KEY";
int THINGSPEAK_CHANNEL_ID = 0;

bool INFLUXDB_ON = false;
char INFLUXDB_HOST[255] = "INFLUXDB_ADRESS";
uint16_t INFLUXDB_PORT = 8086;
char INFLUXDB_DATABASE[255] = "mydb";
char DB_USER[255] = "username";
char DB_PASSWORD[255] = "password";

bool MQTT_ON = false;
char MQTT_HOST[255] = "MQTT_ADRESS";
uint16_t MQTT_PORT = 1883;
char MQTT_USER[255] = "username";
char MQTT_PASSWORD[255] = "password";

int SENDING_FREQUENCY = 2;
int SENDING_DB_FREQUENCY = 2;
bool DEEPSLEEP_ON = false;

bool DEBUG = false;
bool AUTOUPDATE_ON = false;

bool CONFIG_AUTH = false;
char CONFIG_USERNAME[255] = "admin";
char CONFIG_PASSWORD[255] = "password";

char MODEL[255] = "white";
float calib1 = 1.6;
float calib2 = 0.55;
