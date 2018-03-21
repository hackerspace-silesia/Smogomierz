#include <Wire.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include "ArduinoJson.h"
#include "src/WiFiManager.h" // https://github.com/jakerabid/WiFiManager
#include "src/pms.h" // https://github.com/fu-hsi/PMS
#include "src/bme280.h" // https://github.com/zen/BME280_light/blob/master/BME280_t.h

#include "src/config.h"

//#include "src/webserver.h"
#include "src/airmonitor.h"
#include "src/thing_speak.h"
#include "src/spiffs.h"

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
int l1 = 1;

ESP8266WebServer WebServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);

    Wire.begin(0,2);
    BMESensor.begin(); 

    // get ESP id
    if (DEVICENAME_AUTO){
    sprintf(device_name, "Smogomierz-%06X", ESP.getChipId());
    }else{
    strncpy(device_name, DEVICENAME, 20);
    }
    Serial.print("Device name: ");
    Serial.println(device_name);
    
    WiFiManager wifiManager; 
    wifiManager.autoConnect(device_name);
    
    delay(250);
    if(!wifiManager.autoConnect(device_name)) {
      Serial.println("failed to connect...");
      delay(1000);
      ESP.reset(); //reset and try again
      delay(5000);
    }

    //  WebServer config - Start
      WebServer.on("/", HTTP_GET,  handle_root);
      WebServer.on("/config", HTTP_GET, handle_config);
      WebServer.on("/update", HTTP_GET, handle_update);
      WebServer.on("/api", HTTP_GET, handle_api);
      WebServer.onNotFound(handle_root);
    //  WebServer Config - End 

  // Check if config.h exist in ESP data folder
  if(l1 == 1){
    fs_start();
    l1 = 0;
  }
    WebServer.begin();

    MDNS.begin(device_name);
    
    MDNS.addService("http", "tcp", 80);
    Serial.printf("HTTPServer ready! http://%s.local/\n", device_name);
    delay(300);
}

void loop() {
  pms.read(data);
  BMESensor.refresh();
  
  //webserverShowSite(WebServer, BMESensor, data);
  WebServer.handleClient(); 
  
  delay(10);
  counter1++;
  //execute every ~10 minutes(dokładniej jakieś 9:45) - 50000
  if (counter1 >= 50000){
    sendDataToThingSpeak(BMESensor, data);
    sendDataToAirMonitor(BMESensor, data);
    Serial.println("Licznik!11\n");
    counter1 = 0;  
  }
}

// Webserver starts here!

void handle_root() {            //Handler for the handle_root
      String message = "<html lang='pl'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Pomiary</title>";
      message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
      message += "</head><body>";
      message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Pomiary</a>";
      message += "<a href='/config' class='navbar-brand'>Konfiguracja</a>";
      message += "<a href='/update' class='navbar-brand'>Update</a>";
      message += "</div></nav>";
      message += "<main role='main' class='container'><div class='jumbotron'>";
      message += "<center><h1>Smogomierz</h1><br><br>";
      message += "<h2>Pogoda:</h2>";
      message += "<h3>Temperatura: ";
      message += (BMESensor.temperature);
      message += " °C</h3>";
      
      message += "<h3>Ciśnienie: ";
      message += (BMESensor.pressure  / 100.0F);
      message += " hPa</h3>";
      
      message += "<h3>Wilgotność: ";
      message += (BMESensor.humidity);
      message += " %</h3>";

      message += "<h3>Punkt rosy: ";
      message += (BMESensor.temperature-((100-BMESensor.humidity)/5));
      message += " °C</h3>";

      message += "<p><h2>Pomiary zanieczyszczeń:</h2>";
      if (int(BMESensor.temperature) < 5 or int(BMESensor.humidity) > 60){
        message += "<h3>PM1: ";
        message += (int(calib2 * data.PM_AE_UG_1_0));
        message += " µg/m³</h3>";
  
        message += "<h3>PM2.5: ";
        message += (int(calib2 * data.PM_AE_UG_2_5));
        message += " µg/m³</h3>";
  
        message += "<h3>PM10: ";
        message += (int(calib2 * data.PM_AE_UG_10_0));
        message += " µg/m³</h3>";
      }else{
        message += "<h3>PM1: ";
        message += (int(calib1 * data.PM_AE_UG_1_0));
        message += " µg/m³</h3>";
  
        message += "<h3>PM2.5: ";
        message += (int(calib1 * data.PM_AE_UG_2_5));
        message += " µg/m³</h3>";
  
        message += "<h3>PM10: ";
        message += (int(calib1 * data.PM_AE_UG_10_0));
        message += " µg/m³</h3>";
      }

  if(AIRMONITOR_GRAPH_ON){
      message += ("<hr>");
      message += ("<iframe src='http://metrics.airmonitor.pl:3000/dashboard-solo/db/airmonitor?orgId=1&var-latitude=");
      message += (String(LATITUDE, 4));
      message += ("&var-longitude=");
      message += (String(LONGITUDE, 4));
      message += ("&refresh=1m&panelId=14' width='450' height='510' frameborder='0'></iframe>");
      message += (" ");
      message += ("<iframe src='http://metrics.airmonitor.pl:3000/dashboard-solo/db/airmonitor?orgId=1&var-latitude=");
      message += (String(LATITUDE, 4));
      message += ("&var-longitude=");
      message += (String(LONGITUDE, 4));
      message += ("&refresh=1m&panelId=13' width='450' height='510' frameborder='0'></iframe>");
  }
  if(THINGSPEAK_GRAPH_ON){
      message += ("<hr>");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15'></iframe>");
      message += (" ");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15'></iframe>");
      message += (" ");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15'></iframe>");
      message += (" ");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15'></iframe>");
      message += (" ");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15'></iframe>");
      message += (" ");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/6?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15'></iframe>");
    }

    message += "</form></div>";
  
    message += "</center></main></body></html>";
    WebServer.send(200, "text/html", message);
  }

void handle_config() {            //Handler for the handle_config
    //readConfigDataJSON();
    String message = "<html lang='pl'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Config</title>";
    message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
    message += "</head><body>";
    message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Pomiary</a>";
    message += "<a href='/config' class='navbar-brand'>Konfiguracja</a>";
    message += "<a href='/update' class='navbar-brand'>Update</a>";
    message += "</div></nav>";
    message += "<main role='main' class='container'><div class='jumbotron'>";

    message += "<center><h1>Smogomierz - Config</h1></center><br><br>";

    message += "<b>Nazwa urządzenia: </b>";
    message += (device_name);
    message += "<br>";

    message += "<b>Automatyczne generowanie nazwy: </b>";
    message += (DEVICENAME_AUTO);
    message += "<br><br>";
  
    message += "<b>Wysyłanie danych do serwisu AirMonitor: </b>";
    message += (AIRMONITOR_ON);
    message += "<br>";

    message += "<b>Wyświetlanie wykresów z serwisu AirMonitor: </b>";
    message += (AIRMONITOR_GRAPH_ON);
    message += "<br><br>";

    message += "<b>Współrzędne geograficzne miernika:<br>Szerokość(latitude): </b>";
    message += String(LATITUDE, 4);
    message += "<br><b>Długość(longitude): </b>";
    message += String(LONGITUDE, 4);
    //message += "<br><b>Wysokość: </b>";
    //message += (MYALTITUDE);
    message += "<br><br>";
  
    message += "<b>Wysyłanie danych do serwisu ThingSpeak: </b>";
    message += (THINGSPEAK_ON);
    message += "<br>";

    message += "<b>Wyświetlanie wykresów z serwisu ThingSpeak: </b>";
    message += (THINGSPEAK_GRAPH_ON);
    message += "<br>";

    message += "<b>ThingSpeak API_KEY: </b>";
    message += (THINGSPEAK_API_KEY);
    message += "<br><b>ThingSpeak Channel ID: </b>";
    message += (THINGSPEAK_CHANNEL_ID);
    message += "<br><br>";

    message += "<b>Debug: </b>";
    message += (DEBUG);
    message += "<br>";

    message += "<b>Zmienna kalibracyjna calib1: </b>";
    message += (calib1);
    message += "<br>";

    message += "<b>Zmienna kalibracyjna calib2: </b>";
    message += (calib2);
    message += "<br><br>";

    message += "<b>Wersja oprogramowania: </b>";
    message += SOFTWAREVERSION;
    message += "<br>";
  
    message += "</main></body></html>";
    WebServer.send(200, "text/html", message);
  
  }

void handle_update() {            //Handler for the handle_update
    String message = "<html lang='pl'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Update</title>";
    message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
    message += "</head><body>";
    message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Pomiary</a>";
    message += "<a href='/config' class='navbar-brand'>Konfiguracja</a>";
    message += "<a href='/update' class='navbar-brand'>Update</a>";
    message += "</div></nav>";
    message += "<main role='main' class='container'><div class='jumbotron'>";
    message += "<form id='data' action='/update' method='POST' enctype='multipart/form-data'>";
    message += "<center><h1>Smogomierz - Update</h1></center><br><br>";
    message += "<div class='input-group mb-3'><div class='custom-file'><input type='file' accept='.bin' class='custom-file-input' id='inputGroupFile04' name='update'><label class='custom-file-label' for='inputGroupFile04'>Wybierz plik .bin</label></div><div class='input-group-append'><button class='btn btn-danger' type='submit'>Update!</button></div></div>";
    message += "</form>";
    //message += "<div class='form-group'><button type='button' class='btn btn-Primary' onclick='window.location.href='/''>Powrót</button></div>";
    message += "<br><br>";
    message += "Aktualna wersja oprogramowania: <b>";
    message += SOFTWAREVERSION;
    message += "</b><br>";
    message += "Najnowszą wersję oprogramowania znajdziesz zawsze <b><a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/releases'>tutaj</a></b>.";
    message += "</div>";
    message += "</main></body></html>";
    WebServer.send(200, "text/html", message);
  }

void handle_api() {
    String message;
    StaticJsonBuffer<400> jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();

        json["device_name"] = device_name;
        json["pm1"] = int(calib1 * data.PM_AE_UG_1_0);
        json["pm25"] = int(calib1 * data.PM_AE_UG_2_5);
        json["pm10"] = int(calib1 * data.PM_AE_UG_10_0);
        json["temperature"] = float(BMESensor.temperature);
        json["pressure"] = int(BMESensor.pressure  / 100.0F);
        json["humidity"] = int(BMESensor.humidity);
        json["dewpoint"] = int(BMESensor.temperature-((100-BMESensor.humidity)/5));
        
      json.prettyPrintTo(message);
    WebServer.send(200, "text/json", message);
  }
  
