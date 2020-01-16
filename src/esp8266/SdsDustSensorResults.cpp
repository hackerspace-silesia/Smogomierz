#include "SdsDustSensorResults.h"

#ifdef ARDUINO_ARCH_ESP8266
String Result::statusToString() {
  switch (status) {
    case Status::Ok: return "Ok";
    case Status::NotAvailable: return "Not available";
    case Status::InvalidChecksum: return "Invalid checksum";
    case Status::InvalidResponseId: return "Invalid response id";
    case Status::InvalidHead: return "Invalid head";
    case Status::InvalidTail: return "Invalid tail";
    default: return "undefined status";
  }
}
#elif defined ARDUINO_ARCH_ESP32

#endif

