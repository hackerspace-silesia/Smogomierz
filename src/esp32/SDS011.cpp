// SDS011 dust sensor PM2.5 and PM10
// ---------------------
//
// By R. Zschiegner (rz@madavi.de)
// April 2016
//
// Documentation:
//		- The iNovaFitness SDS011 datasheet
//

#include "SDS011.h"

static constexpr uint8_t SLEEPCMD[19] = {
	0xAA, 0xB4, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0xFF,	0xFF, 0x05, 0xAB
};
static constexpr uint8_t WAKEUPCMD[19] = {
	0xAA, 0xB4, 0x06, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x06, 0xAB
};
static constexpr uint8_t VERSION_CMD[] PROGMEM = {
	0xAA, 0xB4, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x05, 0xAB
};
static constexpr uint8_t CONTINUOUS_MODE_CMD[] PROGMEM = {
	0xAA, 0xB4, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x07, 0xAB
};

SDS011::SDS011(void) {

}

// --------------------------------------------------------
// SDS011:read
// --------------------------------------------------------
int SDS011::read(float *p25, float *p10) {
	byte buffer;
	int value;
	int len = 0;
	int pm10_serial = 0;
	int pm25_serial = 0;
	int checksum_is;
	int checksum_ok = 0;
	int error = 1;
	while ((sds_data->available() > 0) && (sds_data->available() >= (10-len))) {
		buffer = sds_data->read();
		value = int(buffer);
		switch (len) {
			case (0): if (value != 170) { len = -1; }; break;
			case (1): if (value != 192) { len = -1; }; break;
			case (2): pm25_serial = value; checksum_is = value; break;
			case (3): pm25_serial += (value << 8); checksum_is += value; break;
			case (4): pm10_serial = value; checksum_is += value; break;
			case (5): pm10_serial += (value << 8); checksum_is += value; break;
			case (6): checksum_is += value; break;
			case (7): checksum_is += value; break;
			case (8): if (value == (checksum_is % 256)) { checksum_ok = 1; } else { len = -1; }; break;
			case (9): if (value != 171) { len = -1; }; break;
		}
		len++;
		if (len == 10 && checksum_ok == 1) {
			*p10 = (float)pm10_serial/10.0;
			*p25 = (float)pm25_serial/10.0;
			len = 0; checksum_ok = 0; pm10_serial = 0.0; pm25_serial = 0.0; checksum_is = 0;
			error = 0;
		}
		yield();
	}
	return error;
}

// --------------------------------------------------------
// SDS011:sleep
// --------------------------------------------------------
void SDS011::sleep() {
	for (uint8_t i = 0; i < 19; i++) {
		sds_data->write(SLEEPCMD[i]);
	}
	sds_data->flush();
	while (sds_data->available() > 0) {
		sds_data->read();
	}
}

// --------------------------------------------------------
// SDS011:wakeup
// --------------------------------------------------------
void SDS011::wakeup() {
	for (uint8_t i = 0; i < 19; i++) {
		sds_data->write(WAKEUPCMD[i]);
	}
	sds_data->flush();
	while (sds_data->available() > 0) {
		sds_data->read();
	}
}

// --------------------------------------------------------
// SDS011:continuous_mode
// --------------------------------------------------------
void SDS011::continuous_mode() {
	for (uint8_t i = 0; i < 19; i++) {
		sds_data->write(CONTINUOUS_MODE_CMD[i]);
	}
	sds_data->flush();
	while (sds_data->available() > 0) {
		sds_data->read();
	}
}

#ifndef ESP32
void SDS011::begin(uint8_t pin_rx, uint8_t pin_tx) {
	_pin_rx = pin_rx;
	_pin_tx = pin_tx;

	SoftwareSerial *softSerial = new SoftwareSerial(_pin_rx, _pin_tx);

	softSerial->begin(9600);

	sds_data = softSerial;
}

void SDS011::begin(SoftwareSerial* serial) {
	serial->begin(9600);
	sds_data = serial;
}
#endif

#ifdef ESP32
void SDS011::begin(HardwareSerial* serial) {
	serial->begin(9600);
	sds_data = serial;
}

void SDS011::begin(HardwareSerial* serial, int8_t rxPin, int8_t txPin) {
	serial->begin(9600, SERIAL_8N1, rxPin, txPin);
	sds_data = serial;
}
#endif
