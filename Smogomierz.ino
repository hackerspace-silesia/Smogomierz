<<<<<<< Updated upstream
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <SoftwareSerial.h>
#include <ESP8266HTTPUpdateServer.h>
=======
#ifdef ARDUINO_ARCH_ESP8266 // ESP8266 core for Arduino - 2.5.2 or later
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#endif

/*

  Szkic używa 493952 bajtów (47%) pamięci programu. Maksimum to 1044464 bajtów.
  Zmienne globalne używają 54776 bajtów (66%) pamięci dynamicznej, pozostawiając 27144 bajtów dla zmiennych lokalnych. Maksimum to 81920 bajtów.

  Szkic używa 500328 bajtów (47%) pamięci programu. Maksimum to 1044464 bajtów.
  Zmienne globalne używają 54056 bajtów (65%) pamięci dynamicznej, pozostawiając 27864 bajtów dla zmiennych lokalnych. Maksimum to 81920 bajtów.

  Szkic używa 526616 bajtów (50%) pamięci programu. Maksimum to 1044464 bajtów.
  Zmienne globalne używają 51764 bajtów (63%) pamięci dynamicznej, pozostawiając 30156 bajtów dla zmiennych lokalnych. Maksimum to 81920 bajtów.

*/
>>>>>>> Stashed changes

#include <Wire.h>

#include "FS.h"
<<<<<<< Updated upstream
#include "ArduinoJson.h" // 6.5.0 beta or later !!!
#include "src/WiFiManager.h" // https://github.com/jakerabid/WiFiManager
#include "src/pms.h" // https://github.com/fu-hsi/PMS
#include "src/bme280.h" // https://github.com/zen/BME280_light
#include "src/HTU21D.h" // https://github.com/enjoyneering/HTU21D
=======
#include <ArduinoJson.h> // 6.9.0 or later
//#include "src/WiFiManager.h" // https://github.com/jakerabid/WiFiManager // 12.03.2019
#include "src/WiFiManager.h" // https://github.com/tzapu/WiFiManager/tree/development // 20.08.2019  DEV
#include "src/bme280.h" // https://github.com/zen/BME280_light // CUSTOMIZED! 8.04.2019
#include "src/HTU21D.h" // https://github.com/enjoyneering/HTU21D // 12.03.2019
#include "src/Adafruit_BMP280.h" // https://github.com/adafruit/Adafruit_BMP280_Library // 12.03.2019
#include "src/SHT1x.h" // https://github.com/practicalarduino/SHT1x // 12.03.2019
#include <DHT.h>
>>>>>>> Stashed changes

//#include "src/Adafruit_HTU21DF.h" // https://github.com/adafruit/Adafruit_HTU21DF_Library

#include "src/Adafruit_BMP280.h" // https://github.com/adafruit/Adafruit_BMP280_Library
//#include "src/SdsDustSensor.h" // SDS011/SDS021 - https://github.com/lewapek/sds-dust-sensors-arduino-library
#include "src/hpma115S0.h" // Honeywell HPMA115S0-XXX - https://github.com/jalmeroth/ESP8266-Honeywell

#include "src/spiffs.h"
#include "src/airmonitor.h"
#include "src/thing_speak.h"
#include "src/ESPinfluxdb.h"
#include "src/autoupdate.h"

#include "src/config.h"
#include "defaultConfig.h"

/*
  Podłączenie czujnikow dla ESP8266 NodeMCU:
  BME280/BMP280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  SHT21/HTU21D: VIN - 3V; GND - G; SCL - D5; SDA - D6
  PMS7003: Bialy - VIN/5V; Czarny - G; Zielony/TX - D1; Niebieski/RX - D2
  SDS011/21: VIN - 5V; GND - G; TX - D7; RX - D8


  Connection of sensors on ESP8266 NodeMCU:
  BME280/BMP280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  SHT21/HTU21D: VIN - 3V; GND - G; SCL - D5; SDA - D6
  PMS7003:/White - VIN/5V; Black - G; Green/TX - D1; Blue/RX - D2
  SDS011/21: VIN - 5V; GND - G; TX - D7; RX - D8

*/

// BME280 config
#define ASCII_ESC 27
char bufout[10];
BME280<> BMESensor;

// BMP280 config
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
Adafruit_BMP280 bmp;

// Serial for SHT21/HTU21D config
HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);

// Serial for PM detector
SoftwareSerial PMSSerial(5, 4); // TX - D1 and RX - D2 pins
//SoftwareSerial SDSSerial(13, 15); // TX - D7 and RX - D8 pins

// PMS7003 config
PMS pms(PMSSerial);
PMS::DATA PMSdata;

// SDS011/21 config
//SDS011 sds(SDSSerial);
//SdsDustSensor sds(13, 15);

// HPMA115S0 config

char device_name[20];

int AIRMONITOR_counter, DUST_counter, THINGSPEAK_counter;
//int INFLUXDB_counter = -180; // Start sending data after 180 sec
int INFLUXDB_counter = -(DUST_TIME * 120); // Start sending data after ~180 sec
int REBOOT_counter = -(380 * 1440); // Auto reboot evry 24h

int pmMeasurements[20][3];
int iPM;
int averagePM1, averagePM25, averagePM10 = 0;
int bootupdate = 0;
float calib = 1;
float SDSpm25, SDSpm10;
//int SDSdata;

ESP8266WebServer WebServer(80);
ESP8266HTTPUpdateServer httpUpdater;

<<<<<<< Updated upstream
=======
WiFiClient espClient;
PubSubClient mqttclient(espClient);

WiFiManager wifiManager;

// check TEMP/HUMI/PRESS Sensor - START
>>>>>>> Stashed changes
bool checkHTU21DStatus() {
  int temperature_HTU21D_Int = int(myHTU21D.readTemperature());
  int humidity_HTU21D_Int = int(myHTU21D.readHumidity());

  if (temperature_HTU21D_Int == 0 && humidity_HTU21D_Int == 0) {
    if (DEBUG) {
      if (SELECTED_LANGUAGE == 1) {
        Serial.println("No data from HTU21D sensor!\n");
        return false;
      } else if (SELECTED_LANGUAGE == 2) {
        Serial.println("Brak pomiarów z HTU21D!\n");
        return false;
      } else {
        Serial.println("No data from HTU21D sensor!\n");
        return false;
      }
    }
    return false;
  } else if (temperature_HTU21D_Int == 255 && humidity_HTU21D_Int == 255) {
    if (DEBUG) {
      if (SELECTED_LANGUAGE == 1) {
        Serial.println("No data from HTU21D sensor!\n");
        return false;
      } else if (SELECTED_LANGUAGE == 2) {
        Serial.println("Brak pomiarów z HTU21D!\n");
        return false;
      } else {
        Serial.println("No data from HTU21D sensor!\n");
        return false;
      }
    }
    return false;
  } else {
    return true;
  }
}

bool checkBmeStatus() {
  int temperatureInt = BMESensor.temperature;
  int pressureInt = (BMESensor.seaLevelForAltitude(MYALTITUDE));
  int humidityInt = BMESensor.humidity;
  if (temperatureInt == 0 && pressureInt == 0 && humidityInt == 0) {
    if (DEBUG) {
      if (SELECTED_LANGUAGE == 1) {
        Serial.println("No data from BME280 sensor!\n");
        return false;
      } else if (SELECTED_LANGUAGE == 2) {
        Serial.println("Brak pomiarów z BME280!\n");
        return false;
      } else {
        Serial.println("No data from BME280 sensor!\n");
        return false;
      }
    }
    return false;
  } else {
    return true;
  }
}

bool checkBmpStatus() {
  int temperature_BMP_Int = bmp.readTemperature();
  int pressure_BMP_Int = bmp.readPressure();
  if (temperature_BMP_Int == 0 && pressure_BMP_Int == 0) {
    if (DEBUG) {
      if (SELECTED_LANGUAGE == 1) {
        Serial.println("No data from BMP280 sensor!\n");
        return false;
      } else if (SELECTED_LANGUAGE == 2) {
        Serial.println("Brak pomiarów z BMP280!\n");
        return false;
      } else {
        Serial.println("No data from BMP280 sensor!\n");
        return false;
      }
    }
    return false;
  } else {
    return true;
  }
}

// library doesnt support arguments :/
#include "src/webserver.h"

void setup() {
  Serial.begin(115200);
  delay(10);

  fs_setup();
  delay(10);

  if (!strcmp(THP_MODEL, "BME280")) {
    Wire.begin(0, 2);
    BMESensor.begin();
  }
  if (!strcmp(THP_MODEL, "BMP280")) {
    Wire.begin(0, 2);
    bmp.begin();
  }
  if (!strcmp(THP_MODEL, "HTU21")) {
    myHTU21D.begin();
  }
  delay(10);

  if (!strcmp(DUST_MODEL, "PMS7003")) {
    PMSSerial.begin(9600); //PMS7003 serial
  }
  if (!strcmp(DUST_MODEL, "SDS011/21")) {
    //SDSSerial.begin(9600); //SDS011/21 serial
    /*
    sds.begin();  //SDS011/21 sensor begin
    Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
    Serial.println(sds.setQueryReportingMode().toString()); // ensures sensor is in 'query' reporting mode
    Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended
    */
  }
  if (!strcmp(DUST_MODEL, "HPMA115S0")) {
    HPMASerial.begin(9600); //HPMA115S0 serial // do poprawki !!
  }
  delay(10);

  // get ESP id
  if (DEVICENAME_AUTO) {
#ifdef ARDUINO_ARCH_ESP8266
    sprintf(device_name, "Smogomierz-%06X", ESP.getChipId());
#elif defined ARDUINO_ARCH_ESP32
    sprintf(device_name, "Smogomierz-%06X", ESP.getEfuseMac());
#endif
  } else {
    strncpy(device_name, DEVICENAME, 20);
  }

  Serial.print("Device name: ");
  Serial.println(device_name);
  /*
    WiFiManager wifiManager;
    wifiManager.autoConnect(device_name);

    delay(250);

<<<<<<< Updated upstream
  if (!wifiManager.autoConnect(device_name)) {
    Serial.println("failed to connect...");
    delay(1000);
    ESP.reset(); //reset and try again
    delay(5000);
  }

  if (AUTOUPDATE_ON) {
    if (checkUpdate() == true) {
      bootupdate = 1;
    }
=======
    if (!wifiManager.autoConnect(device_name)) {
      Serial.println("Failed to connect...");
      delay(1000);
      ESP.reset(); //reset and try again
      delay(5000);
    }
  */

  if (wifiManager.autoConnect(device_name)) {
    Serial.println("connected...yeey :)");
    //wifiManager.setConfigPortalBlocking(false);
  } else {
    Serial.println("Configportal running");
    wifiManager.setConfigPortalBlocking(false);
  }
  delay(250);

  /*
    // check update
    if (checkUpdate(0) == true) {
      need_update = true;
    } else {
      need_update = false;
    }
  */

  if (MQTT_ON) {
    mqttclient.setServer(MQTT_HOST, MQTT_PORT);
>>>>>>> Stashed changes
  }

  if (INFLUXDB_ON) {
    Influxdb influxdb(INFLUXDB_HOST, INFLUXDB_PORT);
    if (influxdb.opendb(DATABASE, DB_USER, DB_PASSWORD) != DB_SUCCESS) {
      Serial.println("Opening database failed");
    } else {
      Serial.println("Opening database succeed");
    }
  }

  //  WebServer config - Start
  WebServer.on("/", HTTP_GET,  handle_root);
  WebServer.on("/config", HTTP_POST, handle_config_post);
  WebServer.on("/config", HTTP_GET, handle_config);
  WebServer.on("/update", HTTP_GET, handle_update);
  WebServer.on("/help", HTTP_GET, handle_help);
  WebServer.on("/contact", HTTP_GET, handle_contact);
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
  // check update
  if (AUTOUPDATE_ON) {
    if (bootupdate == 1) {
      for (int i = 0; i < 5 ; i++) {
        doUpdate();
        delay(10);
      }
    }
    bootupdate = 0;
  }

  BMESensor.refresh();
  delay(10);

  pm_calibration();
  delay(10);

  if (!strcmp(DUST_MODEL, "PMS7003")) {
    while (PMSSerial.available()) {
      PMSSerial.listen();
      delay(10);
      pms.read(PMSdata);
    }
  }
  /*
    if (!strcmp(DUST_MODEL, "SDS011/21")) {
    while (SDSSerial.available()) {
      SDSSerial.listen();
      delay(10);
      //SDSdata = sds.read(&SDSpm25, &SDSpm25);
      //PmResult SDSdata = sds.queryPm();
    }
    } */
  //PmResult SDSdata = sds.queryPm();
  if (!strcmp(DUST_MODEL, "HPMA115S0")) {

  }
  delay(10);

  //webserverShowSite(WebServer, BMESensor, PMSdata, SDSdata); // dodać HPMAdata !!
  WebServer.handleClient();
  delay(10);

  yield();

  if (AIRMONITOR_ON) {
    AIRMONITOR_counter++; //execute every ~1 minute - 60
    if (AIRMONITOR_counter >= (AIRMONITOR_TIME * 60 * 23)) {
      sendDataToAirMonitor(BMESensor, averagePM1, averagePM25, averagePM10);
      if (DEBUG) {
        if (SELECTED_LANGUAGE == 1) {
          Serial.println("Sending measurement data to the AirMonitor service!\n");
        } else if (SELECTED_LANGUAGE == 2) {
          Serial.println("Wysłanie danych pomiarowych do serwisu AirMonitor!\n");
        } else {
          Serial.println("Sending measurement data to the AirMonitor service!\n");
        }
      }
      AIRMONITOR_counter = 0;
    }
  }

  if (THINGSPEAK_ON) {
    THINGSPEAK_counter++; //execute every ~1 minute - 60
    if (THINGSPEAK_counter >= (THINGSPEAK_TIME * 60 * 23)) {
      sendDataToThingSpeak(BMESensor, averagePM1, averagePM25, averagePM10);
      if (DEBUG) {
        if (SELECTED_LANGUAGE == 1) {
          Serial.println("Sending measurement data to the Thingspeak service!\n");
        } else if (SELECTED_LANGUAGE == 2) {
          Serial.println("Wysłanie danych pomiarowych do serwisu Thingspeak!\n");
        } else {
          Serial.println("Sending measurement data to the Thingspeak service!\n");
        }
      }
      THINGSPEAK_counter = 0;
    }
  }


  if (INFLUXDB_ON) {
    INFLUXDB_counter++; //execute every ~1 minute - 60
    if (INFLUXDB_counter >= (INFLUXDB_TIME * 23)) {
      Influxdb influxdb(INFLUXDB_HOST, INFLUXDB_PORT);
      if (influxdb.opendb(DATABASE, DB_USER, DB_PASSWORD) != DB_SUCCESS) {
        Serial.println("Opening database failed");
      } else {
        dbMeasurement row(device_name);
        if (DEBUG) {
          if (SELECTED_LANGUAGE == 1) {
            Serial.println("Measurements from PM sensor!\n");
          } else if (SELECTED_LANGUAGE == 2) {
            Serial.println("Dane z miernika pyłu!\n");
          } else {
            Serial.println("Measurements from PM sensor!\n");
          }
        }
        row.addField("pm1", averagePM1);
        row.addField("pm25", averagePM25);
        row.addField("pm10", averagePM10);

        if (!strcmp(THP_MODEL, "BME280")) {
          if (checkBmeStatus() == true) {
            if (DEBUG) {
              if (SELECTED_LANGUAGE == 1) {
                Serial.println("Measurements from BME280!\n");
              } else if (SELECTED_LANGUAGE == 2) {
                Serial.println("Dane z BME280!\n");
              } else {
                Serial.println("Measurements from BME280!\n");
              }
            }
            row.addField("temperature", (BMESensor.temperature));
            row.addField("pressure", (BMESensor.seaLevelForAltitude(MYALTITUDE)));
            row.addField("humidity", (BMESensor.humidity));
          } else {
            if (DEBUG) {
              if (SELECTED_LANGUAGE == 1) {
                Serial.println("No measurements from BME280!\n");
              } else if (SELECTED_LANGUAGE == 2) {
                Serial.println("Brak pomiarów z BME280!\n");
              } else {
                Serial.println("No measurements from BME280!\n");
              }
            }
          }
        }

        if (!strcmp(THP_MODEL, "HTU21")) {
          if (checkHTU21DStatus() == true) {
            if (DEBUG) {
              if (SELECTED_LANGUAGE == 1) {
                Serial.println("Measurements from HTU21!\n");
              } else if (SELECTED_LANGUAGE == 2) {
                Serial.println("Dane z HTU21!\n");
              } else {
                Serial.println("Measurements from HTU21!\n");
              }
            }
            row.addField("temperature", (myHTU21D.readTemperature()));
            row.addField("humidity", (myHTU21D.readHumidity()));
          } else {
            if (DEBUG) {
              if (SELECTED_LANGUAGE == 1) {
                Serial.println("No measurements from HTU21D!\n");
              } else if (SELECTED_LANGUAGE == 2) {
                Serial.println("Brak pomiarów z HTU21D!\n");
              } else {
                Serial.println("No measurements from HTU21D!\n");
              }
            }
          }
        }

        if (!strcmp(THP_MODEL, "BMP280")) {
          if (checkBmpStatus() == true) {
            if (DEBUG) {
              if (SELECTED_LANGUAGE == 1) {
                Serial.println("Measurements from BMP280!\n");
              } else if (SELECTED_LANGUAGE == 2) {
                Serial.println("Dane z BMP280!\n");
              } else {
                Serial.println("Measurements from BMP280!\n");
              }
            }
            row.addField("temperature", (bmp.readTemperature()));
            row.addField("pressure", ((bmp.readPressure()) / 100));
          } else {
            if (DEBUG) {
              if (SELECTED_LANGUAGE == 1) {
                Serial.println("No measurements from BMP280!\n");
              } else if (SELECTED_LANGUAGE == 2) {
                Serial.println("Brak pomiarów z BMP280!\n");
              } else {
                Serial.println("No measurements from BMP280!\n");
              }
            }
          }
        }

        if (influxdb.write(row) == DB_SUCCESS) {
          if (DEBUG) {
            if (SELECTED_LANGUAGE == 1) {
              Serial.println("Data sent to InfluxDB\n");
            } else if (SELECTED_LANGUAGE == 2) {
              Serial.println("Dane wysłane do InfluxDB\n");
            } else {
              Serial.println("Data sent to InfluxDB\n");
            }
          } else {
            if (DEBUG) {
              if (SELECTED_LANGUAGE == 1) {
                Serial.println("Error sending data to InfluxDB\n");
              } else if (SELECTED_LANGUAGE == 2) {
                Serial.println("Błąd wysyłania danych do InfluxDB\n");
              } else {
                Serial.println("Error sending data to InfluxDB\n");
              }
            }
          }
          row.empty();
        }
      }
      INFLUXDB_counter = 0;
    }
  }

  DUST_counter++; //execute every ~20 sec (około 20 sekund) - 20
  if (DUST_counter >= (DUST_TIME * 23)) {
    //if (DUST_counter >= 220){
    if (!strcmp(DUST_MODEL, "PMS7003")) {
      pmMeasurements[iPM][0] = float(calib * PMSdata.PM_AE_UG_1_0);
      pmMeasurements[iPM][1] = float(calib * PMSdata.PM_AE_UG_2_5);
      pmMeasurements[iPM][2] = float(calib * PMSdata.PM_AE_UG_10_0);
    }
    if (!strcmp(DUST_MODEL, "SDS011/21")) {
      /*
      if (SDSdata.isOk()) {
        pmMeasurements[iPM][0] = int(calib * 0);
        pmMeasurements[iPM][1] = int(calib * SDSdata.pm25);
        pmMeasurements[iPM][2] = int(calib * SDSdata.pm10);
      } else {
        Serial.println("Could not read values from SDS sensor :( ");
      } */
        pmMeasurements[iPM][0] = int(calib * 0);
        pmMeasurements[iPM][1] = int(calib * pm25);
        pmMeasurements[iPM][2] = int(calib * pm10);
    }
    if (!strcmp(DUST_MODEL, "HPMA115S0")) {
      pmMeasurements[iPM][0] = int(calib * 0);
      pmMeasurements[iPM][1] = int(calib * pm25); // do poprawki !!
      pmMeasurements[iPM][2] = int(calib * pm10); // do poprawki !!
    }
    if (!strcmp(DUST_MODEL, "Non")) {
      pmMeasurements[iPM][0] = int(calib * 0);
      pmMeasurements[iPM][1] = int(calib * 0);
      pmMeasurements[iPM][2] = int(calib * 0);
    }

    if (DEBUG) {
      Serial.print("\n" + String(DUST_TIME) + " sekund!");
      Serial.print("\n\nNumer pomiaru PM: ");
      Serial.print(iPM);
      Serial.print("\nWartość PM1: ");
      Serial.print(pmMeasurements[iPM][0]);
      Serial.print("\nWartość PM2.5: ");
      Serial.print(pmMeasurements[iPM][1]);
      Serial.print("\nWartość PM10: ");
      Serial.print(pmMeasurements[iPM][2]);
    }
    averagePM();
    if (DEBUG) {
      if (SELECTED_LANGUAGE == 1) {
        Serial.print("Average PM1: ");
        Serial.println(averagePM1);
        Serial.print("Average PM2.5: ");
        Serial.println(averagePM25);
        Serial.print("Average PM10: ");
        Serial.println(averagePM10);
      } else if (SELECTED_LANGUAGE == 2) {
        Serial.print("Średnia PM1: ");
        Serial.println(averagePM1);
        Serial.print("Średnia PM2.5: ");
        Serial.println(averagePM25);
        Serial.print("Średnia PM10: ");
        Serial.println(averagePM10);
      } else {
        Serial.print("Average PM1: ");
        Serial.println(averagePM1);
        Serial.print("Average PM2.5: ");
        Serial.println(averagePM25);
        Serial.print("Average PM10: ");
        Serial.println(averagePM10);
      }
    }
    DUST_counter = 0;
    iPM++;
    if (iPM >= NUMBEROFMEASUREMENTS) {
      iPM = 0;
    }
  }

  REBOOT_counter++;
  if (REBOOT_counter >= 0) {
    Serial.println("autoreboot...");
    delay(1000);
    ESP.reset();
    delay(5000);
  }
  //delay(10);
}

void pm_calibration() {
  if (!strcmp(MODEL, "white")) { //if autocalibration
    if (!strcmp(DUST_MODEL, "PMS7003")) {
      if (!strcmp(THP_MODEL, "BME280")) {
        calib1 = float((200 - (BMESensor.humidity)) / 150);
        calib2 = calib1 / 2;
        if (int(BMESensor.temperature) < 5 and int(BMESensor.humidity) > 60) {
          calib = calib2;
        } else {
          calib = calib1;
        }
      }
      if (!strcmp(THP_MODEL, "HTU21")) {
        calib1 = float((200 - (myHTU21D.readCompensatedHumidity())) / 150);
        calib2 = calib1 / 2;
        if (int(myHTU21D.readTemperature()) < 5 and int(myHTU21D.readCompensatedHumidity()) > 60) {
          calib = calib2;
        } else {
          calib = calib1;
        }
      }
    }
    if (!strcmp(DUST_MODEL, "SDS011/21")) {
      if (!strcmp(THP_MODEL, "BME280")) {
        calib1 = float((200 - (BMESensor.humidity)) / 150);
        calib2 = calib1 / 2;
        if (int(BMESensor.temperature) < 5 and int(BMESensor.humidity) > 60) {
          calib = calib2;
        } else {
          calib = calib1;
        }
      }
      if (!strcmp(THP_MODEL, "HTU21")) {
        calib1 = float((200 - (myHTU21D.readCompensatedHumidity())) / 150);
        calib2 = calib1 / 2;
        if (int(myHTU21D.readTemperature()) < 5 and int(myHTU21D.readCompensatedHumidity()) > 60) {
          calib = calib2;
        } else {
          calib = calib1;
        }
      }
    }
    if (!strcmp(DUST_MODEL, "HPMA115S0")) {
      if (!strcmp(THP_MODEL, "BME280")) {
        calib1 = float((200 - (BMESensor.humidity)) / 150);
        calib2 = calib1 / 2;
        if (int(BMESensor.temperature) < 5 and int(BMESensor.humidity) > 60) {
          calib = calib2;
        } else {
          calib = calib1;
        }
      }
      if (!strcmp(THP_MODEL, "HTU21")) {
        calib1 = float((200 - (myHTU21D.readCompensatedHumidity())) / 150);
        calib2 = calib1 / 2;
        if (int(myHTU21D.readTemperature()) < 5 and int(myHTU21D.readCompensatedHumidity()) > 60) {
          calib = calib2;
        } else {
          calib = calib1;
        }
      }
    }
  } else {
    if (!strcmp(THP_MODEL, "BME280")) {
      if (int(BMESensor.temperature) < 5 and int(BMESensor.humidity) > 60) {
        calib = calib2;
      }
      calib = calib1;
    }
    if (!strcmp(THP_MODEL, "HTU21")) {
      if (int(myHTU21D.readTemperature()) < 5 and int(myHTU21D.readCompensatedHumidity()) > 60) {
        calib = calib2;
      }
      calib = calib1;
    }
    if (!strcmp(THP_MODEL, "BMP280")) {
      if (int(bmp.readTemperature()) < 5) {
        calib = calib2;
      }
      calib = calib1;
    }
  }
}

int averagePM() {
  averagePM1 = 0;
  averagePM25 = 0;
  averagePM10 = 0;
  for (int i = 0; i < NUMBEROFMEASUREMENTS; i++) {
    averagePM1 += pmMeasurements[i][0];
    averagePM25 += pmMeasurements[i][1];
    averagePM10 += pmMeasurements[i][2];
  }
  if (DEBUG) {
    Serial.print("\naveragePM1: ");
    Serial.println(averagePM1);
    Serial.print("averagePM25: ");
    Serial.println(averagePM25);
    Serial.print("averagePM10: ");
    Serial.println(averagePM10);
    Serial.print("\n");
  }
  averagePM1 = averagePM1 / NUMBEROFMEASUREMENTS;
  averagePM25 = averagePM25 / NUMBEROFMEASUREMENTS;
  averagePM10 = averagePM10 / NUMBEROFMEASUREMENTS;
  return averagePM1, averagePM25, averagePM10;
}


