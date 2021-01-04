#include "SdsDustSensor.h"

void SdsDustSensor::write(const Command &command) {
  for (int i = 0; i < Command::length; ++i) {
    sdsStream->write(command.bytes[i]);
    #ifdef __DEBUG_SDS_DUST_SENSOR__
    Serial.print("|");
    Serial.print(command.bytes[i], HEX);
    #endif
  }
  #ifdef __DEBUG_SDS_DUST_SENSOR__
  Serial.println("| <- written bytes");
  #endif
  delay(500);
}

Status SdsDustSensor::readIntoBytes(byte responseId) {
  int checksum = 0;
  int readBytesQuantity = 0;

  while (sdsStream->available() >= Result::lenght - readBytesQuantity) {
    byte readByte = sdsStream->read();
    response[readBytesQuantity] = readByte;

    #ifdef __DEBUG_SDS_DUST_SENSOR__
    Serial.print("|");
    Serial.print(readByte, HEX);
    #endif

    ++readBytesQuantity;
    switch (readBytesQuantity) {
      case 1:
        if (readByte != 0xAA) {
          #ifdef __DEBUG_SDS_DUST_SENSOR__
          Serial.println("| <- read bytes with invalid head error");
          #endif
          return Status::InvalidHead;
        }
        break;
      case 2:
        if (readByte != responseId) {
          #ifdef __DEBUG_SDS_DUST_SENSOR__
          Serial.println("| <- read bytes with invalid response id");
          #endif
          return Status::InvalidResponseId;
        }
        break;
      case 3 ... 8:
        checksum += readByte;
        break;
      case 9:
        if (readByte != checksum % 256) {
          #ifdef __DEBUG_SDS_DUST_SENSOR__
          Serial.println("| <- read bytes with invalid checksum");
          #endif
          return Status::InvalidChecksum;
        }
        break;
      case 10:
        if (readByte != 0xAB) {
          #ifdef __DEBUG_SDS_DUST_SENSOR__
          Serial.println("| <- read bytes with invalid tail");
          #endif
          return Status::InvalidTail;
        }
        break;
    }
    if (readBytesQuantity == Result::lenght) {
      #ifdef __DEBUG_SDS_DUST_SENSOR__
      Serial.println("| <- read bytes with success");
      #endif
      return Status::Ok;
    }
    yield();
  }

  #ifdef __DEBUG_SDS_DUST_SENSOR__
  Serial.println("| <- read bytes with no more available");
  #endif

  return Status::NotAvailable;
}

void SdsDustSensor::flushStream() {
  while (sdsStream->available() > 0) {
    #ifdef __DEBUG_SDS_DUST_SENSOR__
    Serial.print("|");
    Serial.print(sdsStream->read(), HEX);
    Serial.println("| <- omitted byte");
    #else
    sdsStream->read();
    #endif
  }
}

Status SdsDustSensor::retryRead(byte responseId) {
  Status status = readIntoBytes(responseId);
  for (int i = 0; status == Status::NotAvailable && i < maxRetriesNotAvailable; ++i) {
    #ifdef __DEBUG_SDS_DUST_SENSOR__
    Serial.print("Retry #");
    Serial.print(i);
    Serial.println(" due to not available response");
    #endif
    delay(retryDelayMs);
    status = readIntoBytes(responseId);
  }

  for (int i = 2; status == Status::InvalidHead && i < Result::lenght; ++i) {
    #ifdef __DEBUG_SDS_DUST_SENSOR__
    Serial.print("Retry #");
    Serial.print(i - 2);
    Serial.println(" due to invalid response head");
    #endif
    status = readIntoBytes(responseId);
  }

  return status;
}
