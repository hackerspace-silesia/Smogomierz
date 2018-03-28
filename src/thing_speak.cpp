#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "pms.h"
#include "bme280.h"

#include "../config.h"

void sendDataToThingSpeak(BME280<> &BMESensor, const PMS::DATA &pms, float calib) {
    if (!(THINGSPEAK_ON)) {
        return;
    }

    WiFiClient client;
    ThingSpeak.begin(client);
    ThingSpeak.setField(1, calib * pms.PM_AE_UG_1_0);
    ThingSpeak.setField(2, calib * pms.PM_AE_UG_2_5);
    ThingSpeak.setField(3, calib * pms.PM_AE_UG_10_0);
    ThingSpeak.setField(4, BMESensor.temperature);
    ThingSpeak.setField(5, BMESensor.pressure  / 100.0F);
    ThingSpeak.setField(6, BMESensor.humidity);
    ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY);
    client.stop();
}
