#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "bme280.h"

#include "config.h"

void sendDataToThingSpeak(BME280<> &BMESensor, int averagePM1, int averagePM25, int averagePM10) {
    if (!(THINGSPEAK_ON)) {
        return;
    }

    WiFiClient client;
    ThingSpeak.begin(client);
    ThingSpeak.setField(1, averagePM1);
    ThingSpeak.setField(2, averagePM25);
    ThingSpeak.setField(3, averagePM10);
	if (!strcmp(THP_MODEL, "BME280")) {
	    ThingSpeak.setField(4, BMESensor.temperature);
	    ThingSpeak.setField(5, BMESensor.seaLevelForAltitude(MYALTITUDE));
	    ThingSpeak.setField(6, BMESensor.humidity);
	}
	ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY);
    client.stop();
}