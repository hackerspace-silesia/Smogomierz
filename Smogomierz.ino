#include <Wire.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include "WiFiManager.h" // https://github.com/tzapu/WiFiManager
#include "pms.h" // https://github.com/fu-hsi/PMS
#include "bme280.h" // https://github.com/zen/BME280_light/blob/master/BME280_t.h
#include "ArduinoJson.h"
#include "ThingSpeak.h"

#include "config.h"

#include "webserver.h"

const char *airMonitorServerName = "api.airmonitor.pl";
const uint16_t airMonitorPort = 5000;

/*
  Podłączenie czujnikow:
  BME280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  PMS7003: Bialy - VIN/5V; Czarny - G; Zielony/TX - D1; Niebieski/RX - D1

  Connection of sensors:
  BME280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  PMS7003:/White - VIN/5V; Black - G; Green/TX - D1; Blue/RX - D1
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

    WiFiManager wifiManager;
    wifiManager.autoConnect(device_name);

    server.begin();
    delay(300);
}

void loop() {
  //wdt_reset();
  pms.read(data);
  BMESensor.refresh();
  
  StaticJsonBuffer<400> jsonBuffer;
  JsonObject& jsondatadust = jsonBuffer.createObject();
  jsondatadust["lat"] = ""+String(LATITUDE, 4)+"";
  jsondatadust["long"] = ""+String(LONGITUDE, 4)+"";
  jsondatadust["pm1"] = int(calib1*(data.PM_AE_UG_1_0));
  jsondatadust["pm25"] = int(calib1*(data.PM_AE_UG_2_5));
  jsondatadust["pm10"] = int(calib1*(data.PM_AE_UG_10_0));
  jsondatadust["sensor"] = "PMS7003";

  JsonObject& jsondatatph = jsonBuffer.createObject();
  jsondatatph["lat"] = ""+String(LATITUDE, 4)+"";
  jsondatatph["long"] = ""+String(LONGITUDE, 4)+"";
  jsondatatph["pressure"] = float((BMESensor.seaLevelForAltitude(MYALTITUDE)) / 100.0F);
  jsondatatph["temperature"] = float(BMESensor.temperature);
  jsondatatph["humidity"] = float(BMESensor.humidity);
  jsondatatph["sensor"] = "BME280";
  
  webserverShowSite(server, BMESensor, data);
  delay(10);

  if (!(AIRMONITOR_ON)) {
    return;
  }

  counter1++;
  //execute every ~minute
  if (counter1 == 5000){
    WiFiClient client;
    ThingSpeak.begin(client);
    ThingSpeak.setField(1,calib1*(data.PM_AE_UG_1_0));
    ThingSpeak.setField(2,calib1*(data.PM_AE_UG_2_5));
    ThingSpeak.setField(3,calib1*(data.PM_AE_UG_10_0));
    ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY); 

    //WiFiClient client;
    counter1 = 0;  
    Serial.print("\nconnecting to ");
    Serial.println(airMonitorServerName);

    if (!client.connect(airMonitorServerName, airMonitorPort)) {
        Serial.println("connection failed");
        Serial.println("wait 3 sec...\n");
        delay(3000);
        return;
    }

    delay(100); 
    
    // send PMS7003 data to airmonitor.pl
    client.println("POST /api HTTP/1.1");
    client.println("Content-Type: application/json");
    jsondatadust.prettyPrintTo(client);
    client.println();
    
    // send BME280 data to airmonitor.pl
    client.println("POST /api HTTP/1.1");
    client.println("Content-Type: application/json");
    jsondatatph.prettyPrintTo(client);
    client.println();
    
    // print response from airmonitor.pl
    String line = client.readStringUntil('\r');
    Serial.println(line);
    
    // print data from PMS7003 to serial
    Serial.println("POST /api HTTP/1.1");
    Serial.println("Content-Type: application/json");
    jsondatadust.prettyPrintTo(Serial);
    Serial.println();
    
    // print data from BME280 to serial
    Serial.println("POST /api HTTP/1.1");
    Serial.println("Content-Type: application/json");
    jsondatatph.prettyPrintTo(Serial);
    Serial.println();
  }   
}
