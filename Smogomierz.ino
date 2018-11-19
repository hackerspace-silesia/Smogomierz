#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "FS.h"
#include "ArduinoJson.h" // 6.5.0 beta or later
#include "src/WiFiManager.h" // https://github.com/jakerabid/WiFiManager
#include "src/bme280.h" // https://github.com/zen/BME280_light
#include "src/HTU21D.h" // https://github.com/enjoyneering/HTU21D
#include "src/Adafruit_BMP280.h" // https://github.com/adafruit/Adafruit_BMP280_Library
#include <DHT.h>

#include "src/pms.h" // https://github.com/fu-hsi/PMS

#include "src/spiffs.h"
#include "src/config.h"
#include "defaultConfig.h"

#include "src/airmonitor.h"
#include "src/thing_speak.h"
#include "src/ESPinfluxdb.h"

/*
  Podłączenie czujnikow dla ESP8266 NodeMCU:
  BME280/BMP280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  SHT21/HTU21D: VIN - 3V; GND - G; SCL - D5; SDA - D6
  DHT22: VIN - 3V; GND - G; D7
  PMS5003/7003: Bialy - VIN/5V; Czarny - G; Zielony/TX - D1; Niebieski/RX - D2


  Connection of sensors on ESP8266 NodeMCU:
  BME280/BMP280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  SHT21/HTU21D: VIN - 3V; GND - G; SCL - D5; SDA - D6
  DHT22: VIN - 3V; GND - G; D7
  PMS5003/7003: White - VIN/5V; Black - G; Green/TX - D1; Blue/RX - D2

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
HTU21D  myHTU21D(HTU21D_RES_RH12_TEMP14);

// DHT22 config
#define DHTPIN 13 // D7 on NodeMCU/WeMos board
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

// Serial for PMS7003 config
SoftwareSerial mySerial(5, 4); // Change TX - D1 and RX - D2 pins
PMS pms(mySerial);
PMS::DATA data;

char device_name[20];

unsigned long DUST_interval = 1000; // 1 second
unsigned long previous_DUST_Millis = 0;

unsigned long AIRMONITOR_interval = 60 * 1000; // 1 minute
unsigned long previous_AIRMONITOR_Millis = 0;
unsigned long THINGSPEAK_interval = 60 * 1000; // 1 minute
unsigned long previous_THINGSPEAK_Millis = 0;
unsigned long INFLUXDB_interval = 1000; // 1 second
unsigned long previous_INFLUXDB_Millis = 0;

unsigned long REBOOT_interval = 24 * 60 * 60 * 1000; // 24 hours
unsigned long previous_REBOOT_Millis = 0;

int pmMeasurements[10][3];
int iPM, averagePM1, averagePM25, averagePM10 = 0;
float calib = 1;

ESP8266WebServer WebServer(80);
ESP8266HTTPUpdateServer httpUpdater;

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
      }
    }
    return false;
  } else {
    return true;
  }
}

bool checkBmeStatus() {
  int temperature_BME280_Int = BMESensor.temperature;
  int pressure_BME280_Int = (BMESensor.seaLevelForAltitude(MYALTITUDE));
  int humidity_BME280_Int = BMESensor.humidity;
  if (temperature_BME280_Int == 0 && pressure_BME280_Int == 0 && humidity_BME280_Int == 0) {
    if (DEBUG) {
      if (SELECTED_LANGUAGE == 1) {
        Serial.println("No data from BME280 sensor!\n");
        return false;
      } else if (SELECTED_LANGUAGE == 2) {
        Serial.println("Brak pomiarów z BME280!\n");
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
      }
    }
    return false;
  } else {
    return true;
  }
}

bool checkDHT22Status() {
  int humidity_DHT_Int = dht.readHumidity();
  int temperature_DHT_Int = dht.readTemperature();
  if (humidity_DHT_Int == 0 && temperature_DHT_Int == 0) {
    if (DEBUG) {
      if (SELECTED_LANGUAGE == 1) {
        Serial.println("No data from DHT22 sensor!\n");
        return false;
      } else if (SELECTED_LANGUAGE == 2) {
        Serial.println("Brak pomiarów z DHT22!\n");
        return false;
      } else {
        Serial.println("No data from DHT22 sensor!\n");
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

  if (!strcmp(DUST_MODEL, "PMS7003")) {
    mySerial.begin(9600); //PMS7003 serial
  }
  delay(10);

  if (strcmp(DUST_MODEL, "Non")) {
    DUST_interval = DUST_TIME * 1000; // seconds
  }
  if (AIRMONITOR_ON) {
    AIRMONITOR_interval = AIRMONITOR_TIME * 60 * 1000; // minutes
  }
  if (THINGSPEAK_ON) {
    THINGSPEAK_interval = THINGSPEAK_TIME * 60 * 1000; // minutes
  }
  if (INFLUXDB_ON) {
    INFLUXDB_interval =  INFLUXDB_TIME * 1000; // seconds
  }
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
  if (!strcmp(THP_MODEL, "DHT22")) {
    dht.begin();
  }
  delay(10);

  // get ESP id
  if (DEVICENAME_AUTO) {
    sprintf(device_name, "Smogomierz-%06X", ESP.getChipId());
  } else {
    strncpy(device_name, DEVICENAME, 20);
  }

  Serial.print("Device name: ");
  Serial.println(device_name);

  WiFiManager wifiManager;
  wifiManager.autoConnect(device_name);

  delay(250);

  if (!wifiManager.autoConnect(device_name)) {
    Serial.println("failed to connect...");
    delay(1000);
    ESP.reset(); //reset and try again
    delay(5000);
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

  if (AIRMONITOR_ON) {
    unsigned long current_AIRMONITOR_Millis = millis();
    if (current_AIRMONITOR_Millis - previous_AIRMONITOR_Millis >= AIRMONITOR_interval) {
      sendDataToAirMonitor(BMESensor, averagePM1, averagePM25, averagePM10);
      if (DEBUG) {
        if (SELECTED_LANGUAGE == 1) {
          Serial.println("Sending measurement data to the AirMonitor service!\n");
        } else if (SELECTED_LANGUAGE == 2) {
          Serial.println("Wysłanie danych pomiarowych do serwisu AirMonitor!\n");
        }
      }
      previous_AIRMONITOR_Millis = millis();
    }
  }

  if (THINGSPEAK_ON) {
    unsigned long current_THINGSPEAK_Millis = millis();
    if (current_THINGSPEAK_Millis - previous_THINGSPEAK_Millis >= THINGSPEAK_interval) {
      sendDataToThingSpeak(BMESensor, averagePM1, averagePM25, averagePM10);
      if (DEBUG) {
        if (SELECTED_LANGUAGE == 1) {
          Serial.println("Sending measurement data to the Thingspeak service!\n");
        } else if (SELECTED_LANGUAGE == 2) {
          Serial.println("Wysłanie danych pomiarowych do serwisu Thingspeak!\n");
        }
      }
      previous_THINGSPEAK_Millis = millis();
    }
  }

  if (INFLUXDB_ON) {
    unsigned long current_INFLUXDB_Millis = millis();
    if (current_INFLUXDB_Millis - previous_INFLUXDB_Millis >= INFLUXDB_interval) {
      Influxdb influxdb(INFLUXDB_HOST, INFLUXDB_PORT);
      if (influxdb.opendb(DATABASE, DB_USER, DB_PASSWORD) != DB_SUCCESS) {
        Serial.println("Opening database failed");
      } else {
        dbMeasurement row(device_name);
        if (!strcmp(DUST_MODEL, "PMS7003")) {
          if (DEBUG) {
            if (SELECTED_LANGUAGE == 1) {
              Serial.println("Measurements from PMS7003!\n");
            } else if (SELECTED_LANGUAGE == 2) {
              Serial.println("Dane z PMS7003!\n");
            }
          }
          row.addField("pm1", averagePM1);
          row.addField("pm25", averagePM25);
          row.addField("pm10", averagePM10);
        } else {
          if (DEBUG) {
            if (SELECTED_LANGUAGE == 1) {
              Serial.println("No measurements from PMS7003!\n");
            } else if (SELECTED_LANGUAGE == 2) {
              Serial.println("Brak danych z PMS7003!\n");
            }
          }
          row.addField("pm1", averagePM1);
          row.addField("pm25", averagePM25);
          row.addField("pm10", averagePM10);
        }
        if (!strcmp(THP_MODEL, "BME280")) {
          if (checkBmeStatus() == true) {
            if (DEBUG) {
              if (SELECTED_LANGUAGE == 1) {
                Serial.println("Measurements from BME280!\n");
              } else if (SELECTED_LANGUAGE == 2) {
                Serial.println("Dane z BME280!\n");
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
              }
            }
          }
        }

        if (!strcmp(THP_MODEL, "DHT22")) {
          if (checkDHT22Status() == true) {
            if (DEBUG) {
              if (SELECTED_LANGUAGE == 1) {
                Serial.println("Measurements from DHT22!\n");
              } else if (SELECTED_LANGUAGE == 2) {
                Serial.println("Dane z DHT22!\n");
              }
            }
            row.addField("temperature", (dht.readTemperature()));
            row.addField("humidity", (dht.readHumidity()));
          } else {
            if (DEBUG) {
              if (SELECTED_LANGUAGE == 1) {
                Serial.println("No measurements from DHT22!\n");
              } else if (SELECTED_LANGUAGE == 2) {
                Serial.println("Brak pomiarów z DHT22!\n");
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
            }
          }
        } else {
          if (DEBUG) {
            if (SELECTED_LANGUAGE == 1) {
              Serial.println("Error sending data to InfluxDB\n");
            } else if (SELECTED_LANGUAGE == 2) {
              Serial.println("Błąd wysyłania danych do InfluxDB\n");
            }
          }
        }
        row.empty();
      }
      previous_INFLUXDB_Millis = millis();
    }
  }

  if (strcmp(DUST_MODEL, "Non")) {
    unsigned long current_DUST_Millis = millis();
    if (current_DUST_Millis - previous_DUST_Millis >= DUST_interval) {

      pmMeasurements[iPM][0] = int(calib * data.PM_AE_UG_1_0);
      pmMeasurements[iPM][1] = int(calib * data.PM_AE_UG_2_5);
      pmMeasurements[iPM][2] = int(calib * data.PM_AE_UG_10_0);

      if (DEBUG) {
        Serial.print("\n\nNumer pomiaru PM: ");
        Serial.print(iPM);
        Serial.print("\nWartość PM1: ");
        Serial.print(pmMeasurements[iPM][0]);
        Serial.print("\nWartość PM2.5: ");
        Serial.print(pmMeasurements[iPM][1]);
        Serial.print("\nWartość PM10: ");
        Serial.print(pmMeasurements[iPM][2]);
        Serial.println("\n20 sekund!\n");
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
        }
      }
      iPM++;
      if (iPM >= NUMBEROFMEASUREMENTS) {
        iPM = 0;
      }
      previous_DUST_Millis = millis();
    }
  }

  unsigned long current_REBOOT_Millis = millis();
  if (current_REBOOT_Millis - previous_REBOOT_Millis >= REBOOT_interval) {
    Serial.println("autoreboot...");
    delay(1000);
    previous_REBOOT_Millis = millis();
    ESP.reset();
    delay(5000);
  }

}
void pm_calibration() {
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
  if (!strcmp(THP_MODEL, "DHT22")) {
    if (int(dht.readTemperature()) < 5 and int(dht.readHumidity()) > 60) {
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
  if (!strcmp(MODEL, "white")) {
    if (!strcmp(THP_MODEL, "BME280")) {
      calib1 = float((200 - (BMESensor.humidity)) / 150);
      calib2 = calib1 / 2;
    }
    if (!strcmp(THP_MODEL, "HTU21")) {
      calib1 = float((200 - (myHTU21D.readCompensatedHumidity())) / 150);
      calib2 = calib1 / 2;
    }
    if (!strcmp(THP_MODEL, "DHT22")) {
      calib1 = float((200 - (dht.readHumidity())) / 150);
      calib2 = calib1 / 2;
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
    averagePM10  += pmMeasurements[i][2];
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


