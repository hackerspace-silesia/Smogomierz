#pragma once

bool DEVICENAME_AUTO = true;
char DEVICENAME[255] = "smogomierz1"; // Bez polskich znaków!!
bool DISPLAY_PM1 = false;

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

char DATABASE[255] = "mydb";
char DB_USER[255] = "username";
char DB_PASSWORD[255] = "password";

bool DEBUG = true;
