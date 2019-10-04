#ifdef ARDUINO_ARCH_ESP8266 // ESP8266 core for Arduino - 2.5.2 or later
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <SoftwareSerial.h>
#elif defined ARDUINO_ARCH_ESP32 // Arduino core for the ESP32 - 1.0.4-rc1 or later // at 1.0.3 autoupdate doesn't work !!!
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <Update.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <time.h>
#include <HardwareSerial.h>
#endif

/*
   ESP32 wymaga zainstalowania „Arduino ESP32 Fileystem Uploader” i przesłania pustego obrazu SPIFFS.
   Musisz to zrobić tylko raz.
   Po tym cała konfiguracja i ustawienia zostaną zapisana w pamięci ESP32.

  https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/

   ESP32 requires installing the "Arduino ESP32 Filesystem Uploader" and uploading a blank SPIFFS image.
   You only have to do this once.
   After that all configuration and settings will be saved in ESP32 memory.
*/

/*
  ESP8266 - NodeMCU 1.0 - 1M SPIFFS

  Szkic używa 527448 bajtów (50%) pamięci programu. Maksimum to 1044464 bajtów.
  Zmienne globalne używają 54108 bajtów (66%) pamięci dynamicznej, pozostawiając 27812 bajtów dla zmiennych lokalnych. Maksimum to 81920 bajtów.

  Szkic używa 527548 bajtów (50%) pamięci programu. Maksimum to 1044464 bajtów.
  Zmienne globalne używają 54216 bajtów (66%) pamięci dynamicznej, pozostawiając 27704 bajtów dla zmiennych lokalnych. Maksimum to 81920 bajtów.

  ESP32 Dev Module - 1.9MB APP with OTA - 190KB SPIFFS

  Szkic używa 1247990 bajtów (63%) pamięci programu. Maksimum to 1966080 bajtów.
  Zmienne globalne używają 62236 bajtów (18%) pamięci dynamicznej, pozostawiając 265444 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.

  Szkic używa 1251682 bajtów (63%) pamięci programu. Maksimum to 1966080 bajtów.
  Zmienne globalne używają 62776 bajtów (19%) pamięci dynamicznej, pozostawiając 264904 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.

*/

#include <PubSubClient.h>
#include <Wire.h>

#include "FS.h"
#include <ArduinoJson.h> // 6.9.0 or later
#include "src/WiFiManager.h" // https://github.com/tzapu/WiFiManager/tree/development // 20.08.2019  DEV
#ifdef ARDUINO_ARCH_ESP8266
#include "src/esp8266/bme280.h" // https://github.com/zen/BME280_light // CUSTOMIZED! 8.04.2019
#elif defined ARDUINO_ARCH_ESP32
#include "src/esp32/Adafruit_BME280.h" // https://github.com/Takatsuki0204/BME280-I2C-ESP32 // 24.05.2019
#endif
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
#ifdef ARDUINO_ARCH_ESP8266 // VIN - 3V; GND - G; SCL - D4; SDA - D3
#define ASCII_ESC 27
char bufout[10];
BME280<> BMESensor;
#elif defined ARDUINO_ARCH_ESP32 // VIN - 3V; GND - G; SCL - D17; SDA - D16
#define I2C_SDA 16
#define I2C_SCL 17
Adafruit_BME280 bme(I2C_SDA, I2C_SCL); // I2C
#endif

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
// Serial for PMSx003 config
#ifdef ARDUINO_ARCH_ESP8266
SoftwareSerial PMS_Serial(5, 4); // Change TX - D1 and RX - D2 pins
PMS pms(PMS_Serial);
PMS::DATA data;
#elif defined ARDUINO_ARCH_ESP32
HardwareSerial PMS_Serial(1); // Change TX - D5 and RX - D4 pins
PMS pms(PMS_Serial);
PMS::DATA data;
#endif
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
float currentTemperature, currentHumidity, currentPressure = 0;
float calib = 1;

bool need_update = false;
char SERVERSOFTWAREVERSION[255] = "";
char CURRENTSOFTWAREVERSION[255] = "";

#ifdef ARDUINO_ARCH_ESP8266
ESP8266WebServer WebServer(80);
ESP8266HTTPUpdateServer httpUpdater;
#elif defined ARDUINO_ARCH_ESP32
WebServer WebServer(80);
#endif

WiFiClient espClient;
PubSubClient mqttclient(espClient);

WiFiManager wifiManager;

// check TEMP/HUMI/PRESS Sensor - START
bool checkHTU21DStatus() {
  int temperature_HTU21D_Int = int(myHTU21D.readTemperature());
  int humidity_HTU21D_Int = int(myHTU21D.readHumidity());
  if ((temperature_HTU21D_Int == 0 && humidity_HTU21D_Int == 0) || (temperature_HTU21D_Int == 255 && humidity_HTU21D_Int == 255)) {
    if (DEBUG) {
      Serial.println("No data from HTU21D sensor!\n");
    }
    return false;
  } else {
    return true;
  }
}

bool checkBmeStatus() {
#ifdef ARDUINO_ARCH_ESP8266
  int temperature_BME280_Int = BMESensor.temperature;
  int pressure_BME280_Int = (BMESensor.seaLevelForAltitude(MYALTITUDE));
  int humidity_BME280_Int = BMESensor.humidity;
#elif defined ARDUINO_ARCH_ESP32
  int temperature_BME280_Int = bme.readTemperature();
  int pressure_BME280_Int = (bme.seaLevelForAltitude(MYALTITUDE, (bme.readPressure() / 100.0F)));
  int humidity_BME280_Int = bme.readHumidity();
#endif
  if (temperature_BME280_Int == 0 && pressure_BME280_Int == 0 && humidity_BME280_Int == 0) {
    if (DEBUG) {
      Serial.println("No data from BME280 sensor!\n");
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
#ifdef ARDUINO_ARCH_ESP8266
    PMS_Serial.begin(9600); //PMSx003 serial
#elif defined ARDUINO_ARCH_ESP32
    PMS_Serial.begin(9600, SERIAL_8N1, 5, 4); //PMSx003 serial
#endif
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
#ifdef ARDUINO_ARCH_ESP32
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  int(SENDING_FREQUENCY_interval/1000)        /* Time ESP32 will go to sleep (in seconds) */
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds\n");
#endif
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
#ifdef ARDUINO_ARCH_ESP8266
    Wire.begin(0, 2);
    BMESensor.begin();
#elif defined ARDUINO_ARCH_ESP32
    bme.begin();
#endif
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    Wire.begin(0, 2);
    bmp.begin();
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    myHTU21D.begin();
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    dht.begin();
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
  }
  delay(10);
  // TEMP/HUMI/PRESS Sensor setup - END

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
    WiFi.mode(WIFI_STA); // https://github.com/hackerspace-silesia/Smogomierz/issues/47#issue-496810438
  } else {
    Serial.println("Configportal running");
    wifiManager.setConfigPortalBlocking(false);
  }
  delay(250);


  // check update
  if (checkUpdate(0) == true) {
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
      Serial.println("Opening InfluxDB failed");
    } else {
      Serial.println("Opening InfluxDB succeed");
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

#ifdef ARDUINO_ARCH_ESP8266
  httpUpdater.setup(&WebServer, "/update");
#elif defined ARDUINO_ARCH_ESP32
  /*handling uploading firmware file */
  WebServer.on("/update", HTTP_POST, []() {
    WebServer.sendHeader("Connection", "close");
    WebServer.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = WebServer.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
#endif
  //  WebServer Config - End

  // Check if config.h exist in ESP data folder
  WebServer.begin();

  MDNS.begin(device_name);

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPServer ready! http://%s.local/\n", device_name);
  delay(300);
}

void loop() {
  if (need_update == true && AUTOUPDATE_ON) {
    for (int i = 0; i < 5 ; i++) {
      doUpdate(0);
      delay(1000);
    }
  }
  delay(10);

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

#ifdef ARDUINO_ARCH_ESP8266
  MDNS.update();
#endif

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
        takeTHPMeasurements();
        sendDataToExternalServices();
      }
      if (THINGSPEAK_ON or INFLUXDB_ON or MQTT_ON) {
        takeTHPMeasurements();
        sendDataToExternalDBs();
      }

#ifdef ARDUINO_ARCH_ESP8266
      Serial.println("Going into deep sleep for " + String(SENDING_FREQUENCY) + " minutes!");
      Serial.flush();
      ESP.deepSleep(SENDING_FREQUENCY * 60 * 1000000); // *1000000 - secunds
      delay(10);
#elif defined ARDUINO_ARCH_ESP32
      Serial.println("Going to sleep now");
      Serial.flush();
      esp_deep_sleep_start();
#endif

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
        takeTHPMeasurements();
        sendDataToExternalServices();
      }
      if (THINGSPEAK_ON or INFLUXDB_ON or MQTT_ON) {
        takeTHPMeasurements();
        sendDataToExternalDBs();
      }
      delay(10);

#ifdef ARDUINO_ARCH_ESP8266
      Serial.println("Going into deep sleep for " + String(SENDING_FREQUENCY) + " minutes!");
      Serial.flush();
      ESP.deepSleep(SENDING_FREQUENCY * 60 * 1000000); // *1000000 - secunds
      delay(10);
#elif defined ARDUINO_ARCH_ESP32
      Serial.println("Going to sleep now");
      Serial.flush();
      esp_deep_sleep_start();
#endif

    }
  }

  if (LUFTDATEN_ON or AIRMONITOR_ON or SMOGLIST_ON) {
    unsigned long current_SENDING_FREQUENCY_Millis = millis();
    if (current_SENDING_FREQUENCY_Millis - previous_SENDING_FREQUENCY_Millis >= SENDING_FREQUENCY_interval) {
      takeTHPMeasurements();
      sendDataToExternalServices();
      previous_SENDING_FREQUENCY_Millis = millis();
    }
  }

  if (THINGSPEAK_ON or INFLUXDB_ON or MQTT_ON) {
    unsigned long current_SENDING_DB_FREQUENCY_Millis = millis();
    if (current_SENDING_DB_FREQUENCY_Millis - previous_SENDING_DB_FREQUENCY_Millis >= SENDING_DB_FREQUENCY_interval) {
      takeTHPMeasurements();
      sendDataToExternalDBs();
      previous_SENDING_DB_FREQUENCY_Millis = millis();
    }
  }

  unsigned long current_REBOOT_Millis = millis();
  if (current_REBOOT_Millis - previous_REBOOT_Millis >= REBOOT_interval) {
    Serial.println("autoreboot...");
    delay(1000);
    previous_REBOOT_Millis = millis();
#ifdef ARDUINO_ARCH_ESP8266
    ESP.reset();
#elif defined ARDUINO_ARCH_ESP32
    ESP.restart();
#endif
    delay(5000);
  }

} // loop() - END

void sendDataToExternalServices() {

  if (LUFTDATEN_ON) {
    sendDataToLuftdaten(currentTemperature, currentPressure, currentHumidity, averagePM1, averagePM25, averagePM4, averagePM10);
    if (DEBUG) {
      Serial.println("Sending measurement data to the LuftDaten service!\n");
    }
  }

  if (AIRMONITOR_ON) {
    sendDataToAirMonitor(currentTemperature, currentPressure, currentHumidity, averagePM1, averagePM25, averagePM4, averagePM10);
    if (DEBUG) {
      Serial.println("Sending measurement data to the AirMonitor service!\n");
    }
  }

  if (SMOGLIST_ON) {
    sendDataToSmoglist(currentTemperature, currentPressure, currentHumidity, averagePM1, averagePM25, averagePM4, averagePM10);
    if (DEBUG) {
      Serial.println("Sending measurement data to the Smoglist service!\n");
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
    sendDataToThingSpeak(currentTemperature, currentPressure, currentHumidity, averagePM1, averagePM25, averagePM4, averagePM10);
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
        row.addField("pm1", averagePM1);
        row.addField("pm25", averagePM25);
        row.addField("pm10", averagePM10);
      } else {
        if (DEBUG) {
          Serial.println("\nNo measurements from PMSx003!\n");
        }
      }
      if (!strcmp(THP_MODEL, "BME280")) {
        if (checkBmeStatus() == true) {
          row.addField("temperature", (currentTemperature));
          row.addField("pressure", (currentPressure));
          row.addField("humidity", (currentHumidity));
        } else {
          if (DEBUG) {
            Serial.println("No measurements from BME280!\n");
          }
        }
      } else if (!strcmp(THP_MODEL, "HTU21")) {
        if (checkHTU21DStatus() == true) {
          row.addField("temperature", (currentTemperature));
          row.addField("humidity", (currentHumidity));
        } else {
          if (DEBUG) {
            Serial.println("No measurements from HTU21D!\n");
          }
        }
      } else if (!strcmp(THP_MODEL, "BMP280")) {
        if (checkBmpStatus() == true) {
          row.addField("temperature", (currentTemperature));
          row.addField("pressure", (currentPressure));
        } else {
          if (DEBUG) {
            Serial.println("No measurements from BMP280!\n");
          }
        }
      } else if (!strcmp(THP_MODEL, "DHT22")) {
        if (checkDHT22Status() == true) {
          row.addField("temperature", (currentTemperature));
          row.addField("humidity", (currentHumidity));
        } else {
          if (DEBUG) {
            Serial.println("No measurements from DHT22!\n");
          }
        }
      } else if (!strcmp(THP_MODEL, "SHT1x")) {
        if (checkSHT1xStatus() == true) {
          row.addField("temperature", (currentTemperature));
          row.addField("humidity", (currentHumidity));
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
#ifdef ARDUINO_ARCH_ESP8266
    String mqttChipId = String(ESP.getChipId());
#elif defined ARDUINO_ARCH_ESP32
    String mqttChipId = String((uint32_t)(ESP.getEfuseMac()));
#endif
    if (strcmp(DUST_MODEL, "Non")) {
      if (DEBUG) {
        Serial.println("Measurements from PM Sensor!\n");
      }
      mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/PM1").c_str(), String(averagePM1).c_str(), true);
      mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/PM2.5").c_str(), String(averagePM25).c_str(), true);
      mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/PM10").c_str(), String(averagePM10).c_str(), true);
      if (averagePM25 <= 10) {
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/airquality").c_str(), "EXCELLENT", true);
      } else if (averagePM25 > 10 && averagePM25 <= 20) {
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/airquality").c_str(), "GOOD", true);
      } else if (averagePM25 > 20 && averagePM25 <= 25) {
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/airquality").c_str(), "FAIR", true);
      } else if (averagePM25 > 25 && averagePM25 <= 50) {
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/airquality").c_str(), "INFERIOR", true);
      } else if (averagePM25 > 50) {
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/airquality").c_str(), "POOR", true);
      } else {
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/airquality").c_str(), "UNKNOWN", true);
      }
    }


    if (!strcmp(THP_MODEL, "BME280")) {
      if (checkBmeStatus() == true) {

        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/temperature").c_str(), String(currentTemperature).c_str(), true);
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/pressure").c_str(), String(currentPressure).c_str(), true);
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/humidity").c_str(), String(currentHumidity).c_str(), true);

      } else {
        if (DEBUG) {
          Serial.println("No measurements from BME280!\n");
        }
      }
    }

    if (!strcmp(THP_MODEL, "BMP280")) {
      if (checkBmpStatus() == true) {

        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/temperature").c_str(), String(currentTemperature).c_str(), true);
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/pressure").c_str(), String(currentPressure).c_str(), true);

      } else {
        if (DEBUG) {
          Serial.println("No measurements from BMP280!\n");
        }
      }
    }

    if (!strcmp(THP_MODEL, "HTU21")) {
      if (checkHTU21DStatus() == true) {
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/temperature").c_str(), String(currentTemperature).c_str(), true);
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/humidity").c_str(), String(currentHumidity).c_str(), true);
      } else {
        if (DEBUG) {
          Serial.println("No measurements from HTU21!\n");
        }
      }
    }

    if (!strcmp(THP_MODEL, "DHT22")) {
      if (checkDHT22Status() == true) {
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/temperature").c_str(), String(currentTemperature).c_str(), true);
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/humidity").c_str(), String(currentHumidity).c_str(), true);
      } else {
        if (DEBUG) {
          Serial.println("No measurements from DHT22!\n");
        }
      }
    }

    if (!strcmp(THP_MODEL, "SHT1x")) {
      if (checkDHT22Status() == true) {
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/temperature").c_str(), String(currentTemperature).c_str(), true);
        mqttclient.publish(String("Smogomierz-" + mqttChipId + "/sensor/humidity").c_str(), String(currentHumidity).c_str(), true);
      } else {
        if (DEBUG) {
          Serial.println("No measurements from SHT1x!\n");
        }
      }
    }

    if (DEEPSLEEP_ON == true) {
      mqttclient.disconnect();
    }
  }

}

void takeTHPMeasurements() {
  if (!strcmp(THP_MODEL, "BME280")) {
#ifdef ARDUINO_ARCH_ESP8266
    BMESensor.refresh();
    delay(10);
#endif
    if (checkBmeStatus() == true) {
      if (DEBUG) {
        Serial.println("Measurements from BME280!\n");
      }
#ifdef ARDUINO_ARCH_ESP8266
      currentTemperature = BMESensor.temperature;
      currentPressure = BMESensor.seaLevelForAltitude(MYALTITUDE);
      currentHumidity = BMESensor.humidity;
#elif defined ARDUINO_ARCH_ESP32
      currentTemperature = (bme.readTemperature()); // maybe *0.89 ?
      currentPressure = (bme.seaLevelForAltitude(MYALTITUDE, (bme.readPressure() / 100.0F)));
      currentHumidity = (bme.readHumidity()); // maybe *0.89 ?
#endif

    } else {
      if (DEBUG) {
        Serial.println("No measurements from BME280!\n");
      }
    }
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    if (checkHTU21DStatus() == true) {
      if (DEBUG) {
        Serial.println("Measurements from HTU21!\n");
      }
      currentTemperature = myHTU21D.readTemperature();
      currentHumidity = myHTU21D.readHumidity();
    } else {
      if (DEBUG) {
        Serial.println("No measurements from HTU21D!\n");
      }
    }
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    if (checkBmpStatus() == true) {
      if (DEBUG) {
        Serial.println("Measurements from BMP280!\n");
      }
      currentTemperature = bmp.readTemperature();
      currentPressure = (bmp.readPressure()) / 100;
    } else {
      if (DEBUG) {
        Serial.println("No measurements from BMP280!\n");
      }
    }
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    if (checkDHT22Status() == true) {
      if (DEBUG) {
        Serial.println("Measurements from DHT22!\n");
      }
      currentTemperature = dht.readTemperature();
      currentHumidity = dht.readHumidity();
    } else {
      if (DEBUG) {
        Serial.println("No measurements from DHT22!\n");
      }
    }
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
    if (checkSHT1xStatus() == true) {
      if (DEBUG) {
        Serial.println("Measurements from SHT1x!\n");
      }
      currentTemperature = sht1x.readTemperatureC();
      currentHumidity = sht1x.readHumidity();
    } else {
      if (DEBUG) {
        Serial.println("No measurements from SHT1x!\n");
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
  if (++iPM == NUMBEROFMEASUREMENTS) {
    averagePM();
    iPM = 0;
  }
}

void takeSleepPMMeasurements() {
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
        takeNormalnPMMeasurements();
        counterNM1++;
      }
      previous_2sec_Millis = millis();
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

void pm_calibration() {
  // Automatic calibration - START
  if (!strcmp(MODEL, "white")) {
    if (!strcmp(THP_MODEL, "BME280")) {
#ifdef ARDUINO_ARCH_ESP8266
      BMESensor.refresh();
      delay(10);
      if (int(BMESensor.temperature) < 5 or int(BMESensor.humidity) > 60) {
        calib1 = float((200 - (BMESensor.humidity)) / 150);
        calib2 = calib1 / 2;
        calib = calib2;
      } else {
        calib = calib1;
      }
#elif defined ARDUINO_ARCH_ESP32
      if (int(bme.readTemperature()) < 5 or int(bme.readHumidity()) > 60) {
        calib1 = float((200 - (bme.readHumidity())) / 150);
        calib2 = calib1 / 2;
        calib = calib2;
      } else {
        calib = calib1;
      }
#endif
    } else if (!strcmp(THP_MODEL, "HTU21")) {
      if (int(myHTU21D.readTemperature()) < 5 or int(myHTU21D.readCompensatedHumidity()) > 60) {
        calib1 = float((200 - (myHTU21D.readCompensatedHumidity())) / 150);
        calib2 = calib1 / 2;
        calib = calib2;
      } else {
        calib = calib1;
      }
    } else if (!strcmp(THP_MODEL, "DHT22")) {
      if (int(dht.readTemperature()) < 5 or int(dht.readHumidity()) > 60) {
        calib1 = float((200 - (dht.readHumidity())) / 150);
        calib2 = calib1 / 2;
        calib = calib2;
      } else {
        calib = calib1;
      }

    } else if (!strcmp(THP_MODEL, "SHT1x")) {
      if (int(sht1x.readTemperatureC()) < 5 or int(sht1x.readHumidity()) > 60) {
        calib1 = float((200 - (sht1x.readHumidity())) / 150);
        calib2 = calib1 / 2;
        calib = calib2;
      } else {
        calib = calib1;
      }
    }
  }
  // Automatic calibration - END

  if (!strcmp(THP_MODEL, "BME280")) {
    calib = calib1;
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    calib = calib1;
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    calib = calib1;
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
    calib = calib1;
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    calib = calib1;
  }

}

void averagePM() {
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
  if (DEBUG) {
    Serial.print("\n\nAverage PM1: ");
    Serial.print(averagePM1);
    Serial.print("\nAverage PM2.5: ");
    Serial.print(averagePM25);
    Serial.print("\nAverage PM10: ");
    Serial.print(averagePM10);
  }
}
