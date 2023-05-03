#include <ArduinoJson.h>
#include <FS.h>
#ifdef ARDUINO_ARCH_ESP32
#include <SPIFFS.h>
#endif

#include "../config.h"
#define FORMAT_SPIFFS_IF_FAILED true

// https://arduinojson.org/v6/faq/automatically-serialize-an-object/
// https://arduinojson.org/v6/example/config/#source-code

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

  if (size > 4096) {
    Serial.println(F("Config file size is too large"));
    Serial.println(F("configFile.size(): ") + String(size)); // 2182
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

  deviceSettings.autoName = json["device_autoName"];
  _safeCpy(deviceSettings.name, json["device_name"], "smogly", 32);
  deviceSettings.displayPM1 = json["device_displayPM1"];
  _safeCpy(deviceSettings.language, json["device_language"], "english", 12);
  deviceSettings.selectedLanguage = json["device_selectedLanguage"];
  deviceSettings.sendingServicesFrequency = json["device_sendingServicesFrequency"];
  deviceSettings.sendingDBFrequency = json["device_sendingDBFrequency"];
  deviceSettings.deepSleep = json["device_deepSleep"];
  deviceSettings.debug = json["device_debug"];
  deviceSettings.autoUpdate = json["device_autoUpdate"];
  deviceSettings.autoCalibration = json["device_autoCalibration"];
  deviceSettings.calib1 = json["device_calib1"];
  deviceSettings.calib2 = json["device_calib2"];
  _safeCpy(deviceSettings.latitude, json["device_latitude"], "50.263911", 16);
  _safeCpy(deviceSettings.longitude, json["device_longitude"], "18.995711", 16);
  deviceSettings.altitude = json["device_altitude"];
  _safeCpy(deviceSettings.email, json["device_email"], "email@mail.pl", 64);


  _safeCpy(sensorsSettings.thpModel, json["sensors_thpModel"], "Non", 12);
  sensorsSettings.secondThp = json["sensors_secondThp"];
  _safeCpy(sensorsSettings.secondThpModel, json["sensors_secondThpModel"], "Non", 12);
  _safeCpy(sensorsSettings.dustModel, json["sensors_dustModel"], "Non", 12);
  sensorsSettings.continuousMeasurement = json["sensors_continuousMeasurement"];
  sensorsSettings.dustTime = json["sensors_dustTime"];
  sensorsSettings.numerOfMeasurements = json["sensors_numerOfMeasurements"];

  _safeCpy(firstThpSettings.sda, json["firstThp_sda"], "D3", 4);
  _safeCpy(firstThpSettings.scl, json["firstThp_scl"], "D4", 4);
  firstThpSettings.address_sda = json["firstThp_address_sda"];
  firstThpSettings.address_scl = json["firstThp_address_scl"];

  _safeCpy(secondThpSettings.sda, json["secondThp_sda"], "D5", 4);
  _safeCpy(secondThpSettings.scl, json["secondThp_scl"], "D6", 4);
  secondThpSettings.address_sda = json["secondThp_address_sda"];
  secondThpSettings.address_scl = json["secondThp_address_scl"];

  _safeCpy(dustSettings.sda, json["firtsDust_sda"], "D1", 4);
  _safeCpy(dustSettings.scl, json["firtsDust_scl"], "D2", 4);
  dustSettings.address_sda = json["firtsDust_address_sda"];
  dustSettings.address_scl = json["firtsDust_address_scl"];

  smoglistSettings.enabled = json["smoglist_enabled"];

  luftdatenSettings.enabled = json["luftdaten_enabled"];
  luftdatenSettings.graph = json["luftdaten_graph"];
  luftdatenSettings.apiId = json["luftdaten_apiId"];

  airMonitorSettings.enabled = json["airMonitor_enabled"];
  airMonitorSettings.graph = json["airMonitor_graph"];
  _safeCpy(airMonitorSettings.apiKey, json["airMonitor_apiKey"], "", 48);

  thingSpeakSettings.enabled = json["thingSpeak_enabled"];
  thingSpeakSettings.graph = json["thingSpeak_graph"];
  _safeCpy(thingSpeakSettings.apiKey, json["thingSpeak_apiKey"], "WRITE_API_KEY", 32);
  thingSpeakSettings.channelId = json["thingSpeak_channelId"];
  _safeCpy(thingSpeakSettings.readApiKey, json["thingSpeak_readApiKey"], "READ_API_KEY", 32);

  aqiEcoSettings.enabled = json["aqiEco_enabled"];
  _safeCpy(aqiEcoSettings.host, json["aqiEco_host"], "???.aqi.eco", 64);
  _safeCpy(aqiEcoSettings.path, json["aqiEco_path"], "/u/???", 64);

  influxDBSettings.enabled = json["influxDB_enabled"];
  _safeCpy(influxDBSettings.version, json["influxDB_version"], "1", 4);
  _safeCpy(influxDBSettings.host, json["influxDB_host"], "INFLUXDB_ADRESS", 128);
  influxDBSettings.port = json["influxDB_port"];
  _safeCpy(influxDBSettings.database, json["influxDB_database"], "mydb", 32);
  _safeCpy(influxDBSettings.user, json["influxDB_user"], "username", 32);
  _safeCpy(influxDBSettings.password, json["influxDB_password"], "password", 64);
  _safeCpy(influxDBSettings.org, json["influxDB_org"], "myOrg", 32);
  _safeCpy(influxDBSettings.bucket, json["influxDB_bucket"], "myBucket", 32);
  _safeCpy(influxDBSettings.token, json["influxDB_token"], "myToken", 128);

  mqttSettings.enabled = json["mqtt_enabled"];
  _safeCpy(mqttSettings.host, json["mqtt_host"], "MQTT_ADRESS", 128);
  mqttSettings.port = json["mqtt_port"];
  _safeCpy(mqttSettings.user, json["mqtt_user"], "username", 32);
  _safeCpy(mqttSettings.password, json["mqtt_password"], "password", 64);
  mqttSettings.ipInTopic = json["mqtt_ipInTopic"];
  mqttSettings.deviceNameInTopic = json["mqtt_deviceNameInTopic"];
  mqttSettings.slashAtBeginning = json["mqtt_slashAtBeginning"];
  mqttSettings.slashAtEnd = json["mqtt_slashAtEnd"];
  _safeCpy(mqttSettings.tempTopic, json["mqtt_tempTopic"], "sensor/temperature", 64);
  _safeCpy(mqttSettings.humiTopic, json["mqtt_humiTopic"], "sensor/humidity", 64);
  _safeCpy(mqttSettings.pressTopic, json["mqtt_pressTopic"], "sensor/pressure", 64);
  _safeCpy(mqttSettings.pm1Topic, json["mqtt_pm1Topic"], "sensor/PM1", 64);
  _safeCpy(mqttSettings.pm25Topic, json["mqtt_pm25Topic"], "sensor/PM2.5", 64);
  _safeCpy(mqttSettings.pm10Topic, json["mqtt_pm10Topic"], "sensor/PM10", 64);
  _safeCpy(mqttSettings.airQualityTopic, json["mqtt_airQualityTopic"], "airquality", 32);

  homeKitSettings.enabled = json["homeKit_enabled"];
  _safeCpy(homeKitSettings.password, json["homeKit_password"], "111-11-111", 16);

  authSettings.enabled = json["auth_enabled"];
  _safeCpy(authSettings.username, json["auth_username"], "admin", 64);
  _safeCpy(authSettings.password, json["auth_password"], "admin", 128);

  // Real world application would store these values in some variables for
  // later use.
    if (deviceSettings.debug) {
#ifdef ARDUINO_ARCH_ESP8266
      Serial.print(F("Loaded DEVICENAME_AUTO: "));
      Serial.println(deviceSettings.autoName);
      Serial.print(F("Loaded DEVICENAME: "));
      Serial.println(deviceSettings.name);
      Serial.print(F("Loaded LANGUAGE: "));
      Serial.println(deviceSettings.language);
      Serial.print(F("Loaded SELECTED_LANGUAGE: "));
      Serial.println(deviceSettings.selectedLanguage);

      Serial.print(F("Loaded THP_MODEL: "));
      Serial.println(sensorsSettings.thpModel);
      Serial.print(F("Loaded DUST_MODEL: "));
      Serial.println(sensorsSettings.dustModel);
      Serial.print(F("Loaded DISPLAY_PM1: "));
      Serial.println(deviceSettings.displayPM1);
      Serial.print(F("Loaded FREQUENTMEASUREMENT: "));
      Serial.println(sensorsSettings.continuousMeasurement);

      Serial.print(F("Loaded SECOND_THP: "));
      Serial.println(sensorsSettings.secondThp);
      Serial.print(F("Loaded SECOND_THP_MODEL: "));
      Serial.println(sensorsSettings.secondThpModel);

      Serial.print(F("Loaded FIRST_THP_SDA: "));
      Serial.println(firstThpSettings.address_sda);
      Serial.print(F("Loaded FIRST_THP_SCL: "));
      Serial.println(firstThpSettings.address_scl);

      Serial.print(F("Loaded SECOND_THP_SDA: "));
      Serial.println(secondThpSettings.address_sda);
      Serial.print(F("Loaded SECOND_THP_SCL: "));
      Serial.println(secondThpSettings.address_scl);

      Serial.print(F("Loaded DUST_TX: "));
      Serial.println(dustSettings.address_sda);
      Serial.print(F("Loaded DUST_RX: "));
      Serial.println(dustSettings.address_scl);

      Serial.print(F("Loaded CONFIG_FIRST_THP_SDA: "));
      Serial.println(firstThpSettings.sda);
      Serial.print(F("Loaded CONFIG_FIRST_THP_SCL: "));
      Serial.println(firstThpSettings.scl);

      Serial.print(F("Loaded CONFIG_SECOND_THP_SDA: "));
      Serial.println(secondThpSettings.sda);
      Serial.print(F("Loaded CONFIG_SECOND_THP_SCL: "));
      Serial.println(secondThpSettings.scl);

      Serial.print(F("Loaded CONFIG_DUST_TX: "));
      Serial.println(dustSettings.sda);
      Serial.print(F("Loaded CONFIG_DUST_RX: "));
      Serial.println(dustSettings.scl);

      Serial.print(F("Loaded DUST_TIME: "));
      Serial.println(sensorsSettings.dustTime);
      Serial.print(F("Loaded NUMBEROFMEASUREMENTS: "));
      Serial.println(sensorsSettings.numerOfMeasurements);

      Serial.print(F("Loaded LUFTDATEN_ON: "));
      Serial.println(luftdatenSettings.enabled);
      Serial.print(F("Loaded LUFTDATEN_GRAPH_ON: "));
      Serial.println(luftdatenSettings.graph);
      Serial.print(F("Loaded LUFTDATEN_APIID: "));
      Serial.println(luftdatenSettings.apiId);
	  	  
      Serial.print(F("Loaded SMOGLIST_ON: "));
      Serial.println(smoglistSettings.enabled);

      Serial.print(F("Loaded AIRMONITOR_ON: "));
      Serial.println(airMonitorSettings.enabled);
      Serial.print(F("Loaded AIRMONITOR_GRAPH_ON: "));
      Serial.println(airMonitorSettings.graph);
      Serial.print(F("Loaded AIRMONITOR_API_KEY: "));
      Serial.println(airMonitorSettings.apiKey);
      Serial.print(F("Loaded LATITUDE: "));
      Serial.println(deviceSettings.latitude);
      Serial.print(F("Loaded LONGITUDE: "));
      Serial.println(deviceSettings.longitude);
      Serial.print(F("Loaded EMAIL: "));
      Serial.println(deviceSettings.email);
      Serial.print(F("Loaded MYALTITUDE: "));
      Serial.println(deviceSettings.altitude);

      Serial.print(F("Loaded THINGSPEAK_ON: "));
      Serial.println(thingSpeakSettings.enabled);
      Serial.print(F("Loaded THINGSPEAK_GRAPH_ON: "));
      Serial.println(thingSpeakSettings.graph);
      Serial.print(F("Loaded THINGSPEAK_API_KEY: "));
      Serial.println(thingSpeakSettings.apiKey);
      Serial.print(F("Loaded THINGSPEAK_CHANNEL_ID: "));
      Serial.println(thingSpeakSettings.channelId);
      Serial.print(F("Loaded THINGSPEAK_READ_API_KEY: "));
      Serial.println(thingSpeakSettings.readApiKey);

      Serial.print(F("Loaded INFLUXDB_ON: "));
      Serial.println(influxDBSettings.enabled);
      Serial.print(F("Loaded INFLUXDB_VERSION: "));
      Serial.println(influxDBSettings.version);
      Serial.print(F("Loaded INFLUXDB_HOST: "));
      Serial.println(influxDBSettings.host);
      Serial.print(F("Loaded INFLUXDB_PORT: "));
      Serial.println(influxDBSettings.port);
      Serial.print(F("Loaded INFLUXDB_DATABASE: "));
      Serial.println(influxDBSettings.database);
      Serial.print(F("Loaded INFLUXDB_USER: "));
      Serial.println(influxDBSettings.user);
      Serial.print(F("Loaded INFLUXDB_PASSWORD: "));
      Serial.println(influxDBSettings.password);
      Serial.print(F("Loaded INFLUXDB_ORG: "));
      Serial.println(influxDBSettings.org);
      Serial.print(F("Loaded INFLUXDB_BUCKET: "));
      Serial.println(influxDBSettings.bucket);
      Serial.print(F("Loaded INFLUXDB_TOKEN: "));
      Serial.println(influxDBSettings.token);

      Serial.print(F("Loaded MQTT_ON: "));
      Serial.println(mqttSettings.enabled);
      Serial.print(F("Loaded MQTT_HOST: "));
      Serial.println(mqttSettings.host);
      Serial.print(F("Loaded MQTT_PORT: "));
      Serial.println(mqttSettings.port);
      Serial.print(F("Loaded MQTT_USER: "));
      Serial.println(mqttSettings.user);
      Serial.print(F("Loaded MQTT_PASSWORD: "));
      Serial.println(mqttSettings.password);

      Serial.print(F("Loaded MQTT_IP_IN_TOPIC: "));
      Serial.println(mqttSettings.ipInTopic);
      Serial.print(F("Loaded MQTT_DEVICENAME_IN_TOPIC: "));
      Serial.println(mqttSettings.deviceNameInTopic);
      Serial.print(F("Loaded MQTT_SLASH_AT_THE_BEGINNING: "));
      Serial.println(mqttSettings.slashAtBeginning);
      Serial.print(F("Loaded MQTT_SLASH_AT_THE_END: "));
      Serial.println(mqttSettings.slashAtEnd);

      Serial.print(F("Loaded MQTT_TOPIC_TEMP: "));
      Serial.println(mqttSettings.tempTopic);
      Serial.print(F("Loaded MQTT_TOPIC_HUMI: "));
      Serial.println(mqttSettings.humiTopic);
      Serial.print(F("Loaded MQTT_TOPIC_PRESS: "));
      Serial.println(mqttSettings.pressTopic);
      Serial.print(F("Loaded MQTT_TOPIC_PM1: "));
      Serial.println(mqttSettings.pm1Topic);
      Serial.print(F("Loaded MQTT_TOPIC_PM25: "));
      Serial.println(mqttSettings.pm25Topic);
      Serial.print(F("Loaded MQTT_TOPIC_PM10: "));
      Serial.println(mqttSettings.pm10Topic);
      Serial.print(F("Loaded MQTT_TOPIC_AIRQUALITY: "));
      Serial.println(mqttSettings.airQualityTopic);

      Serial.print(F("Loaded AQI_ECO_ON: "));
      Serial.println(aqiEcoSettings.enabled);
      Serial.print(F("Loaded AQI_ECO_HOST: "));
      Serial.println(aqiEcoSettings.host);
      Serial.print(F("Loaded AQI_ECO_PATH: "));
      Serial.println(aqiEcoSettings.path);

      Serial.print(F("Loaded SENDING_FREQUENCY: "));
      Serial.println(deviceSettings.sendingServicesFrequency);
      Serial.print(F("Loaded SENDING_DB_FREQUENCY: "));
      Serial.println(deviceSettings.sendingDBFrequency);
      Serial.print(F("Loaded DEEPSLEEP_ON: "));
      Serial.println(deviceSettings.deepSleep);

      Serial.print(F("Loaded DEBUG: "));
      Serial.println(deviceSettings.debug);
      Serial.print(F("Loaded AUTOUPDATE_ON: "));
      Serial.println(deviceSettings.autoUpdate);

      Serial.print(F("Loaded CONFIG_AUTH: "));
      Serial.println(authSettings.enabled);
      Serial.print(F("Loaded CONFIG_USERNAME: "));
      Serial.println(authSettings.username);
      Serial.print(F("Loaded CONFIG_PASSWORD: "));
      Serial.println(authSettings.password);

      Serial.print(F("Loaded AUTO_CALIBRATION: "));
      Serial.println(deviceSettings.autoCalibration);

      Serial.print(F("Loaded HOMEKIT_SUPPORT: "));
      Serial.println(homeKitSettings.enabled);
      Serial.print(F("Saved HOMEKIT_PASSWORD: "));
      Serial.println(homeKitSettings.password);

      Serial.print(F("Loaded PMSENSORVERSION: "));
      Serial.println(pmSensorVersion);

      Serial.print(F("Loaded SOFTWAREVERSION: "));
      Serial.println(softwareVersion);

      Serial.println(F("\n"));
#elif defined ARDUINO_ARCH_ESP32
      Serial.print("Loaded DEVICENAME_AUTO: ");
      Serial.println(deviceSettings.autoName);
      Serial.print("Loaded DEVICENAME: ");
      Serial.println(deviceSettings.name);
      Serial.print("Loaded LANGUAGE: ");
      Serial.println(deviceSettings.language);
      Serial.print("Loaded SELECTED_LANGUAGE: ");
      Serial.println(deviceSettings.selectedLanguage);

      Serial.print("Loaded THP_MODEL: ");
      Serial.println(sensorsSettings.thpModel);
      Serial.print("Loaded DUST_MODEL: ");
      Serial.println(sensorsSettings.dustModel);
      Serial.print("Loaded DISPLAY_PM1: ");
      Serial.println(deviceSettings.displayPM1);
      Serial.print("Loaded FREQUENTMEASUREMENT: ");
      Serial.println(sensorsSettings.continuousMeasurement);

      Serial.print("Loaded SECOND_THP: ");
      Serial.println(sensorsSettings.secondThp);
      Serial.print("Loaded SECOND_THP_MODEL: ");
      Serial.println(sensorsSettings.secondThpModel);

      Serial.print("Loaded FIRST_THP_SDA: ");
      Serial.println(firstThpSettings.address_sda);
      Serial.print("Loaded FIRST_THP_SCL: ");
      Serial.println(firstThpSettings.address_scl);

      Serial.print("Loaded SECOND_THP_SDA: ");
      Serial.println(secondThpSettings.address_sda);
      Serial.print("Loaded SECOND_THP_SCL: ");
      Serial.println(secondThpSettings.address_scl);

      Serial.print("Loaded DUST_TX: ");
      Serial.println(dustSettings.address_sda);
      Serial.print("Loaded DUST_RX: ");
      Serial.println(dustSettings.address_scl);

      Serial.print("Loaded CONFIG_FIRST_THP_SDA: ");
      Serial.println(firstThpSettings.sda);
      Serial.print("Loaded CONFIG_FIRST_THP_SCL: ");
      Serial.println(firstThpSettings.scl);

      Serial.print("Loaded CONFIG_SECOND_THP_SDA: ");
      Serial.println(secondThpSettings.sda);
      Serial.print("Loaded CONFIG_SECOND_THP_SCL: ");
      Serial.println(secondThpSettings.scl);

      Serial.print("Loaded CONFIG_DUST_TX: ");
      Serial.println(dustSettings.sda);
      Serial.print("Loaded CONFIG_DUST_RX: ");
      Serial.println(dustSettings.scl);

      Serial.print("Loaded DUST_TIME: ");
      Serial.println(sensorsSettings.dustTime);
      Serial.print("Loaded NUMBEROFMEASUREMENTS: ");
      Serial.println(sensorsSettings.numerOfMeasurements);

      Serial.print("Loaded LUFTDATEN_ON: ");
      Serial.println(luftdatenSettings.enabled);
      Serial.print("Loaded LUFTDATEN_GRAPH_ON: ");
      Serial.println(luftdatenSettings.graph);
      Serial.print("Loaded LUFTDATEN_APIID: ");
      Serial.println(luftdatenSettings.apiId);
	  	  
      Serial.print("Loaded SMOGLIST_ON: ");
      Serial.println(smoglistSettings.enabled);

      Serial.print("Loaded AIRMONITOR_ON: ");
      Serial.println(airMonitorSettings.enabled);
      Serial.print("Loaded AIRMONITOR_GRAPH_ON: ");
      Serial.println(airMonitorSettings.graph);
      Serial.print("Loaded AIRMONITOR_API_KEY: ");
      Serial.println(airMonitorSettings.apiKey);
      Serial.print("Loaded LATITUDE: ");
      Serial.println(deviceSettings.latitude);
      Serial.print("Loaded LONGITUDE: ");
      Serial.println(deviceSettings.longitude);
      Serial.print("Loaded EMAIL: ");
      Serial.println(deviceSettings.email);
      Serial.print("Loaded MYALTITUDE: ");
      Serial.println(deviceSettings.altitude);

      Serial.print("Loaded THINGSPEAK_ON: ");
      Serial.println(thingSpeakSettings.enabled);
      Serial.print("Loaded THINGSPEAK_GRAPH_ON: ");
      Serial.println(thingSpeakSettings.graph);
      Serial.print("Loaded THINGSPEAK_API_KEY: ");
      Serial.println(thingSpeakSettings.apiKey);
      Serial.print("Loaded THINGSPEAK_CHANNEL_ID: ");
      Serial.println(thingSpeakSettings.channelId);
      Serial.print("Loaded THINGSPEAK_READ_API_KEY: ");
      Serial.println(thingSpeakSettings.readApiKey);

      Serial.print("Loaded INFLUXDB_ON: ");
      Serial.println(influxDBSettings.enabled);
      Serial.print("Loaded INFLUXDB_VERSION: ");
      Serial.println(influxDBSettings.version);
      Serial.print("Loaded INFLUXDB_HOST: ");
      Serial.println(influxDBSettings.host);
      Serial.print("Loaded INFLUXDB_PORT: ");
      Serial.println(influxDBSettings.port);
      Serial.print("Loaded INFLUXDB_DATABASE: ");
      Serial.println(influxDBSettings.database);
      Serial.print("Loaded INFLUXDB_USER: ");
      Serial.println(influxDBSettings.user);
      Serial.print("Loaded INFLUXDB_PASSWORD: ");
      Serial.println(influxDBSettings.password);
      Serial.print("Loaded INFLUXDB_ORG: ");
      Serial.println(influxDBSettings.org);
      Serial.print("Loaded INFLUXDB_BUCKET: ");
      Serial.println(influxDBSettings.bucket);
      Serial.print("Loaded INFLUXDB_TOKEN: ");
      Serial.println(influxDBSettings.token);

      Serial.print("Loaded MQTT_ON: ");
      Serial.println(mqttSettings.enabled);
      Serial.print("Loaded MQTT_HOST: ");
      Serial.println(mqttSettings.host);
      Serial.print("Loaded MQTT_PORT: ");
      Serial.println(mqttSettings.port);
      Serial.print("Loaded MQTT_USER: ");
      Serial.println(mqttSettings.user);
      Serial.print("Loaded MQTT_PASSWORD: ");
      Serial.println(mqttSettings.password);

      Serial.print("Loaded MQTT_IP_IN_TOPIC: ");
      Serial.println(mqttSettings.ipInTopic);
      Serial.print("Loaded MQTT_DEVICENAME_IN_TOPIC: ");
      Serial.println(mqttSettings.deviceNameInTopic);
      Serial.print("Loaded MQTT_SLASH_AT_THE_BEGINNING: ");
      Serial.println(mqttSettings.slashAtBeginning);
      Serial.print("Loaded MQTT_SLASH_AT_THE_END: ");
      Serial.println(mqttSettings.slashAtEnd);

      Serial.print("Loaded MQTT_TOPIC_TEMP: ");
      Serial.println(mqttSettings.tempTopic);
      Serial.print("Loaded MQTT_TOPIC_HUMI: ");
      Serial.println(mqttSettings.humiTopic);
      Serial.print("Loaded MQTT_TOPIC_PRESS: ");
      Serial.println(mqttSettings.pressTopic);
      Serial.print("Loaded MQTT_TOPIC_PM1: ");
      Serial.println(mqttSettings.pm1Topic);
      Serial.print("Loaded MQTT_TOPIC_PM25: ");
      Serial.println(mqttSettings.pm25Topic);
      Serial.print("Loaded MQTT_TOPIC_PM10: ");
      Serial.println(mqttSettings.pm10Topic);
      Serial.print("Loaded MQTT_TOPIC_AIRQUALITY: ");
      Serial.println(mqttSettings.airQualityTopic);

      Serial.print("Loaded AQI_ECO_ON: ");
      Serial.println(aqiEcoSettings.enabled);
      Serial.print("Loaded AQI_ECO_HOST: ");
      Serial.println(aqiEcoSettings.host);
      Serial.print("Loaded AQI_ECO_PATH: ");
      Serial.println(aqiEcoSettings.path);

      Serial.print("Loaded SENDING_FREQUENCY: ");
      Serial.println(deviceSettings.sendingServicesFrequency);
      Serial.print("Loaded SENDING_DB_FREQUENCY: ");
      Serial.println(deviceSettings.sendingDBFrequency);
      Serial.print("Loaded DEEPSLEEP_ON: ");
      Serial.println(deviceSettings.deepSleep);

      Serial.print("Loaded DEBUG: ");
      Serial.println(deviceSettings.debug);
      Serial.print("Loaded AUTOUPDATE_ON: ");
      Serial.println(deviceSettings.autoUpdate);

      Serial.print("Loaded CONFIG_AUTH: ");
      Serial.println(authSettings.enabled);
      Serial.print("Loaded CONFIG_USERNAME: ");
      Serial.println(authSettings.username);
      Serial.print("Loaded CONFIG_PASSWORD: ");
      Serial.println(authSettings.password);

      Serial.print("Loaded AUTO_CALIBRATION: ");
      Serial.println(deviceSettings.autoCalibration);

      Serial.print("Loaded HOMEKIT_SUPPORT: ");
      Serial.println(homeKitSettings.enabled);
      Serial.print("Saved HOMEKIT_PASSWORD: ");
      Serial.println(homeKitSettings.password);

      Serial.print("Loaded PMSENSORVERSION: ");
      Serial.println(pmSensorVersion);

      Serial.print("Loaded SOFTWAREVERSION: ");
      Serial.println(softwareVersion);

      Serial.println("\n");
#endif
    }
    return true;
  }

bool saveConfig() {
	
  if (deviceSettings.debug) {
	  /*
    Serial.print(F("Saved DEVICENAME_AUTO: "));
    Serial.println(deviceSettings.autoName);
    Serial.print(F("Saved DEVICENAME: "));
    Serial.println(deviceSettings.name);
    Serial.print(F("Saved LANGUAGE: "));
    Serial.println(deviceSettings.language);
    Serial.print(F("Saved SELECTED_LANGUAGE: "));
    Serial.println(deviceSettings.selectedLanguage);

    Serial.print(F("Saved THP_MODEL: "));
    Serial.println(sensorsSettings.thpModel);
    Serial.print(F("Saved DUST_MODEL: "));
    Serial.println(sensorsSettings.dustModel);
    Serial.print(F("Saved DISPLAY_PM1: "));
    Serial.println(deviceSettings.displayPM1);
    Serial.print(F("Saved FREQUENTMEASUREMENT: "));
    Serial.println(sensorsSettings.continuousMeasurement);

    Serial.print(F("Saved SECOND_THP: "));
    Serial.println(sensorsSettings.secondThp);
    Serial.print(F("Saved SECOND_THP_MODEL: "));
    Serial.println(sensorsSettings.secondThpModel);

    Serial.print(F("Saved FIRST_THP_SDA: "));
    Serial.println(firstThpSettings.address_sda);
    Serial.print(F("Saved FIRST_THP_SCL: "));
    Serial.println(firstThpSettings.address_scl);

    Serial.print(F("Saved SECOND_THP_SDA: "));
    Serial.println(secondThpSettings.address_sda);
    Serial.print(F("Saved SECOND_THP_SCL: "));
    Serial.println(secondThpSettings.address_scl);

    Serial.print(F("Saved DUST_TX: "));
    Serial.println(dustSettings.address_sda);
    Serial.print(F("Saved DUST_RX: "));
    Serial.println(dustSettings.address_scl);
	
    Serial.print(F("Saved CONFIG_FIRST_THP_SDA: "));
    Serial.println(firstThpSettings.sda);
    Serial.print(F("Saved CONFIG_FIRST_THP_SCL: "));
    Serial.println(firstThpSettings.scl);

    Serial.print(F("Saved CONFIG_SECOND_THP_SDA: "));
    Serial.println(secondThpSettings.sda);
    Serial.print(F("Saved CONFIG_SECOND_THP_SCL: "));
    Serial.println(secondThpSettings.scl);

    Serial.print(F("Saved CONFIG_DUST_TX: "));
    Serial.println(dustSettings.sda);
    Serial.print(F("Saved CONFIG_DUST_RX: "));
    Serial.println(dustSettings.scl);
	
    Serial.print(F("Saved DUST_TIME: "));
    Serial.println(sensorsSettings.dustTime);
    Serial.print(F("Saved NUMBEROFMEASUREMENTS: "));
    Serial.println(sensorsSettings.numerOfMeasurements);
*/
	  /*
    Serial.print(F("Saved LUFTDATEN_ON: "));
    Serial.println(luftdatenSettings.enabled);
    Serial.print(F("Saved SMOGLIST_ON: "));
    Serial.println(smoglistSettings.enabled);
*/
/*	
    Serial.print(F("Saved AIRMONITOR_ON: "));
    Serial.println(airMonitorSettings.enabled);
    Serial.print(F("Saved AIRMONITOR_GRAPH_ON: "));
    Serial.println(airMonitorSettings.graph);
    Serial.print(F("Saved AIRMONITOR_API_KEY: "));
    Serial.println(airMonitorSettings.apiKey);
    Serial.print(F("Saved LATITUDE: "));
    Serial.println(deviceSettings.latitude);
    Serial.print(F("Saved LONGITUDE: "));
    Serial.println(deviceSettings.longitude);
    Serial.print(F("Saved EMAIL: "));
    Serial.println(deviceSettings.email);
    Serial.print(F("Saved MYALTITUDE: "));
    Serial.println(deviceSettings.altitude);
	
    Serial.print(F("Saved THINGSPEAK_ON: "));
    Serial.println(thingSpeakSettings.enabled);
    Serial.print(F("Saved THINGSPEAK_GRAPH_ON: "));
    Serial.println(thingSpeakSettings.graph);
    Serial.print(F("Saved THINGSPEAK_API_KEY: "));
    Serial.println(thingSpeakSettings.apiKey);
    Serial.print(F("Saved THINGSPEAK_CHANNEL_ID: "));
    Serial.println(thingSpeakSettings.channelId);
    Serial.print(F("Saved THINGSPEAK_READ_API_KEY: "));
    Serial.println(thingSpeakSettings.readApiKey);

    Serial.print(F("Saved INFLUXDB_ON: "));
    Serial.println(influxDBSettings.enabled);
    Serial.print(F("Saved INFLUXDB_VERSION: "));
    Serial.println(influxDBSettings.version);
    Serial.print(F("Saved INFLUXDB_HOST: "));
    Serial.println(influxDBSettings.host);
    Serial.print(F("Saved INFLUXDB_PORT: "));
    Serial.println(influxDBSettings.port);
    Serial.print(F("Saved INFLUXDB_DATABASE: "));
    Serial.println(influxDBSettings.database);
    Serial.print(F("Saved INFLUXDB_USER: "));
    Serial.println(influxDBSettings.user);
    Serial.print(F("Saved INFLUXDB_PASSWORD: "));
    Serial.println(influxDBSettings.password);
    Serial.print(F("Saved INFLUXDB_ORG: "));
    Serial.println(influxDBSettings.org);
    Serial.print(F("Saved INFLUXDB_BUCKET: "));
    Serial.println(influxDBSettings.bucket);
    Serial.print(F("Saved INFLUXDB_TOKEN: "));
    Serial.println(influxDBSettings.token);

    Serial.print(F("Saved MQTT_ON: "));
    Serial.println(mqttSettings.enabled);
    Serial.print(F("Saved MQTT_HOST: "));
    Serial.println(mqttSettings.host);
    Serial.print(F("Saved MQTT_PORT: "));
    Serial.println(mqttSettings.port);
    Serial.print(F("Saved MQTT_USER: "));
    Serial.println(mqttSettings.user);
    Serial.print(F("Saved MQTT_PASSWORD: "));
    Serial.println(mqttSettings.password);

    Serial.print(F("Saved MQTT_IP_IN_TOPIC: "));
    Serial.println(mqttSettings.ipInTopic);
    Serial.print(F("Saved MQTT_DEVICENAME_IN_TOPIC: "));
    Serial.println(mqttSettings.deviceNameInTopic);
    Serial.print(F("Saved MQTT_SLASH_AT_THE_BEGINNING: "));
    Serial.println(mqttSettings.slashAtBeginning);
    Serial.print(F("Saved MQTT_SLASH_AT_THE_END: "));
    Serial.println(mqttSettings.slashAtEnd);

    Serial.print(F("Saved MQTT_TOPIC_TEMP: "));
    Serial.println(mqttSettings.tempTopic);
    Serial.print(F("Saved MQTT_TOPIC_HUMI: "));
    Serial.println(mqttSettings.humiTopic);
    Serial.print(F("Saved MQTT_TOPIC_PRESS: "));
    Serial.println(mqttSettings.pressTopic);
    Serial.print(F("Saved MQTT_TOPIC_PM1: "));
    Serial.println(mqttSettings.pm1Topic);
    Serial.print(F("Saved MQTT_TOPIC_PM25: "));
    Serial.println(mqttSettings.pm25Topic);
    Serial.print(F("Saved MQTT_TOPIC_PM10: "));
    Serial.println(mqttSettings.pm10Topic);
    Serial.print(F("Saved MQTT_TOPIC_AIRQUALITY: "));
    Serial.println(mqttSettings.airQualityTopic);

    Serial.print(F("Saved AQI_ECO_ON: "));
    Serial.println(aqiEcoSettings.enabled);
    Serial.print(F("Saved AQI_ECO_HOST: "));
    Serial.println(aqiEcoSettings.host);
    Serial.print(F("Saved AQI_ECO_PATH: "));
    Serial.println(aqiEcoSettings.path);

    Serial.print(F("Saved SENDING_FREQUENCY: "));
    Serial.println(deviceSettings.sendingServicesFrequency);
    Serial.print(F("Saved SENDING_DB_FREQUENCY: "));
    Serial.println(deviceSettings.sendingDBFrequency);
    Serial.print(F("Saved DEEPSLEEP_ON: "));
    Serial.println(deviceSettings.deepSleep);

    Serial.print(F("Saved DEBUG: "));
    Serial.println(deviceSettings.debug);
    Serial.print(F("Saved AUTOUPDATE_ON: "));
    Serial.println(deviceSettings.autoUpdate);

    Serial.print(F("Saved CONFIG_AUTH: "));
    Serial.println(authSettings.enabled);
    Serial.print(F("Saved CONFIG_USERNAME: "));
    Serial.println(authSettings.username);
    Serial.print(F("Saved CONFIG_PASSWORD: "));
    Serial.println(authSettings.password);

    Serial.print(F("Saved AUTO_CALIBRATION: "));
    Serial.println(deviceSettings.autoCalibration);

    Serial.print(F("Saved HOMEKIT_SUPPORT: "));
    Serial.println(homeKitSettings.enabled);
    Serial.print(F("Saved HOMEKIT_PASSWORD: "));
    Serial.println(homeKitSettings.password);

    Serial.print(F("Saved PMSENSORVERSION: "));
    Serial.println(pmSensorVersion);

    Serial.print(F("Saved SOFTWAREVERSION: "));
    Serial.println(softwareVersion);

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

  json["device_autoName"] = bool(deviceSettings.autoName);
  json["device_name"] = String(deviceSettings.name);
  json["device_displayPM1"] = bool(deviceSettings.displayPM1);
  json["device_language"] = String(deviceSettings.language);
  json["device_selectedLanguage"] = short(deviceSettings.selectedLanguage);
  json["device_sendingServicesFrequency"] = (unsigned short)(deviceSettings.sendingServicesFrequency);
  json["device_sendingDBFrequency"] = (unsigned short)(deviceSettings.sendingDBFrequency);
  json["device_deepSleep"] = bool(deviceSettings.deepSleep);
  json["device_debug"] = bool(deviceSettings.debug);
  json["device_autoUpdate"] = bool(deviceSettings.autoUpdate);
  json["device_autoCalibration"] = bool(deviceSettings.autoCalibration);
  json["device_calib1"] = float(deviceSettings.calib1);
  json["device_calib2"] = float(deviceSettings.calib2);
  json["device_latitude"] = String(deviceSettings.latitude);
  json["device_longitude"] = String(deviceSettings.longitude);
  json["device_altitude"] = short(deviceSettings.altitude);
  json["device_email"] = String(deviceSettings.email);

  json["sensors_thpModel"] = String(sensorsSettings.thpModel);
  json["sensors_secondThp"] = bool(sensorsSettings.secondThp);
  json["sensors_secondThpModel"] = String(sensorsSettings.secondThpModel);
  json["sensors_dustModel"] = String(sensorsSettings.dustModel);
  json["sensors_continuousMeasurement"] = bool(sensorsSettings.continuousMeasurement);
  json["sensors_dustTime"] = (unsigned short)(sensorsSettings.dustTime);
  json["sensors_numerOfMeasurements"] = (unsigned short)(sensorsSettings.numerOfMeasurements);

  json["firstThp_sda"] = String(firstThpSettings.sda);
  json["firstThp_scl"] = String(firstThpSettings.scl);
  json["firstThp_address_sda"] = (unsigned short)(firstThpSettings.address_sda);
  json["firstThp_address_scl"] = (unsigned short)(firstThpSettings.address_scl);

  json["secondThp_sda"] = String(secondThpSettings.sda);
  json["secondThp_scl"] = String(secondThpSettings.scl);
  json["secondThp_address_sda"] = (unsigned short)(secondThpSettings.address_sda);
  json["secondThp_address_scl"] = (unsigned short)(secondThpSettings.address_scl);

  json["firtsDust_sda"] = String(dustSettings.sda);
  json["firtsDust_scl"] = String(dustSettings.scl);
  json["firtsDust_address_sda"] = (unsigned short)(dustSettings.address_sda);
  json["firtsDust_address_scl"] = (unsigned short)(dustSettings.address_scl);

  json["smoglist_enabled"] = bool(smoglistSettings.enabled);

  json["luftdaten_enabled"] = bool(luftdatenSettings.enabled);
  json["luftdaten_graph"] = bool(luftdatenSettings.graph);
  json["luftdaten_apiId"] = (unsigned int)(luftdatenSettings.apiId);

  json["airMonitor_enabled"] = bool(airMonitorSettings.enabled);
  json["airMonitor_graph"] = bool(airMonitorSettings.graph);
  json["airMonitor_apiKey"] = String(airMonitorSettings.apiKey);

  json["thingSpeak_enabled"] = bool(thingSpeakSettings.enabled);
  json["thingSpeak_graph"] = bool(thingSpeakSettings.graph);
  json["thingSpeak_apiKey"] = String(thingSpeakSettings.apiKey);
  json["thingSpeak_channelId"] = (unsigned int)(thingSpeakSettings.channelId);
  json["thingSpeak_readApiKey"] = String(thingSpeakSettings.readApiKey);

  json["aqiEco_enabled"] = bool(aqiEcoSettings.enabled);
  json["aqiEco_host"] = String(aqiEcoSettings.host);
  json["aqiEco_path"] = String(aqiEcoSettings.path);
  
  json["influxDB_enabled"] = bool(influxDBSettings.enabled);
  json["influxDB_version"] = String(influxDBSettings.version);
  json["influxDB_host"] = String(influxDBSettings.host);
  json["influxDB_port"] = uint16_t(influxDBSettings.port);
  json["influxDB_database"] = String(influxDBSettings.database);
  json["influxDB_user"] = String(influxDBSettings.user);
  json["influxDB_password"] = String(influxDBSettings.password);
  json["influxDB_org"] = String(influxDBSettings.org);
  json["influxDB_bucket"] = String(influxDBSettings.bucket);
  json["influxDB_token"] = String(influxDBSettings.token);

  json["mqtt_enabled"] = bool(mqttSettings.enabled);
  json["mqtt_host"] = String(mqttSettings.host);
  json["mqtt_port"] = uint16_t(mqttSettings.port);
  json["mqtt_user"] = String(mqttSettings.user);
  json["mqtt_password"] = String(mqttSettings.password);
  json["mqtt_ipInTopic"] = bool(mqttSettings.ipInTopic);
  json["mqtt_deviceNameInTopic"] = bool(mqttSettings.deviceNameInTopic);
  json["mqtt_slashAtBeginning"] = bool(mqttSettings.slashAtBeginning);
  json["mqtt_slashAtEnd"] = bool(mqttSettings.slashAtEnd);
  json["mqtt_tempTopic"] = String(mqttSettings.tempTopic);
  json["mqtt_humiTopic"] = String(mqttSettings.humiTopic);
  json["mqtt_pressTopic"] = String(mqttSettings.pressTopic);
  json["mqtt_pm1Topic"] = String(mqttSettings.pm1Topic);
  json["mqtt_pm25Topic"] = String(mqttSettings.pm25Topic);
  json["mqtt_pm10Topic"] = String(mqttSettings.pm10Topic);
  json["mqtt_airQualityTopic"] = String(mqttSettings.airQualityTopic);

  json["homeKit_enabled"] = bool(homeKitSettings.enabled);
  json["homeKit_password"] = String(homeKitSettings.password);

  json["auth_enabled"] = bool(authSettings.enabled);
  json["auth_username"] = String(authSettings.username);
  json["auth_password"] = String(authSettings.password);

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
  if (deviceSettings.debug) {
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
