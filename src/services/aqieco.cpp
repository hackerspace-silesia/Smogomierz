#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#endif

#include <ArduinoJson.h>
#include <map>
#include <vector>
#include "../../config.h"

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

  // create a dictionary to map sensor models to their respective values
  std::map<std::string, std::vector<std::pair<std::string, float>>> sensor_map{
    {F("PMS7003"), {{F("PMS_P0"), measurementsData.averagePM1}, {F("PMS_P1"), measurementsData.averagePM10}, {F("PMS_P2"), measurementsData.averagePM25}}},
    {F("SDS011/21"), {{F("SDS_P1"), measurementsData.averagePM10}, {F("SDS_P2"), measurementsData.averagePM25}}},
    {F("HPMA115S0"), {{F("HPM_P1"), measurementsData.averagePM10}, {F("HPM_P2"), measurementsData.averagePM25}}},
    {F("SPS30"), {{F("SPS30_P0"), measurementsData.averagePM1}, {F("SPS30_P1"), measurementsData.averagePM10}, {F("SPS30_P2"), measurementsData.averagePM25}, {F("SPS30_P4"), measurementsData.averagePM4}}}
  };

  // use a switch statement to create nested JSON objects
  switch(sensor_map.count(sensorsSettings.dustModel)) {
    case 1:
      for (auto &p : sensor_map[sensorsSettings.dustModel]) {
        JsonObject obj = sensordatavalues.createNestedObject();
        obj[F("value_type")] = p.first.c_str();
        obj[F("value")] = p.second;
      }
      break;  
    default:
      break;
  }

  JsonObject temperature = sensordatavalues.createNestedObject();
  JsonObject humidity = sensordatavalues.createNestedObject();
  JsonObject pressure = sensordatavalues.createNestedObject();
  if (!strcmp(sensorsSettings.thpModel, "BME280")) {
    temperature[F("value_type")] = F("BME280_temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    humidity[F("value_type")] = F("BME280_humidity");
    humidity[F("value")] = String(measurementsData.humidity);
    pressure[F("value_type")] = F("BME280_pressure");
    pressure[F("value")] = String(measurementsData.pressure * 100); //hPa -> Pa
  } else if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
    temperature[F("value_type")] = F("BMP280_temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    pressure[F("value_type")] = F("BMP280_pressure");
    pressure[F("value")] = String(measurementsData.pressure * 100); //hPa -> Pa
  } else if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
    temperature[F("value_type")] = F("HTU21_temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    humidity[F("value_type")] = F("HTU21_humidity");
    humidity[F("value")] = String(measurementsData.humidity);
  } else if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
    temperature[F("value_type")] = F("DHT22_temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    humidity[F("value_type")] = F("DHT22_humidity");
    humidity[F("value")] = String(measurementsData.humidity);
  } else if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
    temperature[F("value_type")] = F("SHT1x_temperature");
    temperature[F("value")] = String(measurementsData.temperature);
    humidity[F("value_type")] = F("SHT1x_humidity");
    humidity[F("value")] = String(measurementsData.humidity);
  } else if (!strcmp(sensorsSettings.thpModel, "DS18B20")) {
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