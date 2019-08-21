#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <time.h>
#endif

#include <ArduinoJson.h>
#include "config.h"

// ********************** Config **********************

const char* GitHubhost PROGMEM = "api.github.com";
const char* HShost PROGMEM = "smogomierz.hs-silesia.pl";
const int httpsPort = 443;

// Last update: 21.03.2019
const char GitHubfingerprint[] PROGMEM = "5F F1 60 31 09 04 3E F2 90 D2 B0 8A 50 38 04 E8 37 9F BC 76"; // api.github.com
const char HSfingerprint[] PROGMEM = "28 8B CC 29 1B 76 4B 93 45 F6 B2 36 A4 10 EF CE DF 0B D1 72";  // https://smogomierz.hs-silesia.pl

// ******************** Config End ********************

#if defined(ARDUINO_ARCH_ESP32)
// Set time via NTP, as required for x.509 validation
void setUpdateClock() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");  // UTC

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
#endif

bool checkUpdate(int checkUpdateSW) {
  const char* ServerSW;
  String Data[3];

#if defined(ARDUINO_ARCH_ESP8266)
  WiFiClient client;
#elif defined(ARDUINO_ARCH_ESP32)
  setUpdateClock();
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  // Reading data over SSL may be slow, use an adequate timeout
  client.setTimeout(12000);
#endif

  HTTPClient http;

#if defined(ARDUINO_ARCH_ESP8266)
  String latestJSONlink = "http://smogomierz.hs-silesia.pl/firmware/latest.json";
#elif defined(ARDUINO_ARCH_ESP32)
  String latestJSONlink = "http://smogomierz.hs-silesia.pl/firmware/latest.json";
#endif

  	if (http.begin(client, latestJSONlink)) {
		delay(50);
    int httpCode = http.GET();
    if (httpCode > 0) {
      // header has been send and Server response header has been handled
      //Serial.printf("GET... code: %d\n", httpCode);
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        delay(10);

        /*
		Serial.println(payload);
		Serial.println(PMSENSORVERSION);
		Serial.println(checkUpdateSW);
		*/

        StaticJsonDocument<400> jsonBuffer;
        deserializeJson(jsonBuffer, payload);
        JsonObject json = jsonBuffer.as<JsonObject>();

        if (checkUpdateSW == 0) {
          ServerSW = json[PMSENSORVERSION];
        } else if (checkUpdateSW == 1) {
          ServerSW = json["PMS-SparkFunBME280"];
        } else if (checkUpdateSW == 2) {
          ServerSW = json["SDS"];
        } else if (checkUpdateSW == 3) {
          ServerSW = json["HPMA115S0"];
        } else if (checkUpdateSW == 4) {
          ServerSW = json["PMS"];
        } else if (checkUpdateSW >= 5) {
          ServerSW = json[PMSENSORVERSION];
        }
      }
    } else {
      if (DEBUG) {
        Serial.printf("GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    }
  } else {
    if (DEBUG) {
      Serial.printf("Unable to connect\n");
    }
  }
  http.end();
  strncpy(SERVERSOFTWAREVERSION, ServerSW, 255);
  Data[0] = SERVERSOFTWAREVERSION;

  if (checkUpdateSW == 0) {
    int SFbuildIndex = String(SOFTWAREVERSION).indexOf("build");
    Data[2] = String(SOFTWAREVERSION).substring(0, SFbuildIndex - 1);
    strncpy(CURRENTSOFTWAREVERSION, (Data[2]).c_str(), 255);
  }

  if (DEBUG) {
    Serial.println("\nSERVER SOFTWARE VERSION: " + Data[0]);
    if (checkUpdateSW == 0) {
      Serial.println("CURRENT SOFTWARE VERSION: " + Data[2]);
    }
  }

  if (checkUpdateSW == 0) {
    int dotIndexRepoSoftVer1 = Data[0].indexOf('.');
    int dotIndexRepoSoftVer2 = Data[0].indexOf('.', dotIndexRepoSoftVer1 + 1);
    String RepoSoftVer = Data[0].substring(0, dotIndexRepoSoftVer1) + Data[0].substring(dotIndexRepoSoftVer1 + 1, dotIndexRepoSoftVer2) + Data[0].substring(dotIndexRepoSoftVer2 + 1);

    int dotIndexCurrentSoftVer1 = Data[2].indexOf('.');
    int dotIndexCurrentSoftVer2 = Data[2].indexOf('.', dotIndexCurrentSoftVer1 + 1);
    String CurrentSoftVer = Data[2].substring(0, dotIndexCurrentSoftVer1) + Data[2].substring(dotIndexCurrentSoftVer1 + 1, dotIndexCurrentSoftVer2) + Data[2].substring(dotIndexCurrentSoftVer2 + 1);

    if (RepoSoftVer.toInt() > CurrentSoftVer.toInt()) {
      if (DEBUG) {
        Serial.println("\nFirmware upgrade required!\n");
      }
      return true;
    }
    if (RepoSoftVer.toInt() == CurrentSoftVer.toInt()) {
      if (DEBUG) {
        Serial.println("\nYou have the current version of the firmware!\n");
      }
      return false;
    }
    if (RepoSoftVer.toInt() < CurrentSoftVer.toInt()) {
      if (DEBUG) {
        Serial.println("\nYou have newer firmware installed than it is available in the official repository!\n");
      }
      return false;
    }
  } else {
    return true;
  }
}

void doUpdate(int doUpdateSW) {
#if defined(ARDUINO_ARCH_ESP32)
  setUpdateClock();
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  // Reading data over SSL may be slow, use an adequate timeout
  client.setTimeout(12000);
#endif

  if (checkUpdate(doUpdateSW)) {
    if (DEBUG) {
      Serial.println("Starting firmware update...\n");
#if defined(ARDUINO_ARCH_ESP8266)
      Serial.println("Free Heap: " + (ESP.getFreeHeap()));
      delay(10);
      Serial.println("Sketch Space: " + (ESP.getFreeSketchSpace()));
      delay(10);
#endif
    }
    String BinURL;
    if (doUpdateSW == 0) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + ".bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + ".bin";
#endif
    } else if (doUpdateSW == 1) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + "PMS-SparkFunBME280" + ".bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + "PMS-SparkFunBME280" + ".bin";
#endif
    } else if (doUpdateSW == 2) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + "SDS011" + ".bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + "SDS011" + ".bin";
#endif
    } else if (doUpdateSW == 3) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + "HPMA115S0" + ".bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + "HPMA115S0" + ".bin";
#endif
    } else if (doUpdateSW == 4) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + "PMS" + ".bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + "PMS" + ".bin";
#endif
    } else if (doUpdateSW >= 5) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + ".bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + ".bin";
#endif
    }

#if defined(ARDUINO_ARCH_ESP8266)
    t_httpUpdate_return ret = ESPhttpUpdate.update(BinURL);
#elif defined(ARDUINO_ARCH_ESP32)
    t_httpUpdate_return ret = httpUpdate.update(client, BinURL);
#endif

    if (DEBUG) {
      switch (ret) {
        case HTTP_UPDATE_FAILED:
#if defined(ARDUINO_ARCH_ESP8266)
        Serial.printf("Updated FAILED (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
#elif defined(ARDUINO_ARCH_ESP32)
        Serial.printf("Updated FAILED (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
#endif
          break;
        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("No update needed!");
          break;
        case HTTP_UPDATE_OK:
          Serial.println("Update OK!");
          break;
        default:
          Serial.printf("Unexpected response code %d from ESPhttpUpdate.update\n", (int)ret);
          break;
      }
      delay(1000);
      ESP.restart();
      delay(1000);
    }
  }
}
