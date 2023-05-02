#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#endif

#include "ThingSpeak.h"
#include "../../config.h"

void sendDataToThingSpeak() {
    if (!(thingSpeakSettings.enabled)) {
        return;
    }

    WiFiClient client;
    ThingSpeak.begin(client);
	if (strcmp(sensorsSettings.dustModel, "Non")) {
		ThingSpeak.setField(1, measurementsData.averagePM1);
    	ThingSpeak.setField(2, measurementsData.averagePM25);
    	ThingSpeak.setField(3, measurementsData.averagePM10);
	}
	if (strcmp(sensorsSettings.thpModel, "Non")) {
	    ThingSpeak.setField(4, measurementsData.temperature);
	    ThingSpeak.setField(5, measurementsData.pressure);
	    ThingSpeak.setField(6, measurementsData.humidity);
	}
    ThingSpeak.writeFields(thingSpeakSettings.channelId, thingSpeakSettings.apiKey);
    client.stop();
}
