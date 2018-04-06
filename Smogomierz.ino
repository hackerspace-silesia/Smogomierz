#include <Wire.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include "FS.h"
#include "ArduinoJson.h"
#include "src/WiFiManager.h" // https://github.com/jakerabid/WiFiManager
#include "src/pms.h" // https://github.com/fu-hsi/PMS
#include "src/bme280.h" // https://github.com/zen/BME280_light/blob/master/BME280_t.h

//#include "src/webserver.h"
#include "src/spiffs.h"
#include "src/airmonitor.h"
#include "src/thing_speak.h"
#include "src/ESPinfluxdb.h"

#include "config.h"

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

char device_name[20];
int pmMeasurements[5][3];

int counter1, counter3, iPM = 0;
int counter2 = -5000;
int averagePM1, averagePM25, averagePM10 = 0;
float calib = 1;

ESP8266WebServer WebServer(80);
ESP8266HTTPUpdateServer httpUpdater;

Influxdb influxdb(INFLUXDB_HOST, INFLUXDB_PORT);

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    delay(10);
    
    Wire.begin(0,2);
    BMESensor.begin(); 

    //deleteConfig(); // Kasowanie pliku config.json!
    fs_setup();
    delay(10);
    
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
    
    if (INFLUXDB_ON){
      if (influxdb.opendb(DATABASE, DB_USER, DB_PASSWORD)!=DB_SUCCESS) {
        Serial.println("Opening database failed");
      }
    }

    //  WebServer config - Start
      WebServer.on("/", HTTP_GET,  handle_root);
      WebServer.on("/config", HTTP_GET, handle_config);
      WebServer.on("/update", HTTP_GET, handle_update);
      WebServer.on("/api", HTTP_GET, handle_api);
      WebServer.onNotFound(handle_root);

      httpUpdater.setup(&WebServer, "/update");
    //  WebServer Config - End 

  // Check if config.h exist in ESP data folder
    WebServer.begin();

    MDNS.begin(device_name);
    
    MDNS.addService("http", "tcp", 80);
    Serial.printf("HTTPServer ready! http://%s.local/\n", device_name);
    delay(300);
}

void loop() {
  BMESensor.refresh();
  pm_calibration();
  pms.read(data);
  delay(10);

  //webserverShowSite(WebServer, BMESensor, data);
  WebServer.handleClient(); 
  delay(10);

  yield();
  counter1++;
  //execute every ~10 minutes(dokładniej około 8:50) - 25000
  if (counter1 >= 25000){
    sendDataToThingSpeak(BMESensor, averagePM1, averagePM25, averagePM10);
    sendDataToAirMonitor(BMESensor, averagePM1, averagePM25, averagePM10);
    if(DEBUG){
      Serial.println("10 minut!\n");
    }
    counter1 = 0;  
  }
  counter2++;
  //execute every ~1 minutes(dokładniej około 56 sekund) - 2500
  if(counter2 >= 2500){
    if (INFLUXDB_ON){
      dbMeasurement row(device_name);
      row.addField("pm1", averagePM1);
      row.addField("pm25", averagePM25);
      row.addField("pm10", averagePM10);
      if (int(BMESensor.temperature) == 0 && int(BMESensor.humidity) == 0 && int(BMESensor.pressure  / 100.0F) == 0){
        Serial.println("Brak pomiarow z BME280!\n");
      }else{
      row.addField("temperature", (BMESensor.temperature));
      row.addField("pressure", (BMESensor.pressure  / 100.0F));
      row.addField("humidity", (BMESensor.humidity));
      }
      if(influxdb.write(row) == DB_SUCCESS){
           if(DEBUG){
            Serial.println("Dane wyslane do InfluxDB");
           }
        } else {
           if(DEBUG){
            Serial.println("Blad wysylania danych do InfluxDB");
           }
        }
      row.empty();
    }
    if(DEBUG){
      Serial.println("1 minuta!\n");
    }
    counter2 = 0;  
  }

  counter3++;
  //execute every ~20 sec (około 20 sekund) - 900
  //execute every ~30 sec (około 30 sekund) - 1350
  if (counter3 >= 900){

    pmMeasurements[iPM][0] = int(calib * data.PM_AE_UG_1_0);
    pmMeasurements[iPM][1] = int(calib * data.PM_AE_UG_2_5);
    pmMeasurements[iPM][2] = int(calib * data.PM_AE_UG_10_0);
    
    if(DEBUG){
      Serial.println(iPM);
      Serial.println(pmMeasurements[iPM][0]);
      Serial.println(pmMeasurements[iPM][1]);
      Serial.println(pmMeasurements[iPM][2]);
      Serial.println("20 sekund!\n");
    }
    averagePM();
    if(DEBUG){
      Serial.print("Srednia PM1: ");
      Serial.println(averagePM1);
      Serial.print("Srednia PM2.5: ");
      Serial.println(averagePM25);
      Serial.print("Srednia PM10: ");
      Serial.println(averagePM10);
    }
    counter3 = 0;
    iPM++;
    if (iPM >= 5) {
      iPM = 0;
    }
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
      message += "<center><h1>Smogomierz</h1><br>";

      if (int(BMESensor.temperature) == 0 && int(BMESensor.humidity) == 0 && int(BMESensor.pressure  / 100.0F) == 0){
        Serial.println("Brak pomiarow z BME280!\n");
      }else{
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
      }
      message += "<p><h2>Pomiary zanieczyszczeń:</h2>";
      
      if (DISPLAY_PM1){
          message += "<h3>PM1: ";
          message += (averagePM1);
          message += " µg/m³</h3>";
        }
        message += "<h3>PM2.5: ";
        if (averagePM25 <= 10){
          message += "<font color='#61EEE4'>";
          message += (averagePM25);
          message += " µg/m³</h3></font>";
        } else if (averagePM25 > 10 && averagePM25 <= 20){
          message += "<font color='#5BCAAA'>";
          message += (averagePM25);
          message += " µg/m³</h3></font>";
        } else if (averagePM25 > 20 && averagePM25 <= 25){
          message += "<font color='#EEE25D'>";
          message += (averagePM25);
          message += " µg/m³</h3></font>";
        } else if (averagePM25 > 25 && averagePM25 <= 50){
          message += "<font color='#F95459'>";
          message += (averagePM25);
          message += " µg/m³</h3></font>";
        } else if (averagePM25 > 50){
          message += "<font color='#920736'>";
          message += (averagePM25);
          message += " µg/m³</h3></font>";
        } else {
          message += (averagePM25);
          message += " µg/m³</h3>";
        }
        
        message += "<h3>PM10: ";
        if (averagePM10 <= 20){
          message += "<font color='#61EEE4'>";
          message += (averagePM10);
          message += " µg/m³</h3></font>";
        } else if (averagePM10 > 20 && averagePM10 <= 35){
          message += "<font color='#5BCAAA'>";
          message += (averagePM10);
          message += " µg/m³</h3></font>";
        } else if (averagePM10 > 35 && averagePM10 <= 50){
          message += "<font color='#EEE25D'>";
          message += (averagePM10);
          message += " µg/m³</h3></font>";
        } else if (averagePM10 > 50 && averagePM10 <= 100){
          message += "<font color='#F95459'>";
          message += (averagePM10);
          message += " µg/m³</h3></font>";
        } else if (averagePM10 > 100){
          message += "<font color='#920736'>";
          message += (averagePM10);
          message += " µg/m³</h3></font>";
        } else {
          message += (averagePM10);
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
      /*
      if (DISPLAY_PM1) {
        message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
        message += (THINGSPEAK_CHANNEL_ID);
        message += ("/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=PM1&type=line&yaxis=ug%2Fm3&update=15'></iframe>");
        message += (" ");
      }*/
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=PM2.5&type=line&yaxis=ug%2Fm3&update=15'></iframe>");
      message += (" ");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=PM10&type=line&yaxis=ug%2Fm3&update=15'></iframe>");
      message += (" ");
      if (int(BMESensor.temperature) == 0 && int(BMESensor.humidity) == 0 && int(BMESensor.pressure  / 100.0F) == 0){
        Serial.println("Brak pomiarow z BME280!\n");
      }else{
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Temperatura&type=line&update=15'></iframe>");
      message += (" ");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Ci%C5%9Bnienie&type=line&update=15'></iframe>");
      message += (" ");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/6?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Wilgotno%C5%9B%C4%87&type=line&update=15'></iframe>");
      }
    }

    message += "</form></div>";
    message += "</center></main></body></html>";
    WebServer.send(200, "text/html", message);
  }

void handle_config() {            //Handler for the handle_config
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
    message += "<br>";
    
    message += "<b>Wyświetlanie pomiarów PM1: </b>";
    message += (DISPLAY_PM1);
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
    message += "<br><b>Wysokość: </b>";
    message += (int(MYALTITUDE));
    message += " m.n.p.m<br><br>";
  
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

    message += "<b>Wysyłanie danych do InfluxDB: </b>";
    message += (INFLUXDB_ON);
    message += "<br><b>Adres bazy danych InfluxDB: </b>";
    message += (INFLUXDB_HOST);
    message += "<br><b>Port InfluxDB: </b>";
    message += (INFLUXDB_PORT);
    message += "<br><b>Nazwa bazy danych: </b>";
    message += (DATABASE);
    message += "<br><b>Użytkownik bazy danych: </b>";
    message += (DB_USER);
    message += "<br><b>Hasło do bazy danych: </b>";
    message += (DB_PASSWORD);
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
    message += (SOFTWAREVERSION);
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
        json["pm1"] = averagePM1;
        json["pm25"] = averagePM25;
        json["pm10"] = averagePM10;
        if (int(BMESensor.temperature) == 0 && int(BMESensor.humidity) == 0 && int(BMESensor.pressure  / 100.0F) == 0){
          Serial.println("Brak pomiarow z BME280!\n");
        }else{
          json["temperature"] = float(BMESensor.temperature);
          json["pressure"] = int(BMESensor.pressure  / 100.0F);
          json["humidity"] = int(BMESensor.humidity);
          json["dewpoint"] = int(BMESensor.temperature-((100-BMESensor.humidity)/5));    
        }

      json.prettyPrintTo(message);
    WebServer.send(200, "text/json", message);
  }

void pm_calibration(){
  if (int(BMESensor.temperature) < 5 or int(BMESensor.humidity) > 60){
    calib = calib2;
    } else {
    calib = calib1;
    }
}

int averagePM() {
  averagePM1=0;
  averagePM25=0;
  averagePM10=0;
  for (int i=0; i < 5; i++){
    averagePM1 += pmMeasurements[i][0];
    averagePM25 += pmMeasurements[i][1];
    averagePM10  += pmMeasurements[i][2];
  }
  if(DEBUG){
    Serial.print("\nWartość averagePM1: ");
    Serial.println(averagePM1);
    Serial.print("Wartość averagePM25: ");
    Serial.println(averagePM25);
    Serial.print("Wartość averagePM10: ");
    Serial.println(averagePM10);
  }
  averagePM1 = averagePM1/5;
  averagePM25 = averagePM25/5;
  averagePM10 = averagePM10/5;
  return averagePM1, averagePM25, averagePM10;
}

