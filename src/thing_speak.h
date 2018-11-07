#pragma once

void sendDataToThingSpeak(BME280<BME280_C, BME280_ADDRESS>&, int averagePM1, int averagePM25, int averagePM10);