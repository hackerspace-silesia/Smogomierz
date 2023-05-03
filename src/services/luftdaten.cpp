#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#endif

#include <ArduinoJson.h>
#include "../../config.h"

const char *luftdatenAPIHOST PROGMEM = "api.luftdaten.info";
const char *luftdatenAPIURL PROGMEM = "/v1/push-sensor-data/";
const uint16_t luftdatenAPIPort PROGMEM = 80;

const char *madavideAPIHOST PROGMEM = "api-rrd.madavi.de";
const char *madavideAPIURL PROGMEM = "/data.php";
const uint16_t madavideAPIPort PROGMEM = 80;

String receivedLuftdatenAPIJSON;

// Luftdaten.info - https://luftdaten.info
void sendDUSTLuftdatenJson(JsonObject& json) {
  WiFiClient client;
  Serial.print(F("\nconnecting to "));
  Serial.println(luftdatenAPIHOST);

  if (!client.connect(luftdatenAPIHOST, luftdatenAPIPort)) {
    Serial.println(F("connection failed"));
    delay(1000);
    return;
  }
  delay(100);
  client.println(F("POST ") + String(luftdatenAPIURL) + F(" HTTP/1.1"));
  client.println(F("Host: ") + String(luftdatenAPIHOST));
  client.println(F("Content-Type: application/json"));
  client.println(F("X-PIN: 1"));
#ifdef ARDUINO_ARCH_ESP8266
  client.println(F("X-Sensor: smogomierz-") + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
  client.println(F("X-Sensor: smogomierz-") + String((uint32_t)(ESP.getEfuseMac())));
#endif
  client.print(F("Content-Length: "));
  client.println(measureJson(json));
  client.println(F("Connection: close"));
  client.println();
  serializeJson(json, client);

  String line = client.readStringUntil('\r');
  // TODO: Support wrong error (!= 200)

  if (deviceSettings.debug) {
    Serial.println();
    Serial.println(F("POST ") + String(luftdatenAPIURL) + F(" HTTP/1.1"));
    Serial.println(F("Host: ") + String(luftdatenAPIHOST));
    Serial.println(F("Content-Type: application/json"));
    Serial.println(F("X-PIN: 1"));
#ifdef ARDUINO_ARCH_ESP8266
    Serial.println(F("X-Sensor: smogomierz-") + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
    Serial.println(F("X-Sensor: smogomierz-") + String((uint32_t)(ESP.getEfuseMac())));
#endif
    Serial.print(F("Content-Length: "));
    Serial.println(measureJson(json));
    Serial.println(F("Connection: close"));
    Serial.println();
    serializeJsonPretty(json, Serial);
    Serial.println(F("\n"));
    Serial.println(line);
  }
  client.stop();
}

void sendTHPLuftdatenJson(JsonObject& json) {
  WiFiClient client;
  Serial.print(F("\nconnecting to "));
  Serial.println(luftdatenAPIHOST);

  if (!client.connect(luftdatenAPIHOST, luftdatenAPIPort)) {
    Serial.println(F("connection failed"));
    delay(1000);
    return;
  }
  delay(100);
  client.println(F("POST ") + String(luftdatenAPIURL) + F(" HTTP/1.1"));
  client.println(F("Host: ") + String(luftdatenAPIHOST));
  client.println(F("Content-Type: application/json"));
  const char* thp_models[] = {"BME280", "BMP280", "HTU21", "DHT22", "SHT1x", "DS18B20"};
  const char* thp_pins[] = {"11", "3", "7", "7", "12", "13"};
  int thp_num_models = sizeof(thp_models) / sizeof(thp_models[0]);
  for (int i = 0; i < thp_num_models; i++) {
    if (!strcmp(sensorsSettings.thpModel, thp_models[i])) {
      client.print(F("X-PIN: "));
      client.println(thp_pins[i]);
      break;
    }
  }
#ifdef ARDUINO_ARCH_ESP8266
  client.println(F("X-Sensor: smogomierz-") + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
  client.println(F("X-Sensor: smogomierz-") + String((uint32_t)(ESP.getEfuseMac())));
#endif
  client.print(F("Content-Length: "));
  client.println(measureJson(json));
  client.println(F("Connection: close"));
  client.println();

  serializeJson(json, client);

  String line = client.readStringUntil('\r');
  // TODO: Support wrong error (!= 200)

  if (deviceSettings.debug) {
    Serial.println();
    Serial.println(F("POST ") + String(luftdatenAPIURL) + F(" HTTP/1.1"));
    Serial.println(F("Host: ") + String(luftdatenAPIHOST));
    Serial.println(F("Content-Type: application/json"));
    for (int i = 0; i < thp_num_models; i++) {
      if (!strcmp(sensorsSettings.thpModel, thp_models[i])) {
        Serial.print(F("X-PIN: "));
        Serial.println(thp_pins[i]);
        break;
      }
    }
#ifdef ARDUINO_ARCH_ESP8266
    Serial.println(F("X-Sensor: smogomierz-") + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
    Serial.println(F("X-Sensor: smogomierz-") + String((uint32_t)(ESP.getEfuseMac())));
#endif
    Serial.print(F("Content-Length: "));
    Serial.println(measureJson(json));
    Serial.println(F("Connection: close"));
    Serial.println();
    serializeJsonPretty(json, Serial);
    Serial.println(F("\n"));
    Serial.println(line);
  }
  client.stop();
}

void sendDUSTDatatoLuftdaten() {
  if (strcmp(sensorsSettings.dustModel, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("software_version")] = F("Smogomierz_") + String(softwareVersion);
    JsonArray sensordatavalues = json.createNestedArray(F("sensordatavalues"));
    if (!strcmp(sensorsSettings.dustModel, "PMS7003")) {
      JsonObject P0 = sensordatavalues.createNestedObject();
      P0[F("value_type")] = F("P0");
      P0[F("value")] = measurementsData.averagePM1;
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1[F("value_type")] = F("P1");
      P1[F("value")] = measurementsData.averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2[F("value_type")] = F("P2");
      P2[F("value")] = measurementsData.averagePM25;
    } else {
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1[F("value_type")] = F("P1");
      P1[F("value")] = measurementsData.averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2[F("value_type")] = F("P2");
      P2[F("value")] = measurementsData.averagePM25;
    }
    sendDUSTLuftdatenJson(json);
  }
}

void sendTHPDatatoLuftdaten() {
  if (strcmp(sensorsSettings.thpModel, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("software_version")] = F("Smogomierz_") + String(softwareVersion);
    JsonArray sensordatavalues = json.createNestedArray(F("sensordatavalues"));
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature[F("value_type")] = F("temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    if (!strcmp(sensorsSettings.thpModel, "BME280")) {
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(measurementsData.humidity);
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure[F("value_type")] = F("pressure");
      pressure[F("value")] = String(measurementsData.pressure * 100); //hPa -> Pa
    } else if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure[F("value_type")] = F("pressure");
      pressure[F("value")] = String(measurementsData.pressure * 100); //hPa -> Pa
    } else if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(measurementsData.humidity);
    } else if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(measurementsData.humidity);
    } else if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(measurementsData.humidity);
    } else if (!strcmp(sensorsSettings.thpModel, "DS18B20")) {
    } 

    sendTHPLuftdatenJson(json);
  }
}

// Madavi.de - https://www.madavi.de/sensor/graph.php
void sendDUSTMadavideJson(JsonObject& json) {
  WiFiClient client;
  Serial.print(F("\nconnecting to "));
  Serial.println(madavideAPIHOST);

  if (!client.connect(madavideAPIHOST, madavideAPIPort)) {
    Serial.println(F("connection failed"));
    delay(1000);
    return;
  }
  delay(100);
  client.println(F("POST ") + String(madavideAPIURL) + F(" HTTP/1.1"));
  client.println(F("Host: ") + String(madavideAPIHOST));
  client.println(F("Content-Type: application/json"));
  client.println(F("X-PIN: 1"));
#ifdef ARDUINO_ARCH_ESP8266
  client.println(F("X-Sensor: smogomierz-") + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
  client.println(F("X-Sensor: smogomierz-") + String((uint32_t)(ESP.getEfuseMac())));
#endif
  client.print(F("Content-Length: "));
  client.println(measureJson(json));
  client.println(F("Connection: close"));
  client.println();

  serializeJson(json, client);

  String line = client.readStringUntil('\r');
  // TODO: Support wrong error (!= 200)

  if (deviceSettings.debug) {
    Serial.println();
    Serial.println(F("POST ") + String(madavideAPIURL) + F(" HTTP/1.1"));
    Serial.println(F("Host: ") + String(madavideAPIHOST));
    Serial.println(F("Content-Type: application/json"));
    Serial.println(F("X-PIN: 1"));
#ifdef ARDUINO_ARCH_ESP8266
    Serial.println(F("X-Sensor: smogomierz-") + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
    Serial.println(F("X-Sensor: smogomierz-") + String((uint32_t)(ESP.getEfuseMac())));
#endif
    Serial.print(F("Content-Length: "));
    Serial.println(measureJson(json));
    Serial.println(F("Connection: close"));
    Serial.println();
    serializeJsonPretty(json, Serial);
    Serial.println(F("\n"));
    Serial.println(line);
  }
  client.stop();
}

void sendTHPMadavideJson(JsonObject& json) {
  WiFiClient client;
  Serial.print(F("\nconnecting to "));
  Serial.println(madavideAPIHOST);

  if (!client.connect(madavideAPIHOST, madavideAPIPort)) {
    Serial.println(F("connection failed"));
    delay(1000);
    return;
  }
  delay(100);
  client.println(F("POST ") + String(madavideAPIURL) + F(" HTTP/1.1"));
  client.println(F("Host: ") + String(madavideAPIHOST));
  client.println(F("Content-Type: application/json"));
  const char* thp_models[] = {"BME280", "BMP280", "HTU21", "DHT22", "SHT1x", "DS18B20"};
  const char* thp_pins[] = {"11", "3", "7", "7", "12", "13"};
  int thp_num_models = sizeof(thp_models) / sizeof(thp_models[0]);
  for (int i = 0; i < thp_num_models; i++) {
    if (!strcmp(sensorsSettings.thpModel, thp_models[i])) {
      client.print(F("X-PIN: "));
      client.println(thp_pins[i]);
      break;
    }
  }
#ifdef ARDUINO_ARCH_ESP8266
  client.println(F("X-Sensor: smogomierz-") + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
  client.println(F("X-Sensor: smogomierz-") + String((uint32_t)(ESP.getEfuseMac())));
#endif
  client.print(F("Content-Length: "));
  client.println(measureJson(json));
  client.println(F("Connection: close"));
  client.println();

  serializeJson(json, client);

  String line = client.readStringUntil('\r');
  // TODO: Support wrong error (!= 200)

  if (deviceSettings.debug) {
    Serial.println();
    Serial.println(F("POST ") + String(madavideAPIURL) + F(" HTTP/1.1"));
    Serial.println(F("Host: ") + String(madavideAPIHOST));
    Serial.println(F("Content-Type: application/json"));
    for (int i = 0; i < thp_num_models; i++) {
      if (!strcmp(sensorsSettings.thpModel, thp_models[i])) {
        Serial.print(F("X-PIN: "));
        Serial.println(thp_pins[i]);
        break;
      }
    }
#ifdef ARDUINO_ARCH_ESP8266
    Serial.println(F("X-Sensor: smogomierz-") + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
    Serial.println(F("X-Sensor: smogomierz-") + String((uint32_t)(ESP.getEfuseMac())));
#endif
    Serial.print(F("Content-Length: "));
    Serial.println(measureJson(json));
    Serial.println(F("Connection: close"));
    Serial.println();
    serializeJsonPretty(json, Serial);
    Serial.println(F("\n"));
    Serial.println(line);
  }
  client.stop();
}

void sendDUSTDatatoMadavide() {
  if (strcmp(sensorsSettings.dustModel, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("software_version")] = F("Smogomierz_") + String(softwareVersion);
    JsonArray sensordatavalues = json.createNestedArray(F("sensordatavalues"));
    if (!strcmp(sensorsSettings.dustModel, "PMS7003")) {
      JsonObject P0 = sensordatavalues.createNestedObject();
      P0[F("value_type")] = F("PMS_P0");
      P0[F("value")] = measurementsData.averagePM1;
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1[F("value_type")] = F("PMS_P1");
      P1[F("value")] = measurementsData.averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2[F("value_type")] = F("PMS_P2");
      P2[F("value")] = measurementsData.averagePM25;
    } else if (!strcmp(sensorsSettings.dustModel, "SDS011/21")) {
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1[F("value_type")] = F("SDS_P1");
      P1[F("value")] = measurementsData.averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2[F("value_type")] = F("SDS_P2");
      P2[F("value")] = measurementsData.averagePM25;
    } else if (!strcmp(sensorsSettings.dustModel, "HPMA115S0")) {
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1[F("value_type")] = F("HPM_P1");
      P1[F("value")] = measurementsData.averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2[F("value_type")] = F("HPM_P2");
      P2[F("value")] = measurementsData.averagePM25;
    }
    sendDUSTMadavideJson(json);
  }
}

void sendTHPDatatoMadavide() {
  if (strcmp(sensorsSettings.thpModel, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("software_version")] = F("Smogomierz_") + String(softwareVersion);
    JsonArray sensordatavalues = json.createNestedArray(F("sensordatavalues"));
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature[F("value_type")] = F("temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    if (!strcmp(sensorsSettings.thpModel, "BME280")) {
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(measurementsData.humidity);
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure[F("value_type")] = F("pressure");
      pressure[F("value")] = String(measurementsData.pressure * 100); //hPa -> Pa
    } else if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure[F("value_type")] = F("pressure");
      pressure[F("value")] = String(measurementsData.pressure * 100); //hPa -> Pa
    } else if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(measurementsData.humidity);
    } else if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(measurementsData.humidity);
    } else if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(measurementsData.humidity);
    } else if (!strcmp(sensorsSettings.thpModel, "DS18B20")) {
    }

    sendTHPMadavideJson(json);
  }
}

bool getLuftdatenJSON() {
	String url_string = ("http://data.sensor.community/airrohr/v1/filter/box=" + String(deviceSettings.latitude, 6) + "00000," + String(deviceSettings.longitude, 6) + "00000," + String(deviceSettings.latitude, 6) + "00000," + String(deviceSettings.longitude, 6) + "00000");
		
    // Check WiFi Status
    if (WiFi.status() == WL_CONNECTED) {
      if (deviceSettings.debug) {
		  // Serial.print("\nWaiting for " + String(url_string));
		  Serial.print("\nWaiting for Luftdaten API data");
      }
#ifdef ARDUINO_ARCH_ESP8266
	    WiFiClient client;
#endif
      HTTPClient http;  // Object of class HTTPClient
      http.setTimeout(12000);
#ifdef ARDUINO_ARCH_ESP8266
      http.begin(client, url_string);
#elif defined ARDUINO_ARCH_ESP32
      http.begin(url_string);
#endif
      int httpCode = http.GET();
      if (httpCode != 200) {
        http.end();
        Serial.println("\nReceived httpCode: " + String(httpCode)); // WiFi - on, internet - off = -1 // // WiFi - on, internet - on, server - off = ???
        return false;
      }
      if (httpCode < 0) {
        http.end();
        Serial.println("\n\tConnection failed!\n");
        return false;
      }
      if (httpCode > 0) {
        receivedLuftdatenAPIJSON = http.getString();
        // Serial.println(receivedLuftdatenAPIJSON);
      }
      http.end();
      return true;
    } else {
    return false;
  }
}

void parsingLuftdatenAPIJSON() {
  DynamicJsonDocument doc(1536);
  deserializeJson(doc, receivedLuftdatenAPIJSON);
  JsonArray json = doc.as<JsonArray>();

if (deviceSettings.debug) {
  // serializeJsonPretty(json, Serial);
	// Serial.println("\n");
}
// Serial.println("json.size(): " + String(json.size())); 
		
#ifdef DUSTSENSOR_HPMA115S0
String temp_DUSTMODEL_Luftdaten = "HPM";
#elif defined DUSTSENSOR_SPS30
String temp_DUSTMODEL_Luftdaten = "SPS30";
#else // If no dust sensor has been defined - use DUSTSENSOR_PMS5003_7003_BME280_0x76
String temp_DUSTMODEL_Luftdaten = "PMS7003";
#endif

 for (int i = 0; i < json.size(); i++) {	
 	if (String(json[i]["sensor"]["id"].as<String>()) != "null") {
#ifdef DUSTSENSOR_PMS5003_7003_BME280_0x76 or DUSTSENSOR_PMS5003_7003_BME280_0x77
		if (String(json[i]["sensor"]["sensor_type"]["name"].as<String>()) == "PMS7003" or String(json[i]["sensor"]["sensor_type"]["name"].as<String>()) == "PMS5003") {
#elif defined DUSTSENSOR_SDS011_21
			if (String(json[i]["sensor"]["sensor_type"]["name"].as<String>()) == "SDS021" or String(json[i]["sensor"]["sensor_type"]["name"].as<String>()) == "SDS011") {
#else
			if (String(json[i]["sensor"]["sensor_type"]["name"].as<String>()) == String(temp_DUSTMODEL_Luftdaten)) {
#endif
 			// Serial.println("json[" + String(i) + "][\"sensor\"][\"id\"]: " + String(json[i]["sensor"]["id"].as<String>()));				
 			luftdatenSettings.apiId = int(json[i]["sensor"]["id"].as<int>()); 
 		}
 	}
 }

  if (deviceSettings.debug) {
    // Output to serial monitor
    Serial.print("luftdatenSettings.apiId: ");
    Serial.println(luftdatenSettings.apiId);
    Serial.print("\n");
 }
}

void sendDataToLuftdaten() {
  if (!(luftdatenSettings.enabled)) {
    return;
  }
  sendDUSTDatatoLuftdaten();
  sendTHPDatatoLuftdaten();
  delay(10);
  sendDUSTDatatoMadavide();
  sendTHPDatatoMadavide();
}

void getAPIIDFromLuftdaten() {
	if (getLuftdatenJSON() == true) {
      if (deviceSettings.debug) {
        Serial.println("Luftdaten API data recived!\n");
      }
	  parsingLuftdatenAPIJSON();
      receivedLuftdatenAPIJSON = "";
    } else {
      if (deviceSettings.debug) {
        Serial.println("Luftdaten API conection error!\n");
      }
    }
}