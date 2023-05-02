#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#endif

#include <ArduinoJson.h>
#include "../../config.h"

const char *SmoglistServerName PROGMEM = "api.smoglist.pl"; // api.smoglist.pl:8090/postjson
const uint16_t SmoglistPort = 8090;

#ifdef ARDUINO_ARCH_ESP8266
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
    if (deviceSettings.debug) {
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
			if (deviceSettings.debug) {
				Serial.println(F("\n")+ line);
			}
		}
	client.stop();
	}
}

void sendSmoglistData() {
	StaticJsonDocument<1024> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
	
#ifdef ARDUINO_ARCH_ESP8266
	json[F("CHIPID")] = F("Smogly-") + String(ESP.getChipId());
#elif defined ARDUINO_ARCH_ESP32
	json[F("CHIPID")] = F("Smogly-") + String((uint32_t)(ESP.getEfuseMac()));
#endif
	json[F("SOFTWAREVERSION")] = String(softwareVersion);
	json[F("HARDWAREVERSION")] = String(hardwareVersion); // "1.0 - ESP8266" or "2.0 - ESP32"
	json[F("PMSENSORVERSION")] = String(pmSensorVersion); // PMS, SDS, HPMA115S0 ora SPS30
	
	json[F("FREQUENTMEASUREMENT")] = int(sensorsSettings.continuousMeasurement); // frequent measurements - True or False
	json[F("DISPLAY_PM1")] = bool(deviceSettings.displayPM1); // True or False
	
	json[F("DUST_TIME")] = int(sensorsSettings.dustTime); // frequency of PM measurements;  default - 1
	json[F("NUMBEROFMEASUREMENTS")] = int(sensorsSettings.numerOfMeasurements); //  default - 10
	
	json[F("SENDING_FREQUENCY")] = int(deviceSettings.sendingServicesFrequency); // default - 2
	json[F("SENDING_DB_FREQUENCY")] = int(deviceSettings.sendingDBFrequency); //  default - 2
	
	json[F("LUFTDATEN_ON")] = bool(luftdatenSettings.enabled); // True or False
	json["LUFTDATEN_GRAPH_ON"] = bool(luftdatenSettings.graph); // True or False
	
	json[F("AIRMONITOR_ON")] = bool(airMonitorSettings.enabled); // True or False
	json[F("AIRMONITOR_GRAPH_ON")] = bool(airMonitorSettings.graph); // True or False
	
	json[F("THINGSPEAK_ON")] = bool(thingSpeakSettings.enabled); // True or False
	json[F("THINGSPEAK_GRAPH_ON")] = bool(thingSpeakSettings.graph); // True or False
	
	json[F("INFLUXDB_ON")] = bool(influxDBSettings.enabled); // True or False
	json[F("MQTT_ON")] = bool(mqttSettings.enabled); // True or False
	
	json[F("DEEPSLEEP_ON")] = bool(deviceSettings.deepSleep); // True or False
	json[F("AUTOUPDATE_ON")] = bool(deviceSettings.autoUpdate); // True or False
	json[F("MODEL")] = bool(deviceSettings.autoCalibration); // default "white" - automatic calibration, "red" - without calibration
	
	json[F("LATITUDE")] = String(deviceSettings.latitude); // default - 50.2639
    json[F("LONGITUDE")] = String(deviceSettings.longitude); //  default - 18.9957

	json[F("MYALTITUDE")] = int(deviceSettings.altitude); // int;  default - 271.00
	
	//PM data
	if (!strcmp(sensorsSettings.dustModel, "PMS7003")) {
		json[F("DUST_MODEL")] = F("PMS5003/7003");
	}
	if (!strcmp(sensorsSettings.dustModel, "HPMA115S0")) {
		json[F("DUST_MODEL")] = F("HPMA115S0");
	}
	if (!strcmp(sensorsSettings.dustModel, "SDS011/21")) {
		json[F("DUST_MODEL")] = F("SDS011/21");
	}
	if (!strcmp(sensorsSettings.dustModel, "SPS30")) {
		json[F("DUST_MODEL")] = F("SPS30");
	}
	if (!strcmp(sensorsSettings.dustModel, "Non")) {
		json[F("DUST_MODEL")] = F("Non");
	}
	json[F("PM1")] = int(measurementsData.averagePM1);
	json[F("PM25")] = int(measurementsData.averagePM25);
	json[F("PM4")] = int(measurementsData.averagePM4);
    json[F("PM10")] = int(measurementsData.averagePM10);
	
	// Temp/Humi/Pressure data
	if (strcmp(sensorsSettings.thpModel, "Non")) {
		if (!strcmp(sensorsSettings.thpModel, "BME280")) {
			json[F("THP_MODEL")] = F("BME280");
		} else if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
			json[F("THP_MODEL")] = F("BMP280");
  		} else if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
			json[F("THP_MODEL")] = F("HTU21");
  		} else if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
			json[F("THP_MODEL")] = F("DHT22");
  		} else if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
  			json[F("THP_MODEL")] = F("SHT1x");
  		} else if (!strcmp(sensorsSettings.thpModel, "DS18B20")) {
  			json[F("THP_MODEL")] = F("DS18B20");
  		}
		json[F("Temperature")] = float(measurementsData.temperature);
		json[F("Humidity")] = float(measurementsData.humidity);
		json[F("Pressure")] = float(measurementsData.pressure);
	} else {
		json[F("THP_MODEL")] = F("Non");
		json[F("Temperature")] = 0;
		json[F("Humidity")] = 0;
		json[F("Pressure")] = 0;
	}
    sendSmoglistJson(json);
}


#elif defined ARDUINO_ARCH_ESP32
void sendSmoglistJson(JsonObject& json) {
    if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    WiFiClient client;
	client.setTimeout(12000);
    Serial.print("\nconnecting to ");
    Serial.println(SmoglistServerName);

    if (!client.connect(SmoglistServerName, SmoglistPort)) {
        Serial.println("connection failed");
        //Serial.println("wait 3 sec...\n";
        //delay(3000);
        return;
    }
    delay(100); 

    client.println("POST /postjson HTTP/1.1");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
	client.println(measureJson(json));
    client.println();
	serializeJson(json, client);
    if (deviceSettings.debug) {
		serializeJsonPretty(json, Serial);
	}
		unsigned long timeout = millis();
		while (client.available() == 0) {
			if (millis() - timeout > 5000) {
	            Serial.println("\n\t>>> Client Timeout!\n");
	            client.stop();
	            return;
	        }
	    }
		if(client.available()) {
			String line = client.readStringUntil('\r');
			line.trim();
			if (deviceSettings.debug) {
				Serial.println("\n"+ line);
			}
		}
	client.stop();
	}
}

void sendSmoglistData() {
	StaticJsonDocument<1024> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
	
#ifdef ARDUINO_ARCH_ESP8266
	json["CHIPID"] = "Smogly-" + String(ESP.getChipId());
#elif defined ARDUINO_ARCH_ESP32
	json["CHIPID"] = "Smogly-" + String((uint32_t)(ESP.getEfuseMac()));
#endif
	json["SOFTWAREVERSION"] = String(softwareVersion);
	json["HARDWAREVERSION"] = String(hardwareVersion); // "1.0 - ESP8266" or "2.0 - ESP32"
	json["PMSENSORVERSION"] = String(pmSensorVersion); // PMS, SDS, HPMA115S0 ora SPS30
	
	json["FREQUENTMEASUREMENT"] = int(sensorsSettings.continuousMeasurement); // frequent measurements - True or False
	json["DISPLAY_PM1"] = bool(deviceSettings.displayPM1); // True or False
	
	json["DUST_TIME"] = int(sensorsSettings.dustTime); // frequency of PM measurements;  default - 1
	json["NUMBEROFMEASUREMENTS"] = int(sensorsSettings.numerOfMeasurements); //  default - 10
	
	json["SENDING_FREQUENCY"] = int(deviceSettings.sendingServicesFrequency); // default - 2
	json["SENDING_DB_FREQUENCY"] = int(deviceSettings.sendingDBFrequency); //  default - 2
	
	json["LUFTDATEN_ON"] = bool(luftdatenSettings.enabled); // True or False
	json["LUFTDATEN_GRAPH_ON"] = bool(luftdatenSettings.graph); // True or False

	json["AIRMONITOR_ON"] = bool(airMonitorSettings.enabled); // True or False
	json["AIRMONITOR_GRAPH_ON"] = bool(airMonitorSettings.graph); // True or False
	
	json["THINGSPEAK_ON"] = bool(thingSpeakSettings.enabled); // True or False
	json["THINGSPEAK_GRAPH_ON"] = bool(thingSpeakSettings.graph); // True or False
	
	json["INFLUXDB_ON"] = bool(influxDBSettings.enabled); // True or False
	json["MQTT_ON"] = bool(mqttSettings.enabled); // True or False
	
	json["DEEPSLEEP_ON"] = bool(deviceSettings.deepSleep); // True or False
	json["AUTOUPDATE_ON"] = bool(deviceSettings.autoUpdate); // True or False
	json["MODEL"] = bool(deviceSettings.autoCalibration); // default "white" - automatic calibration, "red" - without calibration
	
	json["LATITUDE"] = String(deviceSettings.latitude); // default - 50.2639
    json["LONGITUDE"] = String(deviceSettings.longitude); //  default - 18.9957

	json["MYALTITUDE"] = int(deviceSettings.altitude); // int;  default - 271.00
	
	//PM data
	if (!strcmp(sensorsSettings.dustModel, "PMS7003")) {
		json["DUST_MODEL"] = "PMS5003/7003";
	}
	if (!strcmp(sensorsSettings.dustModel, "HPMA115S0")) {
		json["DUST_MODEL"] = "HPMA115S0";
	}
	if (!strcmp(sensorsSettings.dustModel, "SDS011/21")) {
		json["DUST_MODEL"] = "SDS011/21";
	}
	if (!strcmp(sensorsSettings.dustModel, "SPS30")) {
		json["DUST_MODEL"] = "SPS30";
	}
	if (!strcmp(sensorsSettings.dustModel, "Non")) {
		json["DUST_MODEL"] = "Non";
	}
	json["PM1"] = int(measurementsData.averagePM1);
	json["PM25"] = int(measurementsData.averagePM25);
	json["PM4"] = int(measurementsData.averagePM4);
    json["PM10"] = int(measurementsData.averagePM10);
	
	// Temp/Humi/Pressure data
	if (strcmp(sensorsSettings.thpModel, "Non")) {
		if (!strcmp(sensorsSettings.thpModel, "BME280")) {
			json["THP_MODEL"] = "BME280";
		} else if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
			json["THP_MODEL"] = "BMP280";
  		} else if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
			json["THP_MODEL"] = "HTU21";
  		} else if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
			json["THP_MODEL"] = "DHT22";
  		} else if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
  			json["THP_MODEL"] = "SHT1x";
  		} else if (!strcmp(sensorsSettings.thpModel, "DS18B20")) {
  			json["THP_MODEL"] = "DS18B20";
  		}
		json["Temperature"] = float(measurementsData.temperature);
		json["Humidity"] = float(measurementsData.humidity);
		json["Pressure"] = float(measurementsData.pressure);
	} else {
		json["THP_MODEL"] = "Non";
		json["Temperature"] = 0;
		json["Humidity"] = 0;
		json["Pressure"] = 0;
	}
    sendSmoglistJson(json);
}
#endif


void sendDataToSmoglist() {
    if (!(smoglistSettings.enabled)) {
        return;
    }
    sendSmoglistData();
}
