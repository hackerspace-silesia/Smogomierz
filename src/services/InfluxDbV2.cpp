/**
    ESP8266 InfluxDbV2: InfluxdbV2.cpp

    Purpose: Helps with sending measurements to an Influx database.

    @author David G. Simmons Forked from:
    @author Tobias Schürg
*/
#include "InfluxDbV2.h"
#include "Arduino.h"

#include "../../config.h"

/**
 * Construct an InfluxDbV2 instance.
 * @param host the InfluxDb host
 * @param port the InfluxDb port
 */
InfluxdbV2::InfluxdbV2(String host, uint16_t port) {
  _port = port;
  _host = String(host);
}

/**
 * Set the database to be used.
 * @param db the Influx Database to be written to.
 */
void InfluxdbV2::setDb(String db) {
  _db = String(db);
  begin();
}

/**
 * Set the database to be used with authentication.
 */
void InfluxdbV2::setDbAuth(String db, String user, String pass) {
  _db = String(db);
  _user = String(user);
  _pass = String(pass);
  begin();
}

/**
 * Set the Bucket to be used.
 * @param bucket the InfluxDB Bucket which must already exist
 */
void InfluxdbV2::setBucket(String bucket) {
  _bucket = String(bucket);
  //begin();
}

/**
 * Set the Organization to be used
 * @param org the Name of the organization unit to use which must already exist
 */
void InfluxdbV2::setOrg(String org){
  _org = String(org);
  //begin();
}

/**
 * Set the authorization token
 * @param token the Auth Token from InfluxDBv2 *required*
 */
void InfluxdbV2::setToken(String token){
  _token = String(token);
  //begin();
}

void InfluxdbV2::begin() {
  // TODO: recreate HttpClient on db change?
	WiFiClient client_influxDB;

	if (!strcmp(INFLUXDB_VERSION, "2")) {	
		
		http.begin(client_influxDB, _host, _port, "/api/v2/write?org=" + _org + "&bucket=" + _bucket);
		
		http.addHeader("Authorization", "Token " + _token);
	  	http.addHeader("Content-Type", "text/plain");
	} else {
		if (_user and _pass) {
			http.begin(client_influxDB, _host, _port, "/write?u=" + _user + "&p=" + _pass + "&db=" + _db);
		} else {
			http.begin(client_influxDB, _host, _port, "/write?&db=" + _db);
		}
		http.addHeader("Content-Type", "text/plain");
	}
}

/**
 * Prepare a measurement to be sent.
 */
void InfluxdbV2::prepare(InfluxDataV2 data) { prepared.push_back(data); }

/**
 * Write all prepared measurements into the db.
 */
boolean InfluxdbV2::write() {
  String data = "";
  for (auto const& i : prepared) {
    data = (data == "") ? (i.toString()) : (data + "\n" + i.toString());
  }
  prepared.clear();
  return write(data);
}

/**
 * Write a single measurement into the db.
 */
boolean InfluxdbV2::write(InfluxDataV2 data) { return write(data.toString()); }

/**
 * Send raw data to InfluxDb.
 *
 * @see
 * https://github.com/esp8266/Arduino/blob/cc0bfa04d401810ed3f5d7d01be6e88b9011997f/libraries/ESP8266HTTPClient/src/ESP8266HTTPClient.h#L44-L55
 * for a list of error codes.
 */
boolean InfluxdbV2::write(String data) {
	if (!strcmp(INFLUXDB_VERSION, "2")) {	
		if(_token == 0 || _token.length() < 10){
    		//Serial.println("#####\nInvalid Access Token\n#####");
    	return false;
  		}
 	}
  //Serial.print(" --> writing to host: http://" + String(_host) + " Port: " + String(_port) + " URL: /write?" + "u=" + String(_user) + "&p=" + String(_pass) + "&db=" + String(_db) + ":\n");
  //Serial.println(data);

  int httpResponseCode = http.POST(data);
  //Serial.print(" <-- Response: ");
  //Serial.print(httpResponseCode);

  String response = http.getString();
  //Serial.println(" \"" + response + "\"");

  boolean success;
  if (httpResponseCode == 204) {
    success = true;
  } else {
    //Serial.println("#####\nPOST FAILED\n#####");
    success = false;
  }

  http.end();
  return success;
}
