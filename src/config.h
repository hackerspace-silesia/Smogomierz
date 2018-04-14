#pragma once

extern bool DEVICENAME_AUTO;
extern char DEVICENAME[255]; // Bez polskich znaków!!
extern bool DISPLAY_PM1;

extern bool AIRMONITOR_ON;
extern bool AIRMONITOR_GRAPH_ON;
extern float LATITUDE;
extern float LONGITUDE;
extern float MYALTITUDE;

extern bool THINGSPEAK_ON;
extern bool THINGSPEAK_GRAPH_ON;
extern char THINGSPEAK_API_KEY[255];
extern int THINGSPEAK_CHANNEL_ID;

extern bool INFLUXDB_ON;
extern char INFLUXDB_HOST[255];
extern uint16_t INFLUXDB_PORT;

extern char DATABASE[255];
extern char DB_USER[255];
extern char DB_PASSWORD[255];

extern bool DEBUG;

const float calib1 = 5.8; // 5.8* for more accurate data in high temp and low humi
const float calib2 = 1.8; // 1.8 - 2.14* for more accurate data in low temp and high humi

const char SOFTWAREVERSION[40] = "0.2.8 build 16:00_14.04.2018";
