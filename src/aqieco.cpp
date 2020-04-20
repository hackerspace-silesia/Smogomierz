#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#endif

#include <ArduinoJson.h>
#include "config.h"

#ifdef ARDUINO_ARCH_ESP8266
String aqiEcoChipId = String(ESP.getChipId());
#elif defined ARDUINO_ARCH_ESP32
String aqiEcoChipId = String((uint32_t)(ESP.getEfuseMac()));
#endif

void sendDataToAqiEco(float currentTemperature, float currentPressure, float currentHumidity, int averagePM1, int averagePM25, int averagePM4, int averagePM10) {
  if (!(AQI_ECO_ON)) {
    return;
  }

  StaticJsonDocument<768> jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();
  json["esp8266id"] = aqiEcoChipId;
  json["software_version"] = "Smogly_" + String(SOFTWAREVERSION);
  JsonArray sensordatavalues = json.createNestedArray("sensordatavalues");

  if (!strcmp(DUST_MODEL, "PMS7003")) {
    JsonObject P0 = sensordatavalues.createNestedObject();
    P0["value_type"] = "PMS_P0";
    P0["value"] = averagePM1;
    JsonObject P1 = sensordatavalues.createNestedObject();
    P1["value_type"] = "PMS_P1";
    P1["value"] = averagePM10;
    JsonObject P2 = sensordatavalues.createNestedObject();
    P2["value_type"] = "PMS_P2";
    P2["value"] = averagePM25;
  } else if (!strcmp(DUST_MODEL, "SDS011/21")) {
    JsonObject P1 = sensordatavalues.createNestedObject();
    P1["value_type"] = "SDS_P1";
    P1["value"] = averagePM10;
    JsonObject P2 = sensordatavalues.createNestedObject();
    P2["value_type"] = "SDS_P2";
    P2["value"] = averagePM25;
  } else if (!strcmp(DUST_MODEL, "HPMA115S0")) {
    JsonObject P1 = sensordatavalues.createNestedObject();
    P1["value_type"] = "HPM_P1";
    P1["value"] = averagePM10;
    JsonObject P2 = sensordatavalues.createNestedObject();
    P2["value_type"] = "HPM_P2";
    P2["value"] = averagePM25;
  }

  if (!strcmp(THP_MODEL, "BME280")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "BME280_temperature";
    temperature["value"] = String(currentTemperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity["value_type"] = "BME280_humidity";
    humidity["value"] = String(currentHumidity);
    JsonObject pressure = sensordatavalues.createNestedObject();
    pressure["value_type"] = "BME280_pressure";
    pressure["value"] = String(currentPressure * 100); //hPa -> Pa
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "BMP280_temperature";
    temperature["value"] = String(currentTemperature);
    JsonObject pressure = sensordatavalues.createNestedObject();
    pressure["value_type"] = "BMP280_pressure";
    pressure["value"] = String(currentPressure * 100); //hPa -> Pa
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "HTU21_temperature";
    temperature["value"] = String(currentTemperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity["value_type"] = "HTU21_humidity";
    humidity["value"] = String(currentHumidity);
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "DHT22_temperature";
    temperature["value"] = String(currentTemperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity["value_type"] = "DHT22_humidity";
    humidity["value"] = String(currentHumidity);
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "SHT1x_temperature";
    temperature["value"] = String(currentTemperature);
    JsonObject humidity = sensordatavalues.createNestedObject();
    humidity["value_type"] = "SHT1x_humidity";
    humidity["value"] = String(currentHumidity);
  } else if (!strcmp(THP_MODEL, "DS18B20")) {
    JsonObject temperature = sensordatavalues.createNestedObject();
    temperature["value_type"] = "DS18B20_temperature";
    temperature["value"] = String(currentTemperature);
  }

  WiFiClient client;

  Serial.print("\nconnecting to ");
  Serial.println(AQI_ECO_HOST);

  if (!client.connect(AQI_ECO_HOST, 80)) {
    Serial.println("connection failed");
    delay(1000);
    return;
  }
  delay(100);

  client.print("POST ");
  client.print(AQI_ECO_PATH);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(AQI_ECO_HOST);
  client.println("Content-Type: application/json");
  client.println("X-PIN: 1");
  client.print("X-Sensor: smogly-");
  client.println(aqiEcoChipId);
  client.print("Content-Length: ");
  client.println(measureJson(json));
  client.println("Connection: close");
  client.println();
  serializeJson(json, client);

  String line = client.readStringUntil('\r');
  Serial.println(line);
  // TODO: Support wrong error (!= 200)

  if (DEBUG) {
    Serial.println();
    Serial.print("POST /u/");
    Serial.print(AQI_ECO_PATH);
    Serial.println(" HTTP/1.1");
    Serial.print("Host: ");
    Serial.println(AQI_ECO_HOST);
    Serial.println("Content-Type: application/json");
    Serial.println("X-PIN: 1");
    Serial.print("X-Sensor: smogly-");
    Serial.println(aqiEcoChipId);
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