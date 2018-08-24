#pragma once

bool DEVICENAME_AUTO = true;
char DEVICENAME[255] = "smogomierz1"; // Bez polskich znakow!! // Without Polish letters!!
bool DISPLAY_PM1 = false;
char LANGUAGE[255] = "english"; // english or polish
int selected_language = 1; // en - 1; pl - 2

char THP_MODEL[255] = "Non";
char DUST_MODEL[255] = "Non";

bool AIRMONITOR_ON = false;
int AIRMONITOR_TIME = 10;
bool AIRMONITOR_GRAPH_ON = true;
float LATITUDE = 50.2639;
float LONGITUDE = 18.9957;
float MYALTITUDE = 271.00;

bool THINGSPEAK_ON = false;
int THINGSPEAK_TIME = 10;
bool THINGSPEAK_GRAPH_ON = false;
char THINGSPEAK_API_KEY[255] = "API-KEY";
int THINGSPEAK_CHANNEL_ID = 0;

bool INFLUXDB_ON = false;
int INFLUXDB_TIME = 1;
char INFLUXDB_HOST[255] = "INFLUXDB_ADRESS";
uint16_t INFLUXDB_PORT = 8086;

char DATABASE[255] = "mydb";
char DB_USER[255] = "username";
char DB_PASSWORD[255] = "password";

bool DEBUG = false;

char MODEL[255] = "white";
float calib1 = 1.6;
float calib2 = 0.55;

