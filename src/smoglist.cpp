#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "bme280.h"

#include "config.h"

const char *SmogListServerName = "api.smoglist.pl"; // api.smoglist.pl:8090/postjson
const uint16_t SmogListPort = 8090;

void sendSmogListJson(JsonObject& json) {
    WiFiClient client;
    Serial.print("\nconnecting to ");
    Serial.println(SmogListServerName);

    if (!client.connect(SmogListServerName, SmogListPort)) {
        Serial.println("connection failed");
        Serial.println("wait 3 sec...\n");
        delay(3000);
        return;
    }

    delay(100); 

    client.println("POST /postjson HTTP/1.1");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
	client.println(measureJson(json));
    client.println();
	serializeJson(json, client);

    String line = client.readStringUntil('\r');
    // TODO: Support wrong error (!= 200)

    if (DEBUG) {
		serializeJsonPretty(json, Serial);
        Serial.println(line);
    }

    client.stop();
}

void sendSmogListData(BME280<BME280_C, BME280_ADDRESS> &bme, int averagePM1, int averagePM25, int averagePM4, int averagePM10) {
	StaticJsonDocument<1000> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
	
	json["CHIPID"] = "Smogomierz-" + String(ESP.getChipId());
	json["SOFTWAREVERSION"] = SOFTWAREVERSION;
	json["HARDWAREVERSION"] = HARDWAREVERSION; // "1.0 - ESP8266" or "2.0 - ESP32"
	json["PMSENSORVERSION"] = PMSENSORVERSION; // PMS, SDS, HPMA115S0 ora SPS30
	
	json["FREQUENTMEASUREMENT"] = FREQUENTMEASUREMENT; // frequent measurements - True or False
	json["DISPLAY_PM1"] = DISPLAY_PM1; // True or False
	
	json["DUST_TIME"] = DUST_TIME; // frequency of PM measurements;  default - 1
	json["NUMBEROFMEASUREMENTS"] = NUMBEROFMEASUREMENTS; //  default - 10
	
	json["SENDING_FREQUENCY"] = SENDING_FREQUENCY; // default - 2
	json["SENDING_DB_FREQUENCY"] = SENDING_DB_FREQUENCY; //  default - 2
	
	json["LUFTDATEN_ON"] = LUFTDATEN_ON; // True or False
	
	json["AIRMONITOR_ON"] = AIRMONITOR_ON; // True or False
	json["AIRMONITOR_GRAPH_ON"] = AIRMONITOR_GRAPH_ON; // True or False
	
	json["THINGSPEAK_ON"] = THINGSPEAK_ON; // True or False
	json["THINGSPEAK_GRAPH_ON"] = THINGSPEAK_GRAPH_ON; // True or False
	
	json["INFLUXDB_ON"] = INFLUXDB_ON; // True or False
	json["MQTT_ON"] = MQTT_ON; // True or False
	
	json["DEEPSLEEP_ON"] = DEEPSLEEP_ON; // True or False
	json["AUTOUPDATE_ON"] = AUTOUPDATE_ON; // True or False
	json["MODEL"] = MODEL; // default "white" - automatic calibration, "red" - without calibration
	
	json["LATITUDE"] = String(LATITUDE, 6); // default - 50.2639
    json["LONGITUDE"] = String(LONGITUDE, 6); //  default - 18.9957
	json["MYALTITUDE"] = MYALTITUDE; // int;  default - 271.00
	
	//PM data
	if (!strcmp(DUST_MODEL, "PMS7003")) {
		json["DUST_MODEL"] = "PMS5003/7003";
	}
	if (!strcmp(DUST_MODEL, "HPMA115S0")) {
		json["DUST_MODEL"] = "HPMA115S0";
	}
	if (!strcmp(DUST_MODEL, "SDS011/21")) {
		json["DUST_MODEL"] = "SDS011/21";
	}
	if (!strcmp(DUST_MODEL, "SPS30")) {
		json["DUST_MODEL"] = "SPS30";
	}
	if (!strcmp(DUST_MODEL, "Non")) {
		json["DUST_MODEL"] = "Non";
	}
	json["PM1"] = averagePM1;
	json["PM25"] = averagePM25;
	json["PM4"] = averagePM4;
    json["PM10"] = averagePM10;
	
	// Temp/Humi/Pressure data
	if (strcmp(THP_MODEL, "Non")) {
		if (!strcmp(THP_MODEL, "BME280")) {
			json["THP_MODEL"] = "BME280";
    		json["Temperature"] = float(bme.temperature);
    		json["Humidity"] = float(bme.humidity);
			json["Pressure"] = float(bme.seaLevelForAltitude(MYALTITUDE));
		}
	} else {
		json["THP_MODEL"] = "Non";
		json["Temperature"] = 0;
		json["Humidity"] = 0;
		json["Pressure"] = 0;
	}
	
    sendSmogListJson(json);
}

void sendDataToSmogList(BME280<BME280_C, BME280_ADDRESS> &bme, int averagePM1, int averagePM25, int averagePM4, int averagePM10) {
    if (!(SMOGLIST_ON)) {
        return;
    }
    sendSmogListData(bme, averagePM1, averagePM25, averagePM4, averagePM10);
}
