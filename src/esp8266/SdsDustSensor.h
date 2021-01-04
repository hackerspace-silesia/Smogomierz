// Nova Fitness Sds dust sensors library
// A high-level abstaction over Sds sensors family
//
// MIT License
//
// Copyright (c) 2018 Paweł Kołodziejczyk
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __SDS_DUST_SENSOR_H__
#define __SDS_DUST_SENSOR_H__

#include "SdsDustSensorCommands.h"
#include "SdsDustSensorResults.h"
#include "Serials.h"

#define RETRY_DELAY_MS_DEFAULT 5
#define MAX_RETRIES_NOT_AVAILABLE_DEFAULT 100

class SdsDustSensor {
public:

#if !defined(ARDUINO_SAMD_VARIANT_COMPLIANCE) && !defined(ESP32)
  SdsDustSensor(int pinRx,
                int pinTx,
                int retryDelayMs = RETRY_DELAY_MS_DEFAULT,
                int maxRetriesNotAvailable = MAX_RETRIES_NOT_AVAILABLE_DEFAULT):
    abstractSerial(new Serials::InternalSoftware(pinRx, pinTx)),
    retryDelayMs(retryDelayMs),
    maxRetriesNotAvailable(maxRetriesNotAvailable) {
      sdsStream = abstractSerial->getStream();
    }

  SdsDustSensor(SoftwareSerial &softwareSerial,
                int retryDelayMs = RETRY_DELAY_MS_DEFAULT,
                int maxRetriesNotAvailable = MAX_RETRIES_NOT_AVAILABLE_DEFAULT):
    abstractSerial(new Serials::Software(softwareSerial)),
    retryDelayMs(retryDelayMs),
    maxRetriesNotAvailable(maxRetriesNotAvailable) {
      sdsStream = abstractSerial->getStream();
    }
#endif

  SdsDustSensor(HardwareSerial &hardwareSerial,
                int retryDelayMs = RETRY_DELAY_MS_DEFAULT,
                int maxRetriesNotAvailable = MAX_RETRIES_NOT_AVAILABLE_DEFAULT):
    abstractSerial(new Serials::Hardware(hardwareSerial)),
    retryDelayMs(retryDelayMs),
    maxRetriesNotAvailable(maxRetriesNotAvailable) {
      sdsStream = abstractSerial->getStream();
    }

  ~SdsDustSensor() {
    if (abstractSerial != NULL) {
      delete abstractSerial;
    }
  }

  void begin(int baudRate = 9600) {
    abstractSerial->begin(baudRate);
  }

  byte *getLastResponse() {
    return response;
  }

  ReportingModeResult queryReportingMode() {
    Status status = execute(Commands::queryReportingMode);
    return ReportingModeResult(status, response);
  }

  ReportingModeResult setQueryReportingMode() {
    Status status = execute(Commands::setQueryReportingMode);
    return ReportingModeResult(status, response);
  }

  ReportingModeResult setActiveReportingMode() {
    Status status = execute(Commands::setActiveReportingMode);
    return ReportingModeResult(status, response);
  }

  PmResult queryPm() {
    Status status = execute(Commands::queryPm);
    return PmResult(status, response);
  }

  // warning: this method doesn't write anything to the sensor, it just reads incoming bytes
  PmResult readPm() {
    Status status = retryRead(Commands::queryPm.responseId);
    return PmResult(status, response);
  }

  Result setDeviceId(byte newDeviceIdByte1, byte newDeviceIdByte2) {
    (Commands::setDeviceId).setDeviceId(newDeviceIdByte1, newDeviceIdByte2);
    Status status = execute(Commands::setDeviceId);
    return Result(status, response);
  }

  WorkingStateResult queryWorkingState() {
    Status status = execute(Commands::queryWorkingState);
    return WorkingStateResult(status, response);
  }

  WorkingStateResult sleep() {
    Status status = execute(Commands::sleep);
    return WorkingStateResult(status, response);
  }

  // warning: according to 'Laser Dust Sensor Control Protocol V1.3' this method should work
  //          however sensor responds with random bytes or doesn't response at all
  //          despite the above issue it seems that sensor wakes up properly (fan starts working)
  WorkingStateResult wakeupUnsafe() {
    Status status = execute(Commands::wakeup);
    return WorkingStateResult(status, response);
  }

  // warning: double wakeup in order to assure proper sensor response
  //          if you don't care about sensor response you can use 'wakeupUnsafe'
  WorkingStateResult wakeup() {
    Status status = execute(Commands::wakeup);
    if (status != Status::Ok) {
      status = execute(Commands::wakeup);
    }
    return WorkingStateResult(status, response);
  }

  WorkingPeriodResult queryWorkingPeriod() {
    Status status = execute(Commands::queryWorkingPeriod);
    return WorkingPeriodResult(status, response);
  }

  WorkingPeriodResult setContinuousWorkingPeriod() {
    (Commands::setWorkingPeriod).setContinuousWorkingPeriod();
    Status status = execute(Commands::setWorkingPeriod);
    return WorkingPeriodResult(status, response);
  }

  WorkingPeriodResult setCustomWorkingPeriod(byte workingPeriod) {
    (Commands::setWorkingPeriod).setCustomWorkingPeriod(workingPeriod);
    Status status = execute(Commands::setWorkingPeriod);
    return WorkingPeriodResult(status, response);
  }

  FirmwareVersionResult queryFirmwareVersion() {
    Status status = execute(Commands::queryFirmwareVersion);
    return FirmwareVersionResult(status, response);
  }

  Status execute(const Command &command) {
    flushStream();
    write(command);
    return retryRead(command.responseId);
  }

  void write(const Command &command);
  Status readIntoBytes(byte responseId);

private:
  Serials::AbstractSerial *abstractSerial;
  Stream *sdsStream = NULL;
  byte response[Result::lenght];
  int retryDelayMs;
  int maxRetriesNotAvailable;

  void flushStream();
  Status retryRead(byte responseId);
};

#endif // __SDS_DUST_SENSOR_H__
