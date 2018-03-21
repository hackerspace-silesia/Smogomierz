#include "ArduinoJson.h"
#include "FS.h"
#include "config.h"

// FS config
String path = "/config.h";
char json;
String ustawienia;
String conf[20];
// FS config end

void createConfigDataJSON() {
	//#include "config.h"
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();

    json["DEVICENAME_AUTO"] = DEVICENAME_AUTO;
    json["DEVICENAME"] = DEVICENAME;
    
    json["AIRMONITOR_ON"] = AIRMONITOR_ON;
    json["AIRMONITOR_GRAPH_ON"] = AIRMONITOR_GRAPH_ON;
    json["LATITUDE"] = LATITUDE;
    json["LONGITUDE"] = LONGITUDE;
    json["MYALTITUDE"] = MYALTITUDE;
    
    json["THINGSPEAK_ON"] = THINGSPEAK_ON;
    json["THINGSPEAK_GRAPH_ON"] = THINGSPEAK_GRAPH_ON;
    json["THINGSPEAK_API_KEY"] = THINGSPEAK_API_KEY;
    json["THINGSPEAK_CHANNEL_ID"] = THINGSPEAK_CHANNEL_ID;
    
    json["DEBUG"] = DEBUG;
    json["calib1"] = calib1;
    json["calib2"] = calib2;
    json["SOFTWAREVERSION"] = SOFTWAREVERSION;

    json.printTo(ustawienia);
    
    //json.printTo(Serial); // wyświetlenie stowrzonego jsona
}

void readConfigDataJSON() {
    char copy[400];
    int ind[20];
    
    File UserConfig = SPIFFS.open(path, "r");
    String s = UserConfig.readString();
    s.toCharArray(copy, 400);
    
    for(int i = 1; i < 16; i++) {
      ind[i] = s.indexOf(',', ind[i-1]+1 );
      conf[i] = s.substring(ind[i-1]+1, ind[i]);

      // Serial.println(conf[i]);
    }
    UserConfig.close();

    for(int i = 0; i < 15; i++) {
      ind[i] = conf[i+1].indexOf(':');
      if (i==14) {
        conf[i] = conf[i+1].substring(ind[i]+1, conf[i+1].length() - 3);
      }else{
        conf[i] = conf[i+1].substring(ind[i]+1, conf[i+1].length() - 0);
      }
    /*  
      Serial.print("conf[");
      Serial.print(i);
      Serial.print("] - ");
      Serial.print(conf[i]);
      Serial.print("\n");
      */
    }
    if (conf[0]){
       bool DEVICENAME_AUTO = true;
    }else{
       bool DEVICENAME_AUTO = false;
    }
    #define DEVICENAME conf[1]
    if (conf[2]){
       bool AIRMONITOR_ON = true;
    }else{
       bool AIRMONITOR_ON = false;
    }
    if (conf[3]){
       bool AIRMONITOR_GRAPH_ON = true;
    }else{
       bool AIRMONITOR_GRAPH_ON = false;
    }
     float LATITUDE = conf[4].toFloat();
     float LONGITUDE = conf[5].toFloat();
     float MYALTITUDE = conf[6].toFloat();
    if (conf[7]){
       bool THINGSPEAK_ON = true;
    }else{
       bool THINGSPEAK_ON = false;
    }
    if (conf[8]){
       bool THINGSPEAK_GRAPH_ON = true;
    }else{
       bool THINGSPEAK_GRAPH_ON = false;
    }
    #define THINGSPEAK_API_KEY conf[9]
    const int THINGSPEAK_CHANNEL_ID = conf[10].toInt();
    if (conf[11]){
       bool DEBUG = true;
    }else{
       bool DEBUG = false;
    }
     float calib1 = conf[12].toFloat();
     float calib2 = conf[13].toFloat();
    #define SOFTWAREVERSION conf[14]

    Serial.print("\nDane z pliku config.h:\n");
    Serial.print("DEVICENAME_AUTO = ");
    Serial.print(DEVICENAME_AUTO);
    Serial.print("\nDEVICENAME = ");
    Serial.print(DEVICENAME);
    
    Serial.print("\nAIRMONITOR_ON = ");
    Serial.print(AIRMONITOR_ON);
    Serial.print("\nAIRMONITOR_GRAPH_ON = ");
    Serial.print(AIRMONITOR_GRAPH_ON);
    Serial.print("\nLATITUDE = ");
    Serial.print(String(LATITUDE, 4));
    Serial.print("\nLONGITUDE = ");
    Serial.print(String(LONGITUDE, 4));
    Serial.print("\nMYALTITUDE = ");
    Serial.print(int(MYALTITUDE));
    
    Serial.print("\nTHINGSPEAK_ON = ");
    Serial.print(THINGSPEAK_ON);
    Serial.print("\nTHINGSPEAK_GRAPH_ON = ");
    Serial.print(THINGSPEAK_GRAPH_ON);
    Serial.print("\nTHINGSPEAK_API_KEY = ");
    Serial.print(THINGSPEAK_API_KEY);
    Serial.print("\nTHINGSPEAK_CHANNEL_ID = ");
    Serial.print(THINGSPEAK_CHANNEL_ID);
    
    Serial.print("\nDEBUG = ");
    Serial.print(DEBUG);
    Serial.print("\ncalib1 = ");
    Serial.print(calib1);
    Serial.print("\ncalib2 = ");
    Serial.print(calib2);
    Serial.print("\nSOFTWAREVERSION = ");
    Serial.print(SOFTWAREVERSION);
    Serial.print("\n");
}

void deleteConfigDataJSON() {
	SPIFFS.remove(path); 
}

void fs_start() {
    
	if (!SPIFFS.exists(path)){
	    Serial.println("\nconfig.h NIE istnieje!\n");
	    SPIFFS.format();
    
	    Serial.println("Tworzenie pliku config.h!");
	    File UserConfig = SPIFFS.open(path, "w");
    
	    Serial.println("Zapisywanie danych do config.h...");

	    createConfigDataJSON();
	    UserConfig.println(ustawienia);
	    UserConfig.close();
	    Serial.println("\nDane zapisane :)\n");

	    readConfigDataJSON();

	    }else{
    
	    Serial.println("\nconfig.h istnieje!\n");
    
	    readConfigDataJSON();
		
	    //Serial.println("\nKasowanie pliku config.h!\n");
		//deleteConfigDataJSON();
	}
}