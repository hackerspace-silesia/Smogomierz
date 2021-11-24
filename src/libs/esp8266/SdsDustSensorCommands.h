#ifndef __SDS_DUST_SENSOR_COMMAND_H__
#define __SDS_DUST_SENSOR_COMMAND_H__

#include <Arduino.h>

struct Command {
  static const int length = 19;
  static const int headIndex = 0;
  static const int commandIdIndex = 1;
  static const int dataStartIndex = 2, dataEndIndex = 14;
  static const int devideIdByte1Index = 15;
  static const int devideIdByte2Index = 16;
  static const int checksumIndex = 17;
  static const int tailIndex = 18;

  static const byte head = 0xAA;
  static const byte tail = 0xAB;

  byte *bytes = NULL;
  byte responseId;

  Command(byte commandId,
          const byte *commandData,
          int commandDataLength,
          byte responseId,
          byte deviceIdByte1 = 0xFF,
          byte deviceIdByte2 = 0xFF): responseId(responseId) {
    bytes = new byte[length];

    bytes[headIndex] = head;
    bytes[commandIdIndex] = commandId;
    for (int i = 0; i < commandDataLength; ++i) {
      bytes[dataStartIndex + i] = commandData[i]; // data bytes
    }
    for (int i = dataStartIndex + commandDataLength; i <= dataEndIndex; ++i) {
      bytes[i] = 0x00; // other data bytes
    }
    bytes[devideIdByte1Index] = deviceIdByte1;
    bytes[devideIdByte2Index] = deviceIdByte2;
    bytes[checksumIndex] = calculateChecksum();
    bytes[tailIndex] = tail;
  }

  ~Command() {
    if (bytes != NULL) {
      delete bytes;
    }
  }

  byte getChecksum() {
    return bytes[checksumIndex];
  }

  int calculateChecksum() {
    int sum = 0;
    for (int i = dataStartIndex; i <= dataEndIndex; ++i) {
      sum += bytes[i];
    }
    sum += bytes[devideIdByte1Index] + bytes[devideIdByte2Index];
    return (sum % 256);
  }

  void setDeviceId(byte byte1, byte byte2) {
    bytes[devideIdByte1Index] = byte1;
    bytes[devideIdByte2Index] = byte2;
    bytes[checksumIndex] = calculateChecksum();
  }
};

struct SetWorkingPeriodCommand: public Command {
  static const int workingPeriodIndex = 4;

  using Command::Command;

  void setCustomWorkingPeriod(byte period) {
    bytes[workingPeriodIndex] = period;
    bytes[checksumIndex] = calculateChecksum();
  }

  void setContinuousWorkingPeriod() {
    bytes[workingPeriodIndex] = 0x00;
    bytes[checksumIndex] = calculateChecksum();
  }
};

struct SetDeviceIdCommand: public Command {
  static const int newDeviceIdIndex1 = 13;
  static const int newDeviceIdIndex2 = 14;

  using Command::Command;

  void setNewDeviceId(byte byte1, byte byte2) {
    bytes[newDeviceIdIndex1] = byte1;
    bytes[newDeviceIdIndex2] = byte2;
    bytes[checksumIndex] = calculateChecksum();
  }
};

namespace Commands {
  // reporting mode
  static const byte queryReportingModeData[1] = {0x02};
  static const Command queryReportingMode(0xB4, queryReportingModeData, sizeof(queryReportingModeData), 0xC5);

  static const byte setActiveReportingModeData[2] = {0x02, 0x01};
  static const Command setActiveReportingMode(0xB4, setActiveReportingModeData, sizeof(setActiveReportingModeData), 0xC5);

  static const byte setQueryReportingModeData[3] = {0x02, 0x01, 0x01};
  static const Command setQueryReportingMode(0xB4, setQueryReportingModeData, sizeof(setQueryReportingModeData), 0xC5);

  // query data
  static const byte queryPmData[1] = {0x04};
  static const Command queryPm(0xB4, queryPmData, sizeof(queryPmData), 0xC0);

  // set device id
  static const byte setDeviceIdData[1] = {0x05};
  static SetDeviceIdCommand setDeviceId(0xB4, setDeviceIdData, sizeof(setDeviceIdData), 0xC5);

  // sleep and work
  static const byte queryWorkingStateData[1] = {0x06};
  static const Command queryWorkingState(0xB4, queryWorkingStateData, sizeof(queryWorkingStateData), 0xC5);

  static const byte sleepData[2] = {0x06, 0x01};
  static const Command sleep(0xB4, sleepData, sizeof(sleepData), 0xC5);

  static const byte wakeupData[3] = {0x06, 0x01, 0x01};
  static const Command wakeup(0xB4, wakeupData, sizeof(wakeupData), 0xC5);

  // working period
  static const byte queryWorkingPeriodData[1] = {0x08};
  static const Command queryWorkingPeriod(0xB4, queryWorkingPeriodData, sizeof(queryWorkingPeriodData), 0xC5);

  static const byte setWorkingPeriodData[2] = {0x08, 0x01};
  static SetWorkingPeriodCommand setWorkingPeriod(0xB4, setWorkingPeriodData, sizeof(setWorkingPeriodData), 0xC5);

  // firmware version
  static const byte queryFirmwareVersionData[1] = {0x07};
  static const Command queryFirmwareVersion(0xB4, queryFirmwareVersionData, sizeof(queryFirmwareVersionData), 0xC5);
};

#endif // __SDS_DUST_SENSOR_COMMAND_H__
