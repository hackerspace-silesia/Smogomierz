#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#endif

#include <ArduinoJson.h>
#include "config.h"

const char *SmoglistServerName PROGMEM = "api.smoglist.pl"; // api.smoglist.pl:8090/postjson
const uint16_t SmoglistPort = 8090;

void sendSmoglistJson(JsonObject& json) {
    if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    WiFiClient client;
	client.setTimeout(12000);
    Serial.print(F("\nconnecting to "));
    Serial.println(SmoglistServerName);

    if (!client.connect(SmoglistServerName, SmoglistPort)) {
        Serial.println(F("connection failed"));
        //Serial.println("wait 3 sec...\n");
        //delay(3000);
        return;
    }
    delay(100); 

    client.println(F("POST /postjson HTTP/1.1"));
    client.println(F("Content-Type: application/json"));
    client.print(F("Content-Length: "));
	client.println(measureJson(json));
    client.println();
	serializeJson(json, client);
    if (DEBUG) {
		serializeJsonPretty(json, Serial);
	}
		unsigned long timeout = millis();
		while (client.available() == 0) {
			if (millis() - timeout > 5000) {
	            Serial.println(F("\n\t>>> Client Timeout!\n"));
	            client.stop();
	            return;
	        }
	    }
		if(client.available()) {
			String line = client.readStringUntil('\r');
			line.trim();
			if (DEBUG) {
				Serial.println(F("\n")+ line);
			}
		}
	client.stop();
	}
}

void sendSmoglistData(float & currentTemperature, float & currentPressure, float & currentHumidity, unsigned short & averagePM1, unsigned short & averagePM25, unsigned short & averagePM4, unsigned short & averagePM10) {
	StaticJsonDocument<1024> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
	
#ifdef ARDUINO_ARCH_ESP8266
	json[F("CHIPID")] = F("Smogly-") + String(ESP.getChipId());
#elif defined ARDUINO_ARCH_ESP32
	json[F("CHIPID")] = F("Smogly-") + String((uint32_t)(ESP.getEfuseMac()));
#endif
	json[F("SOFTWAREVERSION")] = String(SOFTWAREVERSION);
	json[F("HARDWAREVERSION")] = String(HARDWAREVERSION); // "1.0 - ESP8266" or "2.0 - ESP32"
	json[F("PMSENSORVERSION")] = String(PMSENSORVERSION); // PMS, SDS, HPMA115S0 ora SPS30
	
	json[F("FREQUENTMEASUREMENT")] = int(FREQUENTMEASUREMENT); // frequent measurements - True or False
	json[F("DISPLAY_PM1")] = bool(DISPLAY_PM1); // True or False
	
	json[F("DUST_TIME")] = int(DUST_TIME); // frequency of PM measurements;  default - 1
	json[F("NUMBEROFMEASUREMENTS")] = int(NUMBEROFMEASUREMENTS); //  default - 10
	
	json[F("SENDING_FREQUENCY")] = int(SENDING_FREQUENCY); // default - 2
	json[F("SENDING_DB_FREQUENCY")] = int(SENDING_DB_FREQUENCY); //  default - 2
	
	json[F("LUFTDATEN_ON")] = bool(LUFTDATEN_ON); // True or False
	
	json[F("AIRMONITOR_ON")] = bool(AIRMONITOR_ON); // True or False
	json[F("AIRMONITOR_GRAPH_ON")] = bool(AIRMONITOR_GRAPH_ON); // True or False
	
	json[F("THINGSPEAK_ON")] = bool(THINGSPEAK_ON); // True or False
	json[F("THINGSPEAK_GRAPH_ON")] = bool(THINGSPEAK_GRAPH_ON); // True or False
	
	json[F("INFLUXDB_ON")] = bool(INFLUXDB_ON); // True or False
	json[F("MQTT_ON")] = bool(MQTT_ON); // True or False
	
	json[F("DEEPSLEEP_ON")] = bool(DEEPSLEEP_ON); // True or False
	json[F("AUTOUPDATE_ON")] = bool(AUTOUPDATE_ON); // True or False
	json[F("MODEL")] = String(MODEL); // default "white" - automatic calibration, "red" - without calibration
	
	json[F("LATITUDE")] = String(LATITUDE); // default - 50.2639
    json[F("LONGITUDE")] = String(LONGITUDE); //  default - 18.9957

	json[F("MYALTITUDE")] = int(MYALTITUDE); // int;  default - 271.00
	
	//PM data
	if (!strcmp(DUST_MODEL, "PMS7003")) {
		json[F("DUST_MODEL")] = F("PMS5003/7003");
	}
	if (!strcmp(DUST_MODEL, "HPMA115S0")) {
		json[F("DUST_MODEL")] = F("HPMA115S0");
	}
	if (!strcmp(DUST_MODEL, "SDS011/21")) {
		json[F("DUST_MODEL")] = F("SDS011/21");
	}
	if (!strcmp(DUST_MODEL, "SPS30")) {
		json[F("DUST_MODEL")] = F("SPS30");
	}
	if (!strcmp(DUST_MODEL, "Non")) {
		json[F("DUST_MODEL")] = F("Non");
	}
	json[F("PM1")] = int(averagePM1);
	json[F("PM25")] = int(averagePM25);
	json[F("PM4")] = int(averagePM4);
    json[F("PM10")] = int(averagePM10);
	
	// Temp/Humi/Pressure data
	if (strcmp(THP_MODEL, "Non")) {
		if (!strcmp(THP_MODEL, "BME280")) {
			json[F("THP_MODEL")] = F("BME280");
		} else if (!strcmp(THP_MODEL, "BMP280")) {
			json[F("THP_MODEL")] = F("BMP280");
  		} else if (!strcmp(THP_MODEL, "HTU21")) {
			json[F("THP_MODEL")] = F("HTU21");
  		} else if (!strcmp(THP_MODEL, "DHT22")) {
			json[F("THP_MODEL")] = F("DHT22");
  		} else if (!strcmp(THP_MODEL, "SHT1x")) {
  			json[F("THP_MODEL")] = F("SHT1x");
  		} else if (!strcmp(THP_MODEL, "DS18B20")) {
  			json[F("THP_MODEL")] = F("DS18B20");
  		}
		json[F("Temperature")] = float(currentTemperature);
		json[F("Humidity")] = float(currentHumidity);
		json[F("Pressure")] = float(currentPressure);
	} else {
		json[F("THP_MODEL")] = F("Non");
		json[F("Temperature")] = 0;
		json[F("Humidity")] = 0;
		json[F("Pressure")] = 0;
	}
    sendSmoglistJson(json);
}

void sendDataToSmoglist(float & currentTemperature, float & currentPressure, float & currentHumidity, unsigned short & averagePM1, unsigned short & averagePM25, unsigned short & averagePM4, unsigned short & averagePM10) {
    if (!(SMOGLIST_ON)) {
        return;
    }
    sendSmoglistData(currentTemperature, currentPressure, currentHumidity, averagePM1, averagePM25, averagePM4, averagePM10);
}
