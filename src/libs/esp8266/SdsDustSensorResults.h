#ifndef __SDS_DUST_SENSOR_RESULTS_H__
#define __SDS_DUST_SENSOR_RESULTS_H__

#include "SdsDustSensorCommands.h"
#if !defined(ARDUINO_SAMD_VARIANT_COMPLIANCE) && !defined(ESP32)
#include <SoftwareSerial.h>
#endif

enum class Status {
  Ok, NotAvailable, InvalidChecksum, InvalidResponseId, InvalidHead, InvalidTail
};

struct Result {
  static const int lenght = 10;
  static const int deviceIdStartIndex = 6;
  Status status;
  byte rawBytes[lenght];

  Result(const Status &status, byte *bytes): status(status) {
    if (isOk()) {
      for (int i = 0; i < lenght; ++i) {
        rawBytes[i] = bytes[i];
      }
    } else {
      for (int i = 0; i < lenght; ++i) {
        rawBytes[i] |= 0xFF;
      }
    }
  };

  bool isOk() {
    return status == Status::Ok;
  }

  byte *deviceId() {
    // warning: device id has 2 bytes, to access 2nd byte: deviceId()[1]
    return rawBytes + deviceIdStartIndex;
  }

  String statusToString();
};

struct ReportingModeResult: public Result {
  enum class Mode { Query, Active, Undefined };

  static const int modeIndex = 4;
  Mode mode;

  ReportingModeResult(const Status &status, byte *bytes): Result(status, bytes) {
    switch (rawBytes[modeIndex]) {
      case 0: mode = Mode::Active; break;
      case 1: mode = Mode::Query; break;
      default: mode = Mode::Undefined; break;
    }
  }

  bool isActive() {
    return mode == Mode::Active;
  }

  String toString() {
    switch (mode) {
      case Mode::Query: return "Mode: query";
      case Mode::Active: return "Mode: active";
      default: return "Mode: undefined";
    }
  }
};

struct PmResult: public Result {
  float pm25 = -1.0;
  float pm10 = -1.0;

  PmResult(const Status &status, byte *bytes): Result(status, bytes) {
    if (isOk()) {
      pm25 = (rawBytes[2] | (rawBytes[3] << 8)) / 10.0;
      pm10 = (rawBytes[4] | (rawBytes[5] << 8)) / 10.0;
    }
  }

  String toString() {
    return "pm25: " + String(pm25) + ", pm10: " + String(pm10);
  }
};

struct WorkingStateResult: public Result {
  enum class State { Sleeping, Working, Undefined };

  static const int stateIndex = 4;
  State state;

  WorkingStateResult(const Status &status, byte *bytes): Result(status, bytes) {
    switch (rawBytes[stateIndex]) {
      case 0: state = State::Sleeping; break;
      case 1: state = State::Working; break;
      default: state = State::Undefined; break;
    }
  }

  bool isWorking() {
    return state == State::Working;
  }

  String toString() {
    switch (state) {
      case State::Sleeping: return "State: sleeping";
      case State::Working: return "State: working";
      default: return "State: undefined";
    }
  }
};

struct WorkingPeriodResult: public Result {
  static const int periodIndex = 4;
  byte period;

  WorkingPeriodResult(const Status &status, byte *bytes): Result(status, bytes) {
    period = rawBytes[periodIndex];
  }

  bool isContinuous() {
    return period == 0;
  }

  String toString() {
    switch (period) {
      case 0: return "Working period: continuous";
      case 1 ... 30: return "Working period: " + String(period) + " min cycles: work 30 seconds, measure and sleep";
      default: return "Working period: undefined";
    }
  }
};

struct FirmwareVersionResult: public Result {
  static const int periodIndex = 4;
  int year = -1, month = -1, day = -1;

  FirmwareVersionResult(const Status &status, byte *bytes): Result(status, bytes) {
    if (isOk()) {
      year = rawBytes[3];
      month = rawBytes[4];
      day = rawBytes[5];
    }
  }

  String toString() {
    return "Firmware version [year.month.day]: " + String(year) + "." + String(month) + "." + String(day);
  }
};


#endif // __SDS_DUST_SENSOR_RESULTS_H__
