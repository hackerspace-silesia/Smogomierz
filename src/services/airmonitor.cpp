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
    client.println(F("X-Api-Key: ") + String(airMonitorSettings.apiKey));
    client.println();
    serializeJson(json, client);

    String line = client.readStringUntil('\r');
    // TODO: Support wrong error (!= 200)

    if (deviceSettings.debug) {
        Serial.print(F("Length:"));
    Serial.println(measureJson(json));
    serializeJsonPretty(json, Serial);
        Serial.println(line);
    }
    client.stop();

}

void sendDUSTData() {
  if (strcmp(sensorsSettings.dustModel, "Non")) {
    StaticJsonDocument<400> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("lat")] = String(deviceSettings.latitude);
    json[F("long")] = String(deviceSettings.longitude);
    json[F("pm1")] = float(measurementsData.averagePM1);
    json[F("pm25")] = float(measurementsData.averagePM25);
    json[F("pm10")] = float(measurementsData.averagePM10);
    if (!strcmp(sensorsSettings.dustModel, "PMS7003")) {
      json[F("sensor")] = F("PMS7003");
    }
    if (!strcmp(sensorsSettings.dustModel, "HPMA115S0")) {
      json[F("sensor")] = F("HPMA115S0");
    }
    if (!strcmp(sensorsSettings.dustModel, "SDS011/21")) {
      json[F("sensor")] = F("SDS021");
    }
    if (!strcmp(sensorsSettings.dustModel, "SPS30")) {
      json[F("sensor")] = F("SPS30");
    }
    sendJson(json);
  }
}

void sendTHPData() {
  if (strcmp(sensorsSettings.thpModel, "Non")) {
    StaticJsonDocument<400> jsonBuffer;
    JsonObject json = jsonBuffer.to<JsonObject>();
    json[F("lat")] = String(deviceSettings.latitude);
    json[F("long")] = String(deviceSettings.longitude);
    json[F("temperature")] = float(measurementsData.temperature);
    if (!strcmp(sensorsSettings.thpModel, "BME280")) {
      json[F("pressure")] = float(measurementsData.pressure);
      json[F("humidity")] = float(measurementsData.humidity);
      json[F("sensor")] = F("BME280");
    } else if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
      json[F("pressure")] = float(measurementsData.pressure);
      json[F("sensor")] = F("BMP280");
    } else if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
      json[F("humidity")] = float(measurementsData.humidity);
      json[F("sensor")] = F("HTU21");
    } else if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
      json[F("humidity")] = float(measurementsData.humidity);
      json[F("sensor")] = F("DHT22");
    } else if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
      json[F("humidity")] = float(measurementsData.humidity);
      json[F("sensor")] = F("SHT1x");
    } else if (!strcmp(sensorsSettings.thpModel, "DS18B20")) {
      json[F("sensor")] = F("DS18B20");
    }
    sendJson(json);
  }
}

void sendDataToAirMonitor() {
  if (!(airMonitorSettings.enabled)) {
    return;
  }

  sendDUSTData();
  sendTHPData();
}