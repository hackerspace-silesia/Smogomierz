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
/**
 * This is lets-encrypt-x3-cross-signed.pem
 */
const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n" \
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
"DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \
"SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n" \
"GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \
"AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n" \
"q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n" \
"SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n" \
"Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n" \
"a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n" \
"/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n" \
"AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n" \
"CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n" \
"bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n" \
"c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n" \
"VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n" \
"ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n" \
"MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n" \
"Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n" \
"AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n" \
"uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n" \
"wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n" \
"X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n" \
"PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n" \
"KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n" \
"-----END CERTIFICATE-----\n";
#endif

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
  String latestJSONlink = "http://smogomierz.hs-silesia.pl/firmware/latest_esp8266.json";
#elif defined(ARDUINO_ARCH_ESP32)
  String latestJSONlink = "https://smogomierz.hs-silesia.pl/firmware/latest_esp32.json";
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
      Serial.println("Starting firmware upgrade...\n");
#if defined(ARDUINO_ARCH_ESP8266)
      Serial.println("Free Heap: " + String(ESP.getFreeHeap()));
      delay(10);
      Serial.println("Sketch Space: " + String(ESP.getFreeSketchSpace()));
      delay(10);
#endif
    }
    String BinURL;
    if (doUpdateSW == 0) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP32.bin";
#endif
    } else if (doUpdateSW == 1) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_" + "PMS-SparkFunBME280" + "_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + "PMS-SparkFunBME280" + "_ESP32.bin";
#endif
    } else if (doUpdateSW == 2) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_" + "SDS011" + "_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + "SDS011" + "_ESP32.bin";
#endif
    } else if (doUpdateSW == 3) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_" + "HPMA115S0" + "_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + "HPMA115S0" + "_ESP32.bin";
#endif
    } else if (doUpdateSW == 4) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_" + "PMS" + "_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + "PMS" + "_ESP32.bin";
#endif
    } else if (doUpdateSW >= 5) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP32.bin";
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
