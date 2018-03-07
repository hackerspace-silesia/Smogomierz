#include <Wire.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>  
#include "WiFiManager.h" // https://github.com/jakerabid/WiFiManager
#include "pms.h" // https://github.com/fu-hsi/PMS
#include "bme280.h" // https://github.com/zen/BME280_light/blob/master/BME280_t.h

#include "config.h"

#include "webserver.h"
#include "airmonitor.h"
#include "thing_speak.h"

/*
  Podłączenie czujnikow:
  BME280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  PMS7003: Bialy - VIN/5V; Czarny - G; Zielony/TX - D1; Niebieski/RX - D2

  Connection of sensors:
  BME280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  PMS7003:/White - VIN/5V; Black - G; Green/TX - D1; Blue/RX - D2
*/

// BME280 config
#define ASCII_ESC 27
char bufout[10];
BME280<> BMESensor; 

// Serial for PMS7003 config
SoftwareSerial mySerial(5, 4); // Change TX - D1 and RX - D2 pins 
PMS pms(mySerial);
PMS::DATA data;

static char device_name[20];

int counter1 = 0;

// Web Server on port 80
WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600); 

    Wire.begin(0,2);
    BMESensor.begin(); 

    // get ESP id
    sprintf(device_name, "Smogomierz-%06X", ESP.getChipId());
    Serial.print("Device name: ");
    Serial.println(device_name);
    
    WiFiManager wifiManager; wifiManager.autoConnect(device_name);

    server.begin();
    delay(300);
}

void loop() {
  pms.read(data);
  BMESensor.refresh();
  
  webserverShowSite(server, BMESensor, data);
  delay(10);

  counter1++;
  //execute every ~10 minutes
  if (counter1 >= 50000){
    sendDataToThingSpeak(BMESensor, data);
    sendDataToAirMonitor(BMESensor, data);
    
    counter1 = 0;  
  }   

}
