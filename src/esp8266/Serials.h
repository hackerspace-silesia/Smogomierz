#ifndef __SDS_ABSTRACT_SERIAL_H__
#define __SDS_ABSTRACT_SERIAL_H__

#ifndef ARDUINO_SAMD_VARIANT_COMPLIANCE // there is no SoftwareSerial available (needed) on SAMD boards.
#include <SoftwareSerial.h>
#endif
#include <HardwareSerial.h>

namespace Serials {

  // cpp is really weird...
  // there was compilation warning about missing virtual destructor for class AbstractSerial
  // I added virtual destructor with empty body
  // remaining virtual methods didn't have body (they were 'pure virtual')
  // after that the following error appeared: undefined reference to `vtable for Serials::AbstractSerial'
  // what a nonsense ...
  // just to satisfy linker in gcc I needed to add empty parentheses to other virtual methods...
  class AbstractSerial {
  public:
    virtual void begin(int baudRate) = 0;
    virtual Stream *getStream() = 0;
    virtual ~AbstractSerial() {};
  };

  struct Hardware: public AbstractSerial {
    Hardware(HardwareSerial &serial): serial(serial) {}

    void begin(int baudRate) {
      serial.begin(baudRate);
    }

    Stream *getStream() {
      return &serial;
    }

    HardwareSerial &serial;
  };

#ifndef ARDUINO_SAMD_VARIANT_COMPLIANCE // there is no SoftwareSerial available (needed) on SAMD boards.
  struct Software: public AbstractSerial {
    Software(SoftwareSerial &serial): serial(serial) {}

    void begin(int baudRate) {
      serial.begin(baudRate);
    }

    Stream *getStream() {
      return &serial;
    }

    SoftwareSerial &serial;
  };

  struct InternalSoftware: public AbstractSerial {
    InternalSoftware(const int &pinRx, const int &pinTx):
      serial(new SoftwareSerial(pinRx, pinTx)) {}

    ~InternalSoftware() {
      if (serial != NULL) {
        delete serial;
      }
    }

    void begin(int baudRate) {
      serial->begin(baudRate);
    }

    Stream *getStream() {
      return serial;
    }

    SoftwareSerial *serial;
  };
#endif // ARDUINO_SAMD_VARIANT_COMPLIANCE

}

#endif // __SDS_ABSTRACT_SERIAL_H__
