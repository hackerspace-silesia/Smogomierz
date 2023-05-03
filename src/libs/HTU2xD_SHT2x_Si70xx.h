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

#ifndef HTU2XD_SHT2X_SI70XX_h
#define HTU2XD_SHT2X_SI70XX_h


#include <Arduino.h>
#include <Wire.h>

#if defined (__AVR__)
#include <avr/pgmspace.h>              //for Arduino AVR PROGMEM support
#elif defined (ESP8266)
#include <pgmspace.h>                  //for Arduino ESP8266 PROGMEM support
#elif defined (ARDUINO_ARCH_STM32)
#include <avr/pgmspace.h>              //for Arduino STM32 PROGMEM support
#endif


/* list of I2C addresses */
#define HTU2XD_SHT2X_SI702X_SI700X_ADDRESS_X40  0x40    //HTU2xD, SHT2x, Si702x, Si700x  address
#define SI701X_ADDRESS_X41                      0x41    //SI701x address

/* list of command registers */
#define HTU2XD_SHT2X_SI70XX_USER_WRITE_REG      0xE6    //write to user register
#define HTU2XD_SHT2X_SI70XX_USER_READ_REG       0xE7    //read from user register
#define SI70XX_HEATER_WRITE_REG                 0x51    //write to heater register, for Si7xx only
#define SI70XX_HEATER_READ_REG                  0x11    //read from heater register, for Si7xx only
#define HTU2XD_SHT2X_SI70XX_SOFT_RESET_REG      0xFE    //write soft reset
#define HTU2XD_SHT2X_SI70XX_SERIAL1_READ1_REG   0xFA    //read 1-st 2-bytes of serial number
#define HTU2XD_SHT2X_SI70XX_SERIAL1_READ2_REG   0x0F    //read 2-nd 2-bytes of serial number
#define HTU2XD_SHT2X_SI70XX_SERIAL2_READ1_REG   0xFC    //read 3-rd 2-bytes of serial number
#define HTU2XD_SHT2X_SI70XX_SERIAL2_READ2_REG   0xC9    //read 4-th 2-bytes of serial number
#define HTU2XD_SHT2X_SI70XX_FW_READ1_REG        0x84    //read firmware revision, 1-st part of the command
#define HTU2XD_SHT2X_SI70XX_FW_READ2_REG        0xB8    //read firmware revision, 2-nd part of the command
//HTU2XD_SHT2X_SI70XX_TEMP_OPERATION_MODE_REG           //start temperature measurement, see below
//HTU2XD_SHT2X_SI70XX_HUMD_OPERATION_MODE_REG           //start humidity measurement, see below

/* list of user registers controls */
#define HTU2XD_SHT2X_USER_CTRL_OTP_ON           0x02    //disable OTP reload for HTU2xD/SHT2x only, bit[1] in user register (default after power-on or reset)
#define HTU2XD_SHT2X_SI70XX_USER_CTRL_HEATER_ON 0x04    //heater ON, bit[2] in user register (OFF by default after power-on or reset)
#define HTU2XD_SHT2X_SI70XX_USER_CTRL_VDD_LOW   0x40    //operating voltage LOW, bit[6] in user register

#define HTU2XD_SHT2X_SI70XX_FW_READ_CTRL_V1     0xFF    //sensor firmware v1.0
#define HTU2XD_SHT2X_SI70XX_FW_READ_CTRL_V2     0x20    //sensor firmware v2.0
//HTU2XD_SHT2X_SI70XX_USER_CTRL_RES                     //RH/T measurement resolution, bit [7:0] in user register, see below

#define SI7000_SERIAL2_READ_CTRL_CHIP_ID        0x00    //Si70xx engineering device ID, 1-st byte in second memory access
#define SI7006_SERIAL2_READ_CTRL_CHIP_ID        0x06    //Si7006 device ID, 1-st byte in second memory access
#define SI7013_SERIAL2_READ_CTRL_CHIP_ID        0x0D    //Si7013 device ID, 1-st byte in second memory access
#define SI7020_SERIAL2_READ_CTRL_CHIP_ID        0x14    //Si7020 device ID, 1-st byte in second memory access
#define SI7021_SERIAL2_READ_CTRL_CHIP_ID        0x15    //Si7021 device ID, 1-st byte in second memory access
#define HTU2XD_SHT2X_SERIAL2_READ_CTRL_CHIP_ID  0x32    //HTU2xD/SHT2x device ID, 1-st byte in second memory access
#define SI70FF_SERIAL2_READ_CTRL_CHIP_ID        0xFF    //Si70xx engineering device ID, 1-st byte in second memory access

/* speed & delay */
#define HTU2XD_SHT2X_SI70XX_I2C_SPEED_HZ        100000  //sensor I2C speed 100KHz..400KHz, in Hz
#define HTU2XD_SHT2X_SI70XX_I2C_STRETCH_USEC    1000    //I2C stretch time, in usec

#define HTU2XD_SHT2X_POWER_ON_DELAY             15      //wait for HTU2xD/SHT2x to initialize after power-on, in milliseconds
#define SI70XX_POWER_ON_DELAY                   80      //wait for Si70xx to initialize to full range after power-on, in milliseconds
#define HTU2XD_SHT2X_SI70XX_SOFT_RESET_DELAY    15      //wait for HTU2xD/SHT2x to initialize after reset, in milliseconds

#define SHT2X_HUMD_12BIT_RES_DELAY              29      //12-bit RH-resolution measurement delay, HTU2xD 14..16msec | SHT2x 22..29msec | Si70xx 10+7..12+10.8msec
#define SHT2X_HUMD_11BIT_RES_DELAY              15      //11-bit RH-resolution measurement delay, HTU2xD 7..8msec | SHT2x 12..15msec | Si70xx 7+1.5..10.8+2.4msec
#define SI70XX_HUMD_10BIT_RES_DELAY             13      //10-bit RH-resolution measurement delay, HTU2xD 4..5msec | SHT2x 7..9msec | Si70xx 3.7+4..5.4+6.2msec
#define SI70XX_HUMD_08BIT_RES_DELAY             7       //8-bit  RH-resolution measurement delay, HTU2xD 2..3msec | SHT2x 3..4msec | Si70xx 2.6+2.4..3.1+3.8msec

#define SHT2X_TEMP_14BIT_RES_DELAY              85      //14-bit T-resolution measurement delay, HTU2xD 44..50msec | SHT2x 66..85msec | Si70xx 7..10.8msec
#define SHT2X_TEMP_13BIT_RES_DELAY              43      //13-bit T-resolution measurement delay, HTU2xD 22..25msec | SHT2x 33.43msec | Si70xx 4..6.2msec
#define SHT2X_TEMP_12BIT_RES_DELAY              22      //12-bit T-resolution measurement delay, HTU2xD 11..13msec | SHT2x 17..22msec | Si70xx 2.4..3.8msec
#define SHT2X_TEMP_11BIT_RES_DELAY              11      //11-bit T-resolution measurement delay, HTU2xD 6..7msec | SHT2x 9..11msec | Si70xx 1.5..2.4msec

/* misc */
#define HTU2XD_SHT2X_TEMP_COEF_0C_80C           -0.15   //temperature coefficient for RH compensation at range 0C..80C, for HTU2xD/SHT2x only 
#define HTU2XD_SHT2X_SI70XX_CRC8_POLYNOMINAL    0x13100 //16-bit CRC8 polynomial for CRC8 -> x^8 + x^5 + x^4 + 1
#define HTU2XD_SHT2X_SI70XX_ERROR               0xFF    //returns 255, if CRC8 or communication error is occurred


/* list of user registers controls, continue */
typedef enum : uint8_t
{
  HUMD_12BIT_TEMP_14BIT = 0x00,                         //resolution RH 12-bit / T 14-bit, bit[7:0] in user register (default after power-on or reset)
  HUMD_08BIT_TEMP_12BIT = 0x01,                         //resolution RH 8-bit  / T 12-bit, bit[7:0] in user register
  HUMD_10BIT_TEMP_13BIT = 0x80,                         //resolution RH 10-bit / T 13-bit, bit[7:0] in user register
  HUMD_11BIT_TEMP_11BIT = 0x81                          //resolution RH 11-bit / T 11-bit, bit[7:0] in user register
}
HTU2XD_SHT2X_SI70XX_USER_CTRL_RES;


/* list of command registers, continue */
typedef enum : uint8_t
{
  START_HUMD_HOLD_I2C   = 0xE5,                         //start humidity measurement & blocking/stretching I2C bus during measurement
  START_HUMD_NOHOLD_I2C = 0xF5                          //start humidity measurement & without blocking/stretching I2C bus during measurement
}
HTU2XD_SHT2X_SI70XX_HUMD_OPERATION_MODE_REG;


/* list of command registers, continue */
typedef enum : uint8_t
{
  START_TEMP_HOLD_I2C   = 0xE3,                         //start temperature measurement & blocking I2C bus during measurement
  START_TEMP_NOHOLD_I2C = 0xF3,                         //start temperature measurement & without blocking I2C bus during measurement
  READ_TEMP_AFTER_RH    = 0xE0                          //read temperature value from previous humidity measurement, for Si70xx only
}
HTU2XD_SHT2X_SI70XX_TEMP_OPERATION_MODE_REG;


/* custom list of supported sensors */
typedef enum : uint8_t
{
  HTU2xD_SENSOR = 0x00,
  SHT2x_SENSOR  = 0x01,
  SI700x_SENSOR = 0x02,
  SI701x_SENSOR = 0x03,
  SI702x_SENSOR = 0x04
}
HTU2XD_SHT2X_SI70XX_I2C_SENSOR;


class HTU2xD_SHT2x_SI70xx
{
  public:
   HTU2xD_SHT2x_SI70xx(HTU2XD_SHT2X_SI70XX_I2C_SENSOR = HTU2xD_SENSOR, HTU2XD_SHT2X_SI70XX_USER_CTRL_RES = HUMD_12BIT_TEMP_14BIT);

  #if defined (__AVR__)
   bool     begin(uint32_t speed = HTU2XD_SHT2X_SI70XX_I2C_SPEED_HZ, uint32_t stretch = HTU2XD_SHT2X_SI70XX_I2C_STRETCH_USEC);
  #elif defined (ESP8266)
   bool     begin(uint8_t sda = SDA, uint8_t scl = SCL, uint32_t speed = HTU2XD_SHT2X_SI70XX_I2C_SPEED_HZ, uint32_t stretch = HTU2XD_SHT2X_SI70XX_I2C_STRETCH_USEC);
  #elif defined (ESP32)
   bool     begin(int32_t sda = SDA, int32_t scl = SCL, uint32_t speed = HTU2XD_SHT2X_SI70XX_I2C_SPEED_HZ, uint32_t stretch = HTU2XD_SHT2X_SI70XX_I2C_STRETCH_USEC);
  #elif defined (ARDUINO_ARCH_STM32)
   bool     begin(uint32_t sda = SDA, uint32_t scl = SCL, uint32_t speed = HTU2XD_SHT2X_SI70XX_I2C_SPEED_HZ);
  #else
   bool     begin();
  #endif

   float    readHumidity(HTU2XD_SHT2X_SI70XX_HUMD_OPERATION_MODE_REG = START_HUMD_HOLD_I2C); //ARDUINO_ARCH_STM32
   float    getCompensatedHumidity(float temperature);
   float    readTemperature(HTU2XD_SHT2X_SI70XX_TEMP_OPERATION_MODE_REG = START_TEMP_HOLD_I2C); //ARDUINO_ARCH_STM32

   void     setType(HTU2XD_SHT2X_SI70XX_I2C_SENSOR = HTU2xD_SENSOR);
   void     setResolution(HTU2XD_SHT2X_SI70XX_USER_CTRL_RES sensorResolution);
   bool     voltageStatus();
   void     setHeater(bool heaterOn, uint8_t powerLevel = 16);
   uint16_t readDeviceID();
   uint8_t  readFirmwareVersion();
   void     softReset();

  private:
   HTU2XD_SHT2X_SI70XX_USER_CTRL_RES _resolution;
   HTU2XD_SHT2X_SI70XX_I2C_SENSOR    _sensorType;
   uint8_t                           _address;

   uint8_t _readRegister(uint8_t reg);
   uint8_t _writeRegister(uint8_t reg, uint8_t value);
   uint8_t _getMeasurementDelay(bool isHumdRead);
   void    _setSi7xxHeaterLevel(uint8_t powerLevel);
   uint8_t _checkCRC8(uint16_t data);
};

#endif
