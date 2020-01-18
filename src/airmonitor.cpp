#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#endif

#include <ArduinoJson.h>
#include "config.h"

const char *airMonitorServerName = "api.airmonitor.pl";
const uint16_t airMonitorPort = 5000;

void sendJson(JsonObject& json) {
    WiFiClient client;
    Serial.print("\nconnecting to ");
    Serial.println(airMonitorServerName);

    if (!client.connect(airMonitorServerName, airMonitorPort)) {
        Serial.println("connection failed");
        Serial.println("wait 1 sec...\n");
        delay(1000);
        return;
    }

    delay(100); 

    client.println("POST /api HTTP/1.1");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
	client.println(measureJson(json));
    client.println();
	serializeJson(json, client);

    String line = client.readStringUntil('\r');
    // TODO: Support wrong error (!= 200)

    if (DEBUG) {
        Serial.print("Length:");
		Serial.println(measureJson(json));
		serializeJsonPretty(json, Serial);
        Serial.println(line);
    }
    client.stop();
}

void sendDUSTData(int averagePM1, int averagePM25, int averagePM10) {
	if (strcmp(DUST_MODEL, "Non")) {
	StaticJsonDocument<400> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
    json["lat"] = String(LATITUDE);
    json["long"] = String(LONGITUDE);
    json["pm1"] = averagePM1;
    json["pm25"] = averagePM25;
    json["pm10"] = averagePM10;
	if (!strcmp(DUST_MODEL, "PMS7003")) {
		json["sensor"] = "PMS7003";
	}
	if (!strcmp(DUST_MODEL, "HPMA115S0")) {
		json["sensor"] = "HPMA115S0";
	}
	if (!strcmp(DUST_MODEL, "SDS011/21")) {
		json["sensor"] = "SDS011";
	}
	if (!strcmp(DUST_MODEL, "SPS30")) {
		json["sensor"] = "SPS30";
	}
    sendJson(json);
	}
}

void sendTHPData(float currentTemperature, float currentPressure, float currentHumidity) {
	if (strcmp(THP_MODEL, "Non")) {
	StaticJsonDocument<400> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
    json["lat"] = String(LATITUDE);
    json["long"] = String(LONGITUDE);
	if (!strcmp(THP_MODEL, "BME280")) {
	    json["pressure"] = currentPressure;
	    json["temperature"] = currentTemperature;
	    json["humidity"] = currentHumidity;
		json["sensor"] = "BME280";
	} else if (!strcmp(THP_MODEL, "BMP280")) {
	    json["pressure"] = currentPressure;
	    json["temperature"] = currentTemperature;
		json["sensor"] = "BMP280";
	} else if (!strcmp(THP_MODEL, "HTU21")) {
	    json["temperature"] = currentTemperature;
	    json["humidity"] = currentHumidity;
		json["sensor"] = "HTU21";
	} else if (!strcmp(THP_MODEL, "DHT22")) {
	    json["temperature"] = currentTemperature;
	    json["humidity"] = currentHumidity;
		json["sensor"] = "DHT22";
	} else if (!strcmp(THP_MODEL, "SHT1x")) {
	    json["temperature"] = currentTemperature;
	    json["humidity"] = currentHumidity;
		json["sensor"] = "SHT1x";
	}
    sendJson(json);
	}
}

void sendDataToAirMonitor(float currentTemperature, float currentPressure, float currentHumidity, int averagePM1, int averagePM25, int averagePM4, int averagePM10) {
    if (!(AIRMONITOR_ON)) {
        return;
    }

    sendDUSTData(averagePM1, averagePM25, averagePM10);
    sendTHPData(currentTemperature, currentPressure, currentHumidity);
}



