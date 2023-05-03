/***************************************************************************************************/
/*
   This is an Arduino library for TE Connectivity HTU20D/HTU21D, Sensirion SHT20/SHT21/SHT25, 
   Silicone Labs Si7006/Si7013/Si7020/Si7021 Digital Humidity & Temperature Sensor

   written by : enjoyneering
   sourse code: https://github.com/enjoyneering/

   Sensors features:
   - HTU2xD +1.5v..+3.6v, SHT2x +2.1v..+3.6v, Si70xx +1.9v..+3.6v
   - HTU2xD 0.14uA..0.500uA, SHT2x 0.04uA..0.330uA, Si70xx 0.6uA..180uA
   - integrated resistive heater HTU2xD/SHT2x 1.83mA, Si70xx 3.1mA..94.2mA
   - temperature range HTU2xD/SHT2x -40C..+125C, Si702x (G-grade) -40C..+80C
   - humidity range 0%..100%
   - typical accuracy T +-0.3C, RH +-2%
   - typical resolution T 0.01C at 14-bits, RH 0.04% at 12-bits
   - maximum T measurement time Si70xx 11msec, HTU2xD 50ms, HTU2xD 85ms
   - maximum RH measurement time HTU2xD 16ms, Si70xx 23msec, HTU2xD 25ms
   - I2C bus speed 100KHz..400KHz
   - response time 8..30sec*
   - recommended to route VDD or GND between I2C lines to reduce crosstalk between SCL & SDA
   - recomended 100nF decoupling capacitor between VDD & GND
     *measurement with high frequency leads to heating of the sensor,
      measurements must be >= 0.5 seconds apart to keep self-heating below 0.1C

   This device uses I2C bus to communicate, specials pins are required to interface
   Board                                     SDA              SCL              Level
   Uno, Mini, Pro, ATmega168, ATmega328..... A4               A5               5v
   Mega2560................................. 20               21               5v
   Due, SAM3X8E............................. 20               21               3.3v
   Leonardo, Micro, ATmega32U4.............. 2                3                5v
   Digistump, Trinket, Gemma, ATtiny85...... PB0/D0           PB2/D2           3.3v/5v
   Blue Pill*, STM32F103xxxx boards*........ PB7/PB9          PB6/PB8          3.3v/5v
   ESP8266 ESP-01**......................... GPIO0            GPIO2            3.3v/5v
   NodeMCU 1.0**, WeMos D1 Mini**........... GPIO4/D2         GPIO5/D1         3.3v/5v
   ESP32***................................. GPIO21/D21       GPIO22/D22       3.3v
                                             GPIO16/D16       GPIO17/D17       3.3v
                                            *hardware I2C Wire mapped to Wire1 in stm32duino
                                             see https://github.com/stm32duino/wiki/wiki/API#I2C
                                           **most boards has 10K..12K pullup-up resistor
                                             on GPIO0/D3, GPIO2/D4/LED & pullup-down on
                                             GPIO15/D8 for flash & boot
                                          ***hardware I2C Wire mapped to TwoWire(0) aka GPIO21/GPIO22 in Arduino ESP32

   Supported frameworks:
   Arduino Core - https://github.com/arduino/Arduino/tree/master/hardware
   ATtiny  Core - https://github.com/SpenceKonde/ATTinyCore
   ESP8266 Core - https://github.com/esp8266/Arduino
   ESP32   Core - https://github.com/espressif/arduino-esp32
   STM32   Core - https://github.com/stm32duino/Arduino_Core_STM32


   GNU GPL license, all text above must be included in any redistribution,
   see link for details - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/

#include "HTU2xD_SHT2x_Si70xx.h"


/**************************************************************************/
/*
    Constructor
*/
/**************************************************************************/
HTU2xD_SHT2x_SI70xx::HTU2xD_SHT2x_SI70xx(HTU2XD_SHT2X_SI70XX_I2C_SENSOR sensorType, HTU2XD_SHT2X_SI70XX_USER_CTRL_RES sensorResolution)
{
  _resolution = sensorResolution;

  setType(sensorType);            //set sensor type & sensor I2C address 
}

/**************************************************************************/
/*
    begin()

    Initialize I2C & sensor

    NOTE:
    - call this function before doing anything else!!!
    - speed in Hz, stretch in usec
    - sensor needs 15ms to reach idle state after power-up
    - current consumption during start up maximum 350uA
    - whenever the sensor is powered on but does not measure/communicate,
      it will automatically enter standby/sleep state

    - returned value by "Wire.endTransmission()":
      - 0 success
      - 1 data too long to fit in transmit data buffer
      - 2 received NACK on transmit of address
      - 3 received NACK on transmit of data
      - 4 other error
*/
/**************************************************************************/
#if defined (__AVR__)
bool HTU2xD_SHT2x_SI70xx::begin(uint32_t speed, uint32_t stretch)
{
  Wire.begin();

  Wire.setClock(speed);                                    //experimental! AVR I2C bus speed 31kHz..400kHz, default 100000Hz

  #if !defined (__AVR_ATtiny85__)                          //for backwards compatibility with ATtiny Core
  Wire.setWireTimeout(stretch, false);                     //experimental! default 25000usec, true=Wire hardware will be automatically reset to default on timeout
  #endif

#elif defined (ESP8266)
bool HTU2xD_SHT2x_SI70xx::begin(uint8_t sda, uint8_t scl, uint32_t speed, uint32_t stretch)
{
  Wire.begin(sda, scl);

  Wire.setClock(speed);                                    //experimental! ESP8266 I2C bus speed 1kHz..400kHz, default 100000Hz

  Wire.setClockStretchLimit(stretch);                      //experimental! default 150000usec

#elif defined (ESP32)
bool HTU2xD_SHT2x_SI70xx::begin(int32_t sda, int32_t scl, uint32_t speed, uint32_t stretch)//"int32_t" for Master SDA & SCL, "uint8_t" for Slave SDA & SCL
{
  if (Wire.begin(sda, scl, speed) != true) {return false;} //experimental! ESP32 I2C bus speed ???kHz..400kHz, default 100000Hz

  Wire.setTimeout(stretch / 1000);                         //experimental! default 50msec

#elif defined (ARDUINO_ARCH_STM32)
bool HTU2xD_SHT2x_SI70xx::begin(uint32_t sda, uint32_t scl, uint32_t speed) //"uint32_t" for pins only, "uint8_t" calls wrong "setSCL(PinName scl)"
{
  Wire.begin(sda, scl);

  Wire.setClock(speed);                                    //experimental! STM32 I2C bus speed ???kHz..400kHz, default 100000Hz

#else
bool HTU2xD_SHT2x_SI70xx::begin()
{
  Wire.begin();
#endif

  switch (_sensorType)
  {
    case HTU2xD_SENSOR:
    case SHT2x_SENSOR:
      delay(HTU2XD_SHT2X_POWER_ON_DELAY); //wait for HTU2xD/SHT2x to initialize after power-on, current consumption 350uA
      break;

    case SI700x_SENSOR:
    case SI701x_SENSOR:
    case SI702x_SENSOR:
      delay(SI70XX_POWER_ON_DELAY);       //wait for Si70xx to initialize to full range after power-on
      break;

    default:
      //empty
      break;
  }

  Wire.beginTransmission(_address);
  if (Wire.endTransmission(true) != 0) {return false;} //safety check, make sure sensor is connected

  softReset();                //soft reset is recommended at start

  setHeater(false);           //false=off

  setResolution(_resolution);

  return voltageStatus();     //check power before using, sensor doesn't operate correctly if VDD < threshold 
}


/**************************************************************************/
/*
    readHumidity()

    Read relative humidity, in %

    NOTE:
    - range........... 0%..100%
    - resolution...... 0.04% at 12-bits
    - accuracy........ +-2% in range 20%..80% at 25C
    - response time... 5..30sec

    - measurement with high frequency leads to heating of the
      sensor, must be > 0.5 seconds apart to keep self-heating below 0.1C

    - initiating humidity measurement for Si70xx will also automatically
      start temperature measurement, total conversion time will
      be tCONV(RH) + tCONV(T)

    - "sensorOperationMode":
      - "HOLD_I2C" mode, sensor blocks I2C bus during measurement,
        by keeping SCL line LOW
      - "NOHOLD_I2C" mode, without blocking I2C bus during measurement,
        allows communication with another slave devices on I2C bus

    - WARNING!!! "NOHOLD_I2C" could create collision if more than one
      slave devices are connected to the same bus, had a glitch with
      Bosch BMP180 & BMP085

    - data register controls:
      15    14  13   12   11   10   9    8    7    6    5    4    3    2    1   0
      MSB, MSB, MSB, MSB, MSB, MSB, MSB, MSB, LSB, LSB, LSB, LSB, LSB, LSB, ST, ST
      - ST diagnostic status bits:
        - XXXXXXXXXXXXXX00, temperature measurement
        - XXXXXXXXXXXXXX10, humidity measurement
        - 0000000000000000, open circuit condition or sensor displays RH <=-6%RH or T<=-46.85C
        - 1111111111111111, short circuit condition or sensor displays RH >=119%RH or T>=128.87C
*/
/**************************************************************************/
float HTU2xD_SHT2x_SI70xx::readHumidity(HTU2XD_SHT2X_SI70XX_HUMD_OPERATION_MODE_REG sensorOperationMode)
{
  /* start humidity measurement */
  Wire.beginTransmission(_address);
  Wire.write(sensorOperationMode);
  if (Wire.endTransmission(true) != 0) {return HTU2XD_SHT2X_SI70XX_ERROR;} //no reason to continue, abort

  /* humidity measurement delay */
  delay(_getMeasurementDelay(true)); //true=humidity read, false=temperature read

  /* read humidity result & CRC to "wire.h" rxBuffer */
  Wire.requestFrom(_address, (uint8_t)3, (uint8_t)true);         //read 3-byte to "wire.h" rxBuffer, true-send stop after transmission
  if (Wire.available() != 3) {return HTU2XD_SHT2X_SI70XX_ERROR;} //no reason to continue, abort

  /* reads 16-bit humidity from "wire.h" rxBuffer */
  uint16_t rawHumidity  = Wire.read() << 8; //read MSB byte & shift
           rawHumidity |= Wire.read();      //read LSB byte & sum with MSB byte

  /* read 8-bit checksum from "wire.h" rxBuffer */
  if (_checkCRC8(rawHumidity) != Wire.read()) {return HTU2XD_SHT2X_SI70XX_ERROR;}; //read checksum & compare, no reason to continue

  /* calculate humidity */
  rawHumidity &= 0xFFFC; //clear diagnostic status bits, 14-bit usefull data see NOTE

  float humidity = ((125.0 * rawHumidity) / 0x10000 - 6);

  /* humidity might be slightly smaller 0% or bigger 100% */
  if      (humidity < 0)   {humidity = 0;}
  else if (humidity > 100) {humidity = 100;}

  return humidity;
}


/**************************************************************************/
/*
    getCompensatedHumidity()

    Read humidity & compensate temperature influence on RH, for HTU2xD/SHT2x only

    NOTE:
    - accuracy +-2% in range 0%..100% at 0C..80C
    - Si70xx automatically compensates temperature influence on RH every
      humidity measurement
    - total conversion time will be tCONV(RH) + tCONV(T) = 110msec
*/
/**************************************************************************/
float HTU2xD_SHT2x_SI70xx::getCompensatedHumidity(float temperature)
{
  if (temperature == HTU2XD_SHT2X_SI70XX_ERROR) {return HTU2XD_SHT2X_SI70XX_ERROR;} //no reason to continue, abort

  float humidity = readHumidity();

  if (humidity == HTU2XD_SHT2X_SI70XX_ERROR) {return HTU2XD_SHT2X_SI70XX_ERROR;}    //no reason to continue, abort

  if ((_sensorType == HTU2xD_SENSOR) || (_sensorType == SHT2x_SENSOR))              //for HTU2xD & SHT2x only, Si70xx automatically compensates temperature influence
  {
    if (temperature >= 0 && temperature <= 80) {humidity = humidity + (25.0 - temperature) * HTU2XD_SHT2X_TEMP_COEF_0C_80C;} //apply compensation coefficient
  }

  return humidity;
}


/**************************************************************************/
/*
    readTemperature()

    Read temperature, in C

    NOTE:
    - range........... -40C..+125C
    - resolution...... 0.01C at 14-bits
    - accuracy........ +-0.3C in range 0C..60C
    - response time... 5..30sec

    - measurement with high frequency leads to heating of the
      sensor, must be > 0.5 seconds apart to keep self-heating below 0.1C

    - initiating humidity measurement for Si70xx will also automatically
      start temperature measurement, total conversion time will
      be tCONV(RH) + tCONV(T)

    - "sensorOperationMode":
      - "HOLD_I2C" mode, sensor blocks I2C bus during measurement,
        by keeping SCL line LOW
      - "NOHOLD_I2C" mode, without blocking I2C bus during measurement,
        allows communication with another slave devices on I2C bus
      - "READ_TEMP_AFTER_RH" mode, allow to retrive temperature
        measurement, which was made at previouse RH measurement,
        for Si70xx only

    - WARNING!!! "NOHOLD_I2C" could create collision if more than one
      slave devices are connected to the same bus, had a glitch with
      Bosch BMP180 & BMP085

    - data register controls:
      15    14  13   12   11   10   9    8    7    6    5    4    3    2    1   0
      MSB, MSB, MSB, MSB, MSB, MSB, MSB, MSB, LSB, LSB, LSB, LSB, LSB, LSB, ST, ST
      - ST diagnostic status bits:
        - XXXXXXXXXXXXXX00, temperature measurement
        - XXXXXXXXXXXXXX10, humidity measurement
        - 0000000000000000, open circuit condition or sensor displays RH <=-6%RH or T<=-46.85C
        - 1111111111111111, short circuit condition or sensor displays RH >=119%RH or T>=128.87C
*/
/**************************************************************************/
float HTU2xD_SHT2x_SI70xx::readTemperature(HTU2XD_SHT2X_SI70XX_TEMP_OPERATION_MODE_REG sensorOperationMode)
{
  /* start temperature measurement */
  Wire.beginTransmission(_address);
  Wire.write(sensorOperationMode); 
  if (Wire.endTransmission(true) != 0) {return HTU2XD_SHT2X_SI70XX_ERROR;} //no reason to continue, abort

  /* temperature measurement delay */
  uint8_t dataSize;

  if (sensorOperationMode != READ_TEMP_AFTER_RH)
  {
    dataSize = 3;                       //2-bytes temperature & 1-byte CRC

    delay(_getMeasurementDelay(false)); //true=humidity read, false=temperature read
  }
  else                                  //TODO Si70xx check
  {
    dataSize = 2;                       //skip delay, 2-bytes temperature & CRC is not available with "READ_TEMP_AFTER_RH" command
  }

  /* read temperature result & CRC to "wire.h" rxBuffer */
  Wire.requestFrom(_address, dataSize, (uint8_t)true);                  //read n-byte to "wire.h" rxBuffer, true-send stop after transmission
  if (Wire.available() != dataSize) {return HTU2XD_SHT2X_SI70XX_ERROR;} //no reason to continue, abort

  /* reads 16-bit temperature from "wire.h" rxBuffer */
  uint16_t rawTemperature  = Wire.read() << 8; //read MSB byte & shift
           rawTemperature |= Wire.read();      //read LSB byte & sum with MSB byte

  /* read 8-bit checksum from "wire.h" rxBuffer */
  if (sensorOperationMode != READ_TEMP_AFTER_RH)                                        //CRC is not available with "READ_TEMP_AFTER_RH" command
  {
    if (_checkCRC8(rawTemperature) != Wire.read()) {return HTU2XD_SHT2X_SI70XX_ERROR;}; //read checksum & compare, no reason to continue
  }

  /* calculate temperature */
  rawTemperature &= 0xFFFC;                             //clear diagnostic status bits, 14-bit usefull data see NOTE

  return ((175.72 * rawTemperature) / 0x10000 - 46.85);
}


/**************************************************************************/
/*
    setType()

    Set sensor type & sensor I2C address on the fly

    NOTE:
    - HTU2xD vs SHT2x vs Si70xx:
      - HTU2xD +1.5v..+3.6v, SHT2x +2.1v..+3.6v, Si70xx +1.9v..+3.6v
      - Si70xx very fast & automatically start temperature measurement
        during humidity conversion to compensate temperature influence on RH
      - HTU2xD fast
      - SHT2x slow
      - Si70xx support different heater levels 3.1mA..94.2mA, HTU2xD/SHT2x
        only 1.83mA
*/
/**************************************************************************/
void HTU2xD_SHT2x_SI70xx::setType(HTU2XD_SHT2X_SI70XX_I2C_SENSOR sensorType)
{
  switch (sensorType)
  {
    case HTU2xD_SENSOR:
    case SHT2x_SENSOR:
    case SI700x_SENSOR:
    case SI702x_SENSOR:
      _address = HTU2XD_SHT2X_SI702X_SI700X_ADDRESS_X40;
      break;

    case SI701x_SENSOR:
      _address = SI701X_ADDRESS_X41;
      break;

    default:
      //empty
      break;
  }
}


/**************************************************************************/
/*
    setResolution()

    Set sensor resolution

    NOTE:
    - see "_readRegister()" NOTE for details
*/
/**************************************************************************/
void HTU2xD_SHT2x_SI70xx::setResolution(HTU2XD_SHT2X_SI70XX_USER_CTRL_RES sensorResolution)
{
  uint8_t userRegData = _readRegister(HTU2XD_SHT2X_SI70XX_USER_READ_REG); //read current user register settings, any reserved register must not be written

  if (userRegData == HTU2XD_SHT2X_SI70XX_ERROR) {return;}                 //no reason to continue, abort

  userRegData &= 0x7E;                                                    //clear current resolution bit[7:0]
  userRegData |= sensorResolution;                                        //add new resolution bits

  if (_writeRegister(HTU2XD_SHT2X_SI70XX_USER_WRITE_REG, userRegData) != HTU2XD_SHT2X_SI70XX_ERROR) {_resolution = sensorResolution;} //updates variable if no collision on I2C bus
}


/**************************************************************************/
/*
    voltageStatus()

    Checks battery status, accuracy +-0.1v

    NOTE:
    - true=power OK,   VDD > 2.25v for HTU2xD/SHT2x | VDD > +1.9v for SI70xx
    - false=power LOW, VDD < 2.25v for HTU2xD/SHT2x | VDD < +1.8v for SI70xx

    - see "_readRegister()" NOTE for details
*/
/**************************************************************************/
bool HTU2xD_SHT2x_SI70xx::voltageStatus()
{
  uint8_t userRegData = _readRegister(HTU2XD_SHT2X_SI70XX_USER_READ_REG); //read current user register settings

  if (userRegData == HTU2XD_SHT2X_SI70XX_ERROR) {return false;}           //no reason to continue, abort

  userRegData &= HTU2XD_SHT2X_SI70XX_USER_CTRL_VDD_LOW;                   //clear all bits except bit[6], 1=LOW, 0=OK see NOTE

  return (userRegData != HTU2XD_SHT2X_SI70XX_USER_CTRL_VDD_LOW);          //true=power OK, false=power LOW
}


/**************************************************************************/
/*
    setHeater()

    Activate integrated resistive heating element

    NOTE:
    - HTU2xD/SHT2x heater consumes about 5.5mW/1.83mA @ 3.0v & provides a
      temperature increase of about 0.5..1.5C
    - Si7xx heater consumes 3.09mA..94.20mA @ 3.3v, see
      "_setSi7xxHeaterLevel()" for details
    - Si7xx power levels 0..15, used value > 15 to skip writing to heater
      register if only activate/deactivate heater is needed

    - heater deactivated by default 
    - heater bits doesn't clear at soft reset
    - Si7xx power level 0/3.09mA by default
*/
/**************************************************************************/
void HTU2xD_SHT2x_SI70xx::setHeater(bool heaterOn, uint8_t heaterPower)
{
  _setSi7xxHeaterLevel(heaterPower);                                                 //only Si7xx support power levels, 3.09mA..94.20mA

  uint8_t userRegData = _readRegister(HTU2XD_SHT2X_SI70XX_USER_READ_REG);            //read current user register settings, any reserved register must not be written

  if (userRegData == HTU2XD_SHT2X_SI70XX_ERROR) {return;}                            //no reason to continue, abort

  if   (heaterOn == true) {userRegData |=  HTU2XD_SHT2X_SI70XX_USER_CTRL_HEATER_ON;} //activate, 5.5mW/1.83mA for HTU2xD/SHT2x
  else                    {userRegData &= ~HTU2XD_SHT2X_SI70XX_USER_CTRL_HEATER_ON;} //deactivate, 0.0mW/0.0mA for HTU2xD/SHT2x/Si7xx

  _writeRegister(HTU2XD_SHT2X_SI70XX_USER_WRITE_REG, userRegData);                   //write new user register settings
}


/**************************************************************************/
/*
    softReset()  
 
    Soft reset, switch sensor OFF & ON

    NOTE:
    - takes about 15 milliseconds to initialize after reset
    - all registers (except heater bit in user register) will set to default!!!
*/
/**************************************************************************/
void HTU2xD_SHT2x_SI70xx::softReset()
{
  Wire.beginTransmission(_address);

  Wire.write(HTU2XD_SHT2X_SI70XX_SOFT_RESET_REG);

  Wire.endTransmission(true);

  delay(HTU2XD_SHT2X_SI70XX_SOFT_RESET_DELAY);
}


/***************************************************************************/
/*
    readDeviceID()

    Read device ID

    NOTE:
    - device ID is a part of 64-bit electronic serial number, this serial
      number is located in two different locations on the on-chip memory

    - HTU2xD/SHT2x 1-st memory 4-byte structure:
      - SNB3, CRC, SNB2, CRC, SNB1, CRC, SNB0, CRC
    - HTU2xD/SHT2x 2-nd memory 4-byte structure:
      - SNC1**, SNC0, CRC, SNA1, SNA0, CRC
    - HTU2xD/SHT2x 64-bit electronic serial number:
      - SNA1*, SNA0*, SNB3*, SNB2, SNB1, SNB0, SNC1**, SNC0
        - *fixed values:
          - SNA1, 0x48
          - SNA0, 0x54
          - SNB3, 0x00

    - Si70xx 1-st memory 4-byte structure:
      - SNA3, CRC, SNA2, CRC, SNA1, CRC, SNA0, CRC
    - Si70xx 2-nd memory 4-byte structure:
      - SNB3**, SNB2, CRC, SNB1, SNB0, CRC
    - Si70xx 64-bit electronic serial number:
      - SNA3, SNA2, SNA1, SNA0, SNB3**, SNB2, SNB1, SNB0

    - **chip ID:
        - 0x00, Si70xx engineering sample
        - 0x06, Si7006
        - 0x0D, Si7013
        - 0x14, Si7020
        - 0x15, Si7021 
        - 0x32, HTU2xD/SHT2x
        - 0xFF, Si70xx engineering sample
*/
/**************************************************************************/
uint16_t HTU2xD_SHT2x_SI70xx::readDeviceID()
{
  /* request 2-nd memory */
  Wire.beginTransmission(_address);

  Wire.write(HTU2XD_SHT2X_SI70XX_SERIAL2_READ1_REG);
  Wire.write(HTU2XD_SHT2X_SI70XX_SERIAL2_READ2_REG);

  if (Wire.endTransmission(true) != 0) {return HTU2XD_SHT2X_SI70XX_ERROR;} //collision on I2C bus, no reason to continue

  /* read 2-nd memory to "wire.h" rxBuffer */
  Wire.requestFrom(_address, (uint8_t)3, (uint8_t)true); //read 3-byte to "wire.h" rxBuffer, true-send stop after transmission

  /* reads 16-bit from "wire.h" rxBuffer */
  uint16_t deviceID  = Wire.read() << 8; //SNC1** or SNB3**
           deviceID |= Wire.read();      //add SNC0 or SNB2

  /* read 8-bit checksum from "wire.h" rxBuffer */
  if (_checkCRC8(deviceID) != Wire.read()) {return HTU2XD_SHT2X_SI70XX_ERROR;}; //read checksum & compare, no reason to continue

  /* get ID */
  deviceID = deviceID >> 8; //clear SNC0 or SNB2

  switch(deviceID)
  {
    case HTU2XD_SHT2X_SERIAL2_READ_CTRL_CHIP_ID:
      deviceID = 21;
      break;

    case SI7021_SERIAL2_READ_CTRL_CHIP_ID:
      deviceID = 7021;
      break;

    case SI7020_SERIAL2_READ_CTRL_CHIP_ID:
      deviceID = 7020;
      break;

    case SI7013_SERIAL2_READ_CTRL_CHIP_ID:
      deviceID = 7013;
      break;

    case SI7006_SERIAL2_READ_CTRL_CHIP_ID:
      deviceID = 7006;
      break;

    case SI7000_SERIAL2_READ_CTRL_CHIP_ID:
    case SI70FF_SERIAL2_READ_CTRL_CHIP_ID:
      deviceID = 7000;
      break;

    default:
      deviceID = HTU2XD_SHT2X_SI70XX_ERROR;
      break;
  }

  return deviceID;
}

/***************************************************************************/
/*
    readFirmwareVersion()

    Read firmware version

    NOTE:
    - 1=1.0, 2=2.0
*/
/**************************************************************************/
uint8_t HTU2xD_SHT2x_SI70xx::readFirmwareVersion(void)
{
  /* request firware version */
  Wire.beginTransmission(_address);

  Wire.write(HTU2XD_SHT2X_SI70XX_FW_READ1_REG);
  Wire.write(HTU2XD_SHT2X_SI70XX_FW_READ2_REG);

  if (Wire.endTransmission(true) != 0) {return HTU2XD_SHT2X_SI70XX_ERROR;} //collision on I2C bus, no reason to continue

  /* read firmware version to "wire.h" rxBuffer */
  Wire.requestFrom(_address, (uint8_t)1, (uint8_t)true); //read 1-byte to "wire.h" rxBuffer, true-send stop after transmission

  /* read 8-bit ID from "wire.h" rxBuffer */
  switch(Wire.read())
  {
    case HTU2XD_SHT2X_SI70XX_FW_READ_CTRL_V1:
      return 1;

    case HTU2XD_SHT2X_SI70XX_FW_READ_CTRL_V2:
      return 2;

    default:
      return HTU2XD_SHT2X_SI70XX_ERROR;
  }
}


/**************************************************************************/
/*
    _readRegister()

    Read 8-bit from sensor user register

    NOTE:
    - recommended before write operation, read values from RSVD & write
      back unchanged

    - HTU2xD/SHT2x/Si70xx user register controls:
      7     6    5     4     3     2     1         0
      RES1, VDD, RSVD, RSVD, RSVD, HTRE, OTP/RSVD, RES0
      - RESx (R/W):
        - 00, RH 12-bits / T 14-bits (default after power-on or reset)
        - 01, RH 8-bits  / T 12-bits
        - 10, RH 10-bits / T 13-bits
        - 11, RH 11-bits / T 11-bits
      - VDD (R):
        - 0, OK  VDD > +2.25v for HTU2xD/SHT2x & VDD > +1.9v for Si70xx 
        - 1, LOW VDD < +2.25v for HTU2xD/SHT2x & VDD < +1.9v for Si70xx
      - RSVD (R/W):
        - reserved
      - HTRE (R/W):
        - 1, On-chip heater enable
        - 0, On-chip heater disable
      - OTP/RSVD (R/W):
        - 1 OTP disable reload
          disabled by default & it is not recommended for use (use soft
          reset instead) for HTU2xD/SHT2x only & RSVD for Si70xx
        - 0, OTP enable reload
          default settings loaded after each time a measurement
          command is issued for HTU2xD/SHT2x only & RSVD for Si70xx
*/
/**************************************************************************/
uint8_t HTU2xD_SHT2x_SI70xx::_readRegister(uint8_t reg)
{
  /* send read user register command */
  Wire.beginTransmission(_address);

  Wire.write(reg);

  if (Wire.endTransmission(true) != 0) {return HTU2XD_SHT2X_SI70XX_ERROR;} //collision on I2C bus, probability that all bits in user register will be HIGH is very small

  /* read user register to "wire.h" rxBuffer */
  Wire.requestFrom(_address, (uint8_t)1, (uint8_t)true);                   //read 1-byte to "wire.h" rxBuffer, true-send stop after transmission

  /* read 8-bit ID from "wire.h" rxBuffer */
  if (Wire.available() == 1) {return Wire.read();}                         //read 1-byte from "wire.h" rxBuffer TODO: read CRC8
                              return HTU2XD_SHT2X_SI70XX_ERROR;            //collision on I2C bus, "wire.h" rxBuffer is empty
}


/**************************************************************************/
/*
    _writeRegister()

    Write 8-bit to sensor register
*/
/**************************************************************************/
uint8_t HTU2xD_SHT2x_SI70xx::_writeRegister(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(_address);

  Wire.write(reg);

  Wire.write(value);

  if (Wire.endTransmission(true) != 0) {return HTU2XD_SHT2X_SI70XX_ERROR;} //collision on I2C bus, sensor didn't return ACK
                                        return 0;
}


/**************************************************************************/
/*
    _getMeasurementDelay()

    Get measurement delay

    NOTE:
    - initiating humidity measurement for Si70xx will also automatically
      start temperature measurement, total conversion time will
      be tCONV(RH) + tCONV(T)
*/
/**************************************************************************/
uint8_t HTU2xD_SHT2x_SI70xx::_getMeasurementDelay(bool isHumdRead)
{
  uint8_t delayMsec;

  switch(_resolution)
  {
    case HUMD_12BIT_TEMP_14BIT:
      if   (isHumdRead == true) {delayMsec = SHT2X_HUMD_12BIT_RES_DELAY;}  //HTU2xD 14..16msec | SHT2x 22..29msec | Si70xx 10+7..12+10.8msec
      else                      {delayMsec = SHT2X_TEMP_14BIT_RES_DELAY;}  //HTU2xD 44..50msec | SHT2x 66..85msec | Si70xx 7..10.8msec
      break;

    case HUMD_11BIT_TEMP_11BIT:
      if   (isHumdRead == true) {delayMsec = SHT2X_HUMD_11BIT_RES_DELAY;}  //HTU2xD 7..8msec | SHT2x 12..15msec | Si70xx 7+1.5..10.8+2.4msec
      else                      {delayMsec = SHT2X_TEMP_11BIT_RES_DELAY;}  //HTU2xD 6..7msec | SHT2x 9..11msec | Si70xx 1.5..2.4msec
      break;

    case HUMD_10BIT_TEMP_13BIT:
      if   (isHumdRead == true) {delayMsec = SI70XX_HUMD_10BIT_RES_DELAY;} //HTU2xD 4..5msec | SHT2x 7..9msec | Si70xx 3.7+4..5.4+6.2msec
      else                      {delayMsec = SHT2X_TEMP_13BIT_RES_DELAY;}  //HTU2xD 22..25msec | SHT2x 33.43msec | Si70xx 4..6.2msec
      break;

    case HUMD_08BIT_TEMP_12BIT:
      if   (isHumdRead == true) {delayMsec = SI70XX_HUMD_08BIT_RES_DELAY;} //HTU2xD 2..3msec | SHT2x 3..4msec | Si70xx 2.6+2.4..3.1+3.8msec
      else                      {delayMsec = SHT2X_TEMP_12BIT_RES_DELAY;}  //HTU2xD 11..13msec | SHT2x 17..22msec | Si70xx 2.4..3.8msec
      break;

    default:
      if   (isHumdRead == true) {delayMsec = SHT2X_HUMD_12BIT_RES_DELAY;}  //worst case humidity delay
      else                      {delayMsec = SHT2X_TEMP_14BIT_RES_DELAY;}  //worst case temperature delay
      break;
  }

  return delayMsec;
}


/**************************************************************************/
/*
    _setSi7xxHeaterLevel()

    Set heater power, for Si7xx only

    NOTE:
    - power levels 0..15, > 15 to to skip writing

    - Si70xx heater register controls:
      7     6     5     4     3       2       1       0
      RSVD, RSVD, RSVD, RSVD, HEATER, HEATER, HEATER, HEATER
      - RSVD (R/W):
        - reserved
      - HEATER (R/W):
        - 0/0b0000 3.09mA, default
        - 1/0b0001 9.18mA
        - 2/0b0010 15.24mA
        - ...
        - 4/0b0100 27.39mA
        - 8/0b1000 51.69mA
        - ...
        - 15/0b1111 94.20mA
        - >15, skip writing

    - heater no/off bit doesn't clear at soft reset
    - heater level bits cleared at soft reset to default
*/
/**************************************************************************/
void HTU2xD_SHT2x_SI70xx::_setSi7xxHeaterLevel(uint8_t powerLevel)
{
  if (powerLevel > 15) {return;}                                       //used to skip writing to heater register if only heater bit in user register needs to be changed

  if ((_sensorType != HTU2xD_SENSOR) || (_sensorType != SHT2x_SENSOR)) //only Si7xx support power levels
  {
    uint8_t userRegData = _readRegister(SI70XX_HEATER_READ_REG);       //read current heater register settings, any reserved register must not be written

    if (userRegData == HTU2XD_SHT2X_SI70XX_ERROR) {return;}            //no reason to continue, abort

    userRegData &= 0xF0;                                               //clear heater bits

    userRegData |= powerLevel;                                         //add new power level

    _writeRegister(SI70XX_HEATER_WRITE_REG, userRegData);
  }
}


/**************************************************************************/
/*
    checkCRC8()

    Calculate Cyclic Redundancy Check/CRC8 for 16-bit data

    NOTE:
    - initial value=0xFF, polynomial=x^8 + x^5 + x^4 + 1
*/
/**************************************************************************/
uint8_t HTU2xD_SHT2x_SI70xx::_checkCRC8(uint16_t data)
{
  for (uint8_t bit = 0; bit < 16; bit++)
  {
    if   (data & 0x8000) {data   = (data << 1) ^ HTU2XD_SHT2X_SI70XX_CRC8_POLYNOMINAL;}
    else                 {data <<= 1;}
  }

  return (data >>= 8);
}
