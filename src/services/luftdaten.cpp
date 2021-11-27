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

#ifdef ARDUINO_ARCH_ESP8266
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

  if (DEBUG) {
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
  if (!strcmp(THP_MODEL, "BME280")) {
    client.println(F("X-PIN: 11"));
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    client.println(F("X-PIN: 3"));
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    client.println(F("X-PIN: 7"));
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    client.println(F("X-PIN: 7"));
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
    client.println(F("X-PIN: 12"));
  } else if (!strcmp(THP_MODEL, "DS18B20")) {
    client.println(F("X-PIN: 13"));
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

  if (DEBUG) {
    Serial.println();
    Serial.println(F("POST ") + String(luftdatenAPIURL) + F(" HTTP/1.1"));
    Serial.println(F("Host: ") + String(luftdatenAPIHOST));
    Serial.println(F("Content-Type: application/json"));
    if (!strcmp(THP_MODEL, "BME280")) {
      Serial.println(F("X-PIN: 11"));
    } else if (!strcmp(THP_MODEL, "BMP280")) {
      Serial.println(F("X-PIN: 3"));
    } else if (!strcmp(THP_MODEL, "HTU21")) {
      Serial.println(F("X-PIN: 7"));
    } else if (!strcmp(THP_MODEL, "DHT22")) {
      Serial.println(F("X-PIN: 7"));
    } else if (!strcmp(THP_MODEL, "SHT1x")) {
      Serial.println(F("X-PIN: 12"));
    } else if (!strcmp(THP_MODEL, "DS18B20")) {
      Serial.println(F("X-PIN: 13"));
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

void sendDUSTDatatoLuftdaten(unsigned short & averagePM1, unsigned short & averagePM25, unsigned short & averagePM10) {
  if (strcmp(DUST_MODEL, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("software_version")] = F("Smogomierz_") + String(SOFTWAREVERSION);
    JsonArray sensordatavalues = json.createNestedArray(F("sensordatavalues"));
    if (!strcmp(DUST_MODEL, "PMS7003")) {
      JsonObject P0 = sensordatavalues.createNestedObject();
      P0[F("value_type")] = F("P0");
      P0[F("value")] = averagePM1;
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1[F("value_type")] = F("P1");
      P1[F("value")] = averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2[F("value_type")] = F("P2");
      P2[F("value")] = averagePM25;
    } else {
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1[F("value_type")] = F("P1");
      P1[F("value")] = averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2[F("value_type")] = F("P2");
      P2[F("value")] = averagePM25;
    }
    sendDUSTLuftdatenJson(json);
  }
}

void sendTHPDatatoLuftdaten(float & currentTemperature, float & currentPressure, float & currentHumidity) {
  if (strcmp(THP_MODEL, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("software_version")] = F("Smogomierz_") + String(SOFTWAREVERSION);
    JsonArray sensordatavalues = json.createNestedArray(F("sensordatavalues"));
    if (!strcmp(THP_MODEL, "BME280")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(currentHumidity);
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure[F("value_type")] = F("pressure");
      pressure[F("value")] = String(currentPressure * 100); //hPa -> Pa
    } else if (!strcmp(THP_MODEL, "BMP280")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure[F("value_type")] = F("pressure");
      pressure[F("value")] = String(currentPressure * 100); //hPa -> Pa
    } else if (!strcmp(THP_MODEL, "HTU21")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(currentHumidity);
    } else if (!strcmp(THP_MODEL, "DHT22")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(currentHumidity);
    } else if (!strcmp(THP_MODEL, "SHT1x")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(currentHumidity);
    } else if (!strcmp(THP_MODEL, "DS18B20")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
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

  if (DEBUG) {
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
  if (!strcmp(THP_MODEL, "BME280")) {
    client.println(F("X-PIN: 11"));
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    client.println(F("X-PIN: 3"));
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    client.println(F("X-PIN: 7"));
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    client.println(F("X-PIN: 7"));
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
    client.println(F("X-PIN: 12"));
  } else if (!strcmp(THP_MODEL, "DS18B20")) {
    client.println(F("X-PIN: 13"));
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

  if (DEBUG) {
    Serial.println();
    Serial.println(F("POST ") + String(madavideAPIURL) + F(" HTTP/1.1"));
    Serial.println(F("Host: ") + String(madavideAPIHOST));
    Serial.println(F("Content-Type: application/json"));
    if (!strcmp(THP_MODEL, "BME280")) {
      Serial.println(F("X-PIN: 11"));
    } else if (!strcmp(THP_MODEL, "BMP280")) {
      Serial.println(F("X-PIN: 3"));
    } else if (!strcmp(THP_MODEL, "HTU21")) {
      Serial.println(F("X-PIN: 7"));
    } else if (!strcmp(THP_MODEL, "DHT22")) {
      Serial.println(F("X-PIN: 7"));
    } else if (!strcmp(THP_MODEL, "SHT1x")) {
      Serial.println(F("X-PIN: 12"));
    } else if (!strcmp(THP_MODEL, "DS18B20")) {
      Serial.println(F("X-PIN: 13"));
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

void sendDUSTDatatoMadavide(unsigned short & averagePM1, unsigned short & averagePM25, unsigned short & averagePM10) {
  if (strcmp(DUST_MODEL, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("software_version")] = F("Smogomierz_") + String(SOFTWAREVERSION);
    JsonArray sensordatavalues = json.createNestedArray(F("sensordatavalues"));
    if (!strcmp(DUST_MODEL, "PMS7003")) {
      JsonObject P0 = sensordatavalues.createNestedObject();
      P0[F("value_type")] = F("PMS_P0");
      P0[F("value")] = averagePM1;
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1[F("value_type")] = F("PMS_P1");
      P1[F("value")] = averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2[F("value_type")] = F("PMS_P2");
      P2[F("value")] = averagePM25;
    } else if (!strcmp(DUST_MODEL, "SDS011/21")) {
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1[F("value_type")] = F("SDS_P1");
      P1[F("value")] = averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2[F("value_type")] = F("SDS_P2");
      P2[F("value")] = averagePM25;
    } else if (!strcmp(DUST_MODEL, "HPMA115S0")) {
      JsonObject P1 = sensordatavalues.createNestedObject();
      P1[F("value_type")] = F("HPM_P1");
      P1[F("value")] = averagePM10;
      JsonObject P2 = sensordatavalues.createNestedObject();
      P2[F("value_type")] = F("HPM_P2");
      P2[F("value")] = averagePM25;
    }
    sendDUSTMadavideJson(json);
  }
}

void sendTHPDatatoMadavide(float & currentTemperature, float & currentPressure, float & currentHumidity) {
  if (strcmp(THP_MODEL, "Non")) {
    StaticJsonDocument<600> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("software_version")] = F("Smogomierz_") + String(SOFTWAREVERSION);
    JsonArray sensordatavalues = json.createNestedArray(F("sensordatavalues"));
    if (!strcmp(THP_MODEL, "BME280")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(currentHumidity);
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure[F("value_type")] = F("pressure");
      pressure[F("value")] = String(currentPressure * 100); //hPa -> Pa
    } else if (!strcmp(THP_MODEL, "BMP280")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
      JsonObject pressure = sensordatavalues.createNestedObject();
      pressure[F("value_type")] = F("pressure");
      pressure[F("value")] = String(currentPressure * 100); //hPa -> Pa
    } else if (!strcmp(THP_MODEL, "HTU21")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(currentHumidity);
    } else if (!strcmp(THP_MODEL, "DHT22")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(currentHumidity);
    } else if (!strcmp(THP_MODEL, "SHT1x")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
      JsonObject humidity = sensordatavalues.createNestedObject();
      humidity[F("value_type")] = F("humidity");
      humidity[F("value")] = String(currentHumidity);
    } else if (!strcmp(THP_MODEL, "DS18B20")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature[F("value_type")] = F("temperature");
      temperature[F("value")] = String(currentTemperature);
    }
    sendTHPMadavideJson(json);
  }
}
#elif defined ARDUINO_ARCH_ESP32
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
#ifdef ARDUINO_ARCH_ESP8266
  client.println("X-Sensor: smogomierz-" + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
  client.println("X-Sensor: smogomierz-" + String((uint32_t)(ESP.getEfuseMac())));
#endif
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
#ifdef ARDUINO_ARCH_ESP8266
    Serial.println("X-Sensor: smogomierz-" + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
    Serial.println("X-Sensor: smogomierz-" + String((uint32_t)(ESP.getEfuseMac())));
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
  } else if (!strcmp(THP_MODEL, "DS18B20")) {
    client.println("X-PIN: 13");
  }

#ifdef ARDUINO_ARCH_ESP8266
  client.println("X-Sensor: smogomierz-" + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
  client.println("X-Sensor: smogomierz-" + String((uint32_t)(ESP.getEfuseMac())));
#endif
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
    } else if (!strcmp(THP_MODEL, "DS18B20")) {
      Serial.println("X-PIN: 13");
    }
#ifdef ARDUINO_ARCH_ESP8266
    Serial.println("X-Sensor: smogomierz-" + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
    Serial.println("X-Sensor: smogomierz-" + String((uint32_t)(ESP.getEfuseMac())));
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

void sendDUSTDatatoLuftdaten(unsigned short & averagePM1, unsigned short & averagePM25, unsigned short & averagePM10) {
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

void sendTHPDatatoLuftdaten(float & currentTemperature, float & currentPressure, float & currentHumidity) {
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
    } else if (!strcmp(THP_MODEL, "DS18B20")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
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
#ifdef ARDUINO_ARCH_ESP8266
  client.println("X-Sensor: smogomierz-" + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
  client.println("X-Sensor: smogomierz-" + String((uint32_t)(ESP.getEfuseMac())));
#endif
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
#ifdef ARDUINO_ARCH_ESP8266
    Serial.println("X-Sensor: smogomierz-" + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
    Serial.println("X-Sensor: smogomierz-" + String((uint32_t)(ESP.getEfuseMac())));
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
  } else if (!strcmp(THP_MODEL, "DS18B20")) {
    client.println("X-PIN: 13");
  }
#ifdef ARDUINO_ARCH_ESP8266
  client.println("X-Sensor: smogomierz-" + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
  client.println("X-Sensor: smogomierz-" + String((uint32_t)(ESP.getEfuseMac())));
#endif
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
    } else if (!strcmp(THP_MODEL, "DS18B20")) {
      Serial.println("X-PIN: 13");
    }
#ifdef ARDUINO_ARCH_ESP8266
    Serial.println("X-Sensor: smogomierz-" + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
    Serial.println("X-Sensor: smogomierz-" + String((uint32_t)(ESP.getEfuseMac())));
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

void sendDUSTDatatoMadavide(unsigned short & averagePM1, unsigned short & averagePM25, unsigned short & averagePM10) {
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

void sendTHPDatatoMadavide(float & currentTemperature, float & currentPressure, float & currentHumidity) {
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
    } else if (!strcmp(THP_MODEL, "DS18B20")) {
      JsonObject temperature = sensordatavalues.createNestedObject();
      temperature["value_type"] = "temperature";
      temperature["value"] = String(currentTemperature);
    }
    sendTHPMadavideJson(json);
  }
}
#endif

bool getLuftdatenJSON(float LATITUDE, float LONGITUDE) {
	String url_string = ("http://data.sensor.community/airrohr/v1/filter/box=" + String(LATITUDE, 6) + "00000," + String(LONGITUDE, 6) + "00000," + String(LATITUDE, 6) + "00000," + String(LONGITUDE, 6) + "00000");
		
    // Check WiFi Status
    if (WiFi.status() == WL_CONNECTED) {
      if (DEBUG) {
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

if (DEBUG) {
    // serializeJsonPretty(json, Serial);
	// Serial.println("\n");
	    }
//Serial.println("json.size(): " + String(json.size())); 
		
#ifdef DUSTSENSOR_HPMA115S0
String temp_DUSTMODEL_Luftdaten = "HPM";
#elif defined DUSTSENSOR_SPS30
String temp_DUSTMODEL_Luftdaten = "SPS30";
#else // If no dust sensor has been defined - use DUSTSENSOR_PMS5003_7003_BME280_0x76
String temp_DUSTMODEL_Luftdaten = "PMS7003";
#endif

 for (int i = 0; i < json.size(); i++) {	
 	if (String(json[i]["sensor"]["id"].as<String>()) != "null") {
#ifdef DUSTSENSOR_PMS5003_7003_BME280_0x76 || DUSTSENSOR_PMS5003_7003_BME280_0x77
		if (String(json[i]["sensor"]["sensor_type"]["name"].as<String>()) == "PMS7003" or String(json[i]["sensor"]["sensor_type"]["name"].as<String>()) == "PMS5003") {
#elif defined DUSTSENSOR_SDS011_21
			if (String(json[i]["sensor"]["sensor_type"]["name"].as<String>()) == "SDS021" or String(json[i]["sensor"]["sensor_type"]["name"].as<String>()) == "SDS011") {
#else
			if (String(json[i]["sensor"]["sensor_type"]["name"].as<String>()) == String(temp_DUSTMODEL_Luftdaten)) {
#endif
 			// Serial.println("json[" + String(i) + "][\"sensor\"][\"id\"]: " + String(json[i]["sensor"]["id"].as<String>()));				
 			LUFTDATEN_APIID = int(json[i]["sensor"]["id"].as<int>()); 
 		}
 	}
 }

  if (DEBUG) {
    // Output to serial monitor
    Serial.print("LUFTDATEN_APIID: ");
    Serial.println(LUFTDATEN_APIID);
    Serial.print("\n");
 }
}

void sendDataToLuftdaten(float & currentTemperature, float & currentPressure, float & currentHumidity, unsigned short & averagePM1, unsigned short & averagePM25, unsigned short & averagePM4, unsigned short & averagePM10) {
  if (!(LUFTDATEN_ON)) {
    return;
  }
  sendDUSTDatatoLuftdaten(averagePM1, averagePM25, averagePM10);
  sendTHPDatatoLuftdaten(currentTemperature, currentPressure, currentHumidity);
  delay(10);
  sendDUSTDatatoMadavide(averagePM1, averagePM25, averagePM10);
  sendTHPDatatoMadavide(currentTemperature, currentPressure, currentHumidity);
}

void getAPIIDFromLuftdaten() {
	if (getLuftdatenJSON(atof(LATITUDE), atof(LONGITUDE)) == true) {
      if (DEBUG) {
        Serial.println("Luftdaten API data recived!\n");
      }
	  parsingLuftdatenAPIJSON();
      receivedLuftdatenAPIJSON = "";
    } else {
      if (DEBUG) {
        Serial.println("Luftdaten API conection error!\n");
      }
    }
}
