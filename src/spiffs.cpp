#include "ArduinoJson.h"
#include "FS.h"

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

  StaticJsonDocument<1000> jsonBuffer;
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

  AIRMONITOR_ON = json["AIRMONITOR_ON"];
  AIRMONITOR_TIME = json["AIRMONITOR_TIME"];
  AIRMONITOR_GRAPH_ON = json["AIRMONITOR_GRAPH_ON"];
  LATITUDE = json["LATITUDE"];
  LONGITUDE = json["LONGITUDE"];
  MYALTITUDE = json["MYALTITUDE"];

  THINGSPEAK_ON = json["THINGSPEAK_ON"];
  THINGSPEAK_TIME = json["THINGSPEAK_TIME"];
  THINGSPEAK_GRAPH_ON = json["THINGSPEAK_GRAPH_ON"];
  _safeCpy(THINGSPEAK_API_KEY, json["THINGSPEAK_API_KEY"]);
  THINGSPEAK_CHANNEL_ID = json["THINGSPEAK_CHANNEL_ID"];

  INFLUXDB_ON = json["INFLUXDB_ON"];
  INFLUXDB_TIME = json["INFLUXDB_TIME"];
  _safeCpy(INFLUXDB_HOST, json["INFLUXDB_HOST"], "host");
  INFLUXDB_PORT = json["INFLUXDB_PORT"];
  _safeCpy(DATABASE, json["DATABASE"], "mydb");
  _safeCpy(DB_USER, json["DB_USER"], "user");
  _safeCpy(DB_PASSWORD, json["DB_PASSWORD"], "password");

  _safeCpy(MODEL, json["MODEL"], "black");
  calib1 = json["calib1"];
  calib2 = json["calib2"];

  DEBUG = json["DEBUG"];

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

    Serial.print("Loaded AIRMONITOR_ON: ");
    Serial.println(AIRMONITOR_ON);
    Serial.print("Loaded AIRMONITOR_TIME: ");
    Serial.println(AIRMONITOR_TIME);
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
    Serial.print("Loaded THINGSPEAK_TIME: ");
    Serial.println(THINGSPEAK_TIME);
    Serial.print("Loaded THINGSPEAK_GRAPH_ON: ");
    Serial.println(THINGSPEAK_GRAPH_ON);
    Serial.print("Loaded THINGSPEAK_API_KEY: ");
    Serial.println(THINGSPEAK_API_KEY);
    Serial.print("Loaded THINGSPEAK_CHANNEL_ID: ");
    Serial.println(THINGSPEAK_CHANNEL_ID);

    Serial.print("Loaded INFLUXDB_ON: ");
    Serial.println(INFLUXDB_ON);
    Serial.print("Loaded INFLUXDB_TIME: ");
    Serial.println(INFLUXDB_TIME);
    Serial.print("Loaded INFLUXDB_HOST: ");
    Serial.println(INFLUXDB_HOST);
    Serial.print("Loaded INFLUXDB_PORT: ");
    Serial.println(INFLUXDB_PORT);
    Serial.print("Loaded DATABASE: ");
    Serial.println(DATABASE);
    Serial.print("Loaded DB_USER: ");
    Serial.println(DB_USER);
    Serial.print("Loaded DB_PASSWORD: ");
    Serial.println(DB_PASSWORD);

    Serial.print("Loaded DEBUG: ");
    Serial.println(DEBUG);
    Serial.print("Loaded MODEL: ");
    Serial.println(MODEL);
    Serial.print("Loaded calib1: ");
    Serial.println(calib1);
    Serial.print("Loaded calib2: ");
    Serial.println(calib2);
    Serial.print("Loaded SOFTWAREVERSION: ");
    Serial.println(SOFTWAREVERSION);
    Serial.println("\n");
  }
  return true;
}

bool saveConfig() {
  StaticJsonDocument<1000> jsonBuffer;
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

  json["AIRMONITOR_ON"] = AIRMONITOR_ON;
  json["AIRMONITOR_TIME"] = AIRMONITOR_TIME;
  json["AIRMONITOR_GRAPH_ON"] = AIRMONITOR_GRAPH_ON;
  json["LATITUDE"] = LATITUDE;
  json["LONGITUDE"] = LONGITUDE;
  json["MYALTITUDE"] = MYALTITUDE;

  json["THINGSPEAK_ON"] = THINGSPEAK_ON;
  json["THINGSPEAK_TIME"] = THINGSPEAK_TIME;
  json["THINGSPEAK_GRAPH_ON"] = THINGSPEAK_GRAPH_ON;
  json["THINGSPEAK_API_KEY"] = THINGSPEAK_API_KEY;
  json["THINGSPEAK_CHANNEL_ID"] = THINGSPEAK_CHANNEL_ID;

  json["INFLUXDB_ON"] = INFLUXDB_ON;
  json["INFLUXDB_TIME"] = INFLUXDB_TIME;
  json["INFLUXDB_HOST"] = INFLUXDB_HOST;
  json["INFLUXDB_PORT"] = INFLUXDB_PORT;
  json["DATABASE"] = DATABASE;
  json["DB_USER"] = DB_USER;
  json["DB_PASSWORD"] = DB_PASSWORD;

  json["DEBUG"] = DEBUG;
  json["MODEL"] = MODEL;
  json["calib1"] = calib1;
  json["calib2"] = calib2;

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
