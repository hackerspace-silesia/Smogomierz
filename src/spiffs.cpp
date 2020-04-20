#include <ArduinoJson.h>
#include <FS.h>
#ifdef ARDUINO_ARCH_ESP32
#include <SPIFFS.h>
#endif

#include "config.h"
#define FORMAT_SPIFFS_IF_FAILED true

//const int capacity = JSON_OBJECT_SIZE(70);
#ifdef ARDUINO_ARCH_ESP8266
	const int capacity = 6144;
#elif defined ARDUINO_ARCH_ESP32
	const int capacity = 4096;
#endif
		
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
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 2048) {
      Serial.println("Config file size is too large");
      return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonDocument<capacity> jsonBuffer;
  deserializeJson(jsonBuffer, buf.get());
  JsonObject json = jsonBuffer.as<JsonObject>();

  if (json.isNull()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  // REMEMBER TO ADD/EDIT KEYS IN config.h AND webserver.h!!

  DEVICENAME_AUTO = json["DEVICENAME_AUTO"];
  _safeCpy(DEVICENAME, json["DEVICENAME"], "smogomierz", 32);
  _safeCpy(LANGUAGE, json["LANGUAGE"], "english", 32);
  SELECTED_LANGUAGE = json["SELECTED_LANGUAGE"];

  _safeCpy(THP_MODEL, json["THP_MODEL"], "Non", 32);
  _safeCpy(DUST_MODEL, json["DUST_MODEL"], "Non", 32);
  DISPLAY_PM1 = json["DISPLAY_PM1"];
  FREQUENTMEASUREMENT = json["FREQUENTMEASUREMENT"];
  
  DUST_TIME = json["DUST_TIME"];
  NUMBEROFMEASUREMENTS = json["NUMBEROFMEASUREMENTS"];

  LUFTDATEN_ON = json["LUFTDATEN_ON"];
  SMOGLIST_ON = json["SMOGLIST_ON"];

  AIRMONITOR_ON = json["AIRMONITOR_ON"];
  AIRMONITOR_GRAPH_ON = json["AIRMONITOR_GRAPH_ON"];
  _safeCpy(LATITUDE, json["LATITUDE"], "50.263911", 16);
  _safeCpy(LONGITUDE, json["LONGITUDE"], "18.995711", 16);
  MYALTITUDE = json["MYALTITUDE"];

  THINGSPEAK_ON = json["THINGSPEAK_ON"];
  THINGSPEAK_GRAPH_ON = json["THINGSPEAK_GRAPH_ON"];
  _safeCpy(THINGSPEAK_API_KEY, json["THINGSPEAK_API_KEY"], "WRITE_API_KEY", 32);
  THINGSPEAK_CHANNEL_ID = json["THINGSPEAK_CHANNEL_ID"];
  _safeCpy(THINGSPEAK_READ_API_KEY, json["THINGSPEAK_READ_API_KEY"], "READ_API_KEY", 32);

  INFLUXDB_ON = json["INFLUXDB_ON"];
  _safeCpy(INFLUXDB_VERSION, json["INFLUXDB_VERSION"], "1", 16);
  _safeCpy(INFLUXDB_HOST, json["INFLUXDB_HOST"], "host", 128);
  INFLUXDB_PORT = json["INFLUXDB_PORT"];  
  _safeCpy(INFLUXDB_DATABASE, json["INFLUXDB_DATABASE"], "mydb", 64);
  _safeCpy(INFLUXDB_USER, json["INFLUXDB_USER"], "user", 64);
  _safeCpy(INFLUXDB_PASSWORD, json["INFLUXDB_PASSWORD"], "password", 64);
  _safeCpy(INFLUXDB_ORG, json["INFLUXDB_ORG"], "myOrg", 64);
  _safeCpy(INFLUXDB_BUCKET, json["INFLUXDB_BUCKET"], "myBucket", 64);
  _safeCpy(INFLUXDB_TOKEN, json["INFLUXDB_TOKEN"], "myToken", 64);
  
  MQTT_ON = json["MQTT_ON"];
  _safeCpy(MQTT_HOST, json["MQTT_HOST"], "host", 128);
  MQTT_PORT = json["MQTT_PORT"];  
  _safeCpy(MQTT_USER, json["MQTT_USER"], "user", 64);
  _safeCpy(MQTT_PASSWORD, json["MQTT_PASSWORD"], "password", 64);
  
  MQTT_IP_IN_TOPIC = json["MQTT_IP_IN_TOPIC"];
  MQTT_DEVICENAME_IN_TOPIC = json["MQTT_DEVICENAME_IN_TOPIC"];
  MQTT_SLASH_AT_THE_BEGINNING = json["MQTT_SLASH_AT_THE_BEGINNING"];
  
  _safeCpy(MQTT_TOPIC_TEMP, json["MQTT_TOPIC_TEMP"], "MQTT_TOPIC_TEMP", 128);
  _safeCpy(MQTT_TOPIC_HUMI, json["MQTT_TOPIC_HUMI"], "MQTT_TOPIC_HUMI", 128);
  _safeCpy(MQTT_TOPIC_PRESS, json["MQTT_TOPIC_PRESS"], "MQTT_TOPIC_PRESS", 128);
  _safeCpy(MQTT_TOPIC_PM1, json["MQTT_TOPIC_PM1"], "MQTT_TOPIC_PM1", 128);
  _safeCpy(MQTT_TOPIC_PM25, json["MQTT_TOPIC_PM25"], "MQTT_TOPIC_PM25", 128);
  _safeCpy(MQTT_TOPIC_PM10, json["MQTT_TOPIC_PM10"], "MQTT_TOPIC_PM10", 128);
  _safeCpy(MQTT_TOPIC_AIRQUALITY, json["MQTT_TOPIC_AIRQUALITY"], "MQTT_TOPIC_AIRQUALITY", 128);
  
  AQI_ECO_ON = json["AQI_ECO_ON"];
  _safeCpy(AQI_ECO_HOST, json["AQI_ECO_HOST"], "host", 128);
  _safeCpy(AQI_ECO_PATH, json["AQI_ECO_PATH"], "path", 64);
  
  SENDING_FREQUENCY = json["SENDING_FREQUENCY"];
  SENDING_DB_FREQUENCY = json["SENDING_DB_FREQUENCY"];
  DEEPSLEEP_ON = json["DEEPSLEEP_ON"];

  DEBUG = json["DEBUG"];
  AUTOUPDATE_ON = json["AUTOUPDATE_ON"];
  
  CONFIG_AUTH = json["CONFIG_AUTH"];
  _safeCpy(CONFIG_USERNAME, json["CONFIG_USERNAME"], "admin", 256);
  _safeCpy(CONFIG_PASSWORD, json["CONFIG_PASSWORD"], "password", 256);
  
  _safeCpy(MODEL, json["MODEL"], "black", 32);

  // Real world application would store these values in some variables for
  // later use.
  if (DEBUG) {
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
	
    Serial.print("Loaded DUST_TIME: ");
    Serial.println(DUST_TIME);
    Serial.print("Loaded NUMBEROFMEASUREMENTS: ");
    Serial.println(NUMBEROFMEASUREMENTS);
	
	Serial.print("Loaded LUFTDATEN_ON: ");
	Serial.println(LUFTDATEN_ON);
	Serial.print("Loaded SMOGLIST_ON: ");
	Serial.println(SMOGLIST_ON);
	
    Serial.print("Loaded AIRMONITOR_ON: ");
    Serial.println(AIRMONITOR_ON);
    Serial.print("Loaded AIRMONITOR_GRAPH_ON: ");
    Serial.println(AIRMONITOR_GRAPH_ON);
    Serial.print("Loaded LATITUDE: ");
    Serial.println(LATITUDE);
    Serial.print("Loaded LONGITUDE: ");
    Serial.println(LONGITUDE);
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
	
    Serial.print("Loaded PMSENSORVERSION: ");
    Serial.println(PMSENSORVERSION);
	
    Serial.print("Loaded SOFTWAREVERSION: ");
    Serial.println(SOFTWAREVERSION);
    
	Serial.println("\n");
  }
  return true;
}

bool saveConfig() {
  StaticJsonDocument<capacity> jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();
  json["DEVICENAME_AUTO"] = bool(DEVICENAME_AUTO);
  json["DEVICENAME"] = String(DEVICENAME);
  json["LANGUAGE"] = String(LANGUAGE);
  json["SELECTED_LANGUAGE"] = int(SELECTED_LANGUAGE);

  json["THP_MODEL"] = String(THP_MODEL);
  json["DUST_MODEL"] = String(DUST_MODEL);
  json["DISPLAY_PM1"] = bool(DISPLAY_PM1);
  json["FREQUENTMEASUREMENT"] = bool(FREQUENTMEASUREMENT);
  
  json["DUST_TIME"] = int(DUST_TIME);
  json["NUMBEROFMEASUREMENTS"] = int(NUMBEROFMEASUREMENTS);

  json["LUFTDATEN_ON"] = bool(LUFTDATEN_ON);
  json["SMOGLIST_ON"] = bool(SMOGLIST_ON);

  json["AIRMONITOR_ON"] = bool(AIRMONITOR_ON);
  json["AIRMONITOR_GRAPH_ON"] = bool(AIRMONITOR_GRAPH_ON);
  json["LATITUDE"] = String(LATITUDE);
  json["LONGITUDE"] = String(LONGITUDE);
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
  
#ifdef ARDUINO_ARCH_ESP8266
  File configFile = SPIFFS.open("/config.json", "w");
#elif defined ARDUINO_ARCH_ESP32
  File configFile = SPIFFS.open("/config.json", FILE_WRITE);
#endif
  
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  serializeJson(json, configFile);
  if (DEBUG) {
    Serial.println("config saved");
  }
  return true;
}

void fs_setup() {
#ifdef ARDUINO_ARCH_ESP32
	delay(10);
#endif	

  Serial.println("Mounting FS...");

#ifdef ARDUINO_ARCH_ESP8266
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
#elif defined ARDUINO_ARCH_ESP32
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
#endif

  if (!loadConfig()) {
    Serial.println("Failed to load config");
    Serial.println("Saving the current config...");
    saveConfig();
  } else {
    Serial.println("Config loaded");
  }
  
}

void deleteConfig() {
  SPIFFS.remove("/config.json");
}