/***************************************************************************************************/
/*
  This is an Arduino library for SHT21, HTU21D & Si70xx Digital Humidity and Temperature Sensor

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/


  This chip uses I2C bus to communicate, specials pins are required to interface
  Board:                                    SDA                    SCL                    Level
  Uno, Mini, Pro, ATmega168, ATmega328..... A4                     A5                     5v
  Mega2560................................. 20                     21                     5v
  Due, SAM3X8E............................. 20                     21                     3.3v
  Leonardo, Micro, ATmega32U4.............. 2                      3                      5v
  Digistump, Trinket, ATtiny85............. 0/physical pin no.5    2/physical pin no.7    5v
  Blue Pill, STM32F103xxxx boards.......... PB7                    PB6                    3.3v/5v
  ESP8266 ESP-01........................... GPIO0/D5               GPIO2/D3               3.3v/5v
  NodeMCU 1.0, WeMos D1 Mini............... GPIO4/D2               GPIO5/D1               3.3v/5v
  ESP32.................................... GPIO21/D21             GPIO22/D22             3.3v

   Frameworks & Libraries:
   ATtiny  Core          - https://github.com/SpenceKonde/ATTinyCore
   ESP32   Core          - https://github.com/espressif/arduino-esp32
   ESP8266 Core          - https://github.com/esp8266/Arduino
   STM32   Core          - https://github.com/rogerclarkmelbourne/Arduino_STM32

   GNU GPL license, all text above must be included in any redistribution,
   see link for details  - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/

#include "HTU21D.h"


/**************************************************************************/
/*
    Constructor
*/
/**************************************************************************/
HTU21D::HTU21D(HTU21D_RESOLUTION sensorResolution)
{
  _resolution = sensorResolution;
}

/**************************************************************************/
/*
    begin()

    Initializes I2C and configures the sensor, call this function before
    doing anything else

    NOTE:
    - Wire.endTransmission() returned value:
      - 0 success
      - 1 data too long to fit in transmit data16
      - 2 received NACK on transmit of address
      - 3 received NACK on transmit of data
      - 4 other error
*/
/**************************************************************************/
#if defined(ESP8266)
bool HTU21D::begin(uint8_t sda, uint8_t scl)
{
  Wire.begin(sda, scl);
  Wire.setClock(100000UL);                           //experimental! ESP8266 i2c bus speed: 100kHz..400kHz/100000UL..400000UL, default 100000UL
  Wire.setClockStretchLimit(230);                    //experimental! default 230usec
#else
bool HTU21D::begin(void) 
{
  Wire.begin();
  Wire.setClock(100000UL);                           //experimental! AVR i2c bus speed: 31kHz..400kHz/31000UL..400000UL, default 100000UL
#endif

  Wire.beginTransmission(HTU21D_ADDRESS);
  if (Wire.endTransmission(true) != 0) return false; //safety check, make sure the sensor is connected

  setResolution(_resolution);
  setHeater(HTU21D_OFF);

  return true;
}

/**************************************************************************/
/*
    setResolution()

    Sets sensor's resolution
*/
/**************************************************************************/
void HTU21D::setResolution(HTU21D_RESOLUTION sensorResolution)
{
  uint8_t userRegisterData = 0;

  userRegisterData  = read8(HTU21D_USER_REGISTER_READ); //reads current user register state
  userRegisterData &= 0x7E;                             //clears current resolution bits with 0
  userRegisterData |= sensorResolution;                 //adds new resolution bits to user register byte

  write8(HTU21D_USER_REGISTER_WRITE, userRegisterData); //writes updeted byte to the user register

  _resolution = sensorResolution;                       //updates private variable
}

/**************************************************************************/
/*
    softReset()  
 
    Soft reset, switch sensor OFF & ON

    NOTE:
    - takes ~15ms
    - all registers & bits except heater bit will set to default
*/
/**************************************************************************/
void HTU21D::softReset(void)
{
  Wire.beginTransmission(HTU21D_ADDRESS);

  #if ARDUINO >= 100
  Wire.write(HTU21D_SOFT_RESET);
  #else
  Wire.send(HTU21D_SOFT_RESET);
  #endif

  Wire.endTransmission(true);

  delay(HTU21D_SOFT_RESET_DELAY);
}

/**************************************************************************/
/*
    batteryStatus()

    Checks the battery status.

    NOTE:
    - for SHT21, HTU21D:
      - if VDD > 2.25v ±0.1v return TRUE
      - if VDD < 2.25v ±0.1v return FALSE

    - for Si70xx:
      - if VDD > 1.9v ±0.1v return TRUE
      - if VDD < 1.9v ±0.1v return FALSE
*/
/**************************************************************************/
bool HTU21D::batteryStatus(void)
{
  uint8_t userRegisterData = 0;
  
  userRegisterData  = read8(HTU21D_USER_REGISTER_READ);
  userRegisterData &= 0x40;

  if (userRegisterData == 0x00) return true;
                                return false;
}

/**************************************************************************/
/*
    setHeater()

    Turn ON/OFF build-in heater

    NOTE:
    - prolonged exposure to high humidity will result gradual upward drift
      of the RH reading, the heater is used to drive off condensation &
      reverse drift effect.
    - heater consumtion is 3.09mA - 94.20mA @ 3.3v.
*/
/**************************************************************************/
void HTU21D::setHeater(HTU21D_HEATER_SWITCH heaterSwitch)
{
  uint8_t userRegisterData = 0;

  userRegisterData = read8(HTU21D_USER_REGISTER_READ);

  switch(heaterSwitch)
  {
    case HTU21D_ON:
      userRegisterData |= heaterSwitch;
      break;

    case HTU21D_OFF:
      userRegisterData &= heaterSwitch;
      break;
  }

  write8(HTU21D_USER_REGISTER_WRITE, userRegisterData);
}

/**************************************************************************/
/*
    readHumidity()

    Reads Humidity, %

    NOTE:
    - maximum accuracy ±2%RH in range 20%..80% at 25°C
    - maximum measurement time ~29ms
    - suggested minimum time between measurements 17sec
    - "operationMode" could be set up as:

      - "HTU21D_TRIGGER_TEMP_MEASURE_NOHOLD" mode, allows communication with
        another  slave devices on I2C bus while sensor is measuring.
        WARNING!!! Could create collision if more than one slave devices are
        connected to the same bus.

      - "HTU21D_TRIGGER_HUMD_MEASURE_HOLD" mode, sensor blocks communication
        on I2C bus by keeping SCL line LOW during measurement.
*/
/**************************************************************************/
float HTU21D::readHumidity(HTU21D_HUMD_OPERATION_MODE sensorOperationMode)
{
  uint16_t rawHumidity = 0;
  uint8_t  checksum    = 0;
  float    humidity    = 0;

  /* request humidity measurement */
  Wire.beginTransmission(HTU21D_ADDRESS);
  #if ARDUINO >= 100
  Wire.write(sensorOperationMode);
  #else
  Wire.send(sensorOperationMode);
  #endif
  if (Wire.endTransmission(true) != 0) return HTU21D_ERROR;    //error handler, collision on the i2c bus

  /* humidity measurement delay */
  switch(_resolution)
  {
    case HTU21D_RES_RH12_TEMP14:
      delay(29);                                               //HTU21D - 14..16msec, Si7021 - 10..12msec, SHT21 - 22..29msec
      break;

    case HTU21D_RES_RH11_TEMP11:
      delay(15);                                               //HTU21D - 7..8msec,   Si7021 - 6..7msec,   SHT21 - 12..15msec
      break;

    case HTU21D_RES_RH10_TEMP13:
      delay(9);                                                //HTU21D - 4..5msec,   Si7021 - 4..5msec,   SHT21 - 7..9msec
      break;

    case HTU21D_RES_RH8_TEMP12:
      delay(4);                                                //HTU21D - 2..3msec,   Si7021 - 3..4msec,   SHT21 - 3..4msec
      break;
  }

  /* read humidity measurement to "wire.h" rxBuffer */
  #if defined(_VARIANT_ARDUINO_STM32_)
  Wire.requestFrom(HTU21D_ADDRESS, 3);
  #else
  Wire.requestFrom(HTU21D_ADDRESS, 3, true);                   //true, stop message after transmission & releas the I2C bus
  #endif
  if (Wire.available() != 3) return HTU21D_ERROR;              //check rxBuffer & error handler, collision on the i2c bus

  /* reads MSB, LSB byte & checksum from "wire.h" rxBuffer */
  #if ARDUINO >= 100
  rawHumidity  = Wire.read() << 8;                             //reads MSB byte & shift it to the right
  rawHumidity |= Wire.read();                                  //reads LSB byte & sum with MSB byte
  checksum     = Wire.read();                                  //reads checksum
  #else
  rawHumidity  = Wire.receive() << 8;
  rawHumidity |= Wire.receive();
  checksum     = Wire.receive();
  #endif

  if (checkCRC8(rawHumidity) != checksum) return HTU21D_ERROR; //error handler, checksum verification

  rawHumidity ^= 0x02;                                         //clear status bits, humidity always returns xxxxxx10 in the LSB field
  humidity     = (0.001907 * (float)rawHumidity - 6);
  
  if      (humidity < 0)   humidity = 0;                       //due to RH accuracy, measured value might be slightly less than 0 or more 100
  else if (humidity > 100) humidity = 100;

  return humidity;
}

/**************************************************************************/
/*
    readTemperature()

    Reads Temperature, C

    NOTE:
    - maximum accuracy ±0.3°C in range 0C..60°C
    - maximum measurement time ~85ms
    - suggested minimum time between measurements 17sec
    - "operationMode" could be set up as:

      - "HTU21D_TRIGGER_TEMP_MEASURE_NOHOLD" mode, allows communication with
        another slave devices on I2C bus while sensor is measuring.
        WARNING!!! Could create collision if more than one slave devices are
        connected to the same bus.

      - "HTU21D_TRIGGER_HUMD_MEASURE_HOLD" mode, sensor blocks communication
        on I2C bus by keeping SCL line LOW during measurement.

      - "SI7021_TEMP_READ_AFTER_RH_MEASURMENT" mode, allows to retrive
        temperature measurement, which was made at previouse RH measurement.
        For HTU21D & SHT21 you have to manualy call
        "readCompensatedHumidity()"
*/
/**************************************************************************/
float HTU21D::readTemperature(HTU21D_TEMP_OPERATION_MODE sensorOperationMode)
{
  int8_t   qntRequest     = 3;                                                                //3 bytes -> MSB, LSB byte & checksum or 2 bytes -> MSB, LSB byte
  uint16_t rawTemperature = 0;
  uint8_t  checksum       = 0;

  /* request temperature measurement */
  Wire.beginTransmission(HTU21D_ADDRESS);
  #if ARDUINO >= 100
  Wire.write(sensorOperationMode); 
  #else
  Wire.send(sensorOperationMode);
  #endif
  if (Wire.endTransmission(true) != 0) return HTU21D_ERROR;                                   //error handler, collision on the i2c bus

  /* temperature measurement delay */
  if (sensorOperationMode != SI70xx_TEMP_READ_AFTER_RH_MEASURMENT)
  {
    switch(_resolution)
    {
      case HTU21D_RES_RH12_TEMP14:
        delay(85);                                                                            //HTU21D - 44..50msec, Si7021 - 7..11msec, SHT21 - 66..85msec
        break;

      case HTU21D_RES_RH10_TEMP13:
        delay(43);                                                                            //HTU21D - 22..25msec, Si7021 - 4..7msec,  SHT21 - 33..43msec
        break;

      case HTU21D_RES_RH8_TEMP12:
        delay(22);                                                                            //HTU21D - 11..13msec, Si7021 - 3..4msec,  SHT21 - 17..22msec
        break;

      case HTU21D_RES_RH11_TEMP11:
        delay(11);                                                                            //HTU21D - 6..7msec,   Si7021 - 2..3msec,  SHT21 - 9..11msec
        break;
    }
  }
  else qntRequest = 2;                                                                        //checksum is not available with "SI70xx_TEMP_READ_AFTER_RH_MEASURMENT"

  /* read temperature measurement to "wire.h" rxBuffer */
  #if defined(_VARIANT_ARDUINO_STM32_)
  Wire.requestFrom(HTU21D_ADDRESS, qntRequest);
  #else
  Wire.requestFrom(HTU21D_ADDRESS, qntRequest, true);                                         //true, stop message after transmission & releas the I2C bus
  #endif
  if (Wire.available() != qntRequest) return HTU21D_ERROR;                                    //check rxBuffer & error handler, collision on the i2c bus

  /* reads MSB, LSB byte & checksum from "wire.h" rxBuffer */
  #if ARDUINO >= 100
  rawTemperature  = Wire.read() << 8;                                                         //reads MSB byte & shift it to the right
  rawTemperature |= Wire.read();                                                              //reads LSB byte and sum. with MSB byte
  if (sensorOperationMode != SI70xx_TEMP_READ_AFTER_RH_MEASURMENT) checksum = Wire.read();    //checksum is not available with "SI70xx_TEMP_READ_AFTER_RH_MEASURMENT"
  #else
  rawTemperature  = Wire.receive() << 8;
  rawTemperature |= Wire.receive();
  if (sensorOperationMode != SI70xx_TEMP_READ_AFTER_RH_MEASURMENT) checksum = Wire.receive();
  #endif

  /* checksum is not available with "SI70xx_TEMP_READ_AFTER_RH_MEASURMENT" */
  if (sensorOperationMode != SI70xx_TEMP_READ_AFTER_RH_MEASURMENT && checkCRC8(rawTemperature) != checksum) return HTU21D_ERROR; //error handler, checksum verification

  return (0.002681 * (float)rawTemperature - 46.85);                                          //temperature always returns xxxxxx00 in the LSB field
}

/**************************************************************************/
/*
    readCompensatedHumidity()

    Only for HTU21D & SHT21. Calculates temperature compensated Humidity, %RH

    NOTE:
    - Si7021 automatically compensates temperature influence on RH every
      humidity measurement
    - maximum accuracy ±2%RH in range 0%..100% at 0°C..80°C
    - maximum measurement time ~114ms
    - suggested minimun time between measurements 17sec
*/
/**************************************************************************/
float HTU21D::readCompensatedHumidity(float temperature)
{
  float humidity = 0;

  humidity = readHumidity();

  if (temperature == HTU21D_FORCE_READ_TEMP) temperature = readTemperature();                                    //force to read temperature

  if (humidity == HTU21D_ERROR || temperature == HTU21D_ERROR) return HTU21D_ERROR;                              //error handler, collision on the i2c bus
  
  if (temperature > 0 && temperature < 80) humidity = humidity + (25.0 - temperature) * HTU21D_TEMP_COEFFICIENT; //apply compensation coefficient

  return humidity;
}

/***************************************************************************/
/*
    readDeviceID()

    Reads device id

    NOTE:
    - see p.23 of Si7021 datasheet for details
    - full serial number is {SNA3, SNA2, SNA1, SNA0, SNB3**, SNB2, SNB1, SNB0}
    - **chip ID:
        - 0x0D: Si7013
        - 0x14: Si7020
        - 0x15: Si7021 
        - 0x32: HTU21D & SHT21
*/
/**************************************************************************/
uint16_t HTU21D::readDeviceID(void)
{
  uint16_t deviceID = 0;
  uint8_t  checksum = 0;

  /* request serial_2 -> SNB3**, SNB2, SNB1, SNB0 */
  Wire.beginTransmission(HTU21D_ADDRESS);

  #if ARDUINO >= 100
  Wire.write(HTU21D_SERIAL2_READ1);
  Wire.write(HTU21D_SERIAL2_READ2);
  #else
  Wire.send(HTU21D_SERIAL2_READ1);
  Wire.send(HTU21D_SERIAL2_READ2);
  #endif
  Wire.endTransmission(true);

  /* read serial_2 -> SNB3**, SNB2, CRC */
  #if defined(_VARIANT_ARDUINO_STM32_)
  Wire.requestFrom(HTU21D_ADDRESS, 3);
  #else
  Wire.requestFrom(HTU21D_ADDRESS, 3, true);                //true, stop message after transmission & releas the I2C bus
  #endif

  #if ARDUINO >= 100
  deviceID  = Wire.read() << 8;
  deviceID |= Wire.read();
  checksum  = Wire.read();
  #else
  deviceID  = Wire.receive() << 8;
  deviceID |= Wire.receive();
  checksum  = Wire.receive();
  #endif

  if (checkCRC8(deviceID) != checksum) return HTU21D_ERROR; //error handler, checksum verification

  deviceID = deviceID >> 8;

  switch(deviceID)
  {
    case HTU21D_CHIPID:
      deviceID = 21;
      break;

    case SI7013_CHIPID:
      deviceID = 7013;
      break;

    case SI7020_CHIPID:
      deviceID = 7020;
      break;

    case SI7021_CHIPID:
      deviceID = 7021;
      break;

    default:
      deviceID = HTU21D_ERROR;
      break;
  }
  return deviceID;
}

/***************************************************************************/
/*
    readFirmwareVersion()

    Reads firware version

    NOTE:
    - see p.24 of Si7021 datasheet for details
*/
/**************************************************************************/
uint8_t HTU21D::readFirmwareVersion(void)
{
  uint8_t firmwareVersion = 0;

  /* request firware version */
  Wire.beginTransmission(HTU21D_ADDRESS);

  #if ARDUINO >= 100
  Wire.write(HTU21D_FIRMWARE_READ1);
  Wire.write(HTU21D_FIRMWARE_READ2);
  #else
  Wire.send(HTU21D_FIRMWARE_READ1);
  Wire.send(HTU21D_FIRMWARE_READ2);
  #endif
  Wire.endTransmission(true);

  /* read firware version */
  #if defined(_VARIANT_ARDUINO_STM32_)
  Wire.requestFrom(HTU21D_ADDRESS, 1);
  #else
  Wire.requestFrom(HTU21D_ADDRESS, 1, true); //true, stop message after transmission & releas the I2C bus
  #endif

  #if ARDUINO >= 100
  firmwareVersion = Wire.read();
  #else
  firmwareVersion = Wire.read();
  #endif

  switch(firmwareVersion)
  {
    case HTU21D_FIRMWARE_V1:
      firmwareVersion = 1;
      break;

    case HTU21D_FIRMWARE_V2:
      firmwareVersion = 2;
      break;

    default:
      firmwareVersion = HTU21D_ERROR;
      break;
  }
  return firmwareVersion;
}

/**************************************************************************/
/*
    write8()

    Writes 8-bit to the sensor register over I2C
*/
/**************************************************************************/
void HTU21D::write8(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(HTU21D_ADDRESS);
  #if ARDUINO >= 100
  Wire.write(reg);
  Wire.write(value);
  #else
  Wire.send(reg);
  Wire.send(value);
  #endif
  Wire.endTransmission(true);

}

/**************************************************************************/
/*
    read8()

    Reads 8-bit value from the sensor over I2C
*/
/**************************************************************************/
uint8_t HTU21D::read8(uint8_t reg)
{
  Wire.beginTransmission(HTU21D_ADDRESS);
  #if ARDUINO >= 100
  Wire.write(reg);
  #else
  Wire.send(reg);
  #endif
  if (Wire.endTransmission(true) != 0) return HTU21D_ERROR; //error handler, collision on the i2c bus;

  #if defined(_VARIANT_ARDUINO_STM32_)
  Wire.requestFrom(HTU21D_ADDRESS, 1);
  #else
  Wire.requestFrom(HTU21D_ADDRESS, 1, true);                //true, stop message after transmission & releas the I2C bus
  #endif
  if (Wire.available() != 1) return HTU21D_ERROR;           //check rxBuffer & error handler, collision on the i2c bus

  /* read byte from "wire.h" rxBuffer */
  #if ARDUINO >= 100
  return Wire.read();
  #else
  return Wire.receive();
  #endif
}

/**************************************************************************/
/*
    checkCRC8()

    Calculates CRC8 for 16-bit received data

    NOTE:
    - for more info about Cyclic Redundancy Check (CRC) see
      http://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks
*/
/**************************************************************************/
uint8_t HTU21D::checkCRC8(uint16_t data)
{
  for (uint8_t bit = 0; bit < 16; bit++)
  {
    if   (data & 0x8000) data = (data << 1) ^ HTU21D_CRC8_POLYNOMINAL;
    else data <<= 1;
  }
  return data >>= 8;
}
