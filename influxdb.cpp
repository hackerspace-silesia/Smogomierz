#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include "ArduinoJson.h"
#include "pms.h"
#include "bme280.h"

#include "config.h"

void sendJsonInfluxDB(JsonObject& jsonInfluxDB) {
    WiFiClient client;
    Serial.print("\nconnecting to ");
    Serial.println(INFLUXDB_ADRESS);

    if (!client.connect(INFLUXDB_ADRESS, INFLUXDB_PORT)) {
        Serial.println("connection failed");
        Serial.println("wait 3 sec...\n");
        delay(3000);
        return;
    }

    delay(100); 

    client.print("POST /write?db=");
    client.print(INFLUXDB_DB_NAME);
    client.print(" HTTP/1.1");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonInfluxDB.measureLength());
    client.println();
    jsonInfluxDB.printTo(client);

    String line = client.readStringUntil('\r');
    // TODO: Support wrong error (!= 200)

    if (DEBUG) {
        Serial.println("Length:");
        Serial.println(jsonInfluxDB.measureLength());
        jsonInfluxDB.prettyPrintTo(Serial);

        Serial.println(line);
    }

    client.stop();
}

void sendDataInfluxDB(const PMS::DATA &pms) {
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& jsonInfluxDB = jsonBuffer.createObject();
    jsonInfluxDB["name"] = INFLUXDB_DEVICE_NAME;
    JsonArray& dataInfluxDB = jsonInfluxDB.createNestedArray("value");
    dataInfluxDB.add(int(calib1 * pms.PM_AE_UG_1_0));
    dataInfluxDB.add(int(calib1 * pms.PM_AE_UG_2_5));
    dataInfluxDB.add(int(calib1 * pms.PM_AE_UG_10_0));
    sendJsonInfluxDB(jsonInfluxDB);
    /*
    String content = "PM2.5,device=Smogomierz value=" + String(int(calib1 * pms.PM_AE_UG_2_5));
    udp.beginPacket(INFLUXDB_ADRESS, INFLUXDB_PORT);
    udp.print(content);
    udp.endPacket();
    if (DEBUG) {
        Serial.println(content);
        Serial.println("\r");
    }
    udp.stop();*/
}
/*
void sendBMEDataInfluxDB(BME280<BME280_C, BME280_ADDRESS> &bme) {
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& jsonInfluxDB = jsonBuffer.createObject();
    jsonInfluxDB["name"] = INFLUXDB_DEVICE_NAME;
    JsonArray& dataInfluxDB = jsonInfluxDB.createNestedArray("value");
    jsonInfluxDB["pressure"] = float(bme.seaLevelForAltitude(MYALTITUDE) / 100.0F);
    jsonInfluxDB["temperature"] = float(bme.temperature);
    jsonInfluxDB["humidity"] = float(bme.humidity);
    sendJsonInfluxDB(jsonInfluxDB);
}
*/
void sendDataToInfluxDB(BME280<BME280_C, BME280_ADDRESS> &bme, const PMS::DATA &pms) {
    if (!(INFLUXDB_ON)) {
        return;
    }
    sendDataInfluxDB(pms);
    //sendBMEDataInfluxDB(bme);
}


