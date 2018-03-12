#include <ESP8266WiFi.h>
#include "ArduinoJson.h"
#include "pms.h"
#include "bme280.h"

#include "../config.h"

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
    client.println(json.measureLength());
    client.println();
    json.printTo(client);

    String line = client.readStringUntil('\r');
    // TODO: Support wrong error (!= 200)

    if (DEBUG) {
        Serial.print("Length:");
        Serial.println(json.measureLength());
        json.prettyPrintTo(Serial);

        Serial.println(line);
    }

    client.stop();
}

void sendPMSData(const PMS::DATA &pms, BME280<BME280_C, BME280_ADDRESS> &bme) {
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    if (int(bme.temperature) < 5 or int(bme.humidity) > 60){
    json["lat"] = String(LATITUDE, 4);
    json["long"] = String(LONGITUDE, 4);
    json["pm1"] = int(calib2 * pms.PM_AE_UG_1_0);
    json["pm25"] = int(calib2 * pms.PM_AE_UG_2_5);
    json["pm10"] = int(calib2 * pms.PM_AE_UG_10_0);
    json["sensor"] = "PMS7003";
    }else{
    json["lat"] = String(LATITUDE, 4);
    json["long"] = String(LONGITUDE, 4);
    json["pm1"] = int(calib1 * pms.PM_AE_UG_1_0);
    json["pm25"] = int(calib1 * pms.PM_AE_UG_2_5);
    json["pm10"] = int(calib1 * pms.PM_AE_UG_10_0);
    json["sensor"] = "PMS7003";      
    }
    sendJson(json);
}

void sendBMEData(BME280<BME280_C, BME280_ADDRESS> &bme) {
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["lat"] = String(LATITUDE, 4);
    json["long"] = String(LONGITUDE, 4);
    json["pressure"] = float(bme.pressure  / 100.0F);
    json["temperature"] = float(bme.temperature);
    json["humidity"] = float(bme.humidity);
    json["sensor"] = "BME280";

    sendJson(json);
}

void sendDataToAirMonitor(BME280<BME280_C, BME280_ADDRESS> &bme, const PMS::DATA &pms) {
    if (!(AIRMONITOR_ON)) {
        return;
    }

    sendPMSData(pms, bme);
    sendBMEData(bme);
}



