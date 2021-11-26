#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
// #include <TZ.h>

#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClientSecure.h>
#endif

#include <ArduinoJson.h>
#include "../../config.h"

const char *airMonitorServerName PROGMEM = "airmonitor.pl";

#ifdef ARDUINO_ARCH_ESP8266
const int airMonitorPort PROGMEM = 80; // HTTP
#elif defined ARDUINO_ARCH_ESP32
const int airMonitorPort PROGMEM = 443; // HTTPS!
#endif
/*
   This is lets-encrypt-x3-cross-signed.pem
    Certificate - airmonitor.pl at https://certlogik.com/ssl-checker/
    UPDATED: 12.07.2021
*/

#ifdef ARDUINO_ARCH_ESP8266
#elif defined ARDUINO_ARCH_ESP32
  static const char trustRoot_airmonitor[] PROGMEM = R"EOF(
  -----BEGIN CERTIFICATE-----
  MIIF4TCCBMmgAwIBAgIQAVJECA+oWVpzSn+X60jTPzANBgkqhkiG9w0BAQsFADBG
  MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRUwEwYDVQQLEwxTZXJ2ZXIg
  Q0EgMUIxDzANBgNVBAMTBkFtYXpvbjAeFw0yMTA1MjQwMDAwMDBaFw0yMjA2MjIy
  MzU5NTlaMBgxFjAUBgNVBAMTDWFpcm1vbml0b3IucGwwggEiMA0GCSqGSIb3DQEB
  AQUAA4IBDwAwggEKAoIBAQCtoj7r9Dh/s06aqswppDclCGPUbjQi40P9ADW8Kz9F
  2PEWLpOHNCZ5NILM8+U608vp+FLZD7hXBiBSXNJDkFti54JdjuxQURjE4h2eyCpS
  K5GbV5/yZvWeFNui9mGK/M06IoN/mzLSTBpy5m6HawkKq9W+QrGnvXtdJQ0c/ZG8
  i+ocLJH62TZMzemfYlv5nBi0WlpIJ52d0Tf/NmVAgsnw3dWyrSi+kSycS7YxfeyO
  Q6XVnLFxr+kn552JJnr0d0okqRnTyKKvHgRg/XElKdL2Ko3OGvz4jQ3S6pRQATEk
  HTQvOW3DVEqXei8I7Jo9Foob3XqvH/AxdOuGY03aXPVHAgMBAAGjggL3MIIC8zAf
  BgNVHSMEGDAWgBRZpGYGUqB7lZI8o5QHJ5Z0W/k90DAdBgNVHQ4EFgQUvbPnx/QV
  FzWeDAutsekLifTwHVgwKQYDVR0RBCIwIIINYWlybW9uaXRvci5wbIIPKi5haXJt
  b25pdG9yLnBsMA4GA1UdDwEB/wQEAwIFoDAdBgNVHSUEFjAUBggrBgEFBQcDAQYI
  KwYBBQUHAwIwOwYDVR0fBDQwMjAwoC6gLIYqaHR0cDovL2NybC5zY2ExYi5hbWF6
  b250cnVzdC5jb20vc2NhMWIuY3JsMBMGA1UdIAQMMAowCAYGZ4EMAQIBMHUGCCsG
  AQUFBwEBBGkwZzAtBggrBgEFBQcwAYYhaHR0cDovL29jc3Auc2NhMWIuYW1hem9u
  dHJ1c3QuY29tMDYGCCsGAQUFBzAChipodHRwOi8vY3J0LnNjYTFiLmFtYXpvbnRy
  dXN0LmNvbS9zY2ExYi5jcnQwDAYDVR0TAQH/BAIwADCCAX4GCisGAQQB1nkCBAIE
  ggFuBIIBagFoAHYAKXm+8J45OSHwVnOfY6V35b5XfZxgCvj5TV0mXCVdx4QAAAF5
  nGN7YgAABAMARzBFAiBwpeNKU24j+WMzRtusonm3l0GFwbTeV++cOXz8TKuV7wIh
  AIfEgxfLkDSDW+3N2Q48Z0Rk2V6rLeQH3UGLQPr/QvHLAHYAIkVFB1lVJFaWP6Ev
  8fdthuAjJmOtwEt/XcaDXG7iDwIAAAF5nGN7BgAABAMARzBFAiEAzWFN4Sl4ZfnX
  pIIn7oDHzCoKaoRZvChcsCsaxwaR46UCICWn0oNj9SfNJYzY2XxkZg/RPa1tWM1b
  AfzMMGLyRsPTAHYAUaOw9f0BeZxWbbg3eI8MpHrMGyfL956IQpoN/tSLBeUAAAF5
  nGN7SQAABAMARzBFAiEAhm9ZbLfiV7YdrzU7vSIsrDwUX42uQyNpz7I4VLqg2R8C
  IGGiryvZ6kzKjTXlnAwx91e2H1vRDl7DaOS/RYIfTPnnMA0GCSqGSIb3DQEBCwUA
  A4IBAQBH4ZR77/C9rspVNBaDyEUvieyTUfH+oMYsTsrKEaRlzNe2hpDoASRu8Sm3
  1w+jP0h7R+cxfQo2nZ6TVTZrbzqX2D/GvfzUdNl4eei8gzIgwd+iL2eNXo3AhFNz
  lwYMxqFShDSNSgpFdlMCld0s8AFoTYJo1btUyEC+TuVpRdhfrJnH7KqDLdv3VyLE
  x/UP9GLcj2wXU89hIy3LtJb4WyhuwKMssEFlnitR1u8ZtbqreWlopbR1tTyPcoW+
  NMFG2qCOcvVZIaOvQhqb+Dx9oBdzvbQ2g8nQu0qpUpK6k337NbljMuP+x3LiK5Yz
  8zR5DqISMrbtLeQRznOCSQU5vbAe
  -----END CERTIFICATE-----
  )EOF";

  // X509List cert(trustRoot_airmonitor);
#endif

// Last update: 13.07.2021 - https://www.grc.com/fingerprints.htm
#ifdef ARDUINO_ARCH_ESP8266
const char fingerprint_airMonitor[] PROGMEM = "EC 15 73 25 AF 1C DD 01 10 24 03 3D A6 46 C8 40 51 12 5C 13"; // https://airmonitor.pl/prod/measurements
#elif defined ARDUINO_ARCH_ESP32

#endif

// Set time via NTP, as required for x.509 validation
/*
  void setUpdateClock_airmonitor() {
  #ifdef ARDUINO_ARCH_ESP8266
  configTime(TZ_Europe_Warsaw, "pool.ntp.org");
    //configTime(0, 0, "pool.ntp.org", "pool.ntp.org");  // UTC
  #elif defined ARDUINO_ARCH_ESP32
  configTime(0, 0, "pool.ntp.org", "pool.ntp.org");  // UTC
  #endif

  Serial.print(F("Waiting for NTP time sync: "));
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    yield();
    delay(500);
    Serial.print(F("."));
    now = time(nullptr);
  }

  Serial.println(F(""));
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
  }
*/

#ifdef ARDUINO_ARCH_ESP8266
void sendJson(JsonObject& json) {
    WiFiClient client;

    if (!client.connect(airMonitorServerName, airMonitorPort)) {
        Serial.println(F("connection failed"));
        Serial.println(F("wait 1 sec...\n"));
        delay(1000);
        return;
    }
    delay(100);

    client.println(F("POST /prod/measurements HTTP/1.1"));
    client.print(F("Host: "));
    client.println(airMonitorServerName);
    client.println(F("Content-Type: application/json"));
    client.print(F("Content-Length: "));
    client.println(measureJson(json));
    client.println(F("X-Api-Key: ") + String(AIRMONITOR_API_KEY));
    client.println();
    serializeJson(json, client);

    String line = client.readStringUntil('\r');
    // TODO: Support wrong error (!= 200)

    if (DEBUG) {
        Serial.print(F("Length:"));
    Serial.println(measureJson(json));
    serializeJsonPretty(json, Serial);
        Serial.println(line);
    }
    client.stop();

}

void sendDUSTData(unsigned short & averagePM1, unsigned short & averagePM25, unsigned short & averagePM10) {
  if (strcmp(DUST_MODEL, "Non")) {
    StaticJsonDocument<400> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("lat")] = String(LATITUDE);
    json[F("long")] = String(LONGITUDE);
    json[F("pm1")] = float(averagePM1);
    json[F("pm25")] = float(averagePM25);
    json[F("pm10")] = float(averagePM10);
    if (!strcmp(DUST_MODEL, "PMS7003")) {
      json[F("sensor")] = F("PMS7003");
    }
    if (!strcmp(DUST_MODEL, "HPMA115S0")) {
      json[F("sensor")] = F("HPMA115S0");
    }
    if (!strcmp(DUST_MODEL, "SDS011/21")) {
      json[F("sensor")] = F("SDS021");
    }
    if (!strcmp(DUST_MODEL, "SPS30")) {
      json[F("sensor")] = F("SPS30");
    }
    sendJson(json);
  }
}

void sendTHPData(float & currentTemperature, float & currentPressure, float & currentHumidity) {
  if (strcmp(THP_MODEL, "Non")) {
    StaticJsonDocument<400> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("lat")] = String(LATITUDE);
    json[F("long")] = String(LONGITUDE);
    if (!strcmp(THP_MODEL, "BME280")) {
      json[F("pressure")] = float(currentPressure);
      json[F("temperature")] = float(currentTemperature);
      json[F("humidity")] = float(currentHumidity);
      json[F("sensor")] = F("BME280");
    } else if (!strcmp(THP_MODEL, "BMP280")) {
      json[F("pressure")] = float(currentPressure);
      json[F("temperature")] = float(currentTemperature);
      json[F("sensor")] = F("BMP280");
    } else if (!strcmp(THP_MODEL, "HTU21")) {
      json[F("temperature")] = float(currentTemperature);
      json[F("humidity")] = float(currentHumidity);
      json[F("sensor")] = F("HTU21");
    } else if (!strcmp(THP_MODEL, "DHT22")) {
      json[F("temperature")] = float(currentTemperature);
      json[F("humidity")] = float(currentHumidity);
      json[F("sensor")] = F("DHT22");
    } else if (!strcmp(THP_MODEL, "SHT1x")) {
      json[F("temperature")] = float(currentTemperature);
      json[F("humidity")] = float(currentHumidity);
      json[F("sensor")] = F("SHT1x");
    } else if (!strcmp(THP_MODEL, "DS18B20")) {
      json[F("temperature")] = float(currentTemperature);
      json[F("sensor")] = F("DS18B20");
    }
    sendJson(json);
  }
}

#elif defined ARDUINO_ARCH_ESP32
void sendJson(JsonObject& json) {
	
  Serial.print("\nconnecting to ");
  Serial.println(airMonitorServerName);
  
  String JSONoutput = "";
  serializeJson(json, JSONoutput);

	// setUpdateClock_airmonitor();
  WiFiClientSecure client;
  /*
  if (DEBUG) {
    Serial.print("\nUsing fingerprint: '%s'\n", fingerprint_airMonitor);
  }
  */
  client.setInsecure();
  //client.setCACert(trustRoot_airmonitor);
  //client.setFingerprint(fingerprint_airMonitor);
  client.setTimeout(15000); // 15 Seconds
  delay(1000);

  // Serial.print("\nHTTPS Connecting...");
  if (!client.connect(airMonitorServerName, airMonitorPort)) {
    Serial.println("connection failed");
    Serial.println("wait 1 sec...\n");
    delay(1000);
    return;
  }
  delay(100);
  
  String Link = "/prod/measurements";  
  client.print(String("POST ") + Link + " HTTP/1.1\r\n" +
               "Host: " + String(airMonitorServerName) + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " + String(measureJson(json)) + "\r\n" +
               "X-Api-Key: " + String(AIRMONITOR_API_KEY) + "\r\n\r\n" +
               String(JSONoutput) + "\r\n\r\n");
    
  /*
  if (DEBUG) {
    Serial.print("\n\n\t\t====================\n\n");
    Serial.print(String("POST ") + Link + " HTTP/1.1\r\n" +
                 "Host: " + String(airMonitorServerName) + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + String(measureJson(json)) + "\r\n" +
                 "X-Api-Key: " + String(AIRMONITOR_API_KEY) + "\r\n\r\n" +
                 String(JSONoutput) + "\r\n\r\n");
    Serial.print("\n\t\t====================\n\n");
  }
  */
  
  String line = client.readStringUntil('\r');
  
  if (DEBUG) {
      Serial.print("Length:");
  	  Serial.println(measureJson(json));
  	  serializeJsonPretty(json, Serial);
      Serial.println(line);
  }
  
  client.stop();
}

void sendDUSTData(unsigned short & averagePM1, unsigned short & averagePM25, unsigned short & averagePM10) {
  if (strcmp(DUST_MODEL, "Non")) {
    StaticJsonDocument<400> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json["lat"] = String(LATITUDE);
    json["long"] = String(LONGITUDE);
    json["pm1"] = float(averagePM1);
    json["pm25"] = float(averagePM25);
    json["pm10"] = float(averagePM10);
    if (!strcmp(DUST_MODEL, "PMS7003")) {
      json["sensor"] = "PMS7003";
    }
    if (!strcmp(DUST_MODEL, "HPMA115S0")) {
      json["sensor"] = "HPMA115S0";
    }
    if (!strcmp(DUST_MODEL, "SDS011/21")) {
      json["sensor"] = "SDS021";
    }
    if (!strcmp(DUST_MODEL, "SPS30")) {
      json["sensor"] = "SPS30";
    }
    sendJson(json);
  }
}

void sendTHPData(float & currentTemperature, float & currentPressure, float & currentHumidity) {
  if (strcmp(THP_MODEL, "Non")) {
    StaticJsonDocument<400> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json["lat"] = String(LATITUDE);
    json["long"] = String(LONGITUDE);
    if (!strcmp(THP_MODEL, "BME280")) {
      json["pressure"] = float(currentPressure);
      json["temperature"] = float(currentTemperature);
      json["humidity"] = float(currentHumidity);
      json["sensor"] = "BME280";
    } else if (!strcmp(THP_MODEL, "BMP280")) {
      json["pressure"] = float(currentPressure);
      json["temperature"] = float(currentTemperature);
      json["sensor"] = "BMP280";
    } else if (!strcmp(THP_MODEL, "HTU21")) {
      json["temperature"] = float(currentTemperature);
      json["humidity"] = float(currentHumidity);
      json["sensor"] = "HTU21";
    } else if (!strcmp(THP_MODEL, "DHT22")) {
      json["temperature"] = float(currentTemperature);
      json["humidity"] = float(currentHumidity);
      json["sensor"] = "DHT22";
    } else if (!strcmp(THP_MODEL, "SHT1x")) {
      json["temperature"] = float(currentTemperature);
      json["humidity"] = float(currentHumidity);
      json["sensor"] = "SHT1x";
    } else if (!strcmp(THP_MODEL, "DS18B20")) {
      json["temperature"] = float(currentTemperature);
      json["sensor"] = "DS18B20";
    }
    sendJson(json);
  }
}
#endif

void sendDataToAirMonitor(float & currentTemperature, float & currentPressure, float & currentHumidity, unsigned short & averagePM1, unsigned short & averagePM25, unsigned short & averagePM4, unsigned short & averagePM10) {
  if (!(AIRMONITOR_ON)) {
    return;
  }

  sendDUSTData(averagePM1, averagePM25, averagePM10);
  sendTHPData(currentTemperature, currentPressure, currentHumidity);
}



