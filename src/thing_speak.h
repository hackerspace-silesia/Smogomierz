#pragma once

void sendDataToThingSpeak(BME280<BME280_C, BME280_ADDRESS>&, const PMS::DATA&, float calib);
