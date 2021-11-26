#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiClientSecure.h>
#include <time.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <time.h>
#endif


#include <ArduinoJson.h>
#include "../config.h"

// ********************** Config **********************

// const char* GitHubhost PROGMEM = "api.github.com";
// const char* HShost PROGMEM = "smogomierz.hs-silesia.pl";
// const int httpsPort = 443;

// Last update: 27.01.2021 - https://www.grc.com/fingerprints.htm
// const char GitHubfingerprint[] PROGMEM = "DF B2 29 C6 A6 38 1A 59 9D C9 AD 92 2D 26 F5 3C 83 8F A5 87"; // api.github.com
// const char HSfingerprint[] PROGMEM = "E8 51 FC 5D 35 80 A7 57 61 11 2B 3D EC 19 2E 7F C2 B0 20 06"; // https://smogomierz.hs-silesia.pl

// ******************** Config End ********************

bool safeConnection = true;



#if defined(ARDUINO_ARCH_ESP32)
/**
 * This is lets-encrypt-x3-cross-signed.pem
 *	R3 at https://certlogik.com/ssl-checker/
 */
const char* rootCACertificate PROGMEM = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEZTCCA02gAwIBAgIQQAF1BIMUpMghjISpDBbN3zANBgkqhkiG9w0BAQsFADA/\n" \
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
"DkRTVCBSb290IENBIFgzMB4XDTIwMTAwNzE5MjE0MFoXDTIxMDkyOTE5MjE0MFow\n" \
"MjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxCzAJBgNVBAMT\n" \
"AlIzMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuwIVKMz2oJTTDxLs\n" \
"jVWSw/iC8ZmmekKIp10mqrUrucVMsa+Oa/l1yKPXD0eUFFU1V4yeqKI5GfWCPEKp\n" \
"Tm71O8Mu243AsFzzWTjn7c9p8FoLG77AlCQlh/o3cbMT5xys4Zvv2+Q7RVJFlqnB\n" \
"U840yFLuta7tj95gcOKlVKu2bQ6XpUA0ayvTvGbrZjR8+muLj1cpmfgwF126cm/7\n" \
"gcWt0oZYPRfH5wm78Sv3htzB2nFd1EbjzK0lwYi8YGd1ZrPxGPeiXOZT/zqItkel\n" \
"/xMY6pgJdz+dU/nPAeX1pnAXFK9jpP+Zs5Od3FOnBv5IhR2haa4ldbsTzFID9e1R\n" \
"oYvbFQIDAQABo4IBaDCCAWQwEgYDVR0TAQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8E\n" \
"BAMCAYYwSwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5p\n" \
"ZGVudHJ1c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTE\n" \
"p7Gkeyxx+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEE\n" \
"AYLfEwEBATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2Vu\n" \
"Y3J5cHQub3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0\n" \
"LmNvbS9EU1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYf\n" \
"r52LFMLGMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjANBgkqhkiG9w0B\n" \
"AQsFAAOCAQEA2UzgyfWEiDcx27sT4rP8i2tiEmxYt0l+PAK3qB8oYevO4C5z70kH\n" \
"ejWEHx2taPDY/laBL21/WKZuNTYQHHPD5b1tXgHXbnL7KqC401dk5VvCadTQsvd8\n" \
"S8MXjohyc9z9/G2948kLjmE6Flh9dDYrVYA9x2O+hEPGOaEOa1eePynBgPayvUfL\n" \
"qjBstzLhWVQLGAkXXmNs+5ZnPBxzDJOLxhF2JIbeQAcH5H0tZrUlo5ZYyOqA7s9p\n" \
"O5b85o3AM/OJ+CktFBQtfvBhcJVd9wvlwPsk+uyOy2HI7mNxKKgsBTt375teA2Tw\n" \
"UdHkhVNcsAKX1H7GNNLOEADksd86wuoXvg==\n" \
"-----END CERTIFICATE-----\n";
#endif


#ifdef ARDUINO_ARCH_ESP8266
// Set time via NTP, as required for x.509 validation
void setUpdateClock() {
  configTime(0, 0, "pool.ntp.org", "pool.ntp.org");  // UTC

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

String checkUpdate_UNSAFE(unsigned char & checkUpdateSW) {
  safeConnection = false;
  String UNSAFE_ServerSW;

  if (DEBUG) {
    Serial.printf("\n\t\tcheckUpdate_UNSAFE!!!\n\n");
  }

  WiFiClient UNSAFE_client;
#if defined(ARDUINO_ARCH_ESP32)
  UNSAFE_client.setTimeout(12000);
#endif

  HTTPClient http;
  
#if defined(ARDUINO_ARCH_ESP8266)
  const char* UNSAFE_latestJSONlink = "http://smogomierz.hs-silesia.pl/firmware/latest_esp8266.json";
#elif defined(ARDUINO_ARCH_ESP32)
  String UNSAFE_latestJSONlink = "http://smogomierz.hs-silesia.pl/firmware/latest_esp32.json";
#endif

  if (http.begin(UNSAFE_client, UNSAFE_latestJSONlink)) {
    delay(50);
    int httpCode = http.GET();

    //Serial.println(httpCode);

    if (httpCode > 0) {
      // header has been send and Server response header has been handled
      //Serial.printf("GET... code: %d\n", httpCode);
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String UNSAFE_payload = http.getString();
        delay(10);

        //Serial.println(UNSAFE_payload);
        //Serial.println(DEVICESMODEL);
        //Serial.println(checkUpdateSW);

        StaticJsonDocument<400> jsonBuffer;
        deserializeJson(jsonBuffer, UNSAFE_payload);
        JsonObject UNSAFE_json = jsonBuffer.as<JsonObject>();

        if (checkUpdateSW == 0) {
          UNSAFE_ServerSW = UNSAFE_json[String(PMSENSORVERSION)].as<String>();
        } else if (checkUpdateSW == 1) {
          UNSAFE_ServerSW = UNSAFE_json["PMS-SparkFunBME280"].as<String>();
        } else if (checkUpdateSW == 2) {
          UNSAFE_ServerSW = UNSAFE_json["SDS"].as<String>();
        } else if (checkUpdateSW == 3) {
          UNSAFE_ServerSW = UNSAFE_json["HPMA115S0"].as<String>();
        } else if (checkUpdateSW == 4) {
          UNSAFE_ServerSW = UNSAFE_json["PMS"].as<String>();
        } else if (checkUpdateSW == 5) {
          UNSAFE_ServerSW = UNSAFE_json["SPS30"].as<String>();
        } else if (checkUpdateSW >= 6) {
          UNSAFE_ServerSW = UNSAFE_json[String(PMSENSORVERSION)].as<String>();
        }

      }

      return UNSAFE_ServerSW;
    } else {
      if (DEBUG) {
        Serial.printf("GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      return F("0.0.0");
    }
  } else {
    if (DEBUG) {
      Serial.printf("Unable to connect\n");
    }
    return F("0.0.0");
  }

  UNSAFE_client.stop();
  http.end();
}



bool checkUpdate(unsigned char & checkUpdateSW) {
  const char* ServerSW = "0.0.0";
  //ServerSW = (String(SOFTWAREVERSION).substring(0, int(String(SOFTWAREVERSION).indexOf("build")) - 1)).c_str();
  // ServerSW = "0.0.0";
  String Data[3];

#if defined(ARDUINO_ARCH_ESP8266)
  WiFiClient client;
  // setUpdateClock();
  // WiFiClientSecure client;
  // BearSSL::WiFiClientSecure client;
  // client.setCACert_P(rootCACertificate, strlen(rootCACertificate));
  // Reading data over SSL may be slow, use an adequate timeout
  // client.setInsecure();
  // client.setFingerprint(HSfingerprint);
  // client.setTimeout(12000); // 12 Seconds
#elif defined(ARDUINO_ARCH_ESP32)
  setUpdateClock();
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  // Reading data over SSL may be slow, use an adequate timeout
  client.setTimeout(12000);
#endif
  
  HTTPClient https;
  
#if defined(ARDUINO_ARCH_ESP8266)
  const char* latestJSONlink = "http://smogomierz.hs-silesia.pl/firmware/latest_esp8266.json";
#elif defined(ARDUINO_ARCH_ESP32)
  String latestJSONlink = "https://smogomierz.hs-silesia.pl/firmware/latest_esp32.json";
#endif
	
  	if (https.begin(client, latestJSONlink)) {
		delay(50);
    int httpCode = https.GET();
	//Serial.printf("GET... code: %d\n", httpCode);
    if (httpCode > 0) {
      // header has been send and Server response header has been handled
      //Serial.printf("GET... code: %d\n", httpCode);
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
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
          ServerSW = json[String(PMSENSORVERSION)];
        } else if (checkUpdateSW == 1) {
          ServerSW = json["PMS-SparkFunBME280"];
        } else if (checkUpdateSW == 2) {
          ServerSW = json["SDS"];
        } else if (checkUpdateSW == 3) {
          ServerSW = json["HPMA115S0"];
        } else if (checkUpdateSW == 4) {
          ServerSW = json["PMS"];
        } else if (checkUpdateSW == 5) {
          ServerSW = json["SPS30"];
        } else if (checkUpdateSW >= 6) {
          ServerSW = json[String(PMSENSORVERSION)];
        }
      }
    } else if (httpCode == -1) {

      if (DEBUG) {
        Serial.printf("\nrootCACertificate error!\n");
      }
      client.flush();
      client.stop();
      https.end();

#if defined(ARDUINO_ARCH_ESP8266)
	  ServerSW = "0.0.0";
#elif defined(ARDUINO_ARCH_ESP32)
      ServerSW = String(checkUpdate_UNSAFE(checkUpdateSW)).c_str();
#endif
	
	} else {
      if (DEBUG) {
        Serial.printf("GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      ServerSW = "0.0.0";
	  
    }
  } else {
      ServerSW = "0.0.0";
    if (DEBUG) {
      Serial.print(F("Unable to connect\n"));
    }
  }
  
  https.end();
  
  strncpy(SERVERSOFTWAREVERSION, ServerSW, 32);
  Data[0] = SERVERSOFTWAREVERSION;

  if (checkUpdateSW == 0) {
    int SFbuildIndex = String(SOFTWAREVERSION).indexOf("build");
    Data[2] = String(SOFTWAREVERSION).substring(0, SFbuildIndex - 1);
    strncpy(CURRENTSOFTWAREVERSION, (Data[2]).c_str(), 32);
  }

  if (DEBUG) {
    Serial.println(F("\nSERVER SOFTWARE VERSION: ") + Data[0]);
    if (checkUpdateSW == 0) {
      Serial.println(F("CURRENT SOFTWARE VERSION: ") + Data[2]);
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
        Serial.println(F("\nFirmware upgrade required!\n"));
      }
      return true;
    }
    if (RepoSoftVer.toInt() == CurrentSoftVer.toInt()) {
      if (DEBUG) {
        Serial.println(F("\nYou have the current version of the firmware!\n"));
      }
      return false;
    }
    if (RepoSoftVer.toInt() < CurrentSoftVer.toInt()) {
      if (DEBUG) {
        Serial.println(F("\nYou have newer firmware installed than it is available in the official repository!\n"));
      }
      return false;
    }
  } else {
    return true;
  }
}

void doUpdate(unsigned char & doUpdateSW) {
	
#if defined(ARDUINO_ARCH_ESP8266)
    WiFiClientSecure client;
#elif defined(ARDUINO_ARCH_ESP32)
  setUpdateClock();
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  // Reading data over SSL may be slow, use an adequate timeout
  client.setTimeout(12000);
#endif
  
  if (checkUpdate(doUpdateSW)) {
    if (DEBUG) {
      Serial.println(F("Starting firmware upgrade...\n"));
#if defined(ARDUINO_ARCH_ESP8266)
      Serial.println(F("Free Heap: ") + String(ESP.getFreeHeap()));
      delay(10);
      Serial.println(F("Sketch Space: ") + String(ESP.getFreeSketchSpace()));
      delay(10);
#endif
    }
    String BinURL = "";
    if (doUpdateSW == 0) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = F("http://smogomierz.hs-silesia.pl/firmware/esp8266/") + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + F("_ESP8266.bin");
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
        	BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP32.bin";
	  } else {
        	BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW == 1) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = F("http://smogomierz.hs-silesia.pl/firmware/esp8266/") + String(SERVERSOFTWAREVERSION) + F("_PMS-SparkFunBME280_ESP8266.bin");
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_PMS-SparkFunBME280_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_PMS-SparkFunBME280_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW == 2) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = F("http://smogomierz.hs-silesia.pl/firmware/esp8266/") + String(SERVERSOFTWAREVERSION) + F("_SDS011_ESP8266.bin");
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_SDS011_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_SDS011_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW == 3) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = F("http://smogomierz.hs-silesia.pl/firmware/esp8266/") + String(SERVERSOFTWAREVERSION) + F("_HPMA115S0_ESP8266.bin");
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_HPMA115S0_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_HPMA115S0_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW == 4) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = F("http://smogomierz.hs-silesia.pl/firmware/esp8266/") + String(SERVERSOFTWAREVERSION) + F("_PMS_ESP8266.bin");
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_PMS_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_PMS_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW == 5) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = F("http://smogomierz.hs-silesia.pl/firmware/esp8266/") + String(SERVERSOFTWAREVERSION) + F("_SPS30_ESP8266.bin");
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_SPS30_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_SPS30_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW >= 6) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = F("http://smogomierz.hs-silesia.pl/firmware/esp8266/") + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + F("_ESP8266.bin");
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP32.bin";
	  }
#endif
    }

#if defined(ARDUINO_ARCH_ESP8266)
    t_httpUpdate_return ret = ESPhttpUpdate.update(client, BinURL);
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
          Serial.println(F("No update needed!"));
          break;
        case HTTP_UPDATE_OK:
          Serial.println(F("Update OK!"));
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


#elif defined ARDUINO_ARCH_ESP32
// Set time via NTP, as required for x.509 validation
void setUpdateClock() {
  configTime(0, 0, "pool.ntp.org", "pool.ntp.org");  // UTC

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    yield();
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }

  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

String checkUpdate_UNSAFE(unsigned char & checkUpdateSW) {
  safeConnection = false;
  String UNSAFE_ServerSW;

  if (DEBUG) {
    Serial.printf("\n\t\tcheckUpdate_UNSAFE!!!\n\n");
  }

  WiFiClient UNSAFE_client;
#if defined(ARDUINO_ARCH_ESP32)
  UNSAFE_client.setTimeout(12000);
#endif

  HTTPClient http;
  
#if defined(ARDUINO_ARCH_ESP8266)
  const char* UNSAFE_latestJSONlink = "http://smogomierz.hs-silesia.pl/firmware/latest_esp8266.json";
#elif defined(ARDUINO_ARCH_ESP32)
  String UNSAFE_latestJSONlink = "http://smogomierz.hs-silesia.pl/firmware/latest_esp32.json";
#endif

  if (http.begin(UNSAFE_client, UNSAFE_latestJSONlink)) {
    delay(50);
    int httpCode = http.GET();

    //Serial.println(httpCode);

    if (httpCode > 0) {
      // header has been send and Server response header has been handled
      //Serial.print("GET... code: %d\n", httpCode);
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String UNSAFE_payload = http.getString();
        delay(10);

        //Serial.println(UNSAFE_payload);
        //Serial.println(DEVICESMODEL);
        //Serial.println(checkUpdateSW);

        StaticJsonDocument<400> jsonBuffer;
        deserializeJson(jsonBuffer, UNSAFE_payload);
        JsonObject UNSAFE_json = jsonBuffer.as<JsonObject>();

        if (checkUpdateSW == 0) {
          UNSAFE_ServerSW = UNSAFE_json[String(PMSENSORVERSION)].as<String>();
        } else if (checkUpdateSW == 1) {
          UNSAFE_ServerSW = UNSAFE_json["PMS-SparkFunBME280"].as<String>();
        } else if (checkUpdateSW == 2) {
          UNSAFE_ServerSW = UNSAFE_json["SDS"].as<String>();
        } else if (checkUpdateSW == 3) {
          UNSAFE_ServerSW = UNSAFE_json["HPMA115S0"].as<String>();
        } else if (checkUpdateSW == 4) {
          UNSAFE_ServerSW = UNSAFE_json["PMS"].as<String>();
        } else if (checkUpdateSW == 5) {
          UNSAFE_ServerSW = UNSAFE_json["SPS30"].as<String>();
        } else if (checkUpdateSW >= 6) {
          UNSAFE_ServerSW = UNSAFE_json[String(PMSENSORVERSION)].as<String>();
        }

      }

      return UNSAFE_ServerSW;
    } else {
      if (DEBUG) {
        Serial.printf("GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      return "0.0.0";
    }
  } else {
    if (DEBUG) {
      Serial.printf("Unable to connect\n");
    }
    return "0.0.0";
  }

  UNSAFE_client.stop();
  http.end();
}



bool checkUpdate(unsigned char & checkUpdateSW) {
  const char* ServerSW = "0.0.0";
  //ServerSW = (String(SOFTWAREVERSION).substring(0, int(String(SOFTWAREVERSION).indexOf("build") - 1)).c_str();
  // ServerSW = "0.0.0";
  String Data[3];

#if defined(ARDUINO_ARCH_ESP8266)
  WiFiClient client;
  // setUpdateClock();
  // WiFiClientSecure client;
  // BearSSL::WiFiClientSecure client;
  // client.setCACert_P(rootCACertificate, strlen(rootCACertificate));
  // Reading data over SSL may be slow, use an adequate timeout
  // client.setInsecure();
  // client.setFingerprint(HSfingerprint);
  // client.setTimeout(12000); // 12 Seconds
#elif defined(ARDUINO_ARCH_ESP32)
  setUpdateClock();
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  // Reading data over SSL may be slow, use an adequate timeout
  client.setTimeout(12000);
#endif
  
  HTTPClient https;
  
#if defined(ARDUINO_ARCH_ESP8266)
  const char* latestJSONlink = "http://smogomierz.hs-silesia.pl/firmware/latest_esp8266.json";
#elif defined(ARDUINO_ARCH_ESP32)
  String latestJSONlink = "https://smogomierz.hs-silesia.pl/firmware/latest_esp32.json";
#endif
	
  	if (https.begin(client, latestJSONlink)) {
		delay(50);
    int httpCode = https.GET();
	//Serial.print("GET... code: %d\n", httpCode);
    if (httpCode > 0) {
      // header has been send and Server response header has been handled
      //Serial.print("GET... code: %d\n", httpCode);
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
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
          ServerSW = json[String(PMSENSORVERSION)];
        } else if (checkUpdateSW == 1) {
          ServerSW = json["PMS-SparkFunBME280"];
        } else if (checkUpdateSW == 2) {
          ServerSW = json["SDS"];
        } else if (checkUpdateSW == 3) {
          ServerSW = json["HPMA115S0"];
        } else if (checkUpdateSW == 4) {
          ServerSW = json["PMS"];
        } else if (checkUpdateSW == 5) {
          ServerSW = json["SPS30"];
        } else if (checkUpdateSW >= 6) {
          ServerSW = json[String(PMSENSORVERSION)];
        }
      }
    } else if (httpCode == -1) {

      if (DEBUG) {
        Serial.print("\nrootCACertificate error!\n");
      }
      client.flush();
      client.stop();
      https.end();

#if defined(ARDUINO_ARCH_ESP8266)
	  ServerSW = "0.0.0";
#elif defined(ARDUINO_ARCH_ESP32)
      ServerSW = String(checkUpdate_UNSAFE(checkUpdateSW)).c_str();
#endif
	
	} else {
      if (DEBUG) {
        Serial.printf("GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      ServerSW = "0.0.0";
	  
    }
  } else {
      ServerSW = "0.0.0";
    if (DEBUG) {
      Serial.print("Unable to connect\n");
    }
  }
  
  https.end();
  
  strncpy(SERVERSOFTWAREVERSION, ServerSW, 32);
  Data[0] = SERVERSOFTWAREVERSION;

  if (checkUpdateSW == 0) {
    int SFbuildIndex = String(SOFTWAREVERSION).indexOf("build");
    Data[2] = String(SOFTWAREVERSION).substring(0, SFbuildIndex - 1);
    strncpy(CURRENTSOFTWAREVERSION, (Data[2]).c_str(), 32);
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
    } else if (RepoSoftVer.toInt() == CurrentSoftVer.toInt()) {
      if (DEBUG) {
        Serial.println("\nYou have the current version of the firmware!\n");
      }
      return false;
    } else if (RepoSoftVer.toInt() < CurrentSoftVer.toInt()) {
      if (DEBUG) {
        Serial.println("\nYou have newer firmware installed than it is available in the official repository!\n");
      }
      return false;
    } else {
        return false;
    }
  } else {
    return true;
  }
}

void doUpdate(unsigned char & doUpdateSW) {
	
#if defined(ARDUINO_ARCH_ESP8266)
    WiFiClientSecure client;
#elif defined(ARDUINO_ARCH_ESP32)
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
    String BinURL = "";
    if (doUpdateSW == 0) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
        	BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP32.bin";
	  } else {
        	BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW == 1) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_PMS-SparkFunBME280_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_PMS-SparkFunBME280_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_PMS-SparkFunBME280_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW == 2) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_SDS011_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_SDS011_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_SDS011_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW == 3) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_HPMA115S0_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_HPMA115S0_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_HPMA115S0_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW == 4) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_PMS_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_PMS_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_PMS_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW == 5) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_SPS30_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_SPS30_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_SPS30_ESP32.bin";
	  }
#endif
    } else if (doUpdateSW >= 6) {
#if defined(ARDUINO_ARCH_ESP8266)
      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp8266/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP8266.bin";
#elif defined(ARDUINO_ARCH_ESP32)
	  if (safeConnection == true) {
      BinURL = "https://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP32.bin";
	  } else {
	      BinURL = "http://smogomierz.hs-silesia.pl/firmware/esp32/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + "_ESP32.bin";
	  }
#endif
    }

#if defined(ARDUINO_ARCH_ESP8266)
    t_httpUpdate_return ret = ESPhttpUpdate.update(client, BinURL);
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
#endif
