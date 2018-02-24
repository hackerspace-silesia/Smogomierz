#pragma once

void sendDataToInfluxDB(BME280<BME280_C, BME280_ADDRESS>&, const PMS::DATA&);
