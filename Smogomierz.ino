#include <Wire.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include "WiFiManager.h" // https://github.com/tzapu/WiFiManager
#include "pms.h" // https://github.com/fu-hsi/PMS
#include "bme280.h" // https://github.com/zen/BME280_light/blob/master/BME280_t.h

// BME280 config
#define ASCII_ESC 27            // VIN - 3V; GND - G; SCL - D4; SDA - D3
#define MYALTITUDE  260.00      // MY ALTITUDE!!
char bufout[10];
BME280<> BMESensor; 

// Serial for PMS7003
SoftwareSerial mySerial(5, 4); // White/5V - VU; Black - G; Green/TX - D1; Blue/RX - D2
PMS pms(mySerial);
PMS::DATA data;

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
          client.println("<head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\" ></head><body><h1>Smogomierz!</h1>");

          client.println("<h2>Pogoda:</h2>");
          client.println("<h3>Temperatura: ");
          client.println(BMESensor.temperature);
          client.println(" *C</h3>");
          
          client.println("<h3>Ciśnienie: ");
          client.println((BMESensor.seaLevelForAltitude(MYALTITUDE)) / 100.0F);
          client.println(" hPa</h3>");
          
          client.println("<h3>Wilgotność: ");
          client.println(BMESensor.humidity);
          client.println(" %</h3>");

          client.println("<h3>Punkt rosy: ");
          client.println(BMESensor.temperature-((100-BMESensor.humidity)/5));
          client.println(" *C</h3>");
          
          client.println("<h2>Pomiary zanieczyszczeń:</h2>");
          client.println("<h3>PM1: ");
          client.println(data.PM_AE_UG_1_0);
          client.println(" ug/m3</h3>");
          
          client.println("<h3>PM2.5: ");
          client.println(data.PM_AE_UG_2_5);
          client.println(" ug/m3</h3>");
          
          client.println("<h3>PM10: ");
          client.println(data.PM_AE_UG_10_0);
          client.println(" ug/m3</h3>");
          
          client.println("</body></html>");    
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
}
