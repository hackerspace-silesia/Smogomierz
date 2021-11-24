
/**
    ESP8266 InfluxDbV2: InfluxdbV2.h

    Purpose: Helps with sending measurements to an Influx V2 database.

    @author David G. Simmons Forked from:
    @author Tobias Schürg
*/
#if defined(ESP8266)
#include <ESP8266HTTPClient.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#endif

#include <list>
#include "Arduino.h"

#include "InfluxDataV2.h"

class InfluxdbV2 {
 public:
  InfluxdbV2(String host, uint16_t port = 9999);

  void setDb(String db);
  void setDbAuth(String db, String user, String pass);
  
  void setBucket(String bucket);
  void setOrg(String org);
  void setToken(String token);

  void prepare(InfluxDataV2 data);
  boolean write();

  boolean write(InfluxDataV2 data);
  boolean write(String data);

 private:
  HTTPClient http;
  String _host;
  uint16_t _port;
  String _db;
  String _user;
  String _pass;
  String _bucket;
  String _org;
  String _token;
  std::list<InfluxDataV2> prepared;
  
  void begin();
};
