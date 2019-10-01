#include <ArduinoJson.h>
#include <FS.h>
#ifdef ARDUINO_ARCH_ESP32
#include <SPIFFS.h>
#endif

#include "config.h"

void _safeCpy(char* dest, const JsonVariant &obj, const char* dflt = "") {
  const char* val = obj.as<const char*>();
  if (val) {
    strncpy(dest, val, 255);
  } else {
    strncpy(dest, dflt, 255);
  }
}


bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonDocument<2000> jsonBuffer;
  deserializeJson(jsonBuffer, buf.get());
  JsonObject json = jsonBuffer.as<JsonObject>();

  if (json.isNull()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  // REMEMBER TO ADD/EDIT KEYS IN config.h AND webserver.h!!

  DEVICENAME_AUTO = json["DEVICENAME_AUTO"];
  _safeCpy(DEVICENAME, json["DEVICENAME"], "smogomierz");
  _safeCpy(LANGUAGE, json["LANGUAGE"], "english");
  SELECTED_LANGUAGE = json["SELECTED_LANGUAGE"];

  _safeCpy(THP_MODEL, json["THP_MODEL"], "Non");
  _safeCpy(DUST_MODEL, json["DUST_MODEL"], "Non");
  DISPLAY_PM1 = json["DISPLAY_PM1"];
  FREQUENTMEASUREMENT = json["FREQUENTMEASUREMENT"];
  
  DUST_TIME = json["DUST_TIME"];
  NUMBEROFMEASUREMENTS = json["NUMBEROFMEASUREMENTS"];

  LUFTDATEN_ON = json["LUFTDATEN_ON"];
  SMOGLIST_ON = json["SMOGLIST_ON"];

  AIRMONITOR_ON = json["AIRMONITOR_ON"];
  AIRMONITOR_GRAPH_ON = json["AIRMONITOR_GRAPH_ON"];
  _safeCpy(LATITUDE, json["LATITUDE"], "50.263911");
  _safeCpy(LONGITUDE, json["LONGITUDE"], "18.995711");
  MYALTITUDE = json["MYALTITUDE"];

  THINGSPEAK_ON = json["THINGSPEAK_ON"];
  THINGSPEAK_GRAPH_ON = json["THINGSPEAK_GRAPH_ON"];
  _safeCpy(THINGSPEAK_API_KEY, json["THINGSPEAK_API_KEY"]);
  THINGSPEAK_CHANNEL_ID = json["THINGSPEAK_CHANNEL_ID"];

  INFLUXDB_ON = json["INFLUXDB_ON"];
  _safeCpy(INFLUXDB_HOST, json["INFLUXDB_HOST"], "host");
  INFLUXDB_PORT = json["INFLUXDB_PORT"];  
  _safeCpy(INFLUXDB_DATABASE, json["INFLUXDB_DATABASE"], "mydb");
  _safeCpy(DB_USER, json["DB_USER"], "user");
  _safeCpy(DB_PASSWORD, json["DB_PASSWORD"], "password");
  
  MQTT_ON = json["MQTT_ON"];
  _safeCpy(MQTT_HOST, json["MQTT_HOST"], "host");
  MQTT_PORT = json["MQTT_PORT"];  
  _safeCpy(MQTT_USER, json["MQTT_USER"], "user");
  _safeCpy(MQTT_PASSWORD, json["MQTT_PASSWORD"], "password");
  
  SENDING_FREQUENCY = json["SENDING_FREQUENCY"];
  SENDING_DB_FREQUENCY = json["SENDING_DB_FREQUENCY"];
  DEEPSLEEP_ON = json["DEEPSLEEP_ON"];

  DEBUG = json["DEBUG"];
  AUTOUPDATE_ON = json["AUTOUPDATE_ON"];
  
  CONFIG_AUTH = json["CONFIG_AUTH"];
  _safeCpy(CONFIG_USERNAME, json["CONFIG_USERNAME"], "admin");
  _safeCpy(CONFIG_PASSWORD, json["CONFIG_PASSWORD"], "password");
  
  _safeCpy(MODEL, json["MODEL"], "black");

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

    Serial.print("Loaded INFLUXDB_ON: ");
    Serial.println(INFLUXDB_ON);
    Serial.print("Loaded INFLUXDB_HOST: ");
    Serial.println(INFLUXDB_HOST);
    Serial.print("Loaded INFLUXDB_PORT: ");
    Serial.println(INFLUXDB_PORT);
    Serial.print("Loaded INFLUXDB_DATABASE: ");
    Serial.println(INFLUXDB_DATABASE);
    Serial.print("Loaded DB_USER: ");
    Serial.println(DB_USER);
    Serial.print("Loaded DB_PASSWORD: ");
    Serial.println(DB_PASSWORD);
	
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
	
    Serial.print("Loaded SOFTWAREVERSION: ");
    Serial.println(SOFTWAREVERSION);
    
	Serial.println("\n");
  }
  return true;
}

bool saveConfig() {
  StaticJsonDocument<2000> jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();
  json["DEVICENAME_AUTO"] = DEVICENAME_AUTO;
  json["DEVICENAME"] = DEVICENAME;
  json["LANGUAGE"] = LANGUAGE;
  json["SELECTED_LANGUAGE"] = SELECTED_LANGUAGE;

  json["THP_MODEL"] = THP_MODEL;
  json["DUST_MODEL"] = DUST_MODEL;
  json["DISPLAY_PM1"] = DISPLAY_PM1;
  json["FREQUENTMEASUREMENT"] = FREQUENTMEASUREMENT;
  
  json["DUST_TIME"] = DUST_TIME;
  json["NUMBEROFMEASUREMENTS"] = NUMBEROFMEASUREMENTS;

  json["LUFTDATEN_ON"] = LUFTDATEN_ON;
  json["SMOGLIST_ON"] = SMOGLIST_ON;

  json["AIRMONITOR_ON"] = AIRMONITOR_ON;
  json["AIRMONITOR_GRAPH_ON"] = AIRMONITOR_GRAPH_ON;
  json["LATITUDE"] = LATITUDE;
  json["LONGITUDE"] = LONGITUDE;
  json["MYALTITUDE"] = MYALTITUDE;

  json["THINGSPEAK_ON"] = THINGSPEAK_ON;
  json["THINGSPEAK_GRAPH_ON"] = THINGSPEAK_GRAPH_ON;
  json["THINGSPEAK_API_KEY"] = THINGSPEAK_API_KEY;
  json["THINGSPEAK_CHANNEL_ID"] = THINGSPEAK_CHANNEL_ID;

  json["INFLUXDB_ON"] = INFLUXDB_ON;
  json["INFLUXDB_HOST"] = INFLUXDB_HOST;
  json["INFLUXDB_PORT"] = INFLUXDB_PORT;
  json["INFLUXDB_DATABASE"] = INFLUXDB_DATABASE;
  json["DB_USER"] = DB_USER;
  json["DB_PASSWORD"] = DB_PASSWORD;
  json["DB_PASSWORD"] = String(DB_PASSWORD);
  
  json["MQTT_ON"] = MQTT_ON;
  json["MQTT_HOST"] = MQTT_HOST;
  json["MQTT_PORT"] = MQTT_PORT;
  json["MQTT_USER"] = MQTT_USER;
  json["MQTT_PASSWORD"] = MQTT_PASSWORD;
  json["MQTT_PASSWORD"] = String(MQTT_PASSWORD);

  json["SENDING_FREQUENCY"] = SENDING_FREQUENCY;
  json["SENDING_DB_FREQUENCY"] = SENDING_DB_FREQUENCY;
  
  json["DEEPSLEEP_ON"] = DEEPSLEEP_ON;

  json["DEBUG"] = DEBUG;
  json["AUTOUPDATE_ON"] = AUTOUPDATE_ON;
  
  json["CONFIG_AUTH"] = CONFIG_AUTH;
  json["CONFIG_USERNAME"] = CONFIG_USERNAME;
  json["CONFIG_PASSWORD"] = CONFIG_PASSWORD;
  json["CONFIG_PASSWORD"] = String(CONFIG_PASSWORD);
  
  json["MODEL"] = MODEL;
  
  File configFile = SPIFFS.open("/config.json", "w");
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
  delay(10);
  Serial.println("Mounting FS...");

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

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
