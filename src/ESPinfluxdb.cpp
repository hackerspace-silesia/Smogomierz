#include "Arduino.h"
#include "ESPinfluxdb.h"

#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#endif
	
//#define DEBUG_PRINT // comment this line to disable debug print

#ifndef DEBUG_PRINT
#define DEBUG_PRINT(a)
#else
#define DEBUG_PRINT(a) (Serial.println(String(F("[Debug]: "))+(a)))
#define _DEBUG
#endif

Influxdb::Influxdb(const char *host, uint16_t port) {
        _port = String(port);
        _host = String(host);
}

DB_RESPONSE Influxdb::opendb(String db, String user, String password) {
        _db = db + "&u=" + user + "&p=" + password;
#if defined(ARDUINO_ARCH_ESP32)
        HTTPClient http;
        http.begin("http://" + String(_host) + ":" + String(_port) + "/query?q=show%20databases"); //HTTP
        int httpCode = http.GET();
        if (httpCode == 200) {
                _response = DB_SUCCESS;
                String payload = http.getString();
                http.end();
                if (payload.indexOf("db") > 0) {
                        _db =  db;
                        Serial.println(payload);
                        return _response;
				}
        }
        _response = DB_ERROR;
        DEBUG_PRINT("Database open failed");
        return _response;
#endif
}

DB_RESPONSE Influxdb::opendb(String db) {
        HTTPClient http;
        http.begin("http://" + String(_host) + ":" + String(_port) + "/query?q=show%20databases"); //HTTP
        int httpCode = http.GET();

        if (httpCode == 200) {
                _response = DB_SUCCESS;
                String payload = http.getString();
                http.end();
                if (payload.indexOf("db") > 0) {
                        _db =  db;
                        Serial.println(payload);
                        return _response;
				}
        }
        _response = DB_ERROR;
        DEBUG_PRINT("Database open failed");
        return _response;
}

DB_RESPONSE Influxdb::write(dbMeasurement data) {
        return write(data.postString());
}

DB_RESPONSE Influxdb::write(String data) {


        HTTPClient http;

        DEBUG_PRINT("HTTP post begin...");

        http.begin("http://" + _host + ":" + _port + "/write?db=" + _db); //HTTP
        http.addHeader("Content-Type", "text/plain");

        int httpResponseCode = http.POST(data);

        if (httpResponseCode == 204) {
                _response = DB_SUCCESS;
#if defined(ARDUINO_ARCH_ESP8266)
                String response = http.getString();    //Get the response to the request
                DEBUG_PRINT(String(httpResponseCode)); //Print return code
                DEBUG_PRINT(response);                 //Print request answer
#endif
        } else {
                DEBUG_PRINT("Error on sending POST:");
                DEBUG_PRINT(String(httpResponseCode));
                _response=DB_ERROR;
        }

        http.end();
        return _response;
}

DB_RESPONSE Influxdb::query(String sql) {

        String url = "/query?";
        url += "pretty=true&";
        url += "db=" + _db;
        url += "&q=" + URLEncode(sql);
        DEBUG_PRINT("Requesting URL: ");
        DEBUG_PRINT(url);

        HTTPClient http;

        http.begin("http://" + _host + ":" + _port + url); //HTTP


        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode == 200) {
                // HTTP header has been send and Server response header has been handled
                _response = DB_SUCCESS;
                String reply = http.getString();
                Serial.println(reply);

        } else {
                _response = DB_ERROR;
                DEBUG_PRINT("[HTTP] GET... failed, error: " + httpCode);
        }

        http.end();
        return _response;
}


DB_RESPONSE Influxdb::response() {
        return _response;
}

/* -----------------------------------------------*/
//        Field object
/* -----------------------------------------------*/
dbMeasurement::dbMeasurement(String m) {
        measurement = m;
}

void dbMeasurement::empty() {
        _data = "";
        _tag = "";
}

void dbMeasurement::addTag(String key, String value) {
        _tag += "," + key + "=" + value;
}

void dbMeasurement::addField(String key, float value) {
        _data = (_data == "") ? (" ") : (_data += ",");
        _data += key + "=" + String(value);
}

String dbMeasurement::postString() {
        //  uint32_t utc = 1448114561 + millis() /1000;
        return measurement + _tag + _data;
}

// URL Encode with Arduino String object
String URLEncode(String msg) {
        const char *hex = "0123456789abcdef";
        String encodedMsg = "";

        uint16_t i;
        for (i = 0; i < msg.length(); i++) {
                if (('a' <= msg.charAt(i) && msg.charAt(i) <= 'z') ||
                    ('A' <= msg.charAt(i) && msg.charAt(i) <= 'Z') ||
                    ('0' <= msg.charAt(i) && msg.charAt(i) <= '9')) {
                        encodedMsg += msg.charAt(i);
                } else {
                        encodedMsg += '%';
                        encodedMsg += hex[msg.charAt(i) >> 4];
                        encodedMsg += hex[msg.charAt(i) & 15];
                }
        }
        return encodedMsg;
}
