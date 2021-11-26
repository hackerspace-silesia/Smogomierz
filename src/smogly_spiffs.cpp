#include <ArduinoJson.h>
#include <FS.h>
#ifdef ARDUINO_ARCH_ESP32
#include <SPIFFS.h>
#endif

#include "../config.h"
#define FORMAT_SPIFFS_IF_FAILED true

void _safeCpy(char* dest, const JsonVariant &obj, const char* dflt = "", int CharSize = 255) {
  const char* val = obj.as<const char*>();
  if (val) {
    strncpy(dest, val, CharSize);
  } else {
    strncpy(dest, dflt, CharSize);
  }
}

bool loadConfig() {
#ifdef ARDUINO_ARCH_ESP8266
  File configFile = SPIFFS.open("/config.json", "r");
#elif defined ARDUINO_ARCH_ESP32
  File configFile = SPIFFS.open("/config.json");
#endif

  if (!configFile) {
    Serial.println(F("Failed to open config file"));
    return false;
  }

  size_t size = configFile.size();
  if (size > 2048) {
    Serial.println(F("Config file size is too large"));
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  //StaticJsonDocument<capacity> jsonBuffer;
#ifdef ARDUINO_ARCH_ESP8266
  const short capacity = 6144;
  StaticJsonDocument<capacity> jsonBuffer;
  // DynamicJsonDocument jsonBuffer(ESP.getMaxFreeBlockSize());
#elif defined ARDUINO_ARCH_ESP32
  // const short capacity = 4608;
  // const short capacity = 5620;
  // StaticJsonDocument<capacity> jsonBuffer;
  DynamicJsonDocument jsonBuffer(ESP.getMaxAllocHeap());
#endif

  deserializeJson(jsonBuffer, buf.get());
  JsonObject json = jsonBuffer.as<JsonObject>();

  if (json.isNull()) {
    Serial.println(F("Failed to parse config file"));
    return false;
  }

  // REMEMBER TO ADD/EDIT KEYS IN config.h AND webserver.h!!

  DEVICENAME_AUTO = json["DEVICENAME_AUTO"];
  _safeCpy(DEVICENAME, json["DEVICENAME"], "smogomierz", 32);
  _safeCpy(LANGUAGE, json["LANGUAGE"], "english", 12);
  SELECTED_LANGUAGE = json["SELECTED_LANGUAGE"];

  _safeCpy(THP_MODEL, json["THP_MODEL"], "Non", 12);
  _safeCpy(DUST_MODEL, json["DUST_MODEL"], "Non", 12);
  DISPLAY_PM1 = json["DISPLAY_PM1"];
  FREQUENTMEASUREMENT = json["FREQUENTMEASUREMENT"];

  SECOND_THP = json["SECOND_THP"];
  _safeCpy(SECOND_THP_MODEL, json["SECOND_THP_MODEL"], "Non", 12);

  FIRST_THP_SDA = json["FIRST_THP_SDA"];
  FIRST_THP_SCL = json["FIRST_THP_SCL"];

  SECOND_THP_SDA = json["SECOND_THP_SDA"];
  SECOND_THP_SCL = json["SECOND_THP_SCL"];

  DUST_TX = json["DUST_TX"];
  DUST_RX = json["DUST_RX"];

  _safeCpy(CONFIG_FIRST_THP_SDA, json["CONFIG_FIRST_THP_SDA"], "D3", 4);
  _safeCpy(CONFIG_FIRST_THP_SCL, json["CONFIG_FIRST_THP_SCL"], "D4", 4);

  _safeCpy(CONFIG_SECOND_THP_SDA, json["CONFIG_SECOND_THP_SDA"], "D5", 4);
  _safeCpy(CONFIG_SECOND_THP_SCL, json["CONFIG_SECOND_THP_SCL"], "D6", 4);

  _safeCpy(CONFIG_DUST_TX, json["CONFIG_DUST_TX"], "D1", 4);
  _safeCpy(CONFIG_DUST_RX, json["CONFIG_DUST_RX"], "D2", 4);

  DUST_TIME = json["DUST_TIME"];
  NUMBEROFMEASUREMENTS = json["NUMBEROFMEASUREMENTS"];

  LUFTDATEN_ON = json["LUFTDATEN_ON"];
  LUFTDATEN_GRAPH_ON = json["LUFTDATEN_GRAPH_ON"];
  SMOGLIST_ON = json["SMOGLIST_ON"];

  AIRMONITOR_ON = json["AIRMONITOR_ON"];
  AIRMONITOR_GRAPH_ON = json["AIRMONITOR_GRAPH_ON"];
  _safeCpy(AIRMONITOR_API_KEY, json["AIRMONITOR_API_KEY"], "", 48);
  _safeCpy(LATITUDE, json["LATITUDE"], "50.263911", 16);
  _safeCpy(LONGITUDE, json["LONGITUDE"], "18.995711", 16);
  _safeCpy(EMAIL, json["EMAIL"], "email@mail.pl", 64);
  MYALTITUDE = json["MYALTITUDE"];

  THINGSPEAK_ON = json["THINGSPEAK_ON"];
  THINGSPEAK_GRAPH_ON = json["THINGSPEAK_GRAPH_ON"];
  _safeCpy(THINGSPEAK_API_KEY, json["THINGSPEAK_API_KEY"], "WRITE_API_KEY", 32);
  THINGSPEAK_CHANNEL_ID = json["THINGSPEAK_CHANNEL_ID"];
  _safeCpy(THINGSPEAK_READ_API_KEY, json["THINGSPEAK_READ_API_KEY"], "READ_API_KEY", 32);

  INFLUXDB_ON = json["INFLUXDB_ON"];
  _safeCpy(INFLUXDB_VERSION, json["INFLUXDB_VERSION"], "1", 4);
  _safeCpy(INFLUXDB_HOST, json["INFLUXDB_HOST"], "host", 128);
  INFLUXDB_PORT = json["INFLUXDB_PORT"];
  _safeCpy(INFLUXDB_DATABASE, json["INFLUXDB_DATABASE"], "mydb", 32);
  _safeCpy(INFLUXDB_USER, json["INFLUXDB_USER"], "user", 32);
  _safeCpy(INFLUXDB_PASSWORD, json["INFLUXDB_PASSWORD"], "password", 64);
  _safeCpy(INFLUXDB_ORG, json["INFLUXDB_ORG"], "myOrg", 32);
  _safeCpy(INFLUXDB_BUCKET, json["INFLUXDB_BUCKET"], "myBucket", 32);
  _safeCpy(INFLUXDB_TOKEN, json["INFLUXDB_TOKEN"], "myToken", 128);

  MQTT_ON = json["MQTT_ON"];
  _safeCpy(MQTT_HOST, json["MQTT_HOST"], "host", 128);
  MQTT_PORT = json["MQTT_PORT"];
  _safeCpy(MQTT_USER, json["MQTT_USER"], "user", 32);
  _safeCpy(MQTT_PASSWORD, json["MQTT_PASSWORD"], "password", 64);

  MQTT_IP_IN_TOPIC = json["MQTT_IP_IN_TOPIC"];
  MQTT_DEVICENAME_IN_TOPIC = json["MQTT_DEVICENAME_IN_TOPIC"];
  MQTT_SLASH_AT_THE_BEGINNING = json["MQTT_SLASH_AT_THE_BEGINNING"];
  MQTT_SLASH_AT_THE_END = json["MQTT_SLASH_AT_THE_END"];

  _safeCpy(MQTT_TOPIC_TEMP, json["MQTT_TOPIC_TEMP"], "MQTT_TOPIC_TEMP", 64);
  _safeCpy(MQTT_TOPIC_HUMI, json["MQTT_TOPIC_HUMI"], "MQTT_TOPIC_HUMI", 64);
  _safeCpy(MQTT_TOPIC_PRESS, json["MQTT_TOPIC_PRESS"], "MQTT_TOPIC_PRESS", 64);
  _safeCpy(MQTT_TOPIC_PM1, json["MQTT_TOPIC_PM1"], "MQTT_TOPIC_PM1", 64);
  _safeCpy(MQTT_TOPIC_PM25, json["MQTT_TOPIC_PM25"], "MQTT_TOPIC_PM25", 64);
  _safeCpy(MQTT_TOPIC_PM10, json["MQTT_TOPIC_PM10"], "MQTT_TOPIC_PM10", 64);
  _safeCpy(MQTT_TOPIC_AIRQUALITY, json["MQTT_TOPIC_AIRQUALITY"], "MQTT_TOPIC_AIRQUALITY", 32);

  AQI_ECO_ON = json["AQI_ECO_ON"];
  _safeCpy(AQI_ECO_HOST, json["AQI_ECO_HOST"], "host", 64);
  _safeCpy(AQI_ECO_PATH, json["AQI_ECO_PATH"], "path", 64);

  SENDING_FREQUENCY = json["SENDING_FREQUENCY"];
  SENDING_DB_FREQUENCY = json["SENDING_DB_FREQUENCY"];
  DEEPSLEEP_ON = json["DEEPSLEEP_ON"];

  DEBUG = json["DEBUG"];
  AUTOUPDATE_ON = json["AUTOUPDATE_ON"];

  CONFIG_AUTH = json["CONFIG_AUTH"];
  _safeCpy(CONFIG_USERNAME, json["CONFIG_USERNAME"], "admin", 64);
  _safeCpy(CONFIG_PASSWORD, json["CONFIG_PASSWORD"], "password", 128);

  _safeCpy(MODEL, json["MODEL"], "black", 12);

  HOMEKIT_SUPPORT = json["HOMEKIT_SUPPORT"];

  // Real world application would store these values in some variables for
  // later use.
    if (DEBUG) {
#ifdef ARDUINO_ARCH_ESP8266
      Serial.print(F("Loaded DEVICENAME_AUTO: "));
      Serial.println(DEVICENAME_AUTO);
      Serial.print(F("Loaded DEVICENAME: "));
      Serial.println(DEVICENAME);
      Serial.print(F("Loaded LANGUAGE: "));
      Serial.println(LANGUAGE);
      Serial.print(F("Loaded SELECTED_LANGUAGE: "));
      Serial.println(SELECTED_LANGUAGE);

      Serial.print(F("Loaded THP_MODEL: "));
      Serial.println(THP_MODEL);
      Serial.print(F("Loaded DUST_MODEL: "));
      Serial.println(DUST_MODEL);
      Serial.print(F("Loaded DISPLAY_PM1: "));
      Serial.println(DISPLAY_PM1);
      Serial.print(F("Loaded FREQUENTMEASUREMENT: "));
      Serial.println(FREQUENTMEASUREMENT);

      Serial.print(F("Loaded SECOND_THP: "));
      Serial.println(SECOND_THP_MODEL);
      Serial.print(F("Loaded SECOND_THP_MODEL: "));
      Serial.println(SECOND_THP_MODEL);

      Serial.print(F("Loaded FIRST_THP_SDA: "));
      Serial.println(FIRST_THP_SDA);
      Serial.print(F("Loaded FIRST_THP_SCL: "));
      Serial.println(FIRST_THP_SCL);

      Serial.print(F("Loaded SECOND_THP_SDA: "));
      Serial.println(SECOND_THP_SDA);
      Serial.print(F("Loaded SECOND_THP_SCL: "));
      Serial.println(SECOND_THP_SCL);

      Serial.print(F("Loaded DUST_TX: "));
      Serial.println(DUST_TX);
      Serial.print(F("Loaded DUST_RX: "));
      Serial.println(DUST_RX);

      Serial.print(F("Loaded CONFIG_FIRST_THP_SDA: "));
      Serial.println(CONFIG_FIRST_THP_SDA);
      Serial.print(F("Loaded CONFIG_FIRST_THP_SCL: "));
      Serial.println(CONFIG_FIRST_THP_SCL);

      Serial.print(F("Loaded CONFIG_SECOND_THP_SDA: "));
      Serial.println(CONFIG_SECOND_THP_SDA);
      Serial.print(F("Loaded CONFIG_SECOND_THP_SCL: "));
      Serial.println(CONFIG_SECOND_THP_SCL);

      Serial.print(F("Loaded CONFIG_DUST_TX: "));
      Serial.println(CONFIG_DUST_TX);
      Serial.print(F("Loaded CONFIG_DUST_RX: "));
      Serial.println(CONFIG_DUST_RX);

      Serial.print(F("Loaded DUST_TIME: "));
      Serial.println(DUST_TIME);
      Serial.print(F("Loaded NUMBEROFMEASUREMENTS: "));
      Serial.println(NUMBEROFMEASUREMENTS);

      Serial.print(F("Loaded LUFTDATEN_ON: "));
      Serial.println(LUFTDATEN_ON);
      Serial.print(F("Loaded LUFTDATEN_GRAPH_ON: "));
      Serial.println(LUFTDATEN_GRAPH_ON);
	  
      Serial.print(F("Loaded SMOGLIST_ON: "));
      Serial.println(SMOGLIST_ON);

      Serial.print(F("Loaded AIRMONITOR_ON: "));
      Serial.println(AIRMONITOR_ON);
      Serial.print(F("Loaded AIRMONITOR_GRAPH_ON: "));
      Serial.println(AIRMONITOR_GRAPH_ON);
      Serial.print(F("Loaded AIRMONITOR_API_KEY: "));
      Serial.println(AIRMONITOR_API_KEY);
      Serial.print(F("Loaded LATITUDE: "));
      Serial.println(LATITUDE);
      Serial.print(F("Loaded LONGITUDE: "));
      Serial.println(LONGITUDE);
      Serial.print(F("Loaded EMAIL: "));
      Serial.println(EMAIL);
      Serial.print(F("Loaded MYALTITUDE: "));
      Serial.println(MYALTITUDE);

      Serial.print(F("Loaded THINGSPEAK_ON: "));
      Serial.println(THINGSPEAK_ON);
      Serial.print(F("Loaded THINGSPEAK_GRAPH_ON: "));
      Serial.println(THINGSPEAK_GRAPH_ON);
      Serial.print(F("Loaded THINGSPEAK_API_KEY: "));
      Serial.println(THINGSPEAK_API_KEY);
      Serial.print(F("Loaded THINGSPEAK_CHANNEL_ID: "));
      Serial.println(THINGSPEAK_CHANNEL_ID);
      Serial.print(F("Loaded THINGSPEAK_READ_API_KEY: "));
      Serial.println(THINGSPEAK_READ_API_KEY);

      Serial.print(F("Loaded INFLUXDB_ON: "));
      Serial.println(INFLUXDB_ON);
      Serial.print(F("Loaded INFLUXDB_VERSION: "));
      Serial.println(INFLUXDB_VERSION);
      Serial.print(F("Loaded INFLUXDB_HOST: "));
      Serial.println(INFLUXDB_HOST);
      Serial.print(F("Loaded INFLUXDB_PORT: "));
      Serial.println(INFLUXDB_PORT);
      Serial.print(F("Loaded INFLUXDB_DATABASE: "));
      Serial.println(INFLUXDB_DATABASE);
      Serial.print(F("Loaded INFLUXDB_USER: "));
      Serial.println(INFLUXDB_USER);
      Serial.print(F("Loaded INFLUXDB_PASSWORD: "));
      Serial.println(INFLUXDB_PASSWORD);
      Serial.print(F("Loaded INFLUXDB_ORG: "));
      Serial.println(INFLUXDB_ORG);
      Serial.print(F("Loaded INFLUXDB_BUCKET: "));
      Serial.println(INFLUXDB_BUCKET);
      Serial.print(F("Loaded INFLUXDB_TOKEN: "));
      Serial.println(INFLUXDB_TOKEN);

      Serial.print(F("Loaded MQTT_ON: "));
      Serial.println(MQTT_ON);
      Serial.print(F("Loaded MQTT_HOST: "));
      Serial.println(MQTT_HOST);
      Serial.print(F("Loaded MQTT_PORT: "));
      Serial.println(MQTT_PORT);
      Serial.print(F("Loaded MQTT_USER: "));
      Serial.println(MQTT_USER);
      Serial.print(F("Loaded MQTT_PASSWORD: "));
      Serial.println(MQTT_PASSWORD);

      Serial.print(F("Loaded MQTT_IP_IN_TOPIC: "));
      Serial.println(MQTT_IP_IN_TOPIC);
      Serial.print(F("Loaded MQTT_DEVICENAME_IN_TOPIC: "));
      Serial.println(MQTT_DEVICENAME_IN_TOPIC);
      Serial.print(F("Loaded MQTT_SLASH_AT_THE_BEGINNING: "));
      Serial.println(MQTT_SLASH_AT_THE_BEGINNING);
      Serial.print(F("Loaded MQTT_SLASH_AT_THE_END: "));
      Serial.println(MQTT_SLASH_AT_THE_END);

      Serial.print(F("Loaded MQTT_TOPIC_TEMP: "));
      Serial.println(MQTT_TOPIC_TEMP);
      Serial.print(F("Loaded MQTT_TOPIC_HUMI: "));
      Serial.println(MQTT_TOPIC_HUMI);
      Serial.print(F("Loaded MQTT_TOPIC_PRESS: "));
      Serial.println(MQTT_TOPIC_PRESS);
      Serial.print(F("Loaded MQTT_TOPIC_PM1: "));
      Serial.println(MQTT_TOPIC_PM1);
      Serial.print(F("Loaded MQTT_TOPIC_PM25: "));
      Serial.println(MQTT_TOPIC_PM25);
      Serial.print(F("Loaded MQTT_TOPIC_PM10: "));
      Serial.println(MQTT_TOPIC_PM10);
      Serial.print(F("Loaded MQTT_TOPIC_AIRQUALITY: "));
      Serial.println(MQTT_TOPIC_AIRQUALITY);

      Serial.print(F("Loaded AQI_ECO_ON: "));
      Serial.println(AQI_ECO_ON);
      Serial.print(F("Loaded AQI_ECO_HOST: "));
      Serial.println(AQI_ECO_HOST);
      Serial.print(F("Loaded AQI_ECO_PATH: "));
      Serial.println(AQI_ECO_PATH);

      Serial.print(F("Loaded SENDING_FREQUENCY: "));
      Serial.println(SENDING_FREQUENCY);
      Serial.print(F("Loaded SENDING_DB_FREQUENCY: "));
      Serial.println(SENDING_DB_FREQUENCY);
      Serial.print(F("Loaded DEEPSLEEP_ON: "));
      Serial.println(DEEPSLEEP_ON);

      Serial.print(F("Loaded DEBUG: "));
      Serial.println(DEBUG);
      Serial.print(F("Loaded AUTOUPDATE_ON: "));
      Serial.println(AUTOUPDATE_ON);

      Serial.print(F("Loaded CONFIG_AUTH: "));
      Serial.println(CONFIG_AUTH);
      Serial.print(F("Loaded CONFIG_USERNAME: "));
      Serial.println(CONFIG_USERNAME);
      Serial.print(F("Loaded CONFIG_PASSWORD: "));
      Serial.println(CONFIG_PASSWORD);

      Serial.print(F("Loaded MODEL: "));
      Serial.println(MODEL);

      Serial.print(F("Loaded HOMEKIT_SUPPORT: "));
      Serial.println(HOMEKIT_SUPPORT);

      Serial.print(F("Loaded PMSENSORVERSION: "));
      Serial.println(PMSENSORVERSION);

      Serial.print(F("Loaded SOFTWAREVERSION: "));
      Serial.println(SOFTWAREVERSION);

      Serial.println(F("\n"));
#elif defined ARDUINO_ARCH_ESP32
      Serial.print("Loaded DEVICENAME_AUTO: ");
      Serial.println(DEVICENAME_AUTO);
      Serial.print("Loaded DEVICENAME: ");
      Serial.println(DEVICENAME);
      Serial.print("Loaded LANGUAGE: ");
      Serial.println(LANGUAGE);
      Serial.print("Loaded SELECTED_LANGUAGE: ");
      Serial.println(SELECTED_LANGUAGE);

      Serial.print("Loaded THP_MODEL: ");
      Serial.println(THP_MODEL);
      Serial.print("Loaded DUST_MODEL: ");
      Serial.println(DUST_MODEL);
      Serial.print("Loaded DISPLAY_PM1: ");
      Serial.println(DISPLAY_PM1);
      Serial.print("Loaded FREQUENTMEASUREMENT: ");
      Serial.println(FREQUENTMEASUREMENT);

      Serial.print("Loaded SECOND_THP: ");
      Serial.println(SECOND_THP_MODEL);
      Serial.print("Loaded SECOND_THP_MODEL: ");
      Serial.println(SECOND_THP_MODEL);

      Serial.print("Loaded FIRST_THP_SDA: ");
      Serial.println(FIRST_THP_SDA);
      Serial.print("Loaded FIRST_THP_SCL: ");
      Serial.println(FIRST_THP_SCL);

      Serial.print("Loaded SECOND_THP_SDA: ");
      Serial.println(SECOND_THP_SDA);
      Serial.print("Loaded SECOND_THP_SCL: ");
      Serial.println(SECOND_THP_SCL);

      Serial.print("Loaded DUST_TX: ");
      Serial.println(DUST_TX);
      Serial.print("Loaded DUST_RX: ");
      Serial.println(DUST_RX);

      Serial.print("Loaded CONFIG_FIRST_THP_SDA: ");
      Serial.println(CONFIG_FIRST_THP_SDA);
      Serial.print("Loaded CONFIG_FIRST_THP_SCL: ");
      Serial.println(CONFIG_FIRST_THP_SCL);

      Serial.print("Loaded CONFIG_SECOND_THP_SDA: ");
      Serial.println(CONFIG_SECOND_THP_SDA);
      Serial.print("Loaded CONFIG_SECOND_THP_SCL: ");
      Serial.println(CONFIG_SECOND_THP_SCL);

      Serial.print("Loaded CONFIG_DUST_TX: ");
      Serial.println(CONFIG_DUST_TX);
      Serial.print("Loaded CONFIG_DUST_RX: ");
      Serial.println(CONFIG_DUST_RX);

      Serial.print("Loaded DUST_TIME: ");
      Serial.println(DUST_TIME);
      Serial.print("Loaded NUMBEROFMEASUREMENTS: ");
      Serial.println(NUMBEROFMEASUREMENTS);

      Serial.print("Loaded LUFTDATEN_ON: ");
      Serial.println(LUFTDATEN_ON);
      Serial.print("Loaded LUFTDATEN_GRAPH_ON: ");
      Serial.println(LUFTDATEN_GRAPH_ON);
	  
      Serial.print("Loaded SMOGLIST_ON: ");
      Serial.println(SMOGLIST_ON);

      Serial.print("Loaded AIRMONITOR_ON: ");
      Serial.println(AIRMONITOR_ON);
      Serial.print("Loaded AIRMONITOR_GRAPH_ON: ");
      Serial.println(AIRMONITOR_GRAPH_ON);
      Serial.print("Loaded AIRMONITOR_API_KEY: ");
      Serial.println(AIRMONITOR_API_KEY);
      Serial.print("Loaded LATITUDE: ");
      Serial.println(LATITUDE);
      Serial.print("Loaded LONGITUDE: ");
      Serial.println(LONGITUDE);
      Serial.print("Loaded EMAIL: ");
      Serial.println(EMAIL);
      Serial.print("Loaded MYALTITUDE: ");
      Serial.println(MYALTITUDE);

      Serial.print("Loaded THINGSPEAK_ON: ");
      Serial.println(THINGSPEAK_ON);
      Serial.print("Loaded THINGSPEAK_GRAPH_ON: ");
      Serial.println(THINGSPEAK_GRAPH_ON);
      Serial.print("Loaded THINGSPEAK_API_KEY: ");
      Serial.println(THINGSPEAK_API_KEY);
      Serial.print("Loaded THINGSPEAK_CHANNEL_ID: ");
      Serial.println(THINGSPEAK_CHANNEL_ID);
      Serial.print("Loaded THINGSPEAK_READ_API_KEY: ");
      Serial.println(THINGSPEAK_READ_API_KEY);

      Serial.print("Loaded INFLUXDB_ON: ");
      Serial.println(INFLUXDB_ON);
      Serial.print("Loaded INFLUXDB_VERSION: ");
      Serial.println(INFLUXDB_VERSION);
      Serial.print("Loaded INFLUXDB_HOST: ");
      Serial.println(INFLUXDB_HOST);
      Serial.print("Loaded INFLUXDB_PORT: ");
      Serial.println(INFLUXDB_PORT);
      Serial.print("Loaded INFLUXDB_DATABASE: ");
      Serial.println(INFLUXDB_DATABASE);
      Serial.print("Loaded INFLUXDB_USER: ");
      Serial.println(INFLUXDB_USER);
      Serial.print("Loaded INFLUXDB_PASSWORD: ");
      Serial.println(INFLUXDB_PASSWORD);
      Serial.print("Loaded INFLUXDB_ORG: ");
      Serial.println(INFLUXDB_ORG);
      Serial.print("Loaded INFLUXDB_BUCKET: ");
      Serial.println(INFLUXDB_BUCKET);
      Serial.print("Loaded INFLUXDB_TOKEN: ");
      Serial.println(INFLUXDB_TOKEN);

      Serial.print("Loaded MQTT_ON: ");
      Serial.println(MQTT_ON);
      Serial.print("Loaded MQTT_HOST: ");
      Serial.println(MQTT_HOST);
      Serial.print("Loaded MQTT_PORT: ");
      Serial.println(MQTT_PORT);
      Serial.print("Loaded MQTT_USER: ");
      Serial.println(MQTT_USER);
      Serial.print("Loaded MQTT_PASSWORD: ");
      Serial.println(MQTT_PASSWORD);

      Serial.print("Loaded MQTT_IP_IN_TOPIC: ");
      Serial.println(MQTT_IP_IN_TOPIC);
      Serial.print("Loaded MQTT_DEVICENAME_IN_TOPIC: ");
      Serial.println(MQTT_DEVICENAME_IN_TOPIC);
      Serial.print("Loaded MQTT_SLASH_AT_THE_BEGINNING: ");
      Serial.println(MQTT_SLASH_AT_THE_BEGINNING);
      Serial.print("Loaded MQTT_SLASH_AT_THE_END: ");
      Serial.println(MQTT_SLASH_AT_THE_END);

      Serial.print("Loaded MQTT_TOPIC_TEMP: ");
      Serial.println(MQTT_TOPIC_TEMP);
      Serial.print("Loaded MQTT_TOPIC_HUMI: ");
      Serial.println(MQTT_TOPIC_HUMI);
      Serial.print("Loaded MQTT_TOPIC_PRESS: ");
      Serial.println(MQTT_TOPIC_PRESS);
      Serial.print("Loaded MQTT_TOPIC_PM1: ");
      Serial.println(MQTT_TOPIC_PM1);
      Serial.print("Loaded MQTT_TOPIC_PM25: ");
      Serial.println(MQTT_TOPIC_PM25);
      Serial.print("Loaded MQTT_TOPIC_PM10: ");
      Serial.println(MQTT_TOPIC_PM10);
      Serial.print("Loaded MQTT_TOPIC_AIRQUALITY: ");
      Serial.println(MQTT_TOPIC_AIRQUALITY);

      Serial.print("Loaded AQI_ECO_ON: ");
      Serial.println(AQI_ECO_ON);
      Serial.print("Loaded AQI_ECO_HOST: ");
      Serial.println(AQI_ECO_HOST);
      Serial.print("Loaded AQI_ECO_PATH: ");
      Serial.println(AQI_ECO_PATH);

      Serial.print("Loaded SENDING_FREQUENCY: ");
      Serial.println(SENDING_FREQUENCY);
      Serial.print("Loaded SENDING_DB_FREQUENCY: ");
      Serial.println(SENDING_DB_FREQUENCY);
      Serial.print("Loaded DEEPSLEEP_ON: ");
      Serial.println(DEEPSLEEP_ON);

      Serial.print("Loaded DEBUG: ");
      Serial.println(DEBUG);
      Serial.print("Loaded AUTOUPDATE_ON: ");
      Serial.println(AUTOUPDATE_ON);

      Serial.print("Loaded CONFIG_AUTH: ");
      Serial.println(CONFIG_AUTH);
      Serial.print("Loaded CONFIG_USERNAME: ");
      Serial.println(CONFIG_USERNAME);
      Serial.print("Loaded CONFIG_PASSWORD: ");
      Serial.println(CONFIG_PASSWORD);

      Serial.print("Loaded MODEL: ");
      Serial.println(MODEL);

      Serial.print("Loaded HOMEKIT_SUPPORT: ");
      Serial.println(HOMEKIT_SUPPORT);

      Serial.print("Loaded PMSENSORVERSION: ");
      Serial.println(PMSENSORVERSION);

      Serial.print("Loaded SOFTWAREVERSION: ");
      Serial.println(SOFTWAREVERSION);

      Serial.println("\n");
#endif
    }
    return true;
  }

bool saveConfig() {
	
  if (DEBUG) {
	  /*
    Serial.print(F("Saved DEVICENAME_AUTO: "));
    Serial.println(DEVICENAME_AUTO);
    Serial.print(F("Saved DEVICENAME: "));
    Serial.println(DEVICENAME);
    Serial.print(F("Saved LANGUAGE: "));
    Serial.println(LANGUAGE);
    Serial.print(F("Saved SELECTED_LANGUAGE: "));
    Serial.println(SELECTED_LANGUAGE);

    Serial.print(F("Saved THP_MODEL: "));
    Serial.println(THP_MODEL);
    Serial.print(F("Saved DUST_MODEL: "));
    Serial.println(DUST_MODEL);
    Serial.print(F("Saved DISPLAY_PM1: "));
    Serial.println(DISPLAY_PM1);
    Serial.print(F("Saved FREQUENTMEASUREMENT: "));
    Serial.println(FREQUENTMEASUREMENT);

    Serial.print(F("Saved SECOND_THP: "));
    Serial.println(SECOND_THP_MODEL);
    Serial.print(F("Saved SECOND_THP_MODEL: "));
    Serial.println(SECOND_THP_MODEL);

    Serial.print(F("Saved FIRST_THP_SDA: "));
    Serial.println(FIRST_THP_SDA);
    Serial.print(F("Saved FIRST_THP_SCL: "));
    Serial.println(FIRST_THP_SCL);

    Serial.print(F("Saved SECOND_THP_SDA: "));
    Serial.println(SECOND_THP_SDA);
    Serial.print(F("Saved SECOND_THP_SCL: "));
    Serial.println(SECOND_THP_SCL);

    Serial.print(F("Saved DUST_TX: "));
    Serial.println(DUST_TX);
    Serial.print(F("Saved DUST_RX: "));
    Serial.println(DUST_RX);
	
    Serial.print(F("Saved CONFIG_FIRST_THP_SDA: "));
    Serial.println(CONFIG_FIRST_THP_SDA);
    Serial.print(F("Saved CONFIG_FIRST_THP_SCL: "));
    Serial.println(CONFIG_FIRST_THP_SCL);

    Serial.print(F("Saved CONFIG_SECOND_THP_SDA: "));
    Serial.println(CONFIG_SECOND_THP_SDA);
    Serial.print(F("Saved CONFIG_SECOND_THP_SCL: "));
    Serial.println(CONFIG_SECOND_THP_SCL);

    Serial.print(F("Saved CONFIG_DUST_TX: "));
    Serial.println(CONFIG_DUST_TX);
    Serial.print(F("Saved CONFIG_DUST_RX: "));
    Serial.println(CONFIG_DUST_RX);
	
    Serial.print(F("Saved DUST_TIME: "));
    Serial.println(DUST_TIME);
    Serial.print(F("Saved NUMBEROFMEASUREMENTS: "));
    Serial.println(NUMBEROFMEASUREMENTS);
*/
	  /*
    Serial.print(F("Saved LUFTDATEN_ON: "));
    Serial.println(LUFTDATEN_ON);
    Serial.print(F("Saved SMOGLIST_ON: "));
    Serial.println(SMOGLIST_ON);
*/
/*	
    Serial.print(F("Saved AIRMONITOR_ON: "));
    Serial.println(AIRMONITOR_ON);
    Serial.print(F("Saved AIRMONITOR_GRAPH_ON: "));
    Serial.println(AIRMONITOR_GRAPH_ON);
    Serial.print(F("Saved AIRMONITOR_API_KEY: "));
    Serial.println(AIRMONITOR_API_KEY);
    Serial.print(F("Saved LATITUDE: "));
    Serial.println(LATITUDE);
    Serial.print(F("Saved LONGITUDE: "));
    Serial.println(LONGITUDE);
    Serial.print(F("Saved EMAIL: "));
    Serial.println(EMAIL);
    Serial.print(F("Saved MYALTITUDE: "));
    Serial.println(MYALTITUDE);
	
    Serial.print(F("Saved THINGSPEAK_ON: "));
    Serial.println(THINGSPEAK_ON);
    Serial.print(F("Saved THINGSPEAK_GRAPH_ON: "));
    Serial.println(THINGSPEAK_GRAPH_ON);
    Serial.print(F("Saved THINGSPEAK_API_KEY: "));
    Serial.println(THINGSPEAK_API_KEY);
    Serial.print(F("Saved THINGSPEAK_CHANNEL_ID: "));
    Serial.println(THINGSPEAK_CHANNEL_ID);
    Serial.print(F("Saved THINGSPEAK_READ_API_KEY: "));
    Serial.println(THINGSPEAK_READ_API_KEY);

    Serial.print(F("Saved INFLUXDB_ON: "));
    Serial.println(INFLUXDB_ON);
    Serial.print(F("Saved INFLUXDB_VERSION: "));
    Serial.println(INFLUXDB_VERSION);
    Serial.print(F("Saved INFLUXDB_HOST: "));
    Serial.println(INFLUXDB_HOST);
    Serial.print(F("Saved INFLUXDB_PORT: "));
    Serial.println(INFLUXDB_PORT);
    Serial.print(F("Saved INFLUXDB_DATABASE: "));
    Serial.println(INFLUXDB_DATABASE);
    Serial.print(F("Saved INFLUXDB_USER: "));
    Serial.println(INFLUXDB_USER);
    Serial.print(F("Saved INFLUXDB_PASSWORD: "));
    Serial.println(INFLUXDB_PASSWORD);
    Serial.print(F("Saved INFLUXDB_ORG: "));
    Serial.println(INFLUXDB_ORG);
    Serial.print(F("Saved INFLUXDB_BUCKET: "));
    Serial.println(INFLUXDB_BUCKET);
    Serial.print(F("Saved INFLUXDB_TOKEN: "));
    Serial.println(INFLUXDB_TOKEN);

    Serial.print(F("Saved MQTT_ON: "));
    Serial.println(MQTT_ON);
    Serial.print(F("Saved MQTT_HOST: "));
    Serial.println(MQTT_HOST);
    Serial.print(F("Saved MQTT_PORT: "));
    Serial.println(MQTT_PORT);
    Serial.print(F("Saved MQTT_USER: "));
    Serial.println(MQTT_USER);
    Serial.print(F("Saved MQTT_PASSWORD: "));
    Serial.println(MQTT_PASSWORD);

    Serial.print(F("Saved MQTT_IP_IN_TOPIC: "));
    Serial.println(MQTT_IP_IN_TOPIC);
    Serial.print(F("Saved MQTT_DEVICENAME_IN_TOPIC: "));
    Serial.println(MQTT_DEVICENAME_IN_TOPIC);
    Serial.print(F("Saved MQTT_SLASH_AT_THE_BEGINNING: "));
    Serial.println(MQTT_SLASH_AT_THE_BEGINNING);
    Serial.print(F("Saved MQTT_SLASH_AT_THE_END: "));
    Serial.println(MQTT_SLASH_AT_THE_END);

    Serial.print(F("Saved MQTT_TOPIC_TEMP: "));
    Serial.println(MQTT_TOPIC_TEMP);
    Serial.print(F("Saved MQTT_TOPIC_HUMI: "));
    Serial.println(MQTT_TOPIC_HUMI);
    Serial.print(F("Saved MQTT_TOPIC_PRESS: "));
    Serial.println(MQTT_TOPIC_PRESS);
    Serial.print(F("Saved MQTT_TOPIC_PM1: "));
    Serial.println(MQTT_TOPIC_PM1);
    Serial.print(F("Saved MQTT_TOPIC_PM25: "));
    Serial.println(MQTT_TOPIC_PM25);
    Serial.print(F("Saved MQTT_TOPIC_PM10: "));
    Serial.println(MQTT_TOPIC_PM10);
    Serial.print(F("Saved MQTT_TOPIC_AIRQUALITY: "));
    Serial.println(MQTT_TOPIC_AIRQUALITY);

    Serial.print(F("Saved AQI_ECO_ON: "));
    Serial.println(AQI_ECO_ON);
    Serial.print(F("Saved AQI_ECO_HOST: "));
    Serial.println(AQI_ECO_HOST);
    Serial.print(F("Saved AQI_ECO_PATH: "));
    Serial.println(AQI_ECO_PATH);

    Serial.print(F("Saved SENDING_FREQUENCY: "));
    Serial.println(SENDING_FREQUENCY);
    Serial.print(F("Saved SENDING_DB_FREQUENCY: "));
    Serial.println(SENDING_DB_FREQUENCY);
    Serial.print(F("Saved DEEPSLEEP_ON: "));
    Serial.println(DEEPSLEEP_ON);

    Serial.print(F("Saved DEBUG: "));
    Serial.println(DEBUG);
    Serial.print(F("Saved AUTOUPDATE_ON: "));
    Serial.println(AUTOUPDATE_ON);

    Serial.print(F("Saved CONFIG_AUTH: "));
    Serial.println(CONFIG_AUTH);
    Serial.print(F("Saved CONFIG_USERNAME: "));
    Serial.println(CONFIG_USERNAME);
    Serial.print(F("Saved CONFIG_PASSWORD: "));
    Serial.println(CONFIG_PASSWORD);

    Serial.print(F("Saved MODEL: "));
    Serial.println(MODEL);

    Serial.print(F("Saved HOMEKIT_SUPPORT: "));
    Serial.println(HOMEKIT_SUPPORT);

    Serial.print(F("Saved PMSENSORVERSION: "));
    Serial.println(PMSENSORVERSION);

    Serial.print(F("Saved SOFTWAREVERSION: "));
    Serial.println(SOFTWAREVERSION);

    Serial.println(F("\n"));
*/
  }


#ifdef ARDUINO_ARCH_ESP8266
  const short capacity = 6144;
  StaticJsonDocument<capacity> jsonBuffer;
  // DynamicJsonDocument jsonBuffer(ESP.getMaxFreeBlockSize());
#elif defined ARDUINO_ARCH_ESP32
  // const short capacity = 4608;
  // const short capacity = 5620;
  // StaticJsonDocument<capacity> jsonBuffer;
  DynamicJsonDocument jsonBuffer(ESP.getMaxAllocHeap());  
#endif


  JsonObject json = jsonBuffer.to<JsonObject>();
  json["DEVICENAME_AUTO"] = bool(DEVICENAME_AUTO);
  json["DEVICENAME"] = String(DEVICENAME);
  json["LANGUAGE"] = String(LANGUAGE);
  json["SELECTED_LANGUAGE"] = int(SELECTED_LANGUAGE);

  json["THP_MODEL"] = String(THP_MODEL);
  json["DUST_MODEL"] = String(DUST_MODEL);
  json["DISPLAY_PM1"] = bool(DISPLAY_PM1);
  json["FREQUENTMEASUREMENT"] = bool(FREQUENTMEASUREMENT);

  json["SECOND_THP"] = bool(SECOND_THP);
  json["SECOND_THP_MODEL"] = String(SECOND_THP_MODEL);

  json["FIRST_THP_SDA"] = int(FIRST_THP_SDA);
  json["FIRST_THP_SCL"] = int(FIRST_THP_SCL);

  json["SECOND_THP_SDA"] = int(SECOND_THP_SDA);
  json["SECOND_THP_SCL"] = int(SECOND_THP_SCL);

  json["DUST_TX"] = int(DUST_TX);
  json["DUST_RX"] = int(DUST_RX);

  json["CONFIG_FIRST_THP_SDA"] = String(CONFIG_FIRST_THP_SDA);
  json["CONFIG_FIRST_THP_SCL"] = String(CONFIG_FIRST_THP_SCL);

  json["CONFIG_SECOND_THP_SDA"] = String(CONFIG_SECOND_THP_SDA);
  json["CONFIG_SECOND_THP_SCL"] = String(CONFIG_SECOND_THP_SCL);

  json["CONFIG_DUST_TX"] = String(CONFIG_DUST_TX);
  json["CONFIG_DUST_RX"] = String(CONFIG_DUST_RX);

  json["DUST_TIME"] = int(DUST_TIME);
  json["NUMBEROFMEASUREMENTS"] = int(NUMBEROFMEASUREMENTS);

  json["LUFTDATEN_ON"] = bool(LUFTDATEN_ON);
  json["LUFTDATEN_GRAPH_ON"] = bool(LUFTDATEN_GRAPH_ON);
  
  json["SMOGLIST_ON"] = bool(SMOGLIST_ON);

  json["AIRMONITOR_ON"] = bool(AIRMONITOR_ON);
  json["AIRMONITOR_GRAPH_ON"] = bool(AIRMONITOR_GRAPH_ON);
  json["AIRMONITOR_API_KEY"] = String(AIRMONITOR_API_KEY);

  json["LATITUDE"] = String(LATITUDE);
  json["LONGITUDE"] = String(LONGITUDE);
  json["EMAIL"] = String(EMAIL);
  json["MYALTITUDE"] = int(MYALTITUDE);

  json["THINGSPEAK_ON"] = bool(THINGSPEAK_ON);
  json["THINGSPEAK_GRAPH_ON"] = bool(THINGSPEAK_GRAPH_ON);
  json["THINGSPEAK_API_KEY"] = String(THINGSPEAK_API_KEY);
  json["THINGSPEAK_CHANNEL_ID"] = int(THINGSPEAK_CHANNEL_ID);
  json["THINGSPEAK_READ_API_KEY"] = String(THINGSPEAK_READ_API_KEY);

  json["INFLUXDB_ON"] = bool(INFLUXDB_ON);
  json["INFLUXDB_VERSION"] = String(INFLUXDB_VERSION);
  json["INFLUXDB_HOST"] = String(INFLUXDB_HOST);
  json["INFLUXDB_PORT"] = uint16_t(INFLUXDB_PORT);
  json["INFLUXDB_DATABASE"] = String(INFLUXDB_DATABASE);
  json["INFLUXDB_USER"] = String(INFLUXDB_USER);
  json["INFLUXDB_PASSWORD"] = String(INFLUXDB_PASSWORD);
  json["INFLUXDB_ORG"] = String(INFLUXDB_ORG);
  json["INFLUXDB_BUCKET"] = String(INFLUXDB_BUCKET);
  json["INFLUXDB_TOKEN"] = String(INFLUXDB_TOKEN);

  json["MQTT_ON"] = bool(MQTT_ON);
  json["MQTT_HOST"] = String(MQTT_HOST);
  json["MQTT_PORT"] = uint16_t(MQTT_PORT);
  json["MQTT_USER"] = String(MQTT_USER);
  json["MQTT_PASSWORD"] = String(MQTT_PASSWORD);

  json["MQTT_IP_IN_TOPIC"] = bool(MQTT_IP_IN_TOPIC);
  json["MQTT_DEVICENAME_IN_TOPIC"] = bool(MQTT_DEVICENAME_IN_TOPIC);
  json["MQTT_SLASH_AT_THE_BEGINNING"] = bool(MQTT_SLASH_AT_THE_BEGINNING);
  json["MQTT_SLASH_AT_THE_END"] = bool(MQTT_SLASH_AT_THE_END);

  json["MQTT_TOPIC_TEMP"] = String(MQTT_TOPIC_TEMP);
  json["MQTT_TOPIC_HUMI"] = String(MQTT_TOPIC_HUMI);
  json["MQTT_TOPIC_PRESS"] = String(MQTT_TOPIC_PRESS);
  json["MQTT_TOPIC_PM1"] = String(MQTT_TOPIC_PM1);
  json["MQTT_TOPIC_PM25"] = String(MQTT_TOPIC_PM25);
  json["MQTT_TOPIC_PM10"] = String(MQTT_TOPIC_PM10);
  json["MQTT_TOPIC_AIRQUALITY"] = String(MQTT_TOPIC_AIRQUALITY);

  json["AQI_ECO_ON"] = bool(AQI_ECO_ON);
  json["AQI_ECO_HOST"] = String(AQI_ECO_HOST);
  json["AQI_ECO_PATH"] = String(AQI_ECO_PATH);

  json["SENDING_FREQUENCY"] = int(SENDING_FREQUENCY);
  json["SENDING_DB_FREQUENCY"] = int(SENDING_DB_FREQUENCY);
  json["DEEPSLEEP_ON"] = bool(DEEPSLEEP_ON);

  json["DEBUG"] = bool(DEBUG);
  json["AUTOUPDATE_ON"] = bool(AUTOUPDATE_ON);

  json["CONFIG_AUTH"] = bool(CONFIG_AUTH);
  json["CONFIG_USERNAME"] = String(CONFIG_USERNAME);
  json["CONFIG_PASSWORD"] = String(CONFIG_PASSWORD);

  json["MODEL"] = String(MODEL);

  json["HOMEKIT_SUPPORT"] = bool(HOMEKIT_SUPPORT);

#ifdef ARDUINO_ARCH_ESP8266
  File configFile = SPIFFS.open("/config.json", "w");
#elif defined ARDUINO_ARCH_ESP32
  File configFile = SPIFFS.open("/config.json", FILE_WRITE);
#endif

  if (!configFile) {
#ifdef ARDUINO_ARCH_ESP8266
    Serial.println(F("Failed to open config file for writing"));
#elif defined ARDUINO_ARCH_ESP32
    Serial.println(("Failed to open config file for writing"));
#endif
    return false;
  }

  serializeJson(json, configFile);
  if (DEBUG) {
#ifdef ARDUINO_ARCH_ESP8266
    Serial.println(F("config saved"));
#elif defined ARDUINO_ARCH_ESP32
    Serial.println(("config saved"));
#endif
  }
  return true;
}

void fs_setup() {
#ifdef ARDUINO_ARCH_ESP32
  delay(10);
#endif

#ifdef ARDUINO_ARCH_ESP8266
  Serial.println(F("Mounting FS..."));
  if (!SPIFFS.begin()) {
    Serial.println(F("Failed to mount file system"));
    return;
  }
#elif defined ARDUINO_ARCH_ESP32
  Serial.println(("Mounting FS..."));
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println(("SPIFFS Mount Failed"));
    return;
  }
#endif

#ifdef ARDUINO_ARCH_ESP8266
  if (!loadConfig()) {
    Serial.println(F("Failed to load config"));
    Serial.println(F("Saving the current config..."));
    saveConfig();
  } else {
    Serial.println(F("Config loaded"));
  }
#elif defined ARDUINO_ARCH_ESP32
  if (!loadConfig()) {
    Serial.println(("Failed to load config"));
    Serial.println(("Saving the current config..."));
    saveConfig();
  } else {
    Serial.println(("Config loaded"));
  }
#endif


}

void deleteConfig() {
  SPIFFS.remove("/config.json");
}
