#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#endif

#include <ArduinoJson.h>
#include "config.h"

#ifdef ARDUINO_ARCH_ESP8266
String luftdatenChipId = String(ESP.getChipId());
#elif defined ARDUINO_ARCH_ESP32
String luftdatenChipId = String((uint32_t)(ESP.getEfuseMac()));
#endif

const char *luftdatenAPIHOST = "api.luftdaten.info";
const char *luftdatenAPIURL = "/v1/push-sensor-data/";
const uint16_t luftdatenAPIPort = 80;

const char *madavideAPIHOST = "api-rrd.madavi.de";
const char *madavideAPIURL = "/data.php";
const uint16_t madavideAPIPort = 80;

// Luftdaten.info - https://luftdaten.info
void sendDUSTLuftdatenJson(JsonObject& json) {
  WiFiClient client;
  Serial.print("\nconnecting to ");
  Serial.println(luftdatenAPIHOST);

  if (!client.connect(luftdatenAPIHOST, luftdatenAPIPort)) {
    Serial.println("connection failed");
    delay(1000);
    return;
  }
  delay(100);
  client.println("POST " + String(luftdatenAPIURL) + " HTTP/1.1");
  client.println("Host: " + String(luftdatenAPIHOST));
  client.println("Content-Type: application/json");
  client.println("X-PIN: 1");
  client.println("X-Sensor: smogomierz-" + luftdatenChipId);
  client.print("Content-Length: ");
  client.println(measureJson(json));
  client.println("Connection: close");
  client.println();
  serializeJson(json, client);

  String line = client.readStringUntil('\r');
  // TODO: Support wrong error (!= 200)

  if (DEBUG) {
    Serial.println();
    Serial.println("POST " + String(luftdatenAPIURL) + " HTTP/1.1");
    Serial.println("Host: " + String(luftdatenAPIHOST));
    Serial.println("Content-Type: application/json");
    Serial.println("X-PIN: 1");
    Serial.println("X-Sensor: smogomierz-" + luftdatenChipId);
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

void sendTHPLuftdatenJson(JsonObject& json) {
  WiFiClient client;
  Serial.print("\nconnecting to ");
  Serial.println(luftdatenAPIHOST);

  if (!client.connect(luftdatenAPIHOST, luftdatenAPIPort)) {
    Serial.println("connection failed");
    delay(1000);
    return;
  }
  delay(100);
  client.println("POST " + String(luftdatenAPIURL) + " HTTP/1.1");
  client.println("Host: " + String(luftdatenAPIHOST));
  client.println("Content-Type: application/json");
  if (!strcmp(THP_MODEL, "BME280")) {
    client.println("X-PIN: 11");
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    client.println("X-PIN: 3");
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    client.println("X-PIN: 7");
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    client.println("X-PIN: 7");
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
    client.println("X-PIN: 12");
  }
  client.println("X-Sensor: smogomierz-" + luftdatenChipId);
  client.print("Content-Length: ");
  client.println(measureJson(json));
  client.println("Connection: close");
  client.println();

  serializeJson(json, client);

  String line = client.readStringUntil('\r');
  // TODO: Support wrong error (!= 200)

  if (DEBUG) {
    Serial.println();
    Serial.println("POST " + String(luftdatenAPIURL) + " HTTP/1.1");
    Serial.println("Host: " + String(luftdatenAPIHOST));
    Serial.println("Content-Type: application/json");
    if (!strcmp(THP_MODEL, "BME280")) {
      Serial.println("X-PIN: 11");
    } else if (!strcmp(THP_MODEL, "BMP280")) {
      Serial.println("X-PIN: 3");
    } else if (!strcmp(THP_MODEL, "HTU21")) {
      Serial.println("X-PIN: 7");
    } else if (!strcmp(THP_MODEL, "DHT22")) {
      Serial.println("X-PIN: 7");
    } else if (!strcmp(THP_MODEL, "SHT1x")) {
      Serial.println("X-PIN: 12");
    }
    Serial.println("X-Sensor: smogomierz-" + luftdatenChipId);
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

void sendDUSTDatatoLuftdaten(int averagePM1, int averagePM25, int averagePM10) {
  if (strcmp(DUST_MODEL, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json["software_version"] = "Smogomierz_" + String(SOFTWAREVERSION);
    JsonArray sensordatavalues = json.createNestedArray("sensordatavalues");
    if (!strcmp(DUST_MODEL, "PMS7003")) {
      JsonObject P0 = sensordatavalues.createNestedObject();
      P0["value_type"] = "P0";
      P0["value"] = averagePM1;
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1["value_type"] = "P1";
      P1["value"] = averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2["value_type"] = "P2";
      P2["value"] = averagePM25;
    } else {
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1["value_type"] = "P1";
      P1["value"] = averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2["value_type"] = "P2";
      P2["value"] = averagePM25;
    }
    sendDUSTLuftdatenJson(json);
  }
}

void sendTHPDatatoLuftdaten(float currentTemperature, float currentPressure, float currentHumidity) {
  if (strcmp(THP_MODEL, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json["software_version"] = "Smogomierz_" + String(SOFTWAREVERSION);
    JsonArray sensordatavalues = json.createNestedArray("sensordatavalues");
    if (!strcmp(THP_MODEL, "BME280")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity["value_type"] = "humidity";
      humidity["value"] = String(currentHumidity);
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure["value_type"] = "pressure";
      pressure["value"] = String(currentPressure * 100); //hPa -> Pa
    } else if (!strcmp(THP_MODEL, "BMP280")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure["value_type"] = "pressure";
      pressure["value"] = String(currentPressure * 100); //hPa -> Pa
    } else if (!strcmp(THP_MODEL, "HTU21")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity["value_type"] = "humidity";
      humidity["value"] = String(currentHumidity);
    } else if (!strcmp(THP_MODEL, "DHT22")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity["value_type"] = "humidity";
      humidity["value"] = String(currentHumidity);
    } else if (!strcmp(THP_MODEL, "SHT1x")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity["value_type"] = "humidity";
      humidity["value"] = String(currentHumidity);
    }
    sendTHPLuftdatenJson(json);
  }
}

// Madavi.de - https://www.madavi.de/sensor/graph.php
void sendDUSTMadavideJson(JsonObject& json) {
  WiFiClient client;
  Serial.print("\nconnecting to ");
  Serial.println(madavideAPIHOST);

  if (!client.connect(madavideAPIHOST, madavideAPIPort)) {
    Serial.println("connection failed");
    delay(1000);
    return;
  }
  delay(100);
  client.println("POST " + String(madavideAPIURL) + " HTTP/1.1");
  client.println("Host: " + String(madavideAPIHOST));
  client.println("Content-Type: application/json");
  client.println("X-PIN: 1");
  client.println("X-Sensor: smogomierz-" + luftdatenChipId);
  client.print("Content-Length: ");
  client.println(measureJson(json));
  client.println("Connection: close");
  client.println();

  serializeJson(json, client);

  String line = client.readStringUntil('\r');
  // TODO: Support wrong error (!= 200)

  if (DEBUG) {
    Serial.println();
    Serial.println("POST " + String(madavideAPIURL) + " HTTP/1.1");
    Serial.println("Host: " + String(madavideAPIHOST));
    Serial.println("Content-Type: application/json");
    Serial.println("X-PIN: 1");
    Serial.println("X-Sensor: smogomierz-" + luftdatenChipId);
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

void sendTHPMadavideJson(JsonObject& json) {
  WiFiClient client;
  Serial.print("\nconnecting to ");
  Serial.println(madavideAPIHOST);

  if (!client.connect(madavideAPIHOST, madavideAPIPort)) {
    Serial.println("connection failed");
    delay(1000);
    return;
  }
  delay(100);
  client.println("POST " + String(madavideAPIURL) + " HTTP/1.1");
  client.println("Host: " + String(madavideAPIHOST));
  client.println("Content-Type: application/json");
  if (!strcmp(THP_MODEL, "BME280")) {
    client.println("X-PIN: 11");
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    client.println("X-PIN: 3");
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    client.println("X-PIN: 7");
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    client.println("X-PIN: 7");
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
    client.println("X-PIN: 12");
  }
  client.println("X-Sensor: smogomierz-" + luftdatenChipId);
  client.print("Content-Length: ");
  client.println(measureJson(json));
  client.println("Connection: close");
  client.println();

  serializeJson(json, client);

  String line = client.readStringUntil('\r');
  // TODO: Support wrong error (!= 200)

  if (DEBUG) {
    Serial.println();
    Serial.println("POST " + String(madavideAPIURL) + " HTTP/1.1");
    Serial.println("Host: " + String(madavideAPIHOST));
    Serial.println("Content-Type: application/json");
    if (!strcmp(THP_MODEL, "BME280")) {
      Serial.println("X-PIN: 11");
    } else if (!strcmp(THP_MODEL, "BMP280")) {
      Serial.println("X-PIN: 3");
    } else if (!strcmp(THP_MODEL, "HTU21")) {
      Serial.println("X-PIN: 7");
    } else if (!strcmp(THP_MODEL, "DHT22")) {
      Serial.println("X-PIN: 7");
    } else if (!strcmp(THP_MODEL, "SHT1x")) {
      Serial.println("X-PIN: 12");
    }
    Serial.println("X-Sensor: smogomierz-" + luftdatenChipId);
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

void sendDUSTDatatoMadavide(int averagePM1, int averagePM25, int averagePM10) {
  if (strcmp(DUST_MODEL, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json["software_version"] = "Smogomierz_" + String(SOFTWAREVERSION);
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
    sendDUSTMadavideJson(json);
  }
}

void sendTHPDatatoMadavide(float currentTemperature, float currentPressure, float currentHumidity) {
  if (strcmp(THP_MODEL, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json["software_version"] = "Smogomierz_" + String(SOFTWAREVERSION);
    JsonArray sensordatavalues = json.createNestedArray("sensordatavalues");
    if (!strcmp(THP_MODEL, "BME280")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity["value_type"] = "humidity";
      humidity["value"] = String(currentHumidity);
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure["value_type"] = "pressure";
      pressure["value"] = String(currentPressure * 100); //hPa -> Pa
    } else if (!strcmp(THP_MODEL, "BMP280")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure["value_type"] = "pressure";
      pressure["value"] = String(currentPressure * 100); //hPa -> Pa
    } else if (!strcmp(THP_MODEL, "HTU21")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity["value_type"] = "humidity";
      humidity["value"] = String(currentHumidity);
    } else if (!strcmp(THP_MODEL, "DHT22")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity["value_type"] = "humidity";
      humidity["value"] = String(currentHumidity);
    } else if (!strcmp(THP_MODEL, "SHT1x")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity["value_type"] = "humidity";
      humidity["value"] = String(currentHumidity);
    }
    sendTHPMadavideJson(json);
  }
}

void sendDataToLuftdaten(float currentTemperature, float currentPressure, float currentHumidity, int averagePM1, int averagePM25, int averagePM4, int averagePM10) {
  if (!(LUFTDATEN_ON)) {
    return;
  }	
  sendDUSTDatatoLuftdaten(averagePM1, averagePM25, averagePM10);
  sendTHPDatatoLuftdaten(currentTemperature, currentPressure, currentHumidity);
  delay(10);
  sendDUSTDatatoMadavide(averagePM1, averagePM25, averagePM10);
  sendTHPDatatoMadavide(currentTemperature, currentPressure, currentHumidity);
}