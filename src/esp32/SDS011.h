// SDS011 dust sensor PM2.5 and PM10
// ---------------------------------
//
// By R. Zschiegner (rz@madavi.de)
// April 2016
//
// Documentation:
//		- The iNovaFitness SDS011 datasheet
//

#if ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef ARDUINO_ARCH_ESP8266
#include <SoftwareSerial.h>
#elif defined ARDUINO_ARCH_ESP32
#include <HardwareSerial.h>
#endif

class SDS011 {
	public:
		SDS011(void);
#ifdef ARDUINO_ARCH_ESP8266
		void begin(SoftwareSerial* serial);
		void begin(uint8_t pin_rx, uint8_t pin_tx);
#elif defined ARDUINO_ARCH_ESP32
		void begin(HardwareSerial* serial);
		void begin(HardwareSerial* serial, int8_t pin_rx, int8_t pin_tx);
#endif
		int read(float *p25, float *p10);
		void sleep();
		void wakeup();
	private:
		uint8_t _pin_rx, _pin_tx;
		Stream *sds_data;
};