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

  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  // REMEMBER TO ADD/EDIT KEYS IN config.h AND webserver.h!!

  DEVICENAME_AUTO = json["DEVICENAME_AUTO"];
  _safeCpy(DEVICENAME, json["DEVICENAME"], "smogomierz");
  DISPLAY_PM1 = json["DISPLAY_PM1"];
  AIRMONITOR_ON = json["AIRMONITOR_ON"];
  AIRMONITOR_GRAPH_ON = json["AIRMONITOR_GRAPH_ON"];
  LATITUDE = json["LATITUDE"];
  LONGITUDE = json["LONGITUDE"];
  MYALTITUDE = json["MYALTITUDE"];
  
  THINGSPEAK_ON = json["THINGSPEAK_ON"];
  THINGSPEAK_GRAPH_ON = json["THINGSPEAK_GRAPH_ON"];
  _safeCpy(THINGSPEAK_API_KEY, json["THINGSPEAK_API_KEY"]);
  THINGSPEAK_CHANNEL_ID = json["THINGSPEAK_CHANNEL_ID"];
  
  INFLUXDB_ON = json["INFLUXDB_ON"];
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
  if(DEBUG){
  Serial.print("Loaded DEVICENAME_AUTO: ");
  Serial.println(DEVICENAME_AUTO);
  Serial.print("Loaded DISPLAY_PM1: ");
  Serial.println(DISPLAY_PM1);
  Serial.print("Loaded DEVICENAME: ");
  Serial.println(DEVICENAME);
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
  Serial.print("Loaded DATABASE: ");
  Serial.println(DATABASE);
  Serial.print("Loaded DB_USER: ");
  Serial.println(DB_USER);
  Serial.print("Loaded DB_PASSWORD: ");
  Serial.println(DB_PASSWORD);
  
  Serial.print("Loaded DEBUG: ");
  Serial.println(DEBUG);
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
  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["DEVICENAME_AUTO"] = DEVICENAME_AUTO;
  json["DEVICENAME"] = DEVICENAME;
  json["DISPLAY_PM1"] = DISPLAY_PM1;
  
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

  json.printTo(configFile);
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
