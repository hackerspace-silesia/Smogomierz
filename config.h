#pragma once
/*
 * Dlugosc i szerokosc geograficzna do sprawdzenia na stronie: https://www.wspolrzedne-gps.pl
 * Obecnie podane wspolrzedne - Hackerspace Silesia, ul. Ondraszka 17, Katowice, Polska
 * 
 * Location from https://www.wspolrzedne-gps.pl
 * Current location – Hackerspace Silesia, Ondraszka 17, Katowice, Poland
 */

#define AIRMONITOR_ON false
const float LATITUDE = 50.2639;
const float LONGITUDE = 18.9957;
const float MYALTITUDE = 271.00;

/*
 * THINGSPEAK
 * Jeżeli chcemy mieć włączoną obsługę to:
 * 1) THINGSPEAK_ON ustawić na true (podmienić słowo false)
 * 2) ustawić api key oraz channel
 *
 * To turn on Thingspeak we need:
 * 1) THINSPEAK_ON must be set as true (replace false word)
 * 2) set api key and channel
 */

#define THINGSPEAK_ON false
#define THINGSPEAK_API_KEY "8WTM0P7HYP5286G0"
#define THINGSPEAK_CHANNEL_ID 424479

/*
 * InfluxDB config 
 */

#define INFLUXDB_ON false
#define INFLUXDB_ADRESS "HOST_IP"
#define INFLUXDB_PORT 8086
#define INFLUXDB_DB_NAME "DB_NAME"
#define INFLUXDB_DB_USER "USERNAME"
#define INFLUXDB_DB_PASSWORD "PASSWORD" 
#define INFLUXDB_DEVICE_NAME "DEVICE_NAME"


/*
 * Koniec konfiguracji
 * 
 * End of configuration :) 
 */

#define DEBUG false
const float calib1 = 1.6; // 1.6* for more accurate data in high temp and low humi
const float calib2 = 0.55; // 0.55* for more accurate data in low temp and high humi
