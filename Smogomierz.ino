#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#endif

#include <Wire.h>

#include "FS.h"
#include <ArduinoJson.h> // 6.9.0 or later
#include "src/WiFiManager.h" // https://github.com/jakerabid/WiFiManager // 12.03.2019
#include "src/bme280.h" // https://github.com/zen/BME280_light // 12.03.2019
#include "src/HTU21D.h" // https://github.com/enjoyneering/HTU21D // 12.03.2019
#include "src/Adafruit_BMP280.h" // https://github.com/adafruit/Adafruit_BMP280_Library // 12.03.2019
#include "src/SHT1x.h" // https://github.com/practicalarduino/SHT1x // 12.03.2019
#include <DHT.h>

#include "src/pms.h" // https://github.com/fu-hsi/PMS // 12.03.2019

#include "src/spiffs.h"
#include "src/config.h"
#include "defaultConfig.h"
#include "src/autoupdate.h"
#include "src/smoglist.h"

#include "src/luftdaten.h"
#include "src/airmonitor.h"
#include "src/thing_speak.h"
#include "src/ESPinfluxdb.h" // https://github.com/hwwong/ESP_influxdb // 12.03.2019

/*
  Podłączenie czujnikow dla ESP8266 NodeMCU:
  BME280/BMP280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  SHT1x: VIN - 3V; GND - G; SCL - D5; DATA/SDA - D6 wymaga rezystora 10k podłaczonego do VCC
  SHT21/HTU21D: VIN - 3V; GND - G; SCL - D5; SDA - D6
  DHT22: VIN - 3V; GND - G; D7
  PMS5003/7003: Bialy - VIN/5V; Czarny - G; Zielony/TX - D1; Niebieski/RX - D2


  Connection of sensors on ESP8266 NodeMCU:
  BME280/BMP280: VIN - 3V; GND - G; SCL - D4; SDA - D3
  SHT1x: VIN - 3V; GND - G; SCL - D5; DATA/SDA - D6 required pull-up resistor 10k to VCC
  SHT21/HTU21D: VIN - 3V; GND - G; SCL - D5; SDA - D6
  DHT22: VIN - 3V; GND - G; D7
  PMS5003/7003: White - VIN/5V; Black - G; Green/TX - D1; Blue/RX - D2
*/

// TEMP/HUMI/PRESS Sensor config - START
// BME280 config
#define ASCII_ESC 27
char bufout[10];
BME280<> BMESensor;

// BMP280 config
Adafruit_BMP280 bmp; //I2C

// Serial for SHT21/HTU21D config
HTU21D  myHTU21D(HTU21D_RES_RH12_TEMP14);

// DHT22 config
#define DHTPIN 13 // D7 on NodeMCU/WeMos board
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

// SHT1x – Config
#define dataPin 14 //D5
#define clockPin 12 //D6
SHT1x sht1x(dataPin, clockPin);
// TEMP/HUMI/PRESS Sensor config - END

// DUST Sensor config - START
// Serial for PMS7003 config
SoftwareSerial mySerial(5, 4); // Change TX - D1 and RX - D2 pins
PMS pms(mySerial);
PMS::DATA data;
// DUST Sensor config - END

char device_name[20];

unsigned long DUST_interval = 60 * 1000; // 1 minute
unsigned long previous_DUST_Millis = 0;

unsigned long SENDING_FREQUENCY_interval = 60 * 1000; // 1 minute
unsigned long previous_SENDING_FREQUENCY_Millis = 0;

unsigned long SENDING_DB_FREQUENCY_interval = 60 * 1000; // 1 minute
unsigned long previous_SENDING_DB_FREQUENCY_Millis = 0;

unsigned long previous_2sec_Millis = 0;
unsigned long TwoSec_interval = 2 * 1000; // 2 second

unsigned long REBOOT_interval = 24 * 60 * 60 * 1000; // 24 hours
unsigned long previous_REBOOT_Millis = 0;

int pmMeasurements[10][3];
int iPM, averagePM1, averagePM25, averagePM4, averagePM10 = 0;
float calib = 1;

bool need_update = false;
char SERVERSOFTWAREVERSION[255] = "";
char CURRENTSOFTWAREVERSION[255] = "";

ESP8266WebServer WebServer(80);
ESP8266HTTPUpdateServer httpUpdater;

WiFiClient espClient;
PubSubClient mqttclient(espClient);

// check TEMP/HUMI/PRESS Sensor - START
bool checkHTU21DStatus() {
  int temperature_HTU21D_Int = int(myHTU21D.readTemperature());
  int humidity_HTU21D_Int = int(myHTU21D.readHumidity());
  if (temperature_HTU21D_Int == 0 && humidity_HTU21D_Int == 0) {
    if (DEBUG) {
      Serial.println("No data from HTU21D sensor!\n");
      return false;
    }
    return false;
  } else if (temperature_HTU21D_Int == 255 && humidity_HTU21D_Int == 255) {
    if (DEBUG) {
      Serial.println("No data from HTU21D sensor!\n");
      return false;
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
      Serial.println("No data from BME280 sensor!\n");
      return false;
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
      Serial.println("No data from BMP280 sensor!\n");
      return false;
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
      Serial.println("No data from DHT22 sensor!\n");
      return false;
    }
    return false;
  } else {
    return true;
  }
}

bool checkSHT1xStatus() {
  int humidity_SHT1x_Int = sht1x.readHumidity();
  int temperature_SHT1x_Int = sht1x.readTemperatureC();
  if (humidity_SHT1x_Int == 0 && temperature_SHT1x_Int == 0) {
    if (DEBUG) {
      Serial.println("No data from SHT1x sensor!\n");
      return false;
    }
    return false;
  } else {
    return true;
  }
}
// check TEMP/HUMI/PRESS Sensor - END

void minutesToSeconds() {
  DUST_interval = 1000; // 1 second
  SENDING_FREQUENCY_interval = 1000;
  SENDING_DB_FREQUENCY_interval = 1000;
}

void MQTTreconnect() {
  // Loop until we're reconnected
  if (!mqttclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttclient.connect("ESP8266Client", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttclient.state());
      Serial.println("\n");
    }
  }
}

// default translation - english
#include "src/default_intl.h"

#include "src/translator.h"

// all HTML content
#include "src/html-content.h"

// library doesnt support arguments :/
#include "src/webserver.h"

void setup() {
  Serial.begin(115200);
  delay(10);

  fs_setup();
  delay(10);

  loadtranslation(SELECTED_LANGUAGE);
  delay(10);

  // DUST SENSOR setup - START
  if (!strcmp(DUST_MODEL, "PMS7003")) {
    mySerial.begin(9600); //PMS7003 serial
    if (FREQUENTMEASUREMENT == true) {
      pms.wakeUp();
      delay(500);
      pms.activeMode();
    } else {
      pms.passiveMode();
      delay(500);
      pms.sleep();
    }
  }
  delay(10);
  // DUST SENSOR setup - END

  if (SENDING_FREQUENCY < DUST_TIME) {
    SENDING_FREQUENCY = DUST_TIME;
  }
  if (SENDING_DB_FREQUENCY == 0) {
    SENDING_DB_FREQUENCY = SENDING_FREQUENCY;
  }
  delay(10);

  if (FREQUENTMEASUREMENT == true) {
    minutesToSeconds();
  }

  if (strcmp(DUST_MODEL, "Non")) {
    DUST_interval = DUST_interval * DUST_TIME;
  }
  if (DEEPSLEEP_ON == true) {
    if (LUFTDATEN_ON or AIRMONITOR_ON or SMOGLIST_ON or THINGSPEAK_ON or INFLUXDB_ON or MQTT_ON) {
      SENDING_FREQUENCY_interval = SENDING_FREQUENCY_interval * SENDING_FREQUENCY;
    }
  } else {
    if (LUFTDATEN_ON or AIRMONITOR_ON or SMOGLIST_ON) {
      SENDING_FREQUENCY_interval = SENDING_FREQUENCY_interval * SENDING_FREQUENCY;
    }
    if (THINGSPEAK_ON or INFLUXDB_ON or MQTT_ON) {
      SENDING_DB_FREQUENCY_interval = SENDING_DB_FREQUENCY_interval * SENDING_DB_FREQUENCY;
    }
  }
  delay(10);

  // TEMP/HUMI/PRESS Sensor seturp - START
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
  if (!strcmp(THP_MODEL, "SHT1x")) {

  }
  delay(10);
  // TEMP/HUMI/PRESS Sensor seturp - END

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

  // check update
  if (checkUpdate() == true) {
    need_update = true;
  } else {
    need_update = false;
  }

  if (MQTT_ON) {
    mqttclient.setServer(MQTT_HOST, MQTT_PORT);
  }

  if (INFLUXDB_ON) {
    Influxdb influxdb(INFLUXDB_HOST, INFLUXDB_PORT);
    if (influxdb.opendb(INFLUXDB_DATABASE, DB_USER, DB_PASSWORD) != DB_SUCCESS) {
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
  WebServer.on("/erase_wifi", HTTP_GET, erase_wifi);
  WebServer.on("/restore_config", HTTP_GET, restore_config);
  WebServer.on("/fwupdate", HTTP_GET, fwupdate);
  WebServer.on("/autoupdateon", HTTP_GET, autoupdateon);
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
  if (need_update == true) {
    if (AUTOUPDATE_ON) {
      for (int i = 0; i < 5 ; i++) {
        doUpdate();
        delay(1000);
      }
    }
  }
  delay(10);

  // TEMP/HUMI/PRESS Sensor - START
  if (!strcmp(THP_MODEL, "BME280")) {
    BMESensor.refresh();
  }
  // DUST SENSOR refresh data - END

  pm_calibration();

  // DUST SENSOR refresh data - START
  if (!strcmp(DUST_MODEL, "PMS7003")) {
    pms.read(data);
  }
  // DUST SENSOR refresh data - END
  delay(10);

  //webserverShowSite(WebServer, BMESensor, data);
  WebServer.handleClient();
  delay(10);

  MDNS.update();

  yield();

  if (strcmp(DUST_MODEL, "Non")) {
    unsigned long current_DUST_Millis = millis();
    if (FREQUENTMEASUREMENT == true ) {
      if (current_DUST_Millis - previous_DUST_Millis >= DUST_interval) {
        takeNormalnPMMeasurements();
        previous_DUST_Millis = millis();
      }
    }
    if (DEEPSLEEP_ON == true) {
      Serial.println("\nDeepSleep Mode!\n");

      takeSleepPMMeasurements();
      delay(10);

      if (LUFTDATEN_ON or AIRMONITOR_ON or SMOGLIST_ON) {
        sendDataToExternalServices();
      }
      if (THINGSPEAK_ON or INFLUXDB_ON or MQTT_ON) {
        sendDataToExternalDBs();
      }

      Serial.println("Going into deep sleep for " + String(SENDING_FREQUENCY) + " minutes!");
      ESP.deepSleep(SENDING_FREQUENCY * 60 * 1000000); // *1000000 - secunds
      delay(10);

    } else {
      if (current_DUST_Millis - previous_DUST_Millis >= DUST_interval) {
        takeSleepPMMeasurements();
        previous_DUST_Millis = millis();
      }
    }
  } else {
    if (DEEPSLEEP_ON == true) {
      Serial.println("\nDeepSleep Mode!\n");
      unsigned long current_2sec_Millis = millis();
      previous_2sec_Millis = millis();
      while (previous_2sec_Millis - current_2sec_Millis <= TwoSec_interval * 10) {
        WebServer.handleClient();
        delay(10);
        yield();
        previous_2sec_Millis = millis();
      }
      if (LUFTDATEN_ON or AIRMONITOR_ON or SMOGLIST_ON) {
        sendDataToExternalServices();
      }
      if (THINGSPEAK_ON or INFLUXDB_ON or MQTT_ON) {
        sendDataToExternalDBs();
      }
      delay(10);
      Serial.println("Going into deep sleep for " + String(SENDING_FREQUENCY) + " minutes!");
      ESP.deepSleep(SENDING_FREQUENCY * 60 * 1000000); // *1000000 - secunds
      delay(10);
    }
  }

  if (LUFTDATEN_ON or AIRMONITOR_ON or SMOGLIST_ON) {
    unsigned long current_SENDING_FREQUENCY_Millis = millis();
    if (current_SENDING_FREQUENCY_Millis - previous_SENDING_FREQUENCY_Millis >= SENDING_FREQUENCY_interval) {
      sendDataToExternalServices();
      previous_SENDING_FREQUENCY_Millis = millis();
    }
  }

  if (THINGSPEAK_ON or INFLUXDB_ON or MQTT_ON) {
    unsigned long current_SENDING_DB_FREQUENCY_Millis = millis();
    if (current_SENDING_DB_FREQUENCY_Millis - previous_SENDING_DB_FREQUENCY_Millis >= SENDING_DB_FREQUENCY_interval) {
      sendDataToExternalDBs();
      previous_SENDING_DB_FREQUENCY_Millis = millis();
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

} // loop() - END

void sendDataToExternalServices() {

  if (LUFTDATEN_ON) {
    sendDataToLuftdaten(BMESensor, averagePM1, averagePM25, averagePM10);
    if (DEBUG) {
      Serial.println("Sending measurement data to the LuftDaten service!\n");
    }
  }

  if (AIRMONITOR_ON) {
    sendDataToAirMonitor(BMESensor, averagePM1, averagePM25, averagePM10);
    if (DEBUG) {
      Serial.println("Sending measurement data to the AirMonitor service!\n");
    }
  }

  if (SMOGLIST_ON) {
    sendDataToSmogList(BMESensor, averagePM1, averagePM25, averagePM4, averagePM10);
    if (DEBUG) {
      Serial.println("Sending measurement data to the SmogList service!\n");
    }
  }

}

void sendDataToExternalDBs() {

  if (MQTT_ON) {
    if (!mqttclient.connected()) {
      MQTTreconnect();
    }
    mqttclient.loop();
    delay(10);
  }

  if (THINGSPEAK_ON) {
    sendDataToThingSpeak(BMESensor, averagePM1, averagePM25, averagePM10);
    if (DEBUG) {
      Serial.println("Sending measurement data to the Thingspeak service!\n");
    }
  }

  if (INFLUXDB_ON) {
    Influxdb influxdb(INFLUXDB_HOST, INFLUXDB_PORT);
    if (influxdb.opendb(INFLUXDB_DATABASE, DB_USER, DB_PASSWORD) != DB_SUCCESS) {
      Serial.println("Opening database failed");
    } else {
      dbMeasurement row(device_name);
      if (!strcmp(DUST_MODEL, "PMS7003")) {
        if (DEBUG) {
          Serial.println("\nMeasurements from PMS7003!\n");
        }
        row.addField("pm1", averagePM1);
        row.addField("pm25", averagePM25);
        row.addField("pm10", averagePM10);
      } else {
        if (DEBUG) {
          Serial.println("\nNo measurements from PMS7003!\n");
        }
        row.addField("pm1", averagePM1);
        row.addField("pm25", averagePM25);
        row.addField("pm10", averagePM10);
      }
      if (!strcmp(THP_MODEL, "BME280")) {
        if (checkBmeStatus() == true) {
          if (DEBUG) {
            Serial.println("Measurements from BME280!\n");
          }
          row.addField("temperature", (BMESensor.temperature));
          row.addField("pressure", (BMESensor.seaLevelForAltitude(MYALTITUDE)));
          row.addField("humidity", (BMESensor.humidity));
        } else {
          if (DEBUG) {
            Serial.println("No measurements from BME280!\n");
          }
        }
      }

      if (!strcmp(THP_MODEL, "HTU21")) {
        if (checkHTU21DStatus() == true) {
          if (DEBUG) {
            Serial.println("Measurements from HTU21!\n");
          }
          row.addField("temperature", (myHTU21D.readTemperature()));
          row.addField("humidity", (myHTU21D.readHumidity()));
        } else {
          if (DEBUG) {
            Serial.println("No measurements from HTU21D!\n");
          }
        }
      }

      if (!strcmp(THP_MODEL, "BMP280")) {
        if (checkBmpStatus() == true) {
          if (DEBUG) {
            Serial.println("Measurements from BMP280!\n");
          }
          row.addField("temperature", (bmp.readTemperature()));
          row.addField("pressure", ((bmp.readPressure()) / 100));
        } else {
          if (DEBUG) {
            Serial.println("No measurements from BMP280!\n");
          }
        }
      }

      if (!strcmp(THP_MODEL, "DHT22")) {
        if (checkDHT22Status() == true) {
          if (DEBUG) {
            Serial.println("Measurements from DHT22!\n");
          }
          row.addField("temperature", (dht.readTemperature()));
          row.addField("humidity", (dht.readHumidity()));
        } else {
          if (DEBUG) {
            Serial.println("No measurements from DHT22!\n");
          }
        }
      }

      if (!strcmp(THP_MODEL, "SHT1x")) {
        if (checkSHT1xStatus() == true) {
          if (DEBUG) {
            Serial.println("Measurements from SHT1x!\n");
          }
          row.addField("temperature", (sht1x.readTemperatureC()));
          row.addField("humidity", (sht1x.readHumidity()));
        } else {
          if (DEBUG) {
            Serial.println("No measurements from SHT1x!\n");
          }
        }
      }

      if (influxdb.write(row) == DB_SUCCESS) {
        if (DEBUG) {
          Serial.println("Data sent to InfluxDB\n");
        }
      } else {
        if (DEBUG) {
          Serial.println("Error sending data to InfluxDB\n");
        }
      }
      row.empty();
    }
  }

  if (MQTT_ON) {
    if (strcmp(DUST_MODEL, "Non")) {
      if (DEBUG) {
        Serial.println("Measurements from PM Sensor!\n");
      }
      mqttclient.publish(String("Smogomierz-" + String(ESP.getChipId()) + "/sensor/PM1").c_str(), String(averagePM1).c_str(), true);
      mqttclient.publish(String("Smogomierz-" + String(ESP.getChipId()) + "/sensor/PM2.5").c_str(), String(averagePM25).c_str(), true);
      mqttclient.publish(String("Smogomierz-" + String(ESP.getChipId()) + "/sensor/PM10").c_str(), String(averagePM10).c_str(), true);
    }
    if (!strcmp(THP_MODEL, "BME280")) {
      if (checkBmeStatus() == true) {
        if (DEBUG) {
          Serial.println("Measurements from BME280!\n");
        }
        mqttclient.publish(String("Smogomierz-" + String(ESP.getChipId()) + "/sensor/temperature").c_str(), String(BMESensor.temperature).c_str(), true);
        mqttclient.publish(String("Smogomierz-" + String(ESP.getChipId()) + "/sensor/pressure").c_str(), String(BMESensor.seaLevelForAltitude(MYALTITUDE)).c_str(), true);
        mqttclient.publish(String("Smogomierz-" + String(ESP.getChipId()) + "/sensor/humidity").c_str(), String(BMESensor.humidity).c_str(), true);

      } else {
        if (DEBUG) {
          Serial.println("No measurements from BME280!\n");
        }
      }
    }
  }

}

void takeNormalnPMMeasurements() {
  pmMeasurements[iPM][0] = int(calib * data.PM_AE_UG_1_0);
  pmMeasurements[iPM][1] = int(calib * data.PM_AE_UG_2_5);
  pmMeasurements[iPM][2] = int(calib * data.PM_AE_UG_10_0);
  if (DEBUG) {
    Serial.print("\n\nPM measurement number: ");
    Serial.print(iPM);
    Serial.print("\nValue of PM1: ");
    Serial.print(pmMeasurements[iPM][0]);
    Serial.print("\nValue of PM2.5: ");
    Serial.print(pmMeasurements[iPM][1]);
    Serial.print("\nValue of PM10: ");
    Serial.print(pmMeasurements[iPM][2]);
  }
  averagePM();
  if (DEBUG) {
    Serial.print("\nAverage PM1: ");
    Serial.print(averagePM1);
    Serial.print("\nAverage PM2.5: ");
    Serial.print(averagePM25);
    Serial.print("\nAverage PM10: ");
    Serial.print(averagePM10);
  }
  iPM++;
  if (iPM >= NUMBEROFMEASUREMENTS) {
    iPM = 0;
  }
}

void takeSleepPMMeasurements() {
  if (strcmp(DUST_MODEL, "Non")) {

    if (DEBUG) {
      Serial.print("\nTurning ON PM sensor...");
    }

    if (!strcmp(DUST_MODEL, "PMS7003")) {
      pms.wakeUp();
      unsigned long current_2sec_Millis = millis();
      previous_2sec_Millis = millis();
      while (previous_2sec_Millis - current_2sec_Millis <= TwoSec_interval * 5) {
        WebServer.handleClient();
        yield();
        previous_2sec_Millis = millis();
      }
      previous_2sec_Millis = 0;
      pms.requestRead();
    }

    int counterNM1 = 0;
    while (counterNM1 < NUMBEROFMEASUREMENTS) {
      unsigned long current_2sec_Millis = millis();
      if (current_2sec_Millis - previous_2sec_Millis >= TwoSec_interval) {

        if (pms.readUntil(data)) {
          pmMeasurements[iPM][0] = int(calib * data.PM_AE_UG_1_0);
          pmMeasurements[iPM][1] = int(calib * data.PM_AE_UG_2_5);
          pmMeasurements[iPM][2] = int(calib * data.PM_AE_UG_10_0);
        }

        if (DEBUG) {
          Serial.print("\n\nPM measurement number PM: ");
          Serial.print(iPM);
          Serial.print("\nValue of PM1: ");
          Serial.print(pmMeasurements[iPM][0]);
          Serial.print("\nValue of PM2.5: ");
          Serial.print(pmMeasurements[iPM][1]);
          Serial.print("\nValue of PM10: ");
          Serial.print(pmMeasurements[iPM][2]);
        }
        averagePM();
        if (DEBUG) {
          Serial.print("\nAverage PM1: ");
          Serial.print(averagePM1);
          Serial.print("\nAverage PM2.5: ");
          Serial.print(averagePM25);
          Serial.print("\nAverage PM10: ");
          Serial.print(averagePM10);
        }
        iPM++;
        if (iPM >= NUMBEROFMEASUREMENTS) {
          iPM = 0;
        }
        previous_2sec_Millis = millis();
        counterNM1++;
      }
      WebServer.handleClient();
      yield();
      delay(10);
    }
    if (DEBUG) {
      Serial.print("\nTurning OFF PM sensor...\n");
    }

    if (!strcmp(DUST_MODEL, "PMS7003")) {
      pms.sleep();
    }

  }
}

void pm_calibration() {
  // Automatic calibration - START
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
    if (!strcmp(THP_MODEL, "SHT1x")) {
      calib1 = float((200 - (sht1x.readHumidity())) / 150);
      calib2 = calib1 / 2;
    }
  }
  // Automatic calibration - END
  
  if (!strcmp(THP_MODEL, "BME280")) {
    if (int(BMESensor.temperature) < 5 and int(BMESensor.humidity) > 60) {
      calib = calib2;
    } else {
      calib = calib1;
    }
  }
  if (!strcmp(THP_MODEL, "HTU21")) {
    if (int(myHTU21D.readTemperature()) < 5 and int(myHTU21D.readCompensatedHumidity()) > 60) {
      calib = calib2;
    } else {
      calib = calib1;
    }
  }
  if (!strcmp(THP_MODEL, "DHT22")) {
    if (int(dht.readTemperature()) < 5 and int(dht.readHumidity()) > 60) {
      calib = calib2;
    } else {
      calib = calib1;
    }
  }
  if (!strcmp(THP_MODEL, "SHT1x")) {
    if (int(sht1x.readTemperatureC()) < 5 and int(sht1x.readHumidity()) > 60) {
      calib = calib2;
    } else {
      calib = calib1;
    }
  }

  if (!strcmp(THP_MODEL, "BMP280")) {
    if (int(bmp.readTemperature()) < 5) {
      calib = calib2;
    } else {
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
    averagePM10  += pmMeasurements[i][2];
  }
  averagePM1 = averagePM1 / NUMBEROFMEASUREMENTS;
  averagePM25 = averagePM25 / NUMBEROFMEASUREMENTS;
  averagePM10 = averagePM10 / NUMBEROFMEASUREMENTS;
  return averagePM1, averagePM25, averagePM10;
}

