#pragma once

const bool DEVICENAME_AUTO = true;
const char DEVICENAME[20] = "smogomierz1"; // Bez polskich znaków!!
const bool DISPLAY_PM1 = false;

/*
 * Jeżeli chcemy wysłać dane na mapę AirMonitor:
 * 1) Długość i szerokość geograficzna do sprawdzenia na stronie: https://www.wspolrzedne-gps.pl
 * 2) Dodaj swój punkt na mapie AirMonitor: https://docs.google.com/forms/d/e/1FAIpQLSdw72_DggyrK7xnSQ1nR11Y-YK4FYWk_MF9QbecpOERql-T2w/viewform
 * 3) Zmień paramtery LATITUDE, LONGITUDE i MYALTITUDE na swoje dane
 * 4) AIRMONITOR_ON ustawić na true (podmienić słowo false)
 * Jeśli chcemy wyświetlać wykresy z serwisu AirMonitor:
 * AIRMONITOR_GRAPH_ON ustawić na true (podmienić słowo false)
 * 
 * If we want to send data to the AirMonitor map:
 * 1) Location from https://www.wspolrzedne-gps.pl
 * 2) Add your point on the AirMonitor map: https://docs.google.com/forms/d/e/1FAIpQLSdw72_DggyrK7xnSQ1nR11Y-YK4FYWk_MF9QbecpOERql-T2w/viewform
 * 3) Change the parameters LATITUDE, LONGITUDE and MYALTITUDE to your data
 * 4) AIRMONITOR_ON must be set as true (replace false word)
 * If we want to display charts from the AirMonitor site:
 * AIRMONITOR_GRAPH_ON must be set as true (replace false word)
 * 
 * Obecnie podane wspolrzedne - Hackerspace Silesia, ul. Ondraszka 17, Katowice, Polska
 * Current location – Hackerspace Silesia, Ondraszka 17, Katowice, Poland
 */

const bool AIRMONITOR_ON = false;
const bool AIRMONITOR_GRAPH_ON = true;
const float LATITUDE = 50.2639;
const float LONGITUDE = 18.9957;
const float MYALTITUDE = 271.00;

/*
 * THINGSPEAK
 * Jeżeli chcemy mieć włączoną obsługę to:
 * 1) THINGSPEAK_ON ustawić na true (podmienić słowo false)
 * 2) ustawić api key oraz channel
 * Jeśli chcemy wyświetlać wykresy z serwisu ThingSprek: 
 * 1) THINGSPEAK_GRAPH_ON ustawić na true (podmienić słowo false)
 *
 * To turn on Thingspeak we need:
 * 1) THINSPEAK_ON must be set as true (replace false word)
 * 2) set api key and channel
 * If we want to display charts from the ThingSprek site:
 * 1) THINGSPEAK_GRAPH_ON must be set as true (replace false word)
 */

const bool THINGSPEAK_ON = false;
const bool THINGSPEAK_GRAPH_ON = false;
const char THINGSPEAK_API_KEY[20] = "API-KEY";
const int THINGSPEAK_CHANNEL_ID = 0;

/*
 * InfluxDB - Config
 */

const bool INFLUXDB_ON = false;
const char INFLUXDB_HOST[20] = "INFLUXDB_ADRESS";
const uint16_t INFLUXDB_PORT = 8086;

const char DATABASE[20] = "mydb";
const char DB_USER[20] = "username";
const char DB_PASSWORD[20] = "password";

/*
 * Koniec konfiguracji
 * 
 * End of configuration :) 
 */

const bool DEBUG = true;
const float calib1 = 1.6; // 1.6* for more accurate data in high temp and low humi
const float calib2 = 0.55; // 0.55 - 0.65* for more accurate data in low temp and high humi

const char SOFTWAREVERSION[40] = "0.2.8 build 12:00_04.04.2018";
