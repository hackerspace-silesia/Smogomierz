#include <ESP8266WiFi.h>
#include "ArduinoJson.h"
#include "bme280.h"

#include "config.h"

const char *airMonitorServerName = "api.airmonitor.pl";
const uint16_t airMonitorPort = 5000;

void sendJson(JsonObject& json) {
    WiFiClient client;
    Serial.print("\nconnecting to ");
    Serial.println(airMonitorServerName);

    if (!client.connect(airMonitorServerName, airMonitorPort)) {
        Serial.println("connection failed");
        Serial.println("wait 3 sec...\n");
        delay(3000);
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

void sendPMSData(BME280<BME280_C, BME280_ADDRESS> &bme, int averagePM1, int averagePM25, int averagePM10) {
	if (strcmp(DUST_MODEL, "Non")) {
	StaticJsonDocument<400> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
    json["lat"] = String(LATITUDE, 6);
    json["long"] = String(LONGITUDE, 6);
    json["pm1"] = averagePM1;
    json["pm25"] = averagePM25;
    json["pm10"] = averagePM10;
	if (!strcmp(DUST_MODEL, "PMS7003")) {
    json["sensor"] = "PMS7003";
	}
    sendJson(json);
	}
}

void sendBMEData(BME280<BME280_C, BME280_ADDRESS> &bme) {
	if (strcmp(THP_MODEL, "Non")) {
	StaticJsonDocument<400> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
    json["lat"] = String(LATITUDE, 6);
    json["long"] = String(LONGITUDE, 6);
	if (!strcmp(THP_MODEL, "BME280")) {
    json["pressure"] = float(bme.seaLevelForAltitude(MYALTITUDE));
    json["temperature"] = float(bme.temperature);
    json["humidity"] = float(bme.humidity);
    json["sensor"] = "BME280";
	}
    sendJson(json);
	}
}

void sendDataToAirMonitor(BME280<BME280_C, BME280_ADDRESS> &bme, int averagePM1, int averagePM25, int averagePM10) {
    if (!(AIRMONITOR_ON)) {
        return;
    }

    sendPMSData(bme, averagePM1, averagePM25, averagePM10);
    sendBMEData(bme);
}



