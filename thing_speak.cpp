#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "pms.h"
#include "bme280.h"

#include "config.h"

void sendDataToThingSpeak(BME280<> &bme, const PMS::DATA &pms) {
    if (!(THINGSPEAK_ON)) {
        return;
    }

    WiFiClient client;
    ThingSpeak.begin(client);
    ThingSpeak.setField(1, calib1 * pms.PM_AE_UG_1_0);
    ThingSpeak.setField(2, calib1 * pms.PM_AE_UG_2_5);
    ThingSpeak.setField(3, calib1 * pms.PM_AE_UG_10_0);
    ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY);
    client.stop();
}
