#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecureBearSSL.h>

#include "config.h"

// ********************** Config **********************

const char* GitHubhost PROGMEM = "api.github.com";
const char* HShost PROGMEM = "smogomierz.hs-silesia.pl";
const int httpsPort = 443;

// Last update: 21.03.2019
const char GitHubfingerprint[] PROGMEM = "5F F1 60 31 09 04 3E F2 90 D2 B0 8A 50 38 04 E8 37 9F BC 76"; // api.github.com
const char HSfingerprint[] PROGMEM = "28 8B CC 29 1B 76 4B 93 45 F6 B2 36 A4 10 EF CE DF 0B D1 72";  // https://smogomierz.hs-silesia.pl

// ******************** Config End ********************

bool checkUpdate() {
	const char* ServerSW;
	String Data[3];
		
	WiFiClient client;

	    HTTPClient http;

	    if (http.begin(client, "http://smogomierz.hs-silesia.pl/firmware/latest.json")) {

	      int httpCode = http.GET();
	      if (httpCode > 0) {
	        // header has been send and Server response header has been handled
				  //Serial.printf("GET... code: %d\n", httpCode);			  
	        // file found at server
	        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
	          String payload = http.getString();
			  delay(10);
	          //Serial.println(payload);
			  
			  StaticJsonDocument<200> jsonBuffer;
			        deserializeJson(jsonBuffer, payload);
			        JsonObject json = jsonBuffer.as<JsonObject>();
			        ServerSW = json[PMSENSORVERSION];
			}
	      } else {
			  if (DEBUG){
	        	  Serial.printf("GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
				}
	      }
	    } else {
			if (DEBUG){
	      	  Serial.printf("Unable to connect\n");
	  		}
	    }
		
	http.end();
	strncpy(SERVERSOFTWAREVERSION, ServerSW, 255);
		
  int SFbuildIndex = String(SOFTWAREVERSION).indexOf("build");

  Data[0] = SERVERSOFTWAREVERSION;
  Data[2] = String(SOFTWAREVERSION).substring(0, SFbuildIndex - 1);
  
  strncpy(CURRENTSOFTWAREVERSION, (Data[2]).c_str(), 255);
  
  if (DEBUG) {
    Serial.println("\nSERVER SOFTWARE VERSION: " + Data[0]);
    Serial.println("CURRENT SOFTWARE VERSION: " + Data[2]);
  }
  
  int dotIndexRepoSoftVer1 = Data[0].indexOf('.');
  int dotIndexRepoSoftVer2 = Data[0].indexOf('.', dotIndexRepoSoftVer1 + 1);

  int dotIndexCurrentSoftVer1 = Data[2].indexOf('.');
  int dotIndexCurrentSoftVer2 = Data[2].indexOf('.', dotIndexCurrentSoftVer1 + 1);

  String RepoSoftVer = Data[0].substring(0, dotIndexRepoSoftVer1) + Data[0].substring(dotIndexRepoSoftVer1 + 1, dotIndexRepoSoftVer2) + Data[0].substring(dotIndexRepoSoftVer2 + 1);
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
}

void doUpdate() {
	if (checkUpdate()) {
		if (DEBUG) {
			Serial.println("Starting firmware update...\n");
		    Serial.println("Free Heap: " + (ESP.getFreeHeap()));
			delay(10);
			Serial.println("Sketch Space: " + (ESP.getFreeSketchSpace()));
			delay(10);
		}
  	  	
		String BinURL = "http://smogomierz.hs-silesia.pl/firmware/" + String(SERVERSOFTWAREVERSION) + "_" + String(PMSENSORVERSION) + ".bin";
		
		t_httpUpdate_return ret = ESPhttpUpdate.update(BinURL);

	    if (DEBUG) {
			switch (ret) {
				case HTTP_UPDATE_FAILED:
	          Serial.printf("Updated FAILED (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
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
