#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "bme280.h"

#include "config.h"

const char *luftdatenAPIHOST = "api.luftdaten.info";
const char *luftdatenAPIURL = "/v1/push-sensor-data/";
const uint16_t luftdatenAPIPort = 80;

const char *madavideAPIHOST = "api-rrd.madavi.de";
const char *madavideAPIURL = "/data.php";
const uint16_t madavideAPIPort = 80;

//Luftdaten.info

void sendPMSLuftdatenJson(JsonObject& json) {
    WiFiClient client;
    Serial.print("\nconnecting to ");
    Serial.println(luftdatenAPIHOST);

    if (!client.connect(luftdatenAPIHOST, luftdatenAPIPort)) {
        Serial.println("connection failed");
        Serial.println("wait 3 sec...\n");
        delay(3000);
        return;
    }

    delay(100); 
    client.println("POST "+ String(luftdatenAPIURL) + " HTTP/1.1");
	client.println("Host: "+ String(luftdatenAPIHOST));
	client.println("Content-Type: application/json");
	client.println("X-PIN: 1");
	client.println("X-Sensor: esp8266-" + String(ESP.getChipId()));
	client.print("Content-Length: ");
	client.println(measureJson(json));
	client.println("Connection: close");
    client.println();
	
	serializeJson(json, client);

    String line = client.readStringUntil('\r');
    // TODO: Support wrong error (!= 200)

    if (DEBUG) {
		Serial.println();
		
	    Serial.println("POST "+ String(luftdatenAPIURL) + " HTTP/1.1");
		Serial.println("Host: "+ String(luftdatenAPIHOST));
		Serial.println("Content-Type: application/json");
		Serial.println("X-PIN: 1");
		Serial.println("X-Sensor: esp8266-" + String(ESP.getChipId()));
		Serial.print("Content-Length: ");
		Serial.println(measureJson(json));
		Serial.println("Connection: close");
	    Serial.println();
		
		serializeJsonPretty(json, Serial);
		Serial.println("\n");
		
        Serial.println(line);
		
    }

    client.stop();
	
}

void sendBMELuftdatenJson(JsonObject& json) {
    WiFiClient client;
    Serial.print("\nconnecting to ");
    Serial.println(luftdatenAPIHOST);

    if (!client.connect(luftdatenAPIHOST, luftdatenAPIPort)) {
        Serial.println("connection failed");
        Serial.println("wait 3 sec...\n");
        delay(3000);
        return;
    }

    delay(100); 
    client.println("POST "+ String(luftdatenAPIURL) + " HTTP/1.1");
	client.println("Host: "+ String(luftdatenAPIHOST));
	client.println("Content-Type: application/json");
	client.println("X-PIN: 11");
	client.println("X-Sensor: esp8266-" + String(ESP.getChipId()));
	client.print("Content-Length: ");
	client.println(measureJson(json));
	client.println("Connection: close");
    client.println();
	
	serializeJson(json, client);

    String line = client.readStringUntil('\r');
    // TODO: Support wrong error (!= 200)

    if (DEBUG) {
		Serial.println();
		
	    Serial.println("POST "+ String(luftdatenAPIURL) + " HTTP/1.1");
		Serial.println("Host: "+ String(luftdatenAPIHOST));
		Serial.println("Content-Type: application/json");
		Serial.println("X-PIN: 11");
		Serial.println("X-Sensor: esp8266-" + String(ESP.getChipId()));
		Serial.print("Content-Length: ");
		Serial.println(measureJson(json));
		Serial.println("Connection: close");
	    Serial.println();
		
		serializeJsonPretty(json, Serial);
		Serial.println("\n");
		
        Serial.println(line);
		
    }

    client.stop();
	
}

void sendPMSDatatoLuftdaten(int averagePM1, int averagePM25, int averagePM10) {
	if (strcmp(DUST_MODEL, "Non")) {
	StaticJsonDocument<600> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
	if (!strcmp(DUST_MODEL, "PMS7003")) {
		json["software_version"] = "Smogomierz_" + String(SOFTWAREVERSION);
		JsonArray sensordatavalues = json.createNestedArray("sensordatavalues");
		  JsonObject P1 = sensordatavalues.createNestedObject();
		  	P1["value_type"] = "P1";
		  	P1["value"] = String(averagePM10);
		  JsonObject P2 = sensordatavalues.createNestedObject();
		  	P2["value_type"] = "P2";
		  	P2["value"] = String(averagePM25);
    sendPMSLuftdatenJson(json);
	}
	}
}

void sendBMEDatatoLuftdaten(BME280<BME280_C, BME280_ADDRESS> &bme) {
	if (strcmp(THP_MODEL, "Non")) {
	StaticJsonDocument<600> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
	if (!strcmp(THP_MODEL, "BME280")) {
		json["software_version"] = "Smogomierz_" + String(SOFTWAREVERSION);
		JsonArray sensordatavalues = json.createNestedArray("sensordatavalues");
		  JsonObject temperature = sensordatavalues.createNestedObject();
		  	temperature["value_type"] = "temperature";
		  	temperature["value"] = String(float(bme.temperature));
		  JsonObject humidity = sensordatavalues.createNestedObject();
		  	humidity["value_type"] = "humidity";
		  	humidity["value"] = String(float(bme.humidity));
		  JsonObject pressure = sensordatavalues.createNestedObject();
		  	pressure["value_type"] = "pressure";
		  	pressure["value"] = String(float((bme.seaLevelForAltitude(MYALTITUDE))*100));  //hPa -> Pa
    sendBMELuftdatenJson(json);
	}
	}
}

//Madavi.de

void sendPMSMadavideJson(JsonObject& json) {
    WiFiClient client;
    Serial.print("\nconnecting to ");
    Serial.println(madavideAPIHOST);

    if (!client.connect(madavideAPIHOST, madavideAPIPort)) {
        Serial.println("connection failed");
        Serial.println("wait 3 sec...\n");
        delay(3000);
        return;
    }

    delay(100); 
    client.println("POST "+ String(madavideAPIURL) + " HTTP/1.1");
	client.println("Host: "+ String(madavideAPIHOST));
	client.println("Content-Type: application/json");
	client.println("X-PIN: 1");
	client.println("X-Sensor: esp8266-" + String(ESP.getChipId()));
	client.print("Content-Length: ");
	client.println(measureJson(json));
	client.println("Connection: close");
    client.println();
	
	serializeJson(json, client);

    String line = client.readStringUntil('\r');
    // TODO: Support wrong error (!= 200)

    if (DEBUG) {
		Serial.println();
		
	    Serial.println("POST "+ String(madavideAPIURL) + " HTTP/1.1");
		Serial.println("Host: "+ String(madavideAPIHOST));
		Serial.println("Content-Type: application/json");
		Serial.println("X-PIN: 1");
		Serial.println("X-Sensor: esp8266-" + String(ESP.getChipId()));
		Serial.print("Content-Length: ");
		Serial.println(measureJson(json));
		Serial.println("Connection: close");
	    Serial.println();
		
		serializeJsonPretty(json, Serial);
		Serial.println("\n");
		
        Serial.println(line);
		
    }

    client.stop();
	
}

void sendBMEMadavideJson(JsonObject& json) {
    WiFiClient client;
    Serial.print("\nconnecting to ");
    Serial.println(madavideAPIHOST);

    if (!client.connect(madavideAPIHOST, madavideAPIPort)) {
        Serial.println("connection failed");
        Serial.println("wait 3 sec...\n");
        delay(3000);
        return;
    }

    delay(100); 
    client.println("POST "+ String(madavideAPIURL) + " HTTP/1.1");
	client.println("Host: "+ String(madavideAPIHOST));
	client.println("Content-Type: application/json");
	client.println("X-PIN: 11");
	client.println("X-Sensor: esp8266-" + String(ESP.getChipId()));
	client.print("Content-Length: ");
	client.println(measureJson(json));
	client.println("Connection: close");
    client.println();
	
	serializeJson(json, client);

    String line = client.readStringUntil('\r');
    // TODO: Support wrong error (!= 200)

    if (DEBUG) {
		Serial.println();
		
	    Serial.println("POST "+ String(madavideAPIURL) + " HTTP/1.1");
		Serial.println("Host: "+ String(madavideAPIHOST));
		Serial.println("Content-Type: application/json");
		Serial.println("X-PIN: 11");
		Serial.println("X-Sensor: esp8266-" + String(ESP.getChipId()));
		Serial.print("Content-Length: ");
		Serial.println(measureJson(json));
		Serial.println("Connection: close");
	    Serial.println();
		
		serializeJsonPretty(json, Serial);
		Serial.println("\n");
		
        Serial.println(line);
		
    }

    client.stop();
	
}

void sendPMSDatatoMadavide(int averagePM1, int averagePM25, int averagePM10) {
	if (strcmp(DUST_MODEL, "Non")) {
	StaticJsonDocument<600> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
	if (!strcmp(DUST_MODEL, "PMS7003")) {
		json["software_version"] = "Smogomierz_" + String(SOFTWAREVERSION);
		JsonArray sensordatavalues = json.createNestedArray("sensordatavalues");
		  JsonObject P1 = sensordatavalues.createNestedObject();
		  	P1["value_type"] = "PMS_P1";
		  	P1["value"] = averagePM10;
		  JsonObject P2 = sensordatavalues.createNestedObject();
		  	P2["value_type"] = "PMS_P2";
		  	P2["value"] = averagePM25;
	}
    sendPMSMadavideJson(json);
	}
}

void sendBMEDatatoMadavide(BME280<BME280_C, BME280_ADDRESS> &bme) {
	if (strcmp(THP_MODEL, "Non")) {
	StaticJsonDocument<600> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();
	if (!strcmp(THP_MODEL, "BME280")) {
		json["software_version"] = "Smogomierz_" + String(SOFTWAREVERSION);
		JsonArray sensordatavalues = json.createNestedArray("sensordatavalues");
		  JsonObject temperature = sensordatavalues.createNestedObject();
		  	temperature["value_type"] = "BME280_temperature";
		  	temperature["value"] = float(bme.temperature);
		  JsonObject humidity = sensordatavalues.createNestedObject();
		  	humidity["value_type"] = "BME280_humidity";
		  	humidity["value"] = float(bme.humidity);
		  JsonObject pressure = sensordatavalues.createNestedObject();
		  	pressure["value_type"] = "BME280_pressure";
		  	pressure["value"] = float((bme.seaLevelForAltitude(MYALTITUDE))*100); //hPa -> Pa
	}
    sendBMEMadavideJson(json);
	}
}

void sendDataToLuftdaten(BME280<BME280_C, BME280_ADDRESS> &bme, int averagePM1, int averagePM25, int averagePM10) {
    if (!(LUFTDATEN_ON)) {
        return;
    }

    sendPMSDatatoLuftdaten(averagePM1, averagePM25, averagePM10);
    sendBMEDatatoLuftdaten(bme);
	
	delay(10);
	
    sendPMSDatatoMadavide(averagePM1, averagePM25, averagePM10);
    sendBMEDatatoMadavide(bme);
}



