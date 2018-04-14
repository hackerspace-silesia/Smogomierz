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
int pmMeasurements[10][3];

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
    if (iPM >= 10) {
      iPM = 0;
    }
  }
  
}

// library doesnt support arguments :/
#include "src/webserver.cpp"

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
  for (int i=0; i < 10; i++){
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
  averagePM1 = averagePM1/10;
  averagePM25 = averagePM25/10;
  averagePM10 = averagePM10/10;
  return averagePM1, averagePM25, averagePM10;
}


