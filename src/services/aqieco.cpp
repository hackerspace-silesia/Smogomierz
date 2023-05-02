#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#endif

#include <ArduinoJson.h>
#include "../../config.h"

#ifdef ARDUINO_ARCH_ESP8266	
void sendDataToAqiEco() {
  if (!(aqiEcoSettings.enabled)) {
    return;
  }

  StaticJsonDocument<768> jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();
#ifdef ARDUINO_ARCH_ESP8266
  json[F("esp8266id")] = String(ESP.getChipId());
#elif defined ARDUINO_ARCH_ESP32
  json[F("esp8266id")] = String((uint32_t)(ESP.getEfuseMac()));
#endif
  json[F("software_version")] = F("Smogly_") + String(serverSoftwareVersion);
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
  } else if (!strcmp(sensorsSettings.dustModel, "SPS30")) {
    JsonObject P0 = sensordatavalues.createNestedObject();
    P0[F("value_type")] = F("SPS30_P0");
    P0[F("value")] = measurementsData.averagePM1;
    JsonObject P1 = sensordatavalues.createNestedObject();
    P1[F("value_type")] = F("SPS30_P1");
    P1[F("value")] = measurementsData.averagePM10;
    JsonObject P2 = sensordatavalues.createNestedObject();
    P2[F("value_type")] = F("SPS30_P2");
    P2[F("value")] = measurementsData.averagePM25;
    JsonObject P4 = sensordatavalues.createNestedObject();
    P4[F("value_type")] = F("SPS30_P4");
    P4[F("value")] = measurementsData.averagePM4;
}

  if (!strcmp(sensorsSettings.thpModel, "BME280")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature[F("value_type")] = F("BME280_temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity[F("value_type")] = F("BME280_humidity");
    humidity[F("value")] = String(measurementsData.humidity);
    JsonObject pressure = sensordatavalues.createNestedObject();
    pressure[F("value_type")] = F("BME280_pressure");
    pressure[F("value")] = String(measurementsData.pressure * 100); //hPa -> Pa
  } else if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature[F("value_type")] = F("BMP280_temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    JsonObject pressure = sensordatavalues.createNestedObject();
    pressure[F("value_type")] = F("BMP280_pressure");
    pressure[F("value")] = String(measurementsData.pressure * 100); //hPa -> Pa
  } else if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature[F("value_type")] = F("HTU21_temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity[F("value_type")] = F("HTU21_humidity");
    humidity[F("value")] = String(measurementsData.humidity);
  } else if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature[F("value_type")] = F("DHT22_temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity[F("value_type")] = F("DHT22_humidity");
    humidity[F("value")] = String(measurementsData.humidity);
  } else if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature[F("value_type")] = F("SHT1x_temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity[F("value_type")] = F("SHT1x_humidity");
    humidity[F("value")] = String(measurementsData.humidity);
  } else if (!strcmp(sensorsSettings.thpModel, "DS18B20")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature[F("value_type")] = F("DS18B20_temperature");
    temperature[F("value")] = String(measurementsData.temperature);
  }

  WiFiClient client;

  Serial.print(F("\nconnecting to "));
  Serial.println(aqiEcoSettings.host);

  if (!client.connect(aqiEcoSettings.host, 80)) {
    Serial.println(F("connection failed"));
    delay(1000);
    return;
  }
  delay(100);

  client.print(F("POST "));
  client.print(aqiEcoSettings.path);
  client.println(F(" HTTP/1.1"));
  client.print(F("Host: "));
  client.println(aqiEcoSettings.host);
  client.println(F("Content-Type: application/json"));
  client.println(F("X-PIN: 1"));
  client.print(F("X-Sensor: smogly-"));
#ifdef ARDUINO_ARCH_ESP8266
client.println(String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
client.println(String((uint32_t)(ESP.getEfuseMac())));
#endif	  
  client.print(F("Content-Length: "));
  client.println(measureJson(json));
  client.println(F("Connection: close"));
  client.println();
  serializeJson(json, client);

  String line = client.readStringUntil('\r');
  Serial.println(line);
  // TODO: Support wrong error (!= 200)

  if (deviceSettings.debug) {
    Serial.println();
    Serial.print(F("POST /u/"));
    Serial.print(aqiEcoSettings.path);
    Serial.println(F(" HTTP/1.1"));
    Serial.print(F("Host: "));
    Serial.println(aqiEcoSettings.host);
    Serial.println(F("Content-Type: application/json"));
    Serial.println(F("X-PIN: 1"));
    Serial.print(F("X-Sensor: smogly-"));
#ifdef ARDUINO_ARCH_ESP8266
Serial.println(String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
Serial.println(String((uint32_t)(ESP.getEfuseMac())));
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


#elif defined ARDUINO_ARCH_ESP32
void sendDataToAqiEco() {
  if (!(aqiEcoSettings.enabled)) {
    return;
  }

  StaticJsonDocument<768> jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();
#ifdef ARDUINO_ARCH_ESP8266
  json["esp8266id"] = String(ESP.getChipId());
#elif defined ARDUINO_ARCH_ESP32
  json["esp8266id"] = String((uint32_t)(ESP.getEfuseMac()));
#endif
  json["software_version"] = "Smogly_" + String(serverSoftwareVersion);
  JsonArray sensordatavalues = json.createNestedArray("sensordatavalues");

  if (!strcmp(sensorsSettings.dustModel, "PMS7003")) {
    JsonObject P0 = sensordatavalues.createNestedObject();
    P0["value_type"] = "PMS_P0";
    P0["value"] = measurementsData.averagePM1;
    JsonObject P1 = sensordatavalues.createNestedObject();
    P1["value_type"] = "PMS_P1";
    P1["value"] = measurementsData.averagePM10;
    JsonObject P2 = sensordatavalues.createNestedObject();
    P2["value_type"] = "PMS_P2";
    P2["value"] = measurementsData.averagePM25;
  } else if (!strcmp(sensorsSettings.dustModel, "SDS011/21")) {
    JsonObject P1 = sensordatavalues.createNestedObject();
    P1["value_type"] = "SDS_P1";
    P1["value"] = measurementsData.averagePM10;
    JsonObject P2 = sensordatavalues.createNestedObject();
    P2["value_type"] = "SDS_P2";
    P2["value"] = measurementsData.averagePM25;
  } else if (!strcmp(sensorsSettings.dustModel, "HPMA115S0")) {
    JsonObject P1 = sensordatavalues.createNestedObject();
    P1["value_type"] = "HPM_P1";
    P1["value"] = measurementsData.averagePM10;
    JsonObject P2 = sensordatavalues.createNestedObject();
    P2["value_type"] = "HPM_P2";
    P2["value"] = measurementsData.averagePM25;
  } else if (!strcmp(sensorsSettings.dustModel, "SPS30")) {
    JsonObject P0 = sensordatavalues.createNestedObject();
    P0["value_type"] = "SPS30_P0";
    P0["value"] = measurementsData.averagePM1;
    JsonObject P1 = sensordatavalues.createNestedObject();
    P1["value_type"] = "SPS30_P1";
    P1["value"] = measurementsData.averagePM10;
    JsonObject P2 = sensordatavalues.createNestedObject();
    P2["value_type"] = "SPS30_P2";
    P2["value"] = measurementsData.averagePM25;
    JsonObject P4 = sensordatavalues.createNestedObject();
    P4["value_type"] = "SPS30_P4";
    P4["value"] = measurementsData.averagePM4;
}

  if (!strcmp(sensorsSettings.thpModel, "BME280")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "BME280_temperature";
    temperature["value"] = String(measurementsData.temperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity["value_type"] = "BME280_humidity";
    humidity["value"] = String(measurementsData.humidity);
    JsonObject pressure = sensordatavalues.createNestedObject();
    pressure["value_type"] = "BME280_pressure";
    pressure["value"] = String(measurementsData.pressure * 100); //hPa -> Pa
  } else if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "BMP280_temperature";
    temperature["value"] = String(measurementsData.temperature);
    JsonObject pressure = sensordatavalues.createNestedObject();
    pressure["value_type"] = "BMP280_pressure";
    pressure["value"] = String(measurementsData.pressure * 100); //hPa -> Pa
  } else if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "HTU21_temperature";
    temperature["value"] = String(measurementsData.temperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity["value_type"] = "HTU21_humidity";
    humidity["value"] = String(measurementsData.humidity);
  } else if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "DHT22_temperature";
    temperature["value"] = String(measurementsData.temperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity["value_type"] = "DHT22_humidity";
    humidity["value"] = String(measurementsData.humidity);
  } else if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "SHT1x_temperature";
    temperature["value"] = String(measurementsData.temperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity["value_type"] = "SHT1x_humidity";
    humidity["value"] = String(measurementsData.humidity);
  } else if (!strcmp(sensorsSettings.thpModel, "DS18B20")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "DS18B20_temperature";
    temperature["value"] = String(measurementsData.temperature);
  }

  WiFiClient client;

  Serial.print("\nconnecting to ");
  Serial.println(aqiEcoSettings.host);

  if (!client.connect(aqiEcoSettings.host, 80)) {
    Serial.println("connection failed");
    delay(1000);
    return;
  }
  delay(100);

  client.print("POST ");
  client.print(aqiEcoSettings.path);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(aqiEcoSettings.host);
  client.println("Content-Type: application/json");
  client.println("X-PIN: 1");
  client.print("X-Sensor: smogly-");
#ifdef ARDUINO_ARCH_ESP8266
client.println(String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
client.println(String((uint32_t)(ESP.getEfuseMac())));
#endif	  
  client.print("Content-Length: ");
  client.println(measureJson(json));
  client.println("Connection: close");
  client.println();
  serializeJson(json, client);

  String line = client.readStringUntil('\r');
  Serial.println(line);
  // TODO: Support wrong error (!= 200)

  if (deviceSettings.debug) {
    Serial.println();
    Serial.print("POST /u/");
    Serial.print(aqiEcoSettings.path);
    Serial.println(" HTTP/1.1");
    Serial.print("Host: ");
    Serial.println(aqiEcoSettings.host);
    Serial.println("Content-Type: application/json");
    Serial.println("X-PIN: 1");
    Serial.print("X-Sensor: smogly-");
#ifdef ARDUINO_ARCH_ESP8266
Serial.println(String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
Serial.println(String((uint32_t)(ESP.getEfuseMac())));
#endif
    Serial.print("Content-Length: ");
    Serial.println(measureJson(json));
    Serial.println("Connection: close");
    Serial.println();
    serializeJsonPretty(json, Serial);
    Serial.println("\n");
    Serial.println(line);
  }
  client.stop();
}
#endif