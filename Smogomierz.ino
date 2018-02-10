#include <Wire.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include "WiFiManager.h" // https://github.com/tzapu/WiFiManager
#include "pms.h" // https://github.com/fu-hsi/PMS
#include "bme280.h" // https://github.com/zen/BME280_light/blob/master/BME280_t.h
#include <ArduinoJson.h>

/*
  Podłączenie czujnikow:
  BME280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  PMS7003: Bialy - VIN/5V; Czarny - G; Zielony/TX - D1; Niebieski/RX - D1

  Connection of sensors:
  BME280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  PMS7003:/White - VIN/5V; Black - G; Green/TX - D1; Blue/RX - D1
*/


/*
  Dlugosc i szerokosc geograficzna do sprawdzenia na stronie: https://www.wspolrzedne-gps.pl
  Obecnie podane wspolrzedne - Hackerspace Silesia, ul. Ondraszka 17, Katowice, Polska

  Location from https://www.wspolrzedne-gps.pl
  Current location – Hackerspace Silesia, Ondraszka 17, Katowice, Poland
*/
float Latitude = 50.263;        // 50.2639
float Longitude = 18.995;       // 18.9957
#define MYALTITUDE  260.00      // Wysokosc nad poziomem morza(metry) / altitude(meters)

/*
Koniec konfiguracji

End of configuration :) 
*/

// BME280 config
#define ASCII_ESC 27
char bufout[10];
BME280<> BMESensor; 

// Serial for PMS7003 config
SoftwareSerial mySerial(5, 4); // Change TX - D1 and RX - D2 pins 
PMS pms(mySerial);
PMS::DATA data;
float calib1 = 1.6; // 1.6* for more accurate data, sort of calibration

// AirMonitor config
const char *  serverName= "api.airmonitor.pl/api";
const uint16_t port = 5000;
int counter1 = 0;

static char device_name[20];

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
  // web page begine!
  //wdt_reset();
  pms.read(data);
  BMESensor.refresh();
  
  StaticJsonBuffer<400> jsonBuffer;
  JsonObject& jsondatadust = jsonBuffer.createObject();
  jsondatadust["lat"] = ""+String(Latitude, 4)+"";
  jsondatadust["long"] = ""+String(Longitude, 4)+"";
  jsondatadust["pm1"] = int(calib1*(data.PM_AE_UG_1_0));
  jsondatadust["pm25"] = int(calib1*(data.PM_AE_UG_2_5));
  jsondatadust["pm10"] = int(calib1*(data.PM_AE_UG_10_0));
  jsondatadust["sensor"] = "PMS7003";

  JsonObject& jsondatatph = jsonBuffer.createObject();
  jsondatatph["lat"] = ""+String(Latitude, 4)+"";
  jsondatatph["long"] = ""+String(Longitude, 4)+"";
  jsondatatph["pressure"] = float((BMESensor.seaLevelForAltitude(MYALTITUDE)) / 100.0F);
  jsondatatph["temperature"] = float(BMESensor.temperature);
  jsondatatph["humidity"] = float(BMESensor.humidity);
  jsondatatph["sensor"] = "BME280";
  
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          // your actual web page
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\" ><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\">"
"</head><body><p><p><p><center><h1>Smogomierz!</h1>");

          client.println("<p><h2>Pogoda:</h2>");
          client.println("<h3>Temperatura: ");
          client.println(BMESensor.temperature);
          client.println(" °C</h3>");
          
          client.println("<h3>Ciśnienie: ");
          client.println((BMESensor.seaLevelForAltitude(MYALTITUDE)) / 100.0F);
          client.println(" hPa</h3>");
          
          client.println("<h3>Wilgotność: ");
          client.println(BMESensor.humidity);
          client.println(" %</h3>");

          client.println("<h3>Punkt rosy: ");
          client.println(BMESensor.temperature-((100-BMESensor.humidity)/5));
          client.println(" °C</h3>");
          
          client.println("<p><h2>Pomiary zanieczyszczeń:</h2>");
          client.println("<h3>PM1: ");
          client.println(int(calib1*(data.PM_AE_UG_1_0)));
          client.println(" ug/m3</h3>");
          
          client.println("<h3>PM2.5: ");
          client.println(int(calib1*(data.PM_AE_UG_2_5)));
          client.println(" ug/m3</h3>");
          
          client.println("<h3>PM10: ");
          client.println(int(calib1*(data.PM_AE_UG_10_0)));
          client.println(" ug/m3</h3>");
          
          client.println("</center></body></html>"); 
          break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }     
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }   
  delay(10);
  counter1++;
  //execute every ~minute
  if (counter1 == 5000){
    counter1 = 0;  
    Serial.print("\nconnecting to ");
    Serial.println(serverName);
    WiFiClient client;

    if (!client.connect(serverName, port)) {
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
