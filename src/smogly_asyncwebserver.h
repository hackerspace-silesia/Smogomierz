#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266httpUpdate.h>
#elif defined ARDUINO_ARCH_ESP32
#include <Update.h>
#include <SPIFFS.h>
#include <ESPmDNS.h>
#endif

#include <ArduinoJson.h> // 6.9.0 or later
#include "smogly_spiffs.h"
#include <FS.h>

static String handle_root_processor(const String& var)
{
  // Serial.println(F("var: ") + var);
  String message;
  message = "";

  if (var == F("{Language}")) {
    message += String(TEXT_LANG);
  }

  if (var == F("{WEB_PAGE_CSS}")) {
    message += String(WEB_PAGE_HEADER_CSS);
  }

  if (var == F("{SMOGLY_LOGO_URL}")) {
    message += String(SMOGLY_LOGO_URL);
  }

  if (var == F("{CurrentPageTitle}")) {
    message += String(TEXT_INDEX_PAGE);
  }
  if (var == F("{IndexPageTitle}")) {
    message += String(TEXT_INDEX_PAGE);
  }
  if (var == F("{ConfigPageTitle}")) {
    message += String(TEXT_CONFIG_PAGE);
  }
  if (var == F("{UpdatePageTitle}")) {
    message += String(TEXT_UPDATE_PAGE);
  }

  if (!(deviceSettings.autoUpdate)) {
    if (need_update) {
      if (var == F("{WEB_UPDATE_INFO_WARNING}")) {
        message += String(WEB_UPDATE_INFO_WARNING);
      }

      if (var == F("{TEXT_FWUPDATEAVALIBLE}")) {
        message += String(TEXT_FWUPDATEAVALIBLE);
      }
      if (var == F("{MANUALUPDATEBUTTON}")) {
        message += "";
      }

      if (var == F("{FWUPDATEBUTTON}")) {
        message += String(WEB_UPDATE_BUTTON_FWUPDATE);
      }
      if (var == F("{TEXT_FWUPDATEBUTTON}")) {
        message += String(TEXT_FWUPDATEBUTTON);
      }

      if (var == F("{AUTOUPDATEONBUTTON}")) {
        message += String(WEB_UPDATE_BUTTON_AUTOUPDATEON);
      }
      if (var == F("{TEXT_AUTOUPDATEONBUTTON}")) {
        message += String(TEXT_AUTOUPDATEONBUTTON);
      }

      if (var == F("{TEXT_AUTOUPDATEWARNING}")) {
        message += String(TEXT_AUTOUPDATEWARNING);
      }
      if (var == F("{TEXT_FWUPDATEBUTTON}")) {
        message += String(TEXT_FWUPDATEBUTTON);
      }

    }
    if (var == F("{WEB_UPDATE_INFO_WARNING}")) {
      message += "";
    }
  } else {
    if (var == F("{WEB_UPDATE_INFO_WARNING}")) {
      message += "";
    }
  }

  if (!strcmp(sensorsSettings.thpModel, "Non")) {
    if (var == F("{WEB_ROOT_PAGE_MEASUREMENTS_THP1")) {
      message += "";
    }
  } else {
    if (var == F("{WEB_ROOT_PAGE_MEASUREMENTS_THP1")) {
		takeTHPMeasurements();
      message += String(WEB_ROOT_PAGE_MEASUREMENTS_THP1);
      message.replace(F("{TEXT_WEATHER}"), String(TEXT_WEATHER));
      short dewpoint = (measurementsData.temperature - (14.55 + 0.114 * measurementsData.temperature) * (1 - (0.01 * measurementsData.humidity)) - pow(((2.5 + 0.007 * measurementsData.temperature) * (1 - (0.01 * measurementsData.humidity))),3) - (15.9 + 0.117 * measurementsData.temperature) * pow((1 - (0.01 * measurementsData.humidity)), 14));
      if (!strcmp(sensorsSettings.thpModel, "BME280")) {
        if (checkBmeStatus()) {
            message.replace(F("{TEXT_TEMPERATURE}"), String(TEXT_TEMPERATURE));
            message.replace(F("{TEXT_HUMIDITY}"), String(TEXT_HUMIDITY));
            message.replace(F("{TEXT_PRESSURE}"), String(TEXT_PRESSURE));
            message.replace(F("{TEXT_DEWPOINT}"), String(TEXT_DEWPOINT));

            message.replace(F("{Temperature}"), String(int(measurementsData.temperature)));
            message.replace(F("{Pressure}"), String(int(measurementsData.pressure)));
            message.replace(F("{Humidity}"), String(int(measurementsData.humidity)));
            message.replace(F("{Dewpoint}"), String(dewpoint));
        } else {
          message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
        }
      } else if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
        if (checkHTU21DStatus()) {
          message.replace(F("{TEXT_TEMPERATURE}"), String(TEXT_TEMPERATURE));
          message.replace(F("{TEXT_HUMIDITY}"), String(TEXT_HUMIDITY));
          message.replace(F("{TEXT_DEWPOINT}"), String(TEXT_DEWPOINT));

          message.replace(F("{Temperature}"), String(int(measurementsData.temperature)));
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{Humidity}"), String(int(measurementsData.humidity)));
          message.replace(F("{Dewpoint}"), String(dewpoint));
        } else {
          message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
        }
      } else if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
        if (checkDHT22Status()) {
          message.replace(F("{TEXT_TEMPERATURE}"), String(TEXT_TEMPERATURE));
          message.replace(F("{TEXT_HUMIDITY}"), String(TEXT_HUMIDITY));
          message.replace(F("{TEXT_DEWPOINT}"), String(TEXT_DEWPOINT));

          message.replace(F("{Temperature}"), String(int(measurementsData.temperature)));
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{Humidity}"), String(int(measurementsData.humidity)));
          message.replace(F("{Dewpoint}"), String(dewpoint));
        } else {
          message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
        }
      } else if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
        if (checkBmpStatus()) {
          message.replace(F("{TEXT_TEMPERATURE}"), String(TEXT_TEMPERATURE));
          message.replace(F("{TEXT_PRESSURE}"), String(TEXT_PRESSURE));

          message.replace(F("{Temperature}"), String(int(measurementsData.temperature)));
          message.replace(F("{Pressure}"), String(int(measurementsData.pressure)));
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Pressure} °C"), "");
        } else {
          message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
        }
      } else if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
        if (checkSHT1xStatus()) {
          message.replace(F("{TEXT_TEMPERATURE}"), String(TEXT_TEMPERATURE));
          message.replace(F("{TEXT_HUMIDITY}"), String(TEXT_HUMIDITY));
          message.replace(F("{TEXT_DEWPOINT}"), String(TEXT_DEWPOINT));

          message.replace(F("{Temperature}"), String(int(measurementsData.temperature)));
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{Humidity}"), String(float(measurementsData.humidity)));
          message.replace(F("{Dewpoint}"), String(dewpoint));
        } else {
          message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
        }
      }

    }
  }

  if (strcmp(sensorsSettings.dustModel, "Non")) {
    if (var == F("{WEB_ROOT_PAGE_MEASUREMENTS_AIR}")) {
      message += String(WEB_ROOT_PAGE_MEASUREMENTS_AIR);
      message.replace(F("{TEXT_AIRPOLLUTION}"), String(TEXT_AIRPOLLUTION));

      if (deviceSettings.displayPM1) {
        message.replace(F("{averagePM1}"), String(measurementsData.averagePM1));
      } else {
        message.replace(F("PM1: {averagePM1} µg/m³"), "");
      }

      if (measurementsData.averagePM25 <= 10) {
        message.replace(F("{colorAveragePM25}"), F("<font color='#61EEE4'>"));
      } else if (measurementsData.averagePM25 > 10 && measurementsData.averagePM25 <= 20) {
        message.replace(F("{colorAveragePM25}"), F("<font color='#5BCAAA'>"));
      } else if (measurementsData.averagePM25 > 20 && measurementsData.averagePM25 <= 25) {
        message.replace(F("{colorAveragePM25}"), F("<font color='#EEE25D'>"));
      } else if (measurementsData.averagePM25 > 25 && measurementsData.averagePM25 <= 50) {
        message.replace(F("{colorAveragePM25}"), F("<font color='#F95459'>"));
      } else if (measurementsData.averagePM25 > 50) {
        message.replace(F("{colorAveragePM25}"), F("<font color='#920736'>"));
      } else {
        message.replace(F("{colorAveragePM25}"), F("<font>"));
      }

      message.replace(F("{averagePM25}"), String(measurementsData.averagePM25) + F("</font>"));

      if (measurementsData.averagePM10 <= 20) {
        message.replace(F("{colorAveragePM10}"), F("<font color='#61EEE4'>"));
      } else if (measurementsData.averagePM10 > 20 && measurementsData.averagePM10 <= 35) {
        message.replace(F("{colorAveragePM10}"), F("<font color='#5BCAAA'>"));
      } else if (measurementsData.averagePM10 > 35 && measurementsData.averagePM10 <= 50) {
        message.replace(F("{colorAveragePM10}"), F("<font color='#EEE25D'>"));
      } else if (measurementsData.averagePM10 > 50 && measurementsData.averagePM10 <= 100) {
        message.replace(F("{colorAveragePM10}"), F("<font color='#F95459'>"));
      } else if (measurementsData.averagePM10 > 100) {
        message.replace(F("{colorAveragePM10}"), F("<font color='#920736'>"));
      } else {
        message.replace(F("{colorAveragePM10}"), F("<font>"));
      }
      message.replace(F("{averagePM10}"), String(measurementsData.averagePM10) + F("</font>"));
    }
  } else {
    if (var == F("{WEB_ROOT_PAGE_MEASUREMENTS_AIR}")) {
      message += "";
    }
  }

  if (airMonitorSettings.graph) {
  #ifdef ARDUINO_ARCH_ESP32
    if (var == F("{WEB_ROOT_PAGE_AIRMONITOR_GRAPH}")) {
      message += String(WEB_ROOT_PAGE_AIRMONITOR_GRAPH);
      #ifdef ARDUINO_ARCH_ESP8266
        message.replace(F("{LATITUDE}"), String(deviceSettings.latitude));
        message.replace(F("{LONGITUDE}"), String(deviceSettings.longitude));
      #elif defined ARDUINO_ARCH_ESP32
        message.replace(F("{LATITUDE}"), String(deviceSettings.latitude, 6));
        message.replace(F("{LONGITUDE}"), String(deviceSettings.longitude, 6));
      #endif
    }
  #endif
  } else {
    if (var == F("{WEB_ROOT_PAGE_AIRMONITOR_GRAPH}")) {
      message += "";
    }
  }
  
  if (luftdatenSettings.graph) {
      if (var == F("{WEB_ROOT_PAGE_LUFTDATEN_GRAPH}")) {
          message += String(WEB_ROOT_PAGE_LUFTDATEN_GRAPH);
          message.replace(F("{NODE_LUFTDATEN_ID}"), String(luftdatenSettings.apiId));
      }
  } else {
    if (var == F("{WEB_ROOT_PAGE_LUFTDATEN_GRAPH}")) {
      message += "";
    }
  }

  if (thingSpeakSettings.graph) {
    if (var == F("{WEB_ROOT_PAGE_THINGSPEAK_GRAPH}")) {
      message += String(WEB_ROOT_PAGE_THINGSPEAK_GRAPH);
      message.replace(F("{THINGSPEAK_CHANNEL_ID}"), String(thingSpeakSettings.channelId));
      message.replace(F("{THINGSPEAK_READ_API_KEY}"), String(thingSpeakSettings.readApiKey));
    }
  } else {
    if (var == F("{WEB_ROOT_PAGE_THINGSPEAK_GRAPH}")) {
      message += "";
    }
  }

  return message;
  message = "";
}


static void handle_root(AsyncWebServerRequest *request) {
  if (deviceSettings.debug) {
    Serial.print(F("sizeof(WEB_ROOT_PAGE_ALL): "));
    Serial.println(sizeof(WEB_ROOT_PAGE_ALL)); // sizeof(WEB_ROOT_PAGE_ALL): ~2255
    Serial.print(F("\n"));
  }

  request->send_P(200, "text/html", WEB_ROOT_PAGE_ALL, handle_root_processor);
}

static String _addOption(const String &value, const String &label, const String &srslyValue) {
  String option = FPSTR(WEB_CONFIG_PAGE_ADDOPTION);
  option.replace(F("{value}"), value);
  if (value == srslyValue) {
    option.replace(F("{srslyValue}"), F(" selected>"));
  } else {
    option.replace(F("{srslyValue}"), F(">"));
  }
  option.replace(F("{label}"), label);
  return option;
}

static String _add_FIRST_THP_Option(const String &value, const String &label, const String &srslyValue) {
  String option = FPSTR(WEB_CONFIG_PAGE_ADDOPTION);
  option.replace(F("{value}"), value);
    
  if (strcmp(sensorsSettings.dustModel, "Non")) {
    if (String(dustSettings.tx) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (String(dustSettings.rx) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (sensorsSettings.secondThp) {
    #ifdef ARDUINO_ARCH_ESP32
      if (String(secondThpSettings.sda) == value) {
        option.replace(F("{srslyValue}"), F("disabled>"));
      }
      if (String(secondThpSettings.scl) == value) {
        option.replace(F("{srslyValue}"), F("disabled>"));
      }
    #endif
    }
  }
  
  if (value == srslyValue) {
    option.replace(F("{srslyValue}"), F(" selected>"));
  } else {
    option.replace(F("{srslyValue}"), F(">"));
  }
  option.replace(F("{label}"), label);
   
  return option;
}

#ifdef ARDUINO_ARCH_ESP32
static String _add_SECOND_THP_Option(const String &value, const String &label, const String &srslyValue) {
  String option = FPSTR(WEB_CONFIG_PAGE_ADDOPTION);
  option.replace(F("{value}"), value);

  if (strcmp(sensorsSettings.dustModel, "Non")) {
    if (String(dustSettings.tx) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (String(dustSettings.rx) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (strcmp(sensorsSettings.dustModel, "Non")) {
      if (String(firstThpSettings.sda) == value) {
        option.replace(F("{srslyValue}"), F("disabled>"));
      }
      if (String(firstThpSettings.scl) == value) {
        option.replace(F("{srslyValue}"), F("disabled>"));
      }
    }
  }

  if (value == srslyValue) {
    option.replace(F("{srslyValue}"), F(" selected>"));
  } else {
    option.replace(F("{srslyValue}"), F(">"));
  }
  option.replace(F("{label}"), label);
  return option;
}
#endif

static String _add_DUST_Option(const String &value, const String &label, const String &srslyValue) {
  String option = FPSTR(WEB_CONFIG_PAGE_ADDOPTION);
  option.replace(F("{value}"), value);

  if (strcmp(sensorsSettings.thpModel, "Non")) {
    if (String(firstThpSettings.sda) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (String(firstThpSettings.scl) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (sensorsSettings.secondThp) {
      if (String(secondThpSettings.sda) == value) {
        option.replace(F("{srslyValue}"), F("disabled>"));
      }
      if (String(secondThpSettings.scl) == value) {
        option.replace(F("{srslyValue}"), F("disabled>"));
      }
    }
  }

  if (value == srslyValue) {
    option.replace(F("{srslyValue}"), F(" selected>"));
  } else {
    option.replace(F("{srslyValue}"), F(">"));
  }
  option.replace(F("{label}"), label);
  return option;
}

static String _addBoolSelect(const String &key, const bool &value) {
  String selectValue = value ? "yes" : "no";
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace("{key}", key);
  input += _addOption("yes", (TEXT_YES), selectValue);
  input += _addOption("no", (TEXT_NO), selectValue);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}


static String _addTHP_MODELSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  input += _addOption(F("BME280"), F("BME280"), value);
  if (strcmp(pmSensorVersion, "PMS-SparkFunBME280")) {
    if (!strcmp(sensorsSettings.dustModel, "PMS7003") or !strcmp(sensorsSettings.dustModel, "Non")) {
      input += _addOption(F("BME280-SparkFun"), F("BME280-SparkFun"), value);
    }
  }
  input += _addOption(F("SHT1x"), F("SHT1x"), value);
  input += _addOption(F("HTU21"), F("SHT21/HTU21D"), value);
  input += _addOption(F("DHT22"), F("DHT22"), value);
  input += _addOption(F("BMP280"), F("BMP280"), value);

  input += _addOption(F("Non"), (TEXT_WITHOUTSENSOR), value);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

static String _addDUST_MODELSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  input += _addOption(F("PMS7003"), F("PMS5003/7003"), value);
  input += _addOption(F("SDS011/21"), F("SDS011/21"), value);
  input += _addOption(F("HPMA115S0"), F("HPMA115S0"), value);
  input += _addOption(F("SPS30"), F("SPS30"), value);

  input += _addOption(F("Non"), (TEXT_WITHOUTSENSOR), value);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

static String _add_FIRST_THP_SDA_SCL_Select(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  if (!strcmp(sensorsSettings.thpModel, "DS18B20")) {
#ifdef ARDUINO_ARCH_ESP8266
    input += _add_FIRST_THP_Option(F("D5"), F("D5/GPIO14"), value);
#elif defined ARDUINO_ARCH_ESP32
    input += _add_FIRST_THP_Option(F("D5"), F("D5/GPIO5"), value);
#endif
  } else {
#ifdef ARDUINO_ARCH_ESP8266
    input += _add_FIRST_THP_Option(F("D1"), F("D1/GPIO05"), value);
    input += _add_FIRST_THP_Option(F("D2"), F("D2/GPIO04"), value);
    input += _add_FIRST_THP_Option(F("D3"), F("D3/GPIO00"), value);
    input += _add_FIRST_THP_Option(F("D4"), F("D4/GPIO02"), value);
    input += _add_FIRST_THP_Option(F("D5"), F("D5/GPIO14"), value);
    input += _add_FIRST_THP_Option(F("D6"), F("D6/GPIO12"), value);
    input += _add_FIRST_THP_Option(F("D7"), F("D7/GPIO13"), value);
    // input += _addOption(F("D8"), F("D8/GPIO15"), value);
#elif defined ARDUINO_ARCH_ESP32
    input += _add_FIRST_THP_Option(F("D1"), F("D1/GPIO08"), value);
    input += _add_FIRST_THP_Option(F("D2"), F("D2/GPIO02"), value);
    input += _add_FIRST_THP_Option(F("D4"), F("D4/GPIO04"), value);
    input += _add_FIRST_THP_Option(F("D5"), F("D5/GPIO05"), value);
	
    input += _add_FIRST_THP_Option(F("D15"), F("D15/GPIO15"), value);
    input += _add_FIRST_THP_Option(F("D16"), F("D16/GPIO16"), value);
    input += _add_FIRST_THP_Option(F("D17"), F("D17/GPIO17"), value);
    input += _add_FIRST_THP_Option(F("D18"), F("D18/GPIO18"), value);
    input += _add_FIRST_THP_Option(F("D19"), F("D19/GPIO19"), value);
    input += _add_FIRST_THP_Option(F("D21"), F("D21/GPIO21"), value);
    input += _add_FIRST_THP_Option(F("D22"), F("D22/GPIO22"), value);
    input += _add_FIRST_THP_Option(F("D23"), F("D23/GPIO23"), value);
#endif
  }
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

#ifdef ARDUINO_ARCH_ESP32
static String _add_SECOND_THP_SDA_SCL_Select(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
#ifdef ARDUINO_ARCH_ESP8266
  input += _add_SECOND_THP_Option(F("D1"), F("D1/GPIO05"), value);
  input += _add_SECOND_THP_Option(F("D2"), F("D2/GPIO04"), value);
  input += _add_SECOND_THP_Option(F("D3"), F("D3/GPIO00"), value);
  input += _add_SECOND_THP_Option(F("D4"), F("D4/GPIO02"), value);
  input += _add_SECOND_THP_Option(F("D5"), F("D5/GPIO14"), value);
  input += _add_SECOND_THP_Option(F("D6"), F("D6/GPIO12"), value);
  input += _add_SECOND_THP_Option(F("D7"), F("D7/GPIO13"), value);
  // input += _addOption(F("D8"), F("D8/GPIO15"), value);
#elif defined ARDUINO_ARCH_ESP32
  input += _add_SECOND_THP_Option(F("D1"), F("D1/GPIO08"), value);
  input += _add_SECOND_THP_Option(F("D2"), F("D2/GPIO02"), value);
  input += _add_SECOND_THP_Option(F("D4"), F("D4/GPIO04"), value);
  input += _add_SECOND_THP_Option(F("D5"), F("D5/GPIO05"), value);

  input += _add_SECOND_THP_Option(F("D15"), F("D15/GPIO15"), value);
  input += _add_SECOND_THP_Option(F("D16"), F("D16/GPIO16"), value);
  input += _add_SECOND_THP_Option(F("D17"), F("D17/GPIO17"), value);
  input += _add_SECOND_THP_Option(F("D18"), F("D18/GPIO18"), value);
  input += _add_SECOND_THP_Option(F("D19"), F("D19/GPIO19"), value);
  input += _add_SECOND_THP_Option(F("D21"), F("D21/GPIO21"), value);
  input += _add_SECOND_THP_Option(F("D22"), F("D22/GPIO22"), value);
  input += _add_SECOND_THP_Option(F("D23"), F("D23/GPIO23"), value);
#endif
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}
#endif 

static String _add_DUST_TX_RX_Select(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  if (!strcmp(sensorsSettings.dustModel, "SPS30")) {
    input += _add_DUST_Option(F("D1"), F("D1/GPIO05"), value);
    input += _add_DUST_Option(F("D2"), F("D2/GPIO04"), value);
  } else {
#ifdef ARDUINO_ARCH_ESP8266
    input += _add_DUST_Option(F("D1"), F("D1/GPIO05"), value);
    input += _add_DUST_Option(F("D2"), F("D2/GPIO04"), value);
    input += _add_DUST_Option(F("D3"), F("D3/GPIO00"), value);
    input += _add_DUST_Option(F("D4"), F("D4/GPIO02"), value);
    input += _add_DUST_Option(F("D5"), F("D5/GPIO14"), value);
    input += _add_DUST_Option(F("D6"), F("D6/GPIO12"), value);
    input += _add_DUST_Option(F("D7"), F("D7/GPIO13"), value);
    // input += _addOption(F("D8"), F("D8/GPIO15"), value);
#elif defined ARDUINO_ARCH_ESP32
    // input += _add_DUST_Option(F("D1"), F("D1/GPIO08"), value);
    // input += _add_DUST_Option(F("D2"), F("D2/GPIO02"), value);
    input += _add_DUST_Option(F("D4"), F("D4/GPIO04"), value);
    input += _add_DUST_Option(F("D5"), F("D5/GPIO05"), value);
	
    input += _add_DUST_Option(F("D15"), F("D15/GPIO15"), value);
    input += _add_DUST_Option(F("D16"), F("D16/GPIO16"), value);
    input += _add_DUST_Option(F("D17"), F("D17/GPIO17"), value);
    input += _add_DUST_Option(F("D18"), F("D18/GPIO18"), value);
    input += _add_DUST_Option(F("D19"), F("D19/GPIO19"), value);
    input += _add_DUST_Option(F("D21"), F("D21/GPIO21"), value);
    input += _add_DUST_Option(F("D22"), F("D22/GPIO22"), value);
    input += _add_DUST_Option(F("D23"), F("D23/GPIO23"), value);
#endif
  }
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

static String _addINFLUXDB_VERSIONSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  input += _addOption(F("1"), F("1.x"), value);
  input += _addOption(F("2"), F("2.x"), value);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

static String _addLanguageSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  input += _addOption(F("polish"), (TEXT_INTL_PL), value);
  input += _addOption(F("english"), (TEXT_INTL_EN), value);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

static String _escapeString (const String &value) {
  String trimmed = value;
  trimmed.trim();
  trimmed.replace(F("'"), F("&#39;"));
  return trimmed;
}

static String _addTextInput(const String &key, const String &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_TEXTIMPUT);
  input.replace(F("{key}"), key);
  input.replace(F("{value}"), _escapeString(value));
  input.replace(F("{postfix}"), postfix);
  return input;
}

#ifdef ARDUINO_ARCH_ESP32
static String _addMQTTTextInput(const String &key, const String &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_MQTT_TEXTIMPUT);
  input.replace(F("{key}"), key);
  input.replace(F("{value}"), _escapeString(value));
  input.replace(F("{postfix}"), postfix);
  return input;
}
#endif

static String _addPasswdInput(const String &key, const String &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_PASSWDINPUT);
  input.replace(F("{key}"), key);
  input.replace(F("{value}"), _escapeString(value));
  input.replace(F("{postfix}"), postfix);
  return input;
}

static String _addIntInput(const String &key, const int &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_INTINPUT);
  input.replace(F("{key}"), key);
  input.replace(F("{value}"), String(value));
  input.replace(F("{postfix}"), postfix);
  return input;
}

static String _addFloatInput(const String &key, const double &value, const int &precision = 6, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_FLOATINPUT);
  input.replace(F("{key}"), key);
  input.replace(F("{value}"), String(value, precision));
  input.replace(F("{postfix}"), postfix);
  return input;
}

static String _addSubmitDevice() {
  String submitDevice = FPSTR(WEB_CONFIG_PAGE_SUBMIT_DEVICE_BUTTON);
  submitDevice.replace(F("{TEXT_SAVE}"), (TEXT_SAVE));
  return submitDevice;
}

static String _addSubmitServices() {
  String submitServices = FPSTR(WEB_CONFIG_PAGE_SUBMIT_SERVICES_BUTTON);
  submitServices.replace(F("{TEXT_SAVE}"), (TEXT_SAVE));
  return submitServices;
}

#ifdef ARDUINO_ARCH_ESP32
static String _addSubmitAdvMQTT() {
  String submitAdvMQTT = FPSTR(WEB_CONFIG_ADVANCED_MQTT_PAGE_SUBMIT_SERVICES_BUTTON);
  submitAdvMQTT.replace(F("{TEXT_SAVE}"), (TEXT_SAVE));
  return submitAdvMQTT;
}
#endif

static String _addWiFiErase() {
  String WiFiErase = FPSTR(WEB_CONFIG_PAGE_WIFIERASE);
  WiFiErase.replace(F("{TEXT_ERASEWIFICONFIG}"), (TEXT_ERASEWIFICONFIG));
  return WiFiErase;
}

static String _addRestoreConfig() {
  String RestoreConfig = FPSTR(WEB_CONFIG_PAGE_RESTORECONFIG);
  RestoreConfig.replace(F("{TEXT_RESTORESETTINGS}"), (TEXT_RESTORESETTINGS));
  return RestoreConfig;
}

static String _add_homekit_reset() {
  String homekit_reset = FPSTR(WEB_CONFIG_PAGE_HOMEKIT_RESET);
  homekit_reset.replace(F("{TEXT_PAGE_HOMEKIT_RESET}"), (TEXT_PAGE_HOMEKIT_RESET));
  return homekit_reset;
}

static String handle_config_processor(const String& var)
{
  // Serial.println(F("var: ") + var);
  String message;
  message = "";

  if (var == F("{Language}")) {
    message += String(TEXT_LANG);
  }

  if (var == F("{WEB_PAGE_CSS}")) {
    message += String(WEB_PAGE_HEADER_CSS);
  }

  if (var == F("{SMOGLY_LOGO_URL}")) {
    message += String(SMOGLY_LOGO_URL);
  }

  if (var == F("{CurrentPageTitle}")) {
    message += String(TEXT_CONFIG_PAGE);
  }
  if (var == F("{IndexPageTitle}")) {
    message += String(TEXT_INDEX_PAGE);
  }
  if (var == F("{ConfigPageTitle}")) {
    message += String(TEXT_CONFIG_PAGE);
  }
  if (var == F("{UpdatePageTitle}")) {
    message += String(TEXT_UPDATE_PAGE);
  }

  if (var == F("{TEXT_CONFIG_PAGE}")) {
    message += String(TEXT_CONFIG_PAGE);
  }

  if (var == F("{TEXT_CONFIG_INFO_DEVICE_PAGE}")) {
    message += String(TEXT_CONFIG_INFO_DEVICE_PAGE);
  }
  if (var == F("{TEXT_CONFIG_DEVICE_PAGE}")) {
    message += String(TEXT_CONFIG_DEVICE_PAGE);
  }

  if (var == F("{ConfigDeviceButton}")) {
    message += String(WEB_CONFIG_DEVICE_PAGE_BUTTON);
  }
  if (var == F("{ConfigServicesButton}")) {
    message += String(WEB_CONFIG_SERVICES_PAGE_BUTTON);
  }

  if (var == F("{TEXT_CONFIG_INFO_SERVICES_PAGE}")) {
    message += String(TEXT_CONFIG_INFO_SERVICES_PAGE);
  }
  if (var == F("{TEXT_CONFIG_SERVICES_PAGE}")) {
    message += String(TEXT_CONFIG_SERVICES_PAGE);
  }

  if (var == F("{WiFiEraseButton}")) {
    message += (_addWiFiErase());
  }
  if (var == F("{RestoreConfigButton}")) {
    message += (_addRestoreConfig());
  }

  return message;
  message = "";
}


static void handle_config(AsyncWebServerRequest *request) {
  if (deviceSettings.debug) {
    Serial.print(F("sizeof(WEB_CONFIG_PAGE_ALL): "));
    Serial.println(sizeof(WEB_CONFIG_PAGE_ALL)); // sizeof(WEB_CONFIG_PAGE_ALL): ~3282
    Serial.print(F("\n"));
  }

  request->send_P(200, "text/html", WEB_CONFIG_PAGE_ALL, handle_config_processor);
}

static String handle_config_device_processor(const String& var)
{
	// Serial.println(("var: ") + var);
  
  String message;
  message = "";

  if (var == F("{Language}")) {
    message += String(TEXT_LANG);
  }

  if (var == F("{WEB_PAGE_CSS}")) {
    message += String(WEB_PAGE_HEADER_CSS);
  }

  if (var == F("{SMOGLY_LOGO_URL}")) {
    message += String(SMOGLY_LOGO_URL);
  }

  if (var == F("{CurrentPageTitle}")) {
    message += String(TEXT_CONFIG_PAGE);
  }
  if (var == F("{IndexPageTitle}")) {
    message += String(TEXT_INDEX_PAGE);
  }
  if (var == F("{ConfigPageTitle}")) {
    message += String(TEXT_CONFIG_PAGE);
  }
  if (var == F("{UpdatePageTitle}")) {
    message += String(TEXT_UPDATE_PAGE);
  }

  if (var == F("{TEXT_CONFIG_DEVICE_PAGE}")) {
    message += String(TEXT_CONFIG_DEVICE_PAGE);
  }
  if (var == F("{TEXT_INSTRUCIONSLINK}")) {
    message += String(TEXT_INSTRUCIONSLINK);
    message.replace(F("{GITHUB_LINK}"), String(GITHUB_LINK));
    message.replace(F("{TEXT_HERE}"), String(TEXT_HERE));
  }

  if (var == F("{TEXT_DEVICENAME}")) {
    message += String(TEXT_DEVICENAME);
  }

  if (var == F("{device_name}")) {
    if (deviceSettings.autoName) {
      message += String(device_name);
    } else {
      message += (_addTextInput(F("DEVICENAME"), deviceSettings.name));
    }
  }

  if (var == F("{TEXT_DEVICENAMEAUTO}")) {
    message += String(TEXT_DEVICENAMEAUTO);
  }
  if (var == F("{DEVICENAME_AUTO}")) {
    message += (_addBoolSelect(F("DEVICENAME_AUTO"), deviceSettings.autoName));
  }
  if (var == F("{TEXT_SELECTEDLANGUAGE}")) {
    message += String(TEXT_SELECTEDLANGUAGE);
  }
  if (var == F("{LanguageSelect}")) {
    message += (_addLanguageSelect(F("LANGUAGE"), deviceSettings.language));
  }
  
  
  if (var == F("{TEXT_MY_COORDINATESINFO}")) {
    message += String(TEXT_MY_COORDINATESINFO);
    message.replace(F("{LATLONG_LINK}"), String(LATLONG_LINK));
    message.replace(F("{TEXT_HERE}"), String(TEXT_HERE));
  }
  if (var == F("{TEXT_MY_LATITUDE}")) {
    message += String(TEXT_MY_LATITUDE);
  }
  if (var == F("{LATITUDE}")) {
    message += (_addFloatInput(F("LATITUDE"), atof(deviceSettings.latitude), 6, F("°")));
  }
  if (var == F("{TEXT_MY_LONGITUDE}")) {
    message += String(TEXT_MY_LONGITUDE);
  }
  if (var == F("{LONGITUDE}")) {
    message += (_addFloatInput(F("LONGITUDE"), atof(deviceSettings.longitude), 6, F("°")));
  }
  
  if (var == F("{TEXT_ALTITUDEINFO}")) {
    message += String(TEXT_ALTITUDEINFO);
    message.replace(F("{WSPOLRZEDNE_GPS_LINK}"), String(WSPOLRZEDNE_GPS_LINK));
    message.replace(F("{TEXT_HERE}"), String(TEXT_HERE));
  }
  if (var == F("{MYALTITUDE}")) {
    message += (_addIntInput(F("MYALTITUDE"), deviceSettings.altitude, F("m.n.p.m")));
  }
  
  if (var == F("{TEXT_TEMPHUMIPRESSSENSOR}")) {
    message += String(TEXT_TEMPHUMIPRESSSENSOR);
  }
  if (var == F("{THP_MODELSelect}")) {
    message += (_addTHP_MODELSelect(F("THP_MODEL"), sensorsSettings.thpModel));
  }
  if (var == F("{TEXT_PMSENSOR}")) {
    message += String(TEXT_PMSENSOR);
  }
  if (var == F("{DUST_MODELSelect}")) {
    message += (_addDUST_MODELSelect(F("DUST_MODEL"), sensorsSettings.dustModel));
  }

  if (var == F("{WEB_CONFIG_DEVICE_PAGE_FIRST_TH")) {
    if (strcmp(sensorsSettings.thpModel, "Non")) {
      message += String(WEB_CONFIG_DEVICE_PAGE_FIRST_THP_PINS);
    } else {
      message += "";
    }
  }

  if (var == F("{THP_MODEL}")) {
    message += String(sensorsSettings.thpModel);
  }
  if (var == F("{TEXT_FIRST_THP_SDA_SCL}")) {
    message += String(TEXT_FIRST_THP_SDA_SCL);
  }
  if (var == F("{TEXT_FIRST_THP_SDA}")) {
    message += String(TEXT_FIRST_THP_SDA);
  }
  if (var == F("{FIRST_THP_SDA}")) {
    message += (_add_FIRST_THP_SDA_SCL_Select(F("CONFIG_FIRST_THP_SDA"), firstThpSettings.sda));
  }
  if (var == F("{TEXT_FIRST_THP_SCL}")) {
    if (!strcmp(sensorsSettings.thpModel, "DHT22") or !strcmp(sensorsSettings.thpModel, "DS18B20")) {
      message += ("");
    } else {
      message += String(TEXT_FIRST_THP_SCL);
    }
  }
  if (var == F("{FIRST_THP_SCL}")) {
    if (!strcmp(sensorsSettings.thpModel, "DHT22") or !strcmp(sensorsSettings.thpModel, "DS18B20")) {
      message += ("");
    } else {
      message += (_add_FIRST_THP_SDA_SCL_Select(F("CONFIG_FIRST_THP_SCL"), firstThpSettings.scl));
    }
  }

  // SECOND_THP CONFIG
  /*
  #ifdef ARDUINO_ARCH_ESP32
  if (var == F("{WEB_CONFIG_DEVICE_PAGE_SECOND_T")) {
    if (strcmp(sensorsSettings.thpModel, "Non")) {
      message += String(WEB_CONFIG_DEVICE_PAGE_SECOND_THP_PINS);
    } else {
      message += ("");
    }
  }
  if (var == F("{TEXT_SECOND_THP}")) {
    message += String(TEXT_SECOND_THP);
  }
  if (var == F("{SECOND_THP}")) {
    message += (_addBoolSelect("SECOND_THP", sensorsSettings.secondThp));
  }
  
  if (sensorsSettings.secondThp) {	
    if (var == F("{SECOND_THP_PINS_CONFIG}")) {
      message += String(WEB_CONFIG_DEVICE_PAGE_SECOND_THP_PINS_CONFIG);
    }
    if (var == F("{TEXT_SECOND_THP_SDA_SCL}")) {
      message += String(TEXT_SECOND_THP_SDA_SCL);
    }
    if (var == F("{TEXT_SECOND_THP_SDA}")) {
      message += String(TEXT_SECOND_THP_SDA);
    }
    if (var == F("{SECOND_THP_SDA}")) {
      message += (_add_SECOND_THP_SDA_SCL_Select(F("CONFIG_SECOND_THP_SDA"), secondThpSettings.sda));
    }
    if (var == F("{TEXT_SECOND_THP_SCL}")) {
      message += String(TEXT_SECOND_THP_SCL);
    }
    if (var == F("{SECOND_THP_SCL}")) {
      message += (_add_SECOND_THP_SDA_SCL_Select(F("CONFIG_SECOND_THP_SCL"), secondThpSettings.scl));
    }	
  }
  #endif
  */

/*
  #ifdef ARDUINO_ARCH_ESP8266
  if (var == F("{WEB_CONFIG_DEVICE_PAGE_SECOND_T")) {
      message += ("");
  }
  if (var == F("{TEXT_SECOND_THP}")) {
      message += ("");
  }
  if (var == F("{SECOND_THP}")) {
      message += ("");
  }
  if (sensorsSettings.secondThp) {	
    if (var == F("{SECOND_THP_PINS_CONFIG}")) {
      message += ("");
    }
    if (var == F("{TEXT_SECOND_THP_SDA_SCL}")) {
        message += ("");
    }
    if (var == F("{TEXT_SECOND_THP_SDA}")) {
        message += ("");
    }
    if (var == F("{SECOND_THP_SDA}")) {
        message += ("");
    }
    if (var == F("{TEXT_SECOND_THP_SCL}")) {
        message += ("");
    }
    if (var == F("{SECOND_THP_SCL}")) {
        message += ("");
    }	
  }
  #endif
*/

  if (var == F("{WEB_CONFIG_DEVICE_PAGE_DUST_PIN")) {
	 if (strcmp(sensorsSettings.dustModel, "Non")) {
      message += String(WEB_CONFIG_DEVICE_PAGE_DUST_PINS);
    } else {
      message += "";
    }
  }

  if (var == F("{DUST_MODEL}")) {
    message += String(sensorsSettings.dustModel);
  }
  if (var == F("{TEXT_DUST_TX_RX}")) {
    message += String(TEXT_DUST_TX_RX);
  }
  if (var == F("{TEXT_DUST_TX}")) {
    message += String(TEXT_DUST_TX);
  }
  if (var == F("{DUST_TX}")) {
    message += (_add_DUST_TX_RX_Select(F("CONFIG_DUST_TX"), dustSettings.tx));
  }
  if (var == F("{TEXT_DUST_RX}")) {
    message += String(TEXT_DUST_RX);
  }
  if (var == F("{DUST_RX}")) {
    message += (_add_DUST_TX_RX_Select(F("CONFIG_DUST_RX"), dustSettings.rx));
  }

  if (var == F("{TEXT_FREQUENTMEASUREMENTONOFF}")) {
    message += String(TEXT_FREQUENTMEASUREMENTONOFF);
  }
  if (var == F("{FREQUENTMEASUREMENT_Select}")) {
    message += (_addBoolSelect(F("FREQUENTMEASUREMENT"), sensorsSettings.continuousMeasurement));
  }
  if (var == F("{TEXT_FREQUENTMEASUREMENTINFO}")) {
    message += String(TEXT_FREQUENTMEASUREMENTINFO);
  }
  if (var == F("{TEXT_MEASUREMENTFREQUENCY}")) {
    message += String(TEXT_MEASUREMENTFREQUENCY);
  }
  if (var == F("{FREQUENTMEASUREMENT_time}")) {
    if (sensorsSettings.continuousMeasurement == true) {
      message += (_addIntInput(F("DUST_TIME"), sensorsSettings.dustTime, F("{TEXT_SECONDS}")));
      message.replace(F("{TEXT_SECONDS}"), String(TEXT_SECONDS));
    } else {
      message += (_addIntInput(F("DUST_TIME"), sensorsSettings.dustTime, F("{TEXT_MINUTES}")));
      message.replace(F("{TEXT_MINUTES}"), String(TEXT_MINUTES));
    }
  }

  if (var == F("{TEXT_AVERAGELASTRESULT}")) {
    message += String(TEXT_AVERAGELASTRESULT);
  }
  if (var == F("{NUMBEROFMEASUREMENTS}")) {
    message += (_addIntInput(F("NUMBEROFMEASUREMENTS"), sensorsSettings.numerOfMeasurements, F("{TEXT_PMMEASUREMENTS}")));
    message.replace(F("{TEXT_PMMEASUREMENTS}"), String(TEXT_PMMEASUREMENTS));
  }

  if (var == F("{TEXT_DEEPSLEEPINFO}")) {
#ifdef ARDUINO_ARCH_ESP8266
    message += String(TEXT_DEEPSLEEPINFO);
#elif defined ARDUINO_ARCH_ESP32
    message += String(TEXT_DEEPSLEEPINFO_ESP32);
#endif
    message.replace(F("{INTERFACEWWWONTIME}"), String(int(sensorsSettings.numerOfMeasurements) * 2 + 10 ));
    message.replace(F("{SENDING_FREQUENCY}"), String(deviceSettings.sendingServicesFrequency));
  }
  if (var == F("{DEEPSLEEP_ON}")) {
    message += (_addBoolSelect(F("DEEPSLEEP_ON"), deviceSettings.deepSleep));
  }

  if (!strcmp(sensorsSettings.dustModel, "PMS7003") or !strcmp(sensorsSettings.dustModel, "SPS30")) {
    if (var == F("{WEB_CONFIG_DEVICE_SHOWING_PM1}")) {
        message += String(WEB_CONFIG_DEVICE_SHOWING_PM1);
    }
    if (var == F("{TEXT_DISPLAYPM1}")) {
      message += String(TEXT_DISPLAYPM1);
    }
    if (var == F("{DISPLAY_PM1}")) {
      message += (_addBoolSelect(F("DISPLAY_PM1"), deviceSettings.displayPM1));
      message.replace(F("{TEXT_DISPLAYPM1}"), String(TEXT_DISPLAYPM1));
    }
  } else {
    message += ("");
  }

  if (var == F("{TEXT_SECURECONFIGUPDATEPAGE}")) {
    message += String(TEXT_SECURECONFIGUPDATEPAGE);
  }
  if (var == F("{CONFIG_AUTH}")) {
    message += (_addBoolSelect(F("CONFIG_AUTH"), authSettings.enabled));
  }
  if (var == F("{TEXT_SECURELOGIN}")) {
    message += String(TEXT_SECURELOGIN);
  }
  if (var == F("{CONFIG_USERNAME}")) {
    message += (_addTextInput(F("CONFIG_USERNAME"), authSettings.username));
  }
  if (var == F("{TEXT_SECUREPASSWD}")) {
    message += String(TEXT_SECUREPASSWD);
  }
  if (var == F("{CONFIG_PASSWORD}")) {
    message += (_addPasswdInput(F("CONFIG_PASSWORD"), authSettings.password));
  }

  if (var == F("{TEXT_SECURELOGOUTINFO}")) {
    if (!authSettings.enabled) {
      message += ("");
    } else {
      message += String(TEXT_SECURELOGOUTINFO);
    }
  }

  if (var == F("{DEBUG}")) {
    message += (_addBoolSelect(F("DEBUG"), deviceSettings.debug));
  }

  if (strcmp(sensorsSettings.thpModel, "Non")) {
    if (var == F("{TEXT_CALIBMETHOD}")) {
      message += String(TEXT_CALIBMETHOD);
    }
    if (var == F("{CalibrationModelSelect}")) {
      message += (_addBoolSelect(F("MODEL"), deviceSettings.autoCalibration));
    }
    if (var == F("{TEXT_CALIB1}")) {
      message += String(TEXT_CALIB1);
    }
    if (var == F("{calib1}")) {
      message += (String(deviceSettings.calib1));
    }
    if (var == F("{TEXT_CALIB2}")) {
      message += String(TEXT_CALIB2);
    }
    if (var == F("{calib2}")) {
      message += (String(deviceSettings.calib2));
    }
  } else {
    deviceSettings.autoCalibration = false;
    if (var == F("{TEXT_CALIBMETHOD}")) {
      message += ("</b>");
      message += ("<!-- ");
      message += ("");
    }
    if (var == F("{CalibrationModelSelect}")) {
      message += ("");
    }
    if (var == F("{TEXT_CALIB1}")) {
      message += ("");
    }
    if (var == F("{calib1}")) {
      message += ("");
      message += (" -->");
    }
  }

  if (var == F("{TEXT_SOFTWATEVERSION}")) {
    message += String(TEXT_SOFTWATEVERSION);
  }
  if (var == F("{SOFTWAREVERSION}")) {
    message += String(softwareVersion);
  }
  if (var == F("{TEXT_AUTOUPDATEON}")) {
    message += String(TEXT_AUTOUPDATEON);
  }
  
  if (var == F("{AUTOUPDATEON}")) {
    message += (_addBoolSelect(F("AUTOUPDATE_ON"), (deviceSettings.autoUpdate)));
  }
  
#ifdef ARDUINO_ARCH_ESP8266
  if (var == F("{WEB_CONFIG_DEVICE_HOMEKIT}")) {
    message += ("");
  }
#elif defined ARDUINO_ARCH_ESP32
  if (var == F("{WEB_CONFIG_DEVICE_HOMEKIT}")) {
	  message += ("");
    // message += String(WEB_CONFIG_DEVICE_HOMEKIT);
  }
  /*
  if (var == F("{TEXT_HOMEKIT_SUPPORT}")) {
    message += String(TEXT_HOMEKIT_SUPPORT);
  }
  if (var == F("{HOMEKIT_SUPPORT_ON}")) {
    message += (_addBoolSelect(F("HOMEKIT_SUPPORT"), homeKitSettings.enabled));
  }
  
  if (homeKitSettings.enabled == true and String(homekit_is_paired()) == "1") {
  if (var == F("{WEB_CONFIG_DEVICE_HOMEKIT_RESET")) {
    message += String(WEB_CONFIG_DEVICE_HOMEKIT_RESET_PAIR);
  }
  if (var == F("{TEXT_HOMEKIT_IS_PAIRED}")) {
      // Serial.println(F("homekit_is_paired: ") + String(homekit_is_paired()));
        message += String(TEXT_HOMEKIT_IS_PAIRED);
  }
  if (var == F("{HOMEKIT_PAIRED_RESET}")) {
      // Serial.println(F("homekit_is_paired: ") + String(homekit_is_paired()));
        message += (_add_homekit_reset());
  }
  
}
*/
#endif

if (var == F("{TEXT_UPDATEWARNING}")) {
    message += "";
	/*
  message += (String(TEXT_AUTOUPDATEWARNING));
  message.replace(F("{TEXT_FWUPDATEBUTTON}"), String(TEXT_FWUPDATEBUTTON));
	*/
}

  if (var == F("{WiFiEraseButton}")) {
    message += (_addWiFiErase());
  }
  if (var == F("{RestoreConfigButton}")) {
    message += (_addRestoreConfig());
  }
  if (var == F("{SubmitButton}")) {
    message += (_addSubmitDevice());
  }

  return message;
  message = "";
}


static void handle_config_device(AsyncWebServerRequest *request) {
  if (authSettings.enabled == true) {
    if (!request->authenticate(authSettings.username, authSettings.password))
      return request->requestAuthentication();
  }

  if (deviceSettings.debug) {
    Serial.print(F("sizeof(WEB_CONFIG_DEVICE_PAGE_ALL): "));
    Serial.println(sizeof(WEB_CONFIG_DEVICE_PAGE_ALL)); // sizeof(WEB_CONFIG_DEVICE_PAGE_ALL): ~3897
    Serial.print(F("\n"));
  }

  request->send_P(200, "text/html", WEB_CONFIG_DEVICE_PAGE_ALL, handle_config_device_processor);
}

static String handle_config_services_processor(const String& var)
{
  // Serial.println(F("var: ") + var);
  String message;
  message = "";

  if (var == F("{Language}")) {
    message +=  String(TEXT_LANG);
  }

  if (var == F("{WEB_PAGE_CSS}")) {
    message += String(WEB_PAGE_HEADER_CSS);
  }

  if (var == F("{SMOGLY_LOGO_URL}")) {
    message += String(SMOGLY_LOGO_URL);
  }

  if (var == F("{CurrentPageTitle}")) {
    message += String(TEXT_CONFIG_PAGE);
  }
  if (var == F("{IndexPageTitle}")) {
    message += String(TEXT_INDEX_PAGE);
  }
  if (var == F("{ConfigPageTitle}")) {
    message += String(TEXT_CONFIG_PAGE);
  }
  if (var == F("{UpdatePageTitle}")) {
    message += String(TEXT_UPDATE_PAGE);
  }

  if (var == F("{TEXT_CONFIG_SERVICES_PAGE}")) {
    message += String(TEXT_CONFIG_SERVICES_PAGE);
  }

  if (var == F("{TEXT_INSTRUCIONSLINK}")) {
    message += String(TEXT_INSTRUCIONSLINK);
    message.replace(F("{GITHUB_LINK}"), String(GITHUB_LINK));
    message.replace(F("{TEXT_HERE}"), String(TEXT_HERE));
  }

  if (var == F("{TEXT_SENDINGINTERVAL}")) {
    message += String(TEXT_SENDINGINTERVAL);
  }
  if (var == F("{SENDING_FREQUENCY}")) {
    if (sensorsSettings.continuousMeasurement == true) {
      message += _addIntInput(F("SENDING_FREQUENCY"), deviceSettings.sendingServicesFrequency, F("{TEXT_SECONDS}"));
      message.replace(F("{TEXT_SECONDS}"), String(TEXT_SECONDS));
    } else {
      message += _addIntInput(F("SENDING_FREQUENCY"), deviceSettings.sendingServicesFrequency, F("{TEXT_MINUTES}"));
      message.replace(F("{TEXT_MINUTES}"), String(TEXT_MINUTES));
    }
  }

  if (var == F("{TEXT_DBSENDINGINTERVAL}")) {
    message += String(TEXT_DBSENDINGINTERVAL);
  }
  if (var == F("{SENDING_DB_FREQUENCY}")) {
    if (sensorsSettings.continuousMeasurement == true) {
      message += _addIntInput(F("SENDING_DB_FREQUENCY"), deviceSettings.sendingDBFrequency, F("{TEXT_SECONDS}"));
      message.replace(F("{TEXT_SECONDS}"), String(TEXT_SECONDS));
    } else {
      message += _addIntInput(F("SENDING_DB_FREQUENCY"), deviceSettings.sendingDBFrequency, F("{TEXT_MINUTES}"));
      message.replace(F("{TEXT_MINUTES}"), String(TEXT_MINUTES));
    }
  }

#ifndef DISABLE_SMOGLIST
  if (var == F("{TEXT_SMOGLISTSENDING}")) {
    message += String(TEXT_SMOGLISTSENDING);
    message.replace(F("{SMOGLIST_LINK}"), String(SMOGLIST_LINK));

  }

  if (var == F("{SMOGLIST_ON}")) {
    message += (_addBoolSelect(F("SMOGLIST_ON"), smoglistSettings.enabled));
  }
  if (var == F("{TEXT_SMOGLISTINFO}")) {
    message += String(TEXT_SMOGLISTINFO);
  }
  
  if (var == F("{SMOGLIST_COMMENT_BEGIN}")) {
    message += ("");
  }
  
  if (var == F("{SMOGLIST_COMMENT_END}")) {
    message += ("");
  }
#else
  // smoglistSettings.enabled = false;
  if (var == F("{SMOGLIST_COMMENT_BEGIN}")) {
    message += ("<!--");
  }
  
  if (var == F("{SMOGLIST_COMMENT_END}")) {
    message += ("-->");
  }
#endif

  if (var == F("{TEXT_LUFTDATENSENDING}")) {
    message += String(TEXT_LUFTDATENSENDING);
    message.replace(F("{LUFTDATEN_LINK}"), String(LUFTDATEN_LINK));
    message.replace(F("{LUFTDATENFORM_LINK}"), String(LUFTDATENFORM_LINK));
  }
  if (var == F("{LUFTDATEN_ON}")) {
    message += (_addBoolSelect(F("LUFTDATEN_ON"), luftdatenSettings.enabled));
  }

  if (var == F("{LUFTDATEN_ChipID}")) {
#ifdef ARDUINO_ARCH_ESP8266
    message += (F("smogomierz-") + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
    message += (("smogomierz-") + String((uint32_t)(ESP.getEfuseMac())));
#endif
  }

  if (var == F("{Sensors_Pins_html}")) {
    if (strcmp(sensorsSettings.thpModel, "Non")) {
      message += String(WEB_CONFIG_SERVICES_THP_SENSOR_PIN);
    } else {
      message += "";
    }
  }
  if (!strcmp(sensorsSettings.thpModel, "BME280")) {
    if (var == F("{THPSENSOR}")) {
      message += F("BME280");
    }
    if (var == F("{THPXPIN}")) {
      message += F("11");
    }
  } else if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
    if (var == F("{THPSENSOR}")) {
      message += F("BMP280");
    }
    if (var == F("{THPXPIN}")) {
      message += F("3");
    }
  } else if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
    if (var == F("{THPSENSOR}")) {
      message += F("HTU21");
    }
    if (var == F("{THPXPIN}")) {
      message += F("7");
    }
  } else if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
    if (var == F("{THPSENSOR}")) {
      message += F("DHT22");
    }
    if (var == F("{THPXPIN}")) {
      message += F("7");
    }
  } else if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
    if (var == F("{THPSENSOR}")) {
      message += F("SHT1x");
    }
    if (var == F("{THPXPIN}")) {
      message += F("12");
    }
  } else if (!strcmp(sensorsSettings.thpModel, "DS18B20")) {
    if (var == F("{THPSENSOR}")) {
      message += F("DS18B20");
    }
    if (var == F("{THPXPIN}")) {
      message += F("13");
    }
  }

  if (var == F("{Sensors_Pins_html}")) {
    if (strcmp(sensorsSettings.dustModel, "Non")) {
      message += String(WEB_CONFIG_SERVICES_DUST_MODEL_SENSOR_PIN);
    } else {
      message += "";
    }
  }
  if (!strcmp(sensorsSettings.dustModel, "PMS7003")) {
    if (var == F("{DUSTSENSOR}")) {
      message += F("PMS5003/7003");
    }
    if (var == F("{DUSTXPIN}")) {
      message += F("1");
    }
  } else if (!strcmp(sensorsSettings.dustModel, "SDS011/21")) {
    if (var == F("{DUSTSENSOR}")) {
      message += F("SDS011/21");
    }
    if (var == F("{DUSTXPIN}")) {
      message += F("1");
    }
  } else if (!strcmp(sensorsSettings.dustModel, "HPMA115S0")) {
    if (var == F("{DUSTSENSOR}")) {
      message += F("HPMA115S0");
    }
    if (var == F("{DUSTXPIN}")) {
      message += F("1");
    }
  } else if (!strcmp(sensorsSettings.dustModel, "SPS30")) {
    if (var == F("{DUSTSENSOR}")) {
      message += F("SPS30");
    }
    if (var == F("{DUSTXPIN}")) {
      message += F("1");
    }
  }

  if (var == F("{TEXT_LUFTDATEN_GRAPH_ON}")) {
    message += String(TEXT_LUFTDATEN_GRAPH_ON);
  }
  if (var == F("{LUFTDATEN_GRAPH_ON}")) {
      message += (_addBoolSelect(F("LUFTDATEN_GRAPH_ON"), luftdatenSettings.graph));
  }
  if (var == F("{TEXT_LUFTDATEN_GRAPH_APIID}")) {
    message += String(TEXT_LUFTDATEN_GRAPH_APIID);
    message.replace(F("{LUFTDATEN_APIID_LINK}"), String(LUFTDATEN_APIID_LINK));
    message.replace(F("{TEXT_HERE}"), String(TEXT_HERE));
  } 
  if (var == F("{LUFTDATEN_APIID}")) {
      message += (_addIntInput(F("LUFTDATEN_APIID"), luftdatenSettings.apiId));
  }
   
  if (var == F("{TEXT_AQIECOSENDING}")) {
    message += String(TEXT_AQIECOSENDING);
  }
  if (var == F("{AQI_ECO_ON}")) {
    message += (_addBoolSelect(F("AQI_ECO_ON"), aqiEcoSettings.enabled));
  }
  if (var == F("{TEXT_AQIECOSERVER}")) {
    message += String(TEXT_AQIECOSERVER);
  }
  if (var == F("{AQI_ECO_HOST}")) {
    message += (_addTextInput(F("AQI_ECO_HOST"), aqiEcoSettings.host));
  }
  if (var == F("{TEXT_AQIECOPATH}")) {
    message += String(TEXT_AQIECOPATH);
  }
  if (var == F("{AQI_ECO_PATH}")) {
    message += (_addTextInput(F("AQI_ECO_PATH"), aqiEcoSettings.path));
  }

#ifdef ARDUINO_ARCH_ESP8266
  if (var == F("{ESP_MODEL}")) {
    message += F("ESP8266");
  }
  if (var == F("{AQI_ECO_ChipID}")) {
    message += (String(ESP.getChipId()));
  }
#elif defined ARDUINO_ARCH_ESP32
  if (var == ("{ESP_MODEL}")) {
    message += ("ESP32");
  }
  if (var == ("{AQI_ECO_ChipID}")) {
    message += (String((uint32_t)(ESP.getEfuseMac())));
  }
#endif

#ifdef ARDUINO_ARCH_ESP32
  if (var == F("{TEXT_AIRMONITORSENDING}")) {
    message += String(TEXT_AIRMONITORSENDING);
    char PMSENSORMODEL[16];
    if (!strcmp(sensorsSettings.dustModel, "PMS7003") or !strcmp(sensorsSettings.dustModel, "Non")) {
      strcpy(PMSENSORMODEL, "PMS7003");
    } else if (!strcmp(sensorsSettings.dustModel, "SDS011/21")) {
      strcpy(PMSENSORMODEL, "SDS011");
    } else if (!strcmp(sensorsSettings.dustModel, "HPMA115S0")) {
      strcpy(PMSENSORMODEL, "HPMA115S0");
    } else if (!strcmp(sensorsSettings.dustModel, "SPS30")) {
      strcpy(PMSENSORMODEL, "SPS30");
    }
    message.replace(F("{PMSENSORMODEL}"), String(PMSENSORMODEL));
    message.replace(F("{AIRMONITOR_LINK}"), String(AIRMONITOR_LINK));
    message.replace(F("{AIRMONITORFORM_LINK}"), String(AIRMONITORFORM_LINK));
    message.replace(F("{TEXT_THEFORM}"), String(TEXT_THEFORM));
    message.replace(F("{TEXT_AIRMONITORCHARTS}"), String(TEXT_AIRMONITORCHARTS));
  }
  if (var == F("{AIRMONITOR_ON}")) {
    message += (_addBoolSelect(F("AIRMONITOR_ON"), airMonitorSettings.enabled));
  }
  if (var == F("{TEXT_AIRMONITORCHARTS}")) {
    message += String(TEXT_AIRMONITORCHARTS);
  }
  if (var == F("{AIRMONITOR_GRAPH_ON}")) {
    message += (_addBoolSelect(F("AIRMONITOR_GRAPH_ON"), airMonitorSettings.graph));
  }
  if (var == F("{TEXT_AIRMONITOR_API_KEY}")) {
    message += String(TEXT_AIRMONITOR_API_KEY);
  }
  if (var == F("{AIRMONITOR_API_KEY}")) {
    message += (_addTextInput(F("AIRMONITOR_API_KEY"), airMonitorSettings.apiKey));
  }
  
  if (var == F("{AIRMONITOR_COMMENT_BEGIN}")) {
    message += ("");
  }
  
  if (var == F("{AIRMONITOR_COMMENT_END}")) {
    message += ("");
  }
#else
  airMonitorSettings.enabled = false;
  airMonitorSettings.graph = false;
  // airMonitorSettings.apiKey = "";
  memset(airMonitorSettings.apiKey, 0, sizeof(airMonitorSettings.apiKey));
  if (var == F("{AIRMONITOR_COMMENT_BEGIN}")) {
    message += ("<!--");
  }
  
  if (var == F("{AIRMONITOR_COMMENT_END}")) {
    message += ("-->");
  }
#endif 
  /*
  if (var == F("{TEXT_MY_COORDINATESINFO}")) {
    message += String(TEXT_MY_COORDINATESINFO);
    message.replace(F("{LATLONG_LINK}"), String(LATLONG_LINK));
    message.replace(F("{TEXT_HERE}"), String(TEXT_HERE));
  }
  if (var == F("{TEXT_MY_LATITUDE}")) {
    message += String(TEXT_MY_LATITUDE);
  }
  if (var == F("{LATITUDE}")) {
    message += (_addFloatInput(F("LATITUDE"), atof(deviceSettings.latitude), 6, F("°")));
  }
  if (var == F("{TEXT_MY_LONGITUDE}")) {
    message += String(TEXT_MY_LONGITUDE);
  }
  if (var == F("{LONGITUDE}")) {
    message += (_addFloatInput(F("LONGITUDE"), atof(deviceSettings.longitude), 6, F("°")));
  }
*/
  if (var == F("{TEXT_THINGSPEAKSENDING}")) {
    message += String(TEXT_THINGSPEAKSENDING);
    message.replace(F("{THINGSPEAK_LINK}"), String(THINGSPEAK_LINK));
  }
  if (var == F("{THINGSPEAK_ON}")) {
    message += (_addBoolSelect(F("THINGSPEAK_ON"), thingSpeakSettings.enabled));
  }
  if (var == F("{TEXT_THINGSPEAKCHARTS}")) {
    message += String(TEXT_THINGSPEAKCHARTS);
  }
  if (var == F("{THINGSPEAK_GRAPH_ON}")) {
    message += (_addBoolSelect(F("THINGSPEAK_GRAPH_ON"), thingSpeakSettings.graph));
  }
  if (var == F("{TEXT_THINGSPEAK_WRITE_API_KEY}")) {
    message += String(TEXT_THINGSPEAK_WRITE_API_KEY);
  }
  if (var == F("{THINGSPEAK_API_KEY}")) {
    message += (_addTextInput(F("THINGSPEAK_API_KEY"), thingSpeakSettings.apiKey));
  }
  if (var == F("{TEXT_THINGSPEAKCHANNELID}")) {
    message += String(TEXT_THINGSPEAKCHANNELID);
  }
  if (var == F("{THINGSPEAK_CHANNEL_ID}")) {
    message += (_addIntInput(F("THINGSPEAK_CHANNEL_ID"), thingSpeakSettings.channelId));
  }
  if (var == F("{TEXT_THINGSPEAK_READ_API_KEY}")) {
    message += String(TEXT_THINGSPEAK_READ_API_KEY);
  }
  if (var == F("{THINGSPEAK_READ_API_KEY}")) {
    message += (_addTextInput(F("THINGSPEAK_READ_API_KEY"), thingSpeakSettings.readApiKey));
  }

  if (var == F("{TEXT_INFLUXDBSENDING}")) {
    message += String(TEXT_INFLUXDBSENDING);
  }
  if (var == F("{INFLUXDB_ON}")) {
    message += (_addBoolSelect(F("INFLUXDB_ON"), influxDBSettings.enabled));
  }
  if (var == F("{TEXT_INFLUXDBVERSION}")) {
    message += String(TEXT_INFLUXDBVERSION);
  }
  if (var == F("{INFLUXDB_VERSION}")) {
    message += (_addINFLUXDB_VERSIONSelect(F("INFLUXDB_VERSION"), influxDBSettings.version));
  }
  if (var == F("{TEXT_INFLUXDBSERVER}")) {
    message += String(TEXT_INFLUXDBSERVER);
  }
  if (var == F("{INFLUXDB_HOST}")) {
    message += (_addTextInput(F("INFLUXDB_HOST"), influxDBSettings.host));
  }
  if (var == F("{TEXT_INFLUXDBPORT}")) {
    message += String(TEXT_INFLUXDBPORT);
  }
  if (var == F("{INFLUXDB_PORT}")) {
    message += (_addIntInput(F("INFLUXDB_PORT"), influxDBSettings.port));
  }
  if (var == F("{TEXT_INFLUXDBNAME}")) {
    message += String(TEXT_INFLUXDBNAME);
  }
  if (var == F("{INFLUXDB_DATABASE}")) {
    message += ( _addTextInput(F("INFLUXDB_DATABASE"), influxDBSettings.database));
  }
  if (var == F("{TEXT_INFLUXDBUSER}")) {
    message += String(TEXT_INFLUXDBUSER);
  }
  if (var == F("{INFLUXDB_USER}")) {
    message += (_addTextInput(F("INFLUXDB_USER"), influxDBSettings.user));
  }
  if (var == F("{TEXT_INFLUXDBPASSWD}")) {
    message += String(TEXT_INFLUXDBPASSWD);
  }
  if (var == F("{INFLUXDB_PASSWORD}")) {
    message += (_addPasswdInput(F("INFLUXDB_PASSWORD"), influxDBSettings.password));
  }
  
  // SAVE / RESET / RESTORE SECTION - START
  #ifdef ARDUINO_ARCH_ESP32
    if (var == F("{AdvancedMQTTConfigButton}")) {
      message += String(ASW_WEB_GOTO_CONFIG_ADVANCED_MQTT_PAGE_BUTTON);
      message.replace(F("{TEXT_CONFIG_ADV_MQTT}"), String(TEXT_CONFIG_ADV_MQTT));
    }
  #else
    if (var == F("{AdvancedMQTTConfigButton}")) {
      message += ("");
    }
  #endif

  if (var == F("{WiFiEraseButton}")) {
    message += (_addWiFiErase());
  }

  if (var == F("{RestoreConfigButton}")) {
    message += (_addRestoreConfig());
  }

  if (var == F("{SubmitButton}")) {
    message += F("<input type='submit' name='submit2' class='btn btn-outline-danger' value='{TEXT_SAVE}' />");
    message.replace(F("{TEXT_SAVE}"), String(TEXT_SAVE));
  }
  // SAVE / RESET / RESTORE SECTION - END
  
  if (var == F("{INFLUXDB_V2}")) {	  
    if (!strcmp(influxDBSettings.version, "2")) {
	  message += String(WEB_CONFIG_SERVICES_INFLUXDB_VERSION_V2);
      message.replace(F("{TEXT_INFLUXDBORG}"), String(TEXT_INFLUXDBORG));
      message.replace(F("{INFLUXDB_ORG}"), _addTextInput(F("INFLUXDB_ORG"), influxDBSettings.org));
      message.replace(F("{TEXT_INFLUXDBBUCKET}"), String(TEXT_INFLUXDBBUCKET));
      message.replace(F("{INFLUXDB_BUCKET}"), _addTextInput(F("INFLUXDB_BUCKET"), influxDBSettings.bucket));
      message.replace(F("{TEXT_INFLUXDBTOKEN}"), String(TEXT_INFLUXDBTOKEN));
      message.replace(F("{INFLUXDB_TOKEN}"), _addTextInput(F("INFLUXDB_TOKEN"), influxDBSettings.token));
    } else {
	  message += "";
    }	
  }
    
  /*
    #ifdef ARDUINO_ARCH_ESP32
    if (var == F("{TEXT_MQTTSENDING}")) {
     message += (TEXT_MQTTSENDING);
    }
    if (var == F("{MQTT_ON}")) {
     message += (_addBoolSelect(F("MQTT_ON"), mqttSettings.enabled));
    }
    if (var == F("{TEXT_MQTTSERVER}")) {
    message += (TEXT_MQTTSERVER);
    }
    if (var == F("{MQTT_HOST}")) {
    message += (_addTextInput(F("MQTT_HOST"), mqttSettings.host));
    }
    if (var == F("{TEXT_MQTTPORT}")) {
    message += (TEXT_MQTTPORT);
    }
    if (var == F("{MQTT_PORT}")) {
    message += (_addIntInput(F("MQTT_PORT"), mqttSettings.port));
    }
    if (var == F("{TEXT_MQTTUSER}")) {
    message += (TEXT_MQTTUSER);
    }
    if (var == F("{MQTT_USER}")) {
    message += (_addTextInput(F("MQTT_USER"), mqttSettings.user));
    }
    if (var == F("{TEXT_MQTTPASSWD}")) {
    message += (TEXT_MQTTPASSWD);
    }
    if (var == F("{MQTT_PASSWORD}")) {
    message += (_addPasswdInput(F("MQTT_PASSWORD"), mqttSettings.password));
    }
  */
  /*
    if (var == F("{TEXT_MQTT_TOPIC_INFO}")) {
    message += (TEXT_MQTT_TOPIC_INFO);
    }
    if (var == F("{TEXT_MQTT_IP_IN_TOPIC}")) {
    message += (TEXT_MQTT_IP_IN_TOPIC);
    }
    if (var == F("{MQTT_IP_IN_TOPIC}")) {
    message += (_addBoolSelect(F("MQTT_IP_IN_TOPIC"), mqttSettings.ipInTopic));
    }
    if (var == F("{TEXT_MQTT_DEVICENAME_IN_TOPIC}")) {
    message += (TEXT_MQTT_DEVICENAME_IN_TOPIC);
    }
    if (var == F("{MQTT_DEVICENAME_IN_TOPIC}")) {
    message += (_addBoolSelect(F("MQTT_DEVICENAME_IN_TOPIC"), mqttSettings.deviceNameInTopic));
    }
    if (var == F("{TEXT_MQTT_SLASH_AT_THE_BEGINNIN")) {
    message += (TEXT_MQTT_SLASH_AT_THE_BEGINNING);
    }
    if (var == F("{MQTT_SLASH_AT_THE_BEGINNING}")) {
    message += (_addBoolSelect(F("MQTT_SLASH_AT_THE_BEGINNING"), mqttSettings.slashAtBeginning));
    }
    if (var == F("{TEXT_MQTT_SLASH_AT_THE_END}")) {
    message += (TEXT_MQTT_SLASH_AT_THE_END);
    }
    if (var == F("{MQTT_SLASH_AT_THE_END}")) {
    message += (_addBoolSelect(F("MQTT_SLASH_AT_THE_END"), mqttSettings.slashAtEnd));
    }
  */
  /*
    if (var == F("{TEXT_TEMP_TOPIC}")) {
    message += (TEXT_TEMPERATURE);
    }
    if (var == F("{MQTT_IP}")) {
    message += (String(WiFi.localIP().toString()));
    }
    if (var == F("{MQTT_DEVICENAME}")) {
    message += (String(device_name));
    }
    if (var == F("{MQTT_TOPIC_TEMP}")) {
    message += (_addMQTTTextInput(F("MQTT_TOPIC_TEMP"), mqttSettings.tempTopic));
    }
    if (var == F("{MQTT_TEMP}")) {
    message += (String(int(measurementsData.temperature)));
    }
  #endif
  */

  return message;
  message = "";
}


static void handle_config_services(AsyncWebServerRequest *request) {
  if (authSettings.enabled == true) {
    if (!request->authenticate(authSettings.username, authSettings.password))
      return request->requestAuthentication();
  }

  if (deviceSettings.debug) {
    Serial.print(F("sizeof(WEB_CONFIG_SERVICES_PAGE_ALL): "));
    Serial.println(sizeof(WEB_CONFIG_SERVICES_PAGE_ALL)); // sizeof(WEB_CONFIG_SERVICES_PAGE_ALL): 4178
    Serial.print(F("\n"));
  }

  request->send_P(200, "text/html", WEB_CONFIG_SERVICES_PAGE_ALL, handle_config_services_processor);
}

#ifdef ARDUINO_ARCH_ESP32
static String handle_adv_mqtt_config_processor(const String& var)
{
  // Serial.println(F("var: ") + var);
  String message;
  message = "";

  if (var == F("{Language}")) {
    message +=  String(TEXT_LANG);
  }

  if (var == F("{WEB_PAGE_CSS}")) {
    message += String(WEB_PAGE_HEADER_CSS);
  }

  if (var == F("{SMOGLY_LOGO_URL}")) {
    message += String(SMOGLY_LOGO_URL);
  }

  if (var == F("{CurrentPageTitle}")) {
    message += String(TEXT_CONFIG_PAGE);
  }
  if (var == F("{IndexPageTitle}")) {
    message += String(TEXT_INDEX_PAGE);
  }
  if (var == F("{ConfigPageTitle}")) {
    message += String(TEXT_CONFIG_PAGE);
  }
  if (var == F("{UpdatePageTitle}")) {
    message += String(TEXT_UPDATE_PAGE);
  }

  if (var == F("{TEXT_ADVANCED_MQTT_PAGE}")) {
    message += String(TEXT_CONFIG_ADV_MQTT);
  }
  if (var == F("{TEXT_INSTRUCIONSLINK}")) {
    message += String(TEXT_INSTRUCIONSLINK);
    message.replace(F("{GITHUB_LINK}"), String(GITHUB_LINK));
    message.replace(F("{TEXT_HERE}"), String(TEXT_HERE));
  }

  if (var == F("{TEXT_MQTTSENDING}")) {
    message += String(TEXT_MQTTSENDING);
  }
  if (var == F("{MQTT_ON}")) {
    message += (_addBoolSelect(F("MQTT_ON"), mqttSettings.enabled));
  }
  if (var == F("{TEXT_MQTTSERVER}")) {
    message += String(TEXT_MQTTSERVER);
  }
  if (var == F("{MQTT_HOST}")) {
    message += (_addTextInput(F("MQTT_HOST"), mqttSettings.host));
  }
  if (var == F("{TEXT_MQTTPORT}")) {
    message += String(TEXT_MQTTPORT);
  }
  if (var == F("{MQTT_PORT}")) {
    message += (_addIntInput(F("MQTT_PORT"), mqttSettings.port));
  }
  if (var == F("{TEXT_MQTTUSER}")) {
    message += String(TEXT_MQTTUSER);
  }
  if (var == F("{MQTT_USER}")) {
    message += (_addTextInput(F("MQTT_USER"), mqttSettings.user));
  }
  if (var == F("{TEXT_MQTTPASSWD}")) {
    message += String(TEXT_MQTTPASSWD);
  }
  if (var == F("{MQTT_PASSWORD}")) {
    message += (_addPasswdInput(F("MQTT_PASSWORD"), mqttSettings.password));
  }

  if (var == F("{TEXT_MQTT_TOPIC_INFO}")) {
    message += String(TEXT_MQTT_TOPIC_INFO);
  }
  if (var == F("{TEXT_MQTT_IP_IN_TOPIC}")) {
    message += String(TEXT_MQTT_IP_IN_TOPIC);
  }
  if (var == F("{MQTT_IP_IN_TOPIC}")) {
    message += (_addBoolSelect(F("MQTT_IP_IN_TOPIC"), mqttSettings.ipInTopic));
  }
  if (var == F("{TEXT_MQTT_DEVICENAME_IN_TOPIC}")) {
    message += String(TEXT_MQTT_DEVICENAME_IN_TOPIC);
  }
  if (var == F("{MQTT_DEVICENAME_IN_TOPIC}")) {
    message += (_addBoolSelect(F("MQTT_DEVICENAME_IN_TOPIC"), mqttSettings.deviceNameInTopic));
  }
  if (var == F("{TEXT_MQTT_SLASH_AT_THE_BEGINNIN")) {
    message += String(TEXT_MQTT_SLASH_AT_THE_BEGINNING);
  }
  if (var == F("{MQTT_SLASH_AT_THE_BEGINNING}")) {
    message += (_addBoolSelect(F("MQTT_SLASH_AT_THE_BEGINNING"), mqttSettings.slashAtBeginning));
  }
  if (var == F("{TEXT_MQTT_SLASH_AT_THE_END}")) {
    message += String(TEXT_MQTT_SLASH_AT_THE_END);
  }
  if (var == F("{MQTT_SLASH_AT_THE_END}")) {
    message += (_addBoolSelect(F("MQTT_SLASH_AT_THE_END"), mqttSettings.slashAtEnd));
  }

 if(mqttSettings.enabled){
  if (var == F("{WEB_CONFIG_ADV_MQTT_PAGE_CONFIG}%")) {
	  /*
    if (!mqttSettings.slashAtBeginning && mqttSettings.slashAtEnd) {
      message += String(WEB_CONFIG_ADV_MQTT_PAGE_CONFIG1);
    } else if (!mqttSettings.slashAtEnd && mqttSettings.slashAtBeginning) {
      message += String(WEB_CONFIG_ADV_MQTT_PAGE_CONFIG2);
    } else if (!mqttSettings.slashAtEnd && !mqttSettings.slashAtBeginning) {
      message += String(WEB_CONFIG_ADV_MQTT_PAGE_CONFIG3);
    } else {
      message += String(WEB_CONFIG_ADV_MQTT_PAGE_CONFIG);
    }
	  */
  }
}

  if (strcmp(sensorsSettings.thpModel, "Non")) {
    // takeTHPMeasurements();
    if (var == F("{MQTT_TEMP}")) {
      message += (String(int(measurementsData.temperature)));
    }
    if (var == F("{MQTT_TOPIC_TEMP}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_TEMP"), mqttSettings.tempTopic));
    }

    if (var == F("{MQTT_HUMI}")) {
      message += (String(int(measurementsData.humidity)));
    }
    if (var == F("{MQTT_TOPIC_HUMI}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_HUMI"), mqttSettings.humiTopic));
    }
    if (var == F("{MQTT_PRESS}")) {
      message += (String(int(measurementsData.pressure)));
    }
    if (var == F("{MQTT_TOPIC_PRESS}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_PRESS"), mqttSettings.pressTopic));
    }
  }

  if (strcmp(sensorsSettings.dustModel, "Non")) {
    if (var == F("{MQTT_PM1}")) {
      message += (String(int(measurementsData.averagePM1)));
    }
    if (var == F("{MQTT_TOPIC_PM1}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_PM1"), mqttSettings.pm1Topic));
    }
    if (var == F("{MQTT_PM25}")) {
      message += (String(int(measurementsData.averagePM25)));
    }
    if (var == F("{MQTT_TOPIC_PM25}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_PM25"), mqttSettings.pm25Topic));
    }
    if (var == F("{MQTT_PM10}")) {
      message += (String(int(measurementsData.averagePM10)));
    }
    if (var == F("{MQTT_TOPIC_PM10}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_PM10"), mqttSettings.pm10Topic));
    }

    if (var == F("{MQTT_AIRQUALITY}")) {
      if (measurementsData.averagePM25 <= 10) {
        message += F("EXCELLENT");
      } else if (measurementsData.averagePM25 > 10 && measurementsData.averagePM25 <= 20) {
        message += F("GOOD");
      } else if (measurementsData.averagePM25 > 20 && measurementsData.averagePM25 <= 25) {
        message += F("FAIR");
      } else if (measurementsData.averagePM25 > 25 && measurementsData.averagePM25 <= 50) {
        message += F("INFERIOR");
      } else if (measurementsData.averagePM25 > 50) {
        message += F("POOR");
      } else {
        message += F("UNKNOWN");
      }
    }
    if (var == F("{MQTT_TOPIC_AIRQUALITY}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_AIRQUALITY"), mqttSettings.airQualityTopic));
    }

  }

  if (var == F("{TEXT_TEMP_TOPIC}")) {
    message += String(TEXT_TEMPERATURE);
  }
  if (var == F("{TEXT_HUMI_TOPIC}")) {
    message += String(TEXT_HUMIDITY);
  }
  if (var == F("{TEXT_PRESS_TOPIC}")) {
    message += String(TEXT_PRESSURE);
  }
  if (var == F("{TEXT_PM1_TOPIC}")) {
    message += F("PM1");
  }
  if (var == F("{TEXT_PM25_TOPIC}")) {
    message += F("PM2.5");
  }
  if (var == F("{TEXT_PM10_TOPIC}")) {
    message += F("PM10");
  }
  if (var == F("{TEXT_AIRQUALITY_TOPIC}")) {
    message += String(TEXT_AIRQUALITY_TOPIC);
  }

  if (var == F("{MQTT_DEVICENAME_IN_TOPIC}")) {
    if (mqttSettings.deviceNameInTopic) {
      message += ((String(device_name) + F("/")));
    } else {
      message += F("");
    }
  }
  if (var == F("{MQTT_IP_IN_TOPIC}")) {
    if (mqttSettings.ipInTopic) {
      message += ((String(WiFi.localIP().toString()) + "/"));
    } else {
      message += F("");
    }
  }

  if (var == F("{RestoreConfigButton}")) {
    message += (_addRestoreConfig());
  }

  if (var == F("{SubmitButton}")) {
    message += F("<input type='submit' name='submit3' class='btn btn-outline-danger' value='{TEXT_SAVE}' />");
    message.replace(F("{TEXT_SAVE}"), String(TEXT_SAVE));
  }
  
  return message;
  message = "";
}
#endif

#ifdef ARDUINO_ARCH_ESP32
static void handle_adv_mqtt_config(AsyncWebServerRequest *request) {
  if (authSettings.enabled == true) {
    if (!request->authenticate(authSettings.username, authSettings.password))
      return request->requestAuthentication();
  }

  if (deviceSettings.debug) {
    Serial.print(F("sizeof(WEB_CONFIG_ADV_MQTT_PAGE_ALL): "));
    Serial.println(sizeof(WEB_CONFIG_ADV_MQTT_PAGE_ALL)); // sizeof(WEB_CONFIG_ADV_MQTT_PAGE_ALL): 2998
    Serial.print(F("\n"));
  }
  /*
    request->send_P(200, "text/html", WEB_CONFIG_ADV_MQTT_PAGE_ALL, handle_adv_mqtt_config_processor);
  }
  */
  String message;
  message = "";

  message += FPSTR(WEB_PAGE_HEADER);
  message.replace(F("{WEB_PAGE_CSS}"), FPSTR(WEB_PAGE_HEADER_CSS));
  message.replace(F("{Language}"), (TEXT_LANG));
  message.replace(F("{CurrentPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{IndexPageTitle}"), (TEXT_INDEX_PAGE));
  message.replace(F("{ConfigPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{UpdatePageTitle}"), (TEXT_UPDATE_PAGE));

  message += FPSTR(WEB_CONFIG_ADVANCED_MQTT_PAGE_TOP);
  message.replace(F("{TEXT_ADVANCED_MQTT_PAGE}"), (TEXT_CONFIG_ADV_MQTT));

  message.replace(F("{WEB_CONFIG_TOP_PAGE_INFO}"), "");

  message.replace(F("{TEXT_INSTRUCIONSLINK}"), (TEXT_INSTRUCIONSLINK));
  message.replace(F("{GITHUB_LINK}"), String(GITHUB_LINK));
  message.replace(F("{TEXT_HERE}"), (TEXT_HERE));

  message += FPSTR(WEB_CONFIG_ADVANCED_MQTT_PAGE_CONFIG);

  message.replace(F("{TEXT_MQTTSENDING}"), (TEXT_MQTTSENDING));
  message.replace(F("{MQTT_ON}"), _addBoolSelect("MQTT_ON", mqttSettings.enabled));
  message.replace(F("{TEXT_MQTTSERVER}"), (TEXT_MQTTSERVER));
  message.replace(F("{MQTT_HOST}"), _addTextInput("MQTT_HOST", mqttSettings.host));
  message.replace(F("{TEXT_MQTTPORT}"), (TEXT_MQTTPORT));
  message.replace(F("{MQTT_PORT}"), _addIntInput("MQTT_PORT", mqttSettings.port));
  message.replace(F("{TEXT_MQTTUSER}"), (TEXT_MQTTUSER));
  message.replace(F("{MQTT_USER}"), _addTextInput("MQTT_USER", mqttSettings.user));
  message.replace(F("{TEXT_MQTTPASSWD}"), (TEXT_MQTTPASSWD));
  message.replace(F("{MQTT_PASSWORD}"), _addPasswdInput("MQTT_PASSWORD", mqttSettings.password));

  message.replace(F("{TEXT_MQTT_TOPIC_INFO}"), (TEXT_MQTT_TOPIC_INFO));

  message.replace(F("{TEXT_MQTT_IP_IN_TOPIC}"), (TEXT_MQTT_IP_IN_TOPIC));
  message.replace(F("{MQTT_IP_IN_TOPIC}"), _addBoolSelect("MQTT_IP_IN_TOPIC", mqttSettings.ipInTopic));

  message.replace(F("{TEXT_MQTT_DEVICENAME_IN_TOPIC}"), (TEXT_MQTT_DEVICENAME_IN_TOPIC));
  message.replace(F("{MQTT_DEVICENAME_IN_TOPIC}"), _addBoolSelect("MQTT_DEVICENAME_IN_TOPIC", mqttSettings.deviceNameInTopic));

  message.replace(F("{TEXT_MQTT_SLASH_AT_THE_BEGINNING}"), (TEXT_MQTT_SLASH_AT_THE_BEGINNING));
  message.replace(F("{MQTT_SLASH_AT_THE_BEGINNING}"), _addBoolSelect("MQTT_SLASH_AT_THE_BEGINNING", mqttSettings.slashAtBeginning));

  message.replace(F("{TEXT_MQTT_SLASH_AT_THE_END}"), (TEXT_MQTT_SLASH_AT_THE_END));
  message.replace(F("{MQTT_SLASH_AT_THE_END}"), _addBoolSelect("MQTT_SLASH_AT_THE_END", mqttSettings.slashAtEnd));

  if (!mqttSettings.slashAtBeginning) {
    message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"));
    message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"));
    message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"));
    message.replace(F("<b>{TEXT_PM1_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM1}/{MQTT_PM1}<br />"), F("<b>{TEXT_PM1_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM1}/{MQTT_PM1}<br />"));
    message.replace(F("<b>{TEXT_PM25_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM25}/{MQTT_PM25}<br />"), F("<b>{TEXT_PM25_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM25}/{MQTT_PM25}<br />"));
    message.replace(F("<b>{TEXT_PM10_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM10}/{MQTT_PM10}<br />"), F("<b>{TEXT_PM10_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM10}/{MQTT_PM10}<br />"));
    message.replace(F("<b>{TEXT_AIRQUALITY_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_AIRQUALITY}/{MQTT_AIRQUALITY}<br />"), F("<b>{TEXT_AIRQUALITY_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_AIRQUALITY}/{MQTT_AIRQUALITY}<br />"));
  }

  if (!mqttSettings.slashAtEnd) {
    message.replace(F("{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), F("{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"));
    message.replace(F("{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), F("{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"));
    message.replace(F("{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), F("{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"));
    message.replace(F("{MQTT_TOPIC_PM1}/{MQTT_PM1}<br />"), F("{MQTT_TOPIC_PM1}={MQTT_PM1}<br />"));
    message.replace(F("{MQTT_TOPIC_PM25}/{MQTT_PM25}<br />"), F("{MQTT_TOPIC_PM25}={MQTT_PM25}<br />"));
    message.replace(F("{MQTT_TOPIC_PM10}/{MQTT_PM10}<br />"), F("{MQTT_TOPIC_PM10}={MQTT_PM10}<br />"));
    message.replace(F("{MQTT_TOPIC_AIRQUALITY}/{MQTT_AIRQUALITY}<br />"), F("{MQTT_TOPIC_AIRQUALITY}={MQTT_AIRQUALITY}<br />"));
  }

  if (strcmp(sensorsSettings.thpModel, "Non")) {
    takeTHPMeasurements();
    if (!strcmp(sensorsSettings.thpModel, "BME280")) {
      if (checkBmeStatus() == true) {
        message.replace(F("{MQTT_TEMP}"), String(int(measurementsData.temperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", mqttSettings.tempTopic));

        message.replace(F("{MQTT_HUMI}"), String(int(measurementsData.humidity)));
        message.replace(F("{MQTT_TOPIC_HUMI}"), _addMQTTTextInput("MQTT_TOPIC_HUMI", mqttSettings.humiTopic));

        message.replace(F("{MQTT_PRESS}"), String(int(measurementsData.pressure)));
        message.replace(F("{MQTT_TOPIC_PRESS}"), _addMQTTTextInput("MQTT_TOPIC_PRESS", mqttSettings.pressTopic));
      } else {
        if (deviceSettings.debug) {
          Serial.println(F("No measurements from BME280!\n"));
        }
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      }
    }

    if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
      if (checkBmpStatus() == true) {
        message.replace(F("{MQTT_TEMP}"), String(int(measurementsData.temperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", mqttSettings.tempTopic));

        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");

        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");

        message.replace(F("{MQTT_PRESS}"), String(int(measurementsData.pressure)));
        message.replace(F("{MQTT_TOPIC_PRESS}"), _addMQTTTextInput("MQTT_TOPIC_PRESS", mqttSettings.pressTopic));
      } else {
        if (deviceSettings.debug) {
          Serial.println(F("No measurements from BMP280!\n"));
        }
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      }
    }

    if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
      if (checkHTU21DStatus() == true) {
        message.replace(F("{MQTT_TEMP}"), String(int(measurementsData.temperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", mqttSettings.tempTopic));

        message.replace(F("{MQTT_HUMI}"), String(int(measurementsData.humidity)));
        message.replace(F("{MQTT_TOPIC_HUMI}"), _addMQTTTextInput("MQTT_TOPIC_HUMI", mqttSettings.humiTopic));

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      } else {
        if (deviceSettings.debug) {
          Serial.println(F("No measurements from HTU21!\n"));
        }
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      }
    }

    if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
      if (checkDHT22Status() == true) {
        message.replace(F("{MQTT_TEMP}"), String(int(measurementsData.temperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", mqttSettings.tempTopic));

        message.replace(F("{MQTT_HUMI}"), String(int(measurementsData.humidity)));
        message.replace(F("{MQTT_TOPIC_HUMI}"), _addMQTTTextInput("MQTT_TOPIC_HUMI", mqttSettings.humiTopic));

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      } else {
        if (deviceSettings.debug) {
          Serial.println(F("No measurements from DHT22!\n"));
        }
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      }
    }

    if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
      if (checkDHT22Status() == true) {
        message.replace(F("{MQTT_TEMP}"), String(int(measurementsData.temperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", mqttSettings.tempTopic));

        message.replace(F("{MQTT_HUMI}"), String(int(measurementsData.humidity)));
        message.replace(F("{MQTT_TOPIC_HUMI}"), _addMQTTTextInput("MQTT_TOPIC_HUMI", mqttSettings.humiTopic));

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      } else {
        if (deviceSettings.debug) {
          Serial.println(F("No measurements from SHT1x!\n"));
        }
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      }
    }
    if (!strcmp(sensorsSettings.thpModel, "DS18B20")) {
      if (checkDS18B20Status()) {

        message.replace(F("{MQTT_TEMP}"), String(int(measurementsData.temperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", mqttSettings.tempTopic));

        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      } else {
        if (deviceSettings.debug) {
          Serial.println(F("No measurements from DS18B20!\n"));
        }
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      }
    }
  } else {
    message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
    message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
    message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
    message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), "");
    message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
    message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

    message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
    message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
    message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
    message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"), "");
    message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
    message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
  }

  if (strcmp(sensorsSettings.dustModel, "Non")) {
    message.replace(F("{MQTT_PM1}"), String(int(measurementsData.averagePM1)));
    message.replace(F("{MQTT_TOPIC_PM1}"), _addMQTTTextInput("MQTT_TOPIC_PM1", mqttSettings.pm1Topic));
    message.replace(F("{MQTT_PM25}"), String(int(measurementsData.averagePM25)));
    message.replace(F("{MQTT_TOPIC_PM25}"), _addMQTTTextInput("MQTT_TOPIC_PM25", mqttSettings.pm25Topic));
    message.replace(F("{MQTT_PM10}"), String(int(measurementsData.averagePM10)));
    message.replace(F("{MQTT_TOPIC_PM10}"), _addMQTTTextInput("MQTT_TOPIC_PM10", mqttSettings.pm10Topic));

    if (measurementsData.averagePM25 <= 10) {
      message.replace(F("{MQTT_AIRQUALITY}"), "EXCELLENT");
    } else if (measurementsData.averagePM25 > 10 && measurementsData.averagePM25 <= 20) {
      message.replace(F("{MQTT_AIRQUALITY}"), "GOOD");
    } else if (measurementsData.averagePM25 > 20 && measurementsData.averagePM25 <= 25) {
      message.replace(F("{MQTT_AIRQUALITY}"), "FAIR");
    } else if (measurementsData.averagePM25 > 25 && measurementsData.averagePM25 <= 50) {
      message.replace(F("{MQTT_AIRQUALITY}"), "INFERIOR");
    } else if (measurementsData.averagePM25 > 50) {
      message.replace(F("{MQTT_AIRQUALITY}"), "POOR");
    } else {
      message.replace(F("{MQTT_AIRQUALITY}"), "UNKNOWN");
    }
    message.replace(F("{MQTT_TOPIC_AIRQUALITY}"), _addMQTTTextInput("MQTT_TOPIC_AIRQUALITY", mqttSettings.airQualityTopic));
  } else {
    message.replace(F("<b>{TEXT_PM1_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM1}/{MQTT_PM1}<br />"), "");
    message.replace(F("<b>{TEXT_PM25_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM25}/{MQTT_PM25}<br />"), "");
    message.replace(F("<b>{TEXT_PM10_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM10}/{MQTT_PM10}<br />"), "");
    message.replace(F("<b>{TEXT_AIRQUALITY_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_AIRQUALITY}/{MQTT_AIRQUALITY}<br />"), "");
    message.replace(F("<b>{TEXT_PM1_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM1}/{MQTT_PM1}<br />"), "");
    message.replace(F("<b>{TEXT_PM25_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM25}/{MQTT_PM25}<br />"), "");
    message.replace(F("<b>{TEXT_PM10_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM10}/{MQTT_PM10}<br />"), "");
    message.replace(F("<b>{TEXT_AIRQUALITY_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_AIRQUALITY}/{MQTT_AIRQUALITY}<br />"), "");

    message.replace(F("<b>{TEXT_PM1_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM1}={MQTT_PM1}<br />"), "");
    message.replace(F("<b>{TEXT_PM25_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM25}={MQTT_PM25}<br />"), "");
    message.replace(F("<b>{TEXT_PM10_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM10}={MQTT_PM10}<br />"), "");
    message.replace(F("<b>{TEXT_AIRQUALITY_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_AIRQUALITY}={MQTT_AIRQUALITY}<br />"), "");
    message.replace(F("<b>{TEXT_PM1_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM1}={MQTT_PM1}<br />"), "");
    message.replace(F("<b>{TEXT_PM25_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM25}={MQTT_PM25}<br />"), "");
    message.replace(F("<b>{TEXT_PM10_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM10}={MQTT_PM10}<br />"), "");
    message.replace(F("<b>{TEXT_AIRQUALITY_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_AIRQUALITY}={MQTT_AIRQUALITY}<br />"), "");
  }

  message.replace(F("{TEXT_TEMP_TOPIC}"), (TEXT_TEMPERATURE));
  message.replace(F("{TEXT_HUMI_TOPIC}"), (TEXT_HUMIDITY));
  message.replace(F("{TEXT_PRESS_TOPIC}"), (TEXT_PRESSURE));
  message.replace(F("{TEXT_PM1_TOPIC}"), "PM1");
  message.replace(F("{TEXT_PM25_TOPIC}"), "PM2.5");
  message.replace(F("{TEXT_PM10_TOPIC}"), "PM10");
  message.replace(F("{TEXT_AIRQUALITY_TOPIC}"), (TEXT_AIRQUALITY_TOPIC));

  if (mqttSettings.deviceNameInTopic) {
    message.replace(F("{MQTT_DEVICENAME}"), (String(device_name) + "/"));
  } else {
    message.replace(F("{MQTT_DEVICENAME}"), "");
  }
  if (mqttSettings.ipInTopic) {
    message.replace(F("{MQTT_IP}"), (String(WiFi.localIP().toString()) + "/"));
  } else {
    message.replace(F("{MQTT_IP}"), "");
  }

  message.replace(F("{RestoreConfigButton}"), _addRestoreConfig());
  message.replace(F("{SubmitButton}"), _addSubmitAdvMQTT());
  message += FPSTR(WEB_PAGE_FOOTER);

  if (deviceSettings.debug) {
    Serial.print(F("handle_adv_mqtt_config - message.length(): ")); // 18754
    Serial.println(message.length()); // keep it under 20000!
    Serial.print(F("\n"));
  }
  request->send(200, "text/html", message);
}
#endif

static bool _parseAsBool(String value) {
  return value == "yes";
}

static void _set_language() {
  if (!strcmp(deviceSettings.language, "english")) {
    deviceSettings.selectedLanguage = 1;
  } else if (!strcmp(deviceSettings.language, "polish")) {
    deviceSettings.selectedLanguage = 2;
  } else {
    deviceSettings.selectedLanguage = 1;
  }
}

static void _parseAsCString(char* dest, String value, int CStringSize = 255) {
  strncpy(dest, value.c_str(), CStringSize);
}

void set_SERIAL_PINS(String DUST_PIN, int i) {
#ifdef ARDUINO_ARCH_ESP8266
  if (i == 1) {
    if (DUST_PIN == "D1") {
      dustSettings.address_tx = 5;
    } else if (DUST_PIN == "D2") {
      dustSettings.address_tx = 4;
    } else if (DUST_PIN == "D3") {
      dustSettings.address_tx = 0;
    } else if (DUST_PIN == "D4") {
      dustSettings.address_tx = 2;
    } else if (DUST_PIN == "D5") {
      dustSettings.address_tx = 14;
    } else if (DUST_PIN == "D6") {
      dustSettings.address_tx = 12;
    } else if (DUST_PIN == "D7") {
      dustSettings.address_tx = 13;
    } else if (DUST_PIN == "D8") {
      dustSettings.address_tx = 15;
    } else if (DUST_PIN == "D16") {
      dustSettings.address_tx = 16;
    } else if (DUST_PIN == "D17") {
      dustSettings.address_tx = 17;
    }
  } else if (i == 2) {
    if (DUST_PIN == "D1") {
      dustSettings.address_rx = 5;
    } else if (DUST_PIN == "D2") {
      dustSettings.address_rx = 4;
    } else if (DUST_PIN == "D3") {
      dustSettings.address_rx = 0;
    } else if (DUST_PIN == "D4") {
      dustSettings.address_rx = 2;
    } else if (DUST_PIN == "D5") {
      dustSettings.address_rx = 14;
    } else if (DUST_PIN == "D6") {
      dustSettings.address_rx = 12;
    } else if (DUST_PIN == "D7") {
      dustSettings.address_rx = 13;
    } else if (DUST_PIN == "D8") {
      dustSettings.address_rx = 15;
    } else if (DUST_PIN == "D16") {
      dustSettings.address_rx = 16;
    } else if (DUST_PIN == "D17") {
      dustSettings.address_rx = 17;
    }
  }
#elif defined ARDUINO_ARCH_ESP32
  if (i == 1) {
    if (DUST_PIN == "D1") {
      dustSettings.address_tx = 8;
    } else if (DUST_PIN == "D2") {
      dustSettings.address_tx = 9;
    } else if (DUST_PIN == "D4") {
      dustSettings.address_tx = 4;
    } else if (DUST_PIN == "D5") {
      dustSettings.address_tx = 5;
    } else if (DUST_PIN == "D15") {
      dustSettings.address_tx = 15;
    } else if (DUST_PIN == "D16") {
      dustSettings.address_tx = 16;
    } else if (DUST_PIN == "D17") {
      dustSettings.address_tx = 17;
    } else if (DUST_PIN == "D18") {
      dustSettings.address_tx = 18;
    } else if (DUST_PIN == "D19") {
      dustSettings.address_tx = 19;
    } else if (DUST_PIN == "D21") {
      dustSettings.address_tx = 21;
    } else if (DUST_PIN == "D22") {
      dustSettings.address_tx = 22;
    } else if (DUST_PIN == "D23") {
      dustSettings.address_tx = 23;
    }
  } else if (i == 2) {
    if (DUST_PIN == "D1") {
      dustSettings.address_rx = 8;
    } else if (DUST_PIN == "D2") {
      dustSettings.address_rx = 9;
    } else if (DUST_PIN == "D4") {
      dustSettings.address_rx = 4;
    } else if (DUST_PIN == "D5") {
      dustSettings.address_rx = 5;
    } else if (DUST_PIN == "D15") {
      dustSettings.address_rx = 15;
    } else if (DUST_PIN == "D16") {
      dustSettings.address_rx = 16;
    } else if (DUST_PIN == "D17") {
      dustSettings.address_rx = 17;
    } else if (DUST_PIN == "D18") {
      dustSettings.address_rx = 18;
    } else if (DUST_PIN == "D19") {
      dustSettings.address_rx = 19;
    } else if (DUST_PIN == "D21") {
      dustSettings.address_rx = 21;
    } else if (DUST_PIN == "D22") {
      dustSettings.address_rx = 22;
    } else if (DUST_PIN == "D23") {
      dustSettings.address_rx = 23;
    }
  }
#endif
}


static void handle_config_device_save(AsyncWebServerRequest *request) {
  bool reboot_required = false;
  unsigned char need_update = 0;
  // REMEMBER TO ADD/EDIT KEYS IN config.h AND spiffs.cpp!!
  /*
    int paramsNr = request->params();
    if (deviceSettings.debug) {
    for(int i=0;i<paramsNr;i++){

     AsyncWebParameter* p = request->getParam(i);

     Serial.print(("Param name: "));
     Serial.println(p->name());

     Serial.print(("Param value: "));
     Serial.println(p->value());

     Serial.println(("------"));
    }
    }
	*/

  if (request->hasParam("DEVICENAME_AUTO")) {
    deviceSettings.autoName = _parseAsBool(request->getParam("DEVICENAME_AUTO")->value());
  }

  if (request->hasParam("DEVICENAME")) {
    _parseAsCString(deviceSettings.name, request->getParam("DEVICENAME")->value(), 32);
  }

  if (request->hasParam("DISPLAY_PM1")) {
    deviceSettings.displayPM1 = _parseAsBool(request->getParam("DISPLAY_PM1")->value());
  }

  if (request->hasParam("LANGUAGE")) {
    _parseAsCString(deviceSettings.language, request->getParam("LANGUAGE")->value(), 12);
    _set_language();
  }
  
  if (request->hasParam("LATITUDE")) {
    _parseAsCString(deviceSettings.latitude, request->getParam("LATITUDE")->value(), 16);
  }

  if (request->hasParam("LONGITUDE")) {
    _parseAsCString(deviceSettings.longitude, request->getParam("LONGITUDE")->value(), 16);
  }
  
  if (request->hasParam("MYALTITUDE")) {
    deviceSettings.altitude = (request->getParam("MYALTITUDE")->value()).toInt();
  }

  char old_thpModel[32];
  strcpy(old_thpModel, sensorsSettings.thpModel);
  if (request->hasParam("THP_MODEL")) {
    _parseAsCString(sensorsSettings.thpModel, request->getParam("THP_MODEL")->value(), 12);
    if (strcmp(sensorsSettings.thpModel, old_thpModel)) {
      reboot_required = true;
    }
  }
  if (strcmp(sensorsSettings.thpModel, old_thpModel) and !strcmp(sensorsSettings.thpModel, "BME280-SparkFun")) {
    need_update = 1;
  }

  char old_dustModel[32];
  strcpy(old_dustModel, sensorsSettings.dustModel);
  if (request->hasParam("DUST_MODEL")) {
    _parseAsCString(sensorsSettings.dustModel, request->getParam("DUST_MODEL")->value(), 12);
    if (strcmp(sensorsSettings.dustModel, old_dustModel)) {
      reboot_required = true;
    }
  }

  // DUST Sensor config - START
  if (!strcmp(pmSensorVersion, "PMS")) {
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "SDS011/21")) {
      need_update = 2;
    }
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "HPMA115S0")) {
      need_update = 3;
    }
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "SPS30")) {
      need_update = 5;
    }
  } else if (!strcmp(pmSensorVersion, "SDS")) {
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "HPMA115S0")) {
      need_update = 3;
    }
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "PMS7003")) {
      need_update = 4;
    }
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "SPS30")) {
      need_update = 5;
    }
  } else if (!strcmp(pmSensorVersion, "HPMA115S0")) {
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "SDS011/21")) {
      need_update = 2;
    }
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "PMS7003")) {
      need_update = 4;
    }
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "SPS30")) {
      need_update = 5;
    }
  } else if (!strcmp(pmSensorVersion, "SPS30")) {
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "SDS011/21")) {
      need_update = 2;
    }
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "HPMA115S0")) {
      need_update = 3;
    }
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "PMS7003")) {
      need_update = 4;
    }
  } else if (!strcmp(pmSensorVersion, "PMS-SparkFunBME280")) {
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "SDS011/21")) {
      need_update = 2;
    }
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "HPMA115S0")) {
      need_update = 3;
    }
    if (strcmp(sensorsSettings.dustModel, old_dustModel) and !strcmp(sensorsSettings.dustModel, "SPS30")) {
      need_update = 5;
    }
  }
  // DUST Sensor config - END
  
  if (request->hasParam("CONFIG_FIRST_THP_SDA")) {
    char old_data[8];
    strcpy(old_data, firstThpSettings.sda);
    _parseAsCString(firstThpSettings.sda, request->getParam("CONFIG_FIRST_THP_SDA")->value(), 4);
    if (strcmp(firstThpSettings.sda, old_data)) {
      reboot_required = true;
    }
  }
  if (request->hasParam("CONFIG_FIRST_THP_SCL")) {
    char old_data[8];
    strcpy(old_data, firstThpSettings.scl);
    _parseAsCString(firstThpSettings.scl, request->getParam("CONFIG_FIRST_THP_SCL")->value(), 4);
    if (strcmp(firstThpSettings.scl, old_data)) {
      reboot_required = true;
    }
  }
  #ifdef ARDUINO_ARCH_ESP32
  if (request->hasParam("SECOND_THP")) {
    sensorsSettings.secondThp = _parseAsBool(request->getParam("SECOND_THP")->value());
  }
  
  if (request->hasParam("CONFIG_SECOND_THP_SDA")) {
    char old_data[8];
    strcpy(old_data, secondThpSettings.sda);
    _parseAsCString(secondThpSettings.sda, request->getParam("CONFIG_SECOND_THP_SDA")->value(), 4);
    if (strcmp(secondThpSettings.sda, old_data)) {
      reboot_required = true;
    }
  }
  if (request->hasParam("CONFIG_SECOND_THP_SCL")) {
    char old_data[8];
    strcpy(old_data, secondThpSettings.scl);
    _parseAsCString(secondThpSettings.scl, request->getParam("CONFIG_SECOND_THP_SCL")->value(), 4);
    if (strcmp(secondThpSettings.scl, old_data)) {
      reboot_required = true;
    }
  }
  #endif
  if (request->hasParam("CONFIG_DUST_TX")) {
    char old_data[8];
    strcpy(old_data, dustSettings.tx);
    _parseAsCString(dustSettings.tx, request->getParam("CONFIG_DUST_TX")->value(), 4);
	  set_SERIAL_PINS(dustSettings.tx, 1);
    if (strcmp(dustSettings.tx, old_data)) {
      reboot_required = true;
    }
  }
  
  if (request->hasParam("CONFIG_DUST_RX")) {
    char old_data[8];
    strcpy(old_data, dustSettings.rx);
    _parseAsCString(dustSettings.rx, request->getParam("CONFIG_DUST_RX")->value(), 4);
	  set_SERIAL_PINS(dustSettings.rx, 2);
    if (strcmp(dustSettings.rx, old_data)) {
      reboot_required = true;
    }
  }

  if (request->hasParam("FREQUENTMEASUREMENT")) {
    sensorsSettings.continuousMeasurement = _parseAsBool(request->getParam("FREQUENTMEASUREMENT")->value());
  }

  if (request->hasParam("DUST_TIME")) {
    sensorsSettings.dustTime = (request->getParam("DUST_TIME")->value()).toInt();
  }

  if (request->hasParam("NUMBEROFMEASUREMENTS")) {
    sensorsSettings.numerOfMeasurements = (request->getParam("NUMBEROFMEASUREMENTS")->value()).toInt();
  }

  if (request->hasParam("DEEPSLEEP_ON")) {
    if (sensorsSettings.continuousMeasurement == false) {
      deviceSettings.deepSleep = _parseAsBool(request->getParam("DEEPSLEEP_ON")->value());
    }
  }

  if (request->hasParam("CONFIG_AUTH")) {
    authSettings.enabled = _parseAsBool(request->getParam("CONFIG_AUTH")->value());
  }

  if (request->hasParam("CONFIG_USERNAME")) {
    _parseAsCString(authSettings.username, request->getParam("CONFIG_USERNAME")->value(), 64);
  }

  if (request->hasParam("CONFIG_PASSWORD")) {
    _parseAsCString(authSettings.password, request->getParam("CONFIG_PASSWORD")->value(), 128);
  }

  if (request->hasParam("DEBUG")) {
    deviceSettings.debug = _parseAsBool(request->getParam("DEBUG")->value());
  }

  if (request->hasParam("MODEL")) {
    deviceSettings.autoCalibration = _parseAsBool(request->getParam("MODEL")->value());
  }

  if (request->hasParam("AUTOUPDATE_ON")) {
    deviceSettings.autoUpdate = _parseAsBool(request->getParam("AUTOUPDATE_ON")->value());
  }
#ifdef ARDUINO_ARCH_ESP32
  if (request->hasParam("HOMEKIT_SUPPORT")) {
    homeKitSettings.enabled = _parseAsBool(request->getParam("HOMEKIT_SUPPORT")->value());
  }
#endif
  if (need_update != 0) {
    strcpy(sensorsSettings.thpModel, "Non");
    strcpy(sensorsSettings.dustModel, "Non");
    saveConfig();
    //_handle_config_device(true);
	  unsigned char x = 0;
    switch (need_update) {
      case 1:
          x = 1; // BME280-SparkFun
          break;
      case 2:
          x = 2; // SDS011
          break;
      case 3:
          x = 3; // HPMA115S0
          break;
      case 4:
          x = 4; // PMSx003
          break;
      case 5:
          x = 5; // SPS30
          break;
      default:
          x = 0; // CURRENT SERVERSOFTWARE VERSION
    }
	doUpdate(x);
  }

  if (deviceSettings.debug) {
    Serial.println(F("POST DEVICE CONFIG END!!"));
  }

  saveConfig();

  //_handle_config_device(true);
  // https://github.com/esp8266/Arduino/issues/1722
  // ESP.reset();
  delay(300);
  request->redirect("/");
  
  /*
    int redirect_interval = 3 * 1000; // 3 seconds
    unsigned long current_redirect_Millis = millis();
    unsigned long previous_redirect_Millis = millis();
    while (previous_redirect_Millis - current_redirect_Millis <= redirect_interval * 1) {
      previous_redirect_Millis = millis();
    }
  */
 if (reboot_required) {
  delay(1000);
  ESP.restart();
 }
}

static void handle_config_services_save(AsyncWebServerRequest *request) {
  // REMEMBER TO ADD/EDIT KEYS IN config.h AND spiffs.cpp!!
	/*
    int paramsNr = request->params();
    if (deviceSettings.debug) {
    for(int i=0;i<paramsNr;i++){

     AsyncWebParameter* p = request->getParam(i);

     Serial.print(("Param name: "));
     Serial.println(p->name());

     Serial.print(("Param value: "));
     Serial.println(p->value());

     Serial.println(("------"));
    }
    }
	*/

  if (request->hasParam("SENDING_FREQUENCY")) {
    deviceSettings.sendingServicesFrequency = (request->getParam("SENDING_FREQUENCY")->value()).toInt();
  }

  if (request->hasParam("SENDING_DB_FREQUENCY")) {
    deviceSettings.sendingDBFrequency = (request->getParam("SENDING_DB_FREQUENCY")->value()).toInt();
  }
  
#ifndef DISABLE_SMOGLIST
  if (request->hasParam("SMOGLIST_ON")) {
    smoglistSettings.enabled = _parseAsBool(request->getParam("SMOGLIST_ON")->value());
  }
#endif
  /*
  if (request->hasParam("LUFTDATEN_ON")) {
      luftdatenSettings.enabled = _parseAsBool(request->getParam("LUFTDATEN_ON")->value());
  }
*/
  if (request->hasParam("AQI_ECO_ON")) {
    aqiEcoSettings.enabled = _parseAsBool(request->getParam("AQI_ECO_ON")->value());
  }

  if (request->hasParam("AQI_ECO_HOST")) {
    _parseAsCString(aqiEcoSettings.host, request->getParam("AQI_ECO_HOST")->value(), 128);
  }

  if (request->hasParam("AQI_ECO_PATH")) {
    _parseAsCString(aqiEcoSettings.path, request->getParam("AQI_ECO_PATH")->value(), 64);
  }
#ifdef ARDUINO_ARCH_ESP32
  /*
  if (request->hasParam("AIRMONITOR_ON")) {	  
    airMonitorSettings.enabled = _parseAsBool(request->getParam("AIRMONITOR_ON")->value());
    Serial.println("AIRMONITOR_ON: " + String(airMonitorSettings.enabled));
  }
  // Serial.println("AIRMONITOR_ON: " + String(airMonitorSettings.enabled));

  if (request->hasParam("AIRMONITOR_GRAPH_ON")) {
    airMonitorSettings.graph = _parseAsBool(request->getParam("AIRMONITOR_GRAPH_ON")->value());
  }
  */
  if (request->hasParam("AIRMONITOR_API_KEY")) {
    _parseAsCString(airMonitorSettings.apiKey, request->getParam("AIRMONITOR_API_KEY")->value(), 64);
  }
#endif
  /*
  if (request->hasParam("LATITUDE")) {
    _parseAsCString(deviceSettings.latitude, request->getParam("LATITUDE")->value(), 16);
  }

  if (request->hasParam("LONGITUDE")) {
    _parseAsCString(deviceSettings.longitude, request->getParam("LONGITUDE")->value(), 16);
  }
  */
  if (request->hasParam("THINGSPEAK_ON")) {
    thingSpeakSettings.enabled = _parseAsBool(request->getParam("THINGSPEAK_ON")->value());
  }

  if (request->hasParam("THINGSPEAK_GRAPH_ON")) {
    thingSpeakSettings.graph = _parseAsBool(request->getParam("THINGSPEAK_GRAPH_ON")->value());
  }

  if (request->hasParam("THINGSPEAK_API_KEY")) {
    _parseAsCString(thingSpeakSettings.apiKey, request->getParam("THINGSPEAK_API_KEY")->value(), 32);
  }

  if (request->hasParam("THINGSPEAK_CHANNEL_ID")) {
    thingSpeakSettings.channelId = (request->getParam("THINGSPEAK_CHANNEL_ID")->value()).toInt();
  }

  if (request->hasParam("THINGSPEAK_READ_API_KEY")) {
    _parseAsCString(thingSpeakSettings.readApiKey, request->getParam("THINGSPEAK_READ_API_KEY")->value(), 32);
  }

  if (request->hasParam("INFLUXDB_ON")) {
    influxDBSettings.enabled = _parseAsBool(request->getParam("INFLUXDB_ON")->value());
  }

  if (request->hasParam("INFLUXDB_VERSION")) {
    _parseAsCString(influxDBSettings.version, request->getParam("INFLUXDB_VERSION")->value(), 16);
  }

  if (request->hasParam("INFLUXDB_HOST")) {
    _parseAsCString(influxDBSettings.host, request->getParam("INFLUXDB_HOST")->value(), 128);
  }

  if (request->hasParam("INFLUXDB_PORT")) {
    influxDBSettings.port = (request->getParam("INFLUXDB_PORT")->value()).toInt();
  }

  if (request->hasParam("INFLUXDB_DATABASE")) {
    _parseAsCString(influxDBSettings.database, request->getParam("INFLUXDB_DATABASE")->value(), 64);
  }

  if (request->hasParam("INFLUXDB_USER")) {
    _parseAsCString(influxDBSettings.user, request->getParam("INFLUXDB_USER")->value(), 64);
  }

  if (request->hasParam("INFLUXDB_PASSWORD")) {
    _parseAsCString(influxDBSettings.password, request->getParam("INFLUXDB_PASSWORD")->value(), 64);
  }

  if (request->hasParam("INFLUXDB_ORG")) {
    _parseAsCString(influxDBSettings.org, request->getParam("INFLUXDB_ORG")->value(), 64);
  }
  if (request->hasParam("INFLUXDB_BUCKET")) {
    _parseAsCString(influxDBSettings.bucket, request->getParam("INFLUXDB_BUCKET")->value(), 64);
  }
  if (request->hasParam("INFLUXDB_TOKEN")) {
    _parseAsCString(influxDBSettings.token, request->getParam("INFLUXDB_TOKEN")->value(), 128);
  }
#ifdef ARDUINO_ARCH_ESP32
  if (request->hasParam("AIRMONITOR_ON")) {
    airMonitorSettings.enabled = _parseAsBool(request->getParam("AIRMONITOR_ON")->value());
  }
  if (request->hasParam("AIRMONITOR_GRAPH_ON")) {
    airMonitorSettings.graph = _parseAsBool(request->getParam("AIRMONITOR_GRAPH_ON")->value());
  }
#endif
  
  if (request->hasParam("LUFTDATEN_ON")) {
      luftdatenSettings.enabled = _parseAsBool(request->getParam("LUFTDATEN_ON")->value());
  }
  
  if (request->hasParam("LUFTDATEN_GRAPH_ON")) {
    luftdatenSettings.graph = _parseAsBool(request->getParam("LUFTDATEN_GRAPH_ON")->value());
  }
  
  if (request->hasParam("LUFTDATEN_APIID")) {
    luftdatenSettings.apiId = (request->getParam("LUFTDATEN_APIID")->value()).toInt();
  }
  
  if (deviceSettings.debug) {
    Serial.println(F("POST SERVICES CONFIG END!!"));	
  }    
  
  delay(100);
  saveConfig();

  // _handle_config_services(true);
  // https://github.com/esp8266/Arduino/issues/1722
  delay(300);
  request->redirect("/");
  // delay(1000);
  // ESP.restart();
}

static void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
#ifdef ARDUINO_ARCH_ESP8266
  uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
#elif defined ARDUINO_ARCH_ESP32
  uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
#endif
  if (!index) {
    Serial.println(F("Start Updating..."));
#ifdef ARDUINO_ARCH_ESP8266
    Update.runAsync(true);
	#endif
    if (!Update.begin(free_space)) {
      Update.printError(Serial);
    }
  }

  if (Update.write(data, len) != len) {
    Update.printError(Serial);
  }

  if (final) {
    if (!Update.end(true)) {
      Update.printError(Serial);
    } else {
      Serial.println(F("Update complete"));
      Serial.println(F("Restart"));
      delay(1000);
      ESP.restart();
    }
  }
}

static void handle_update_done(AsyncWebServerRequest *request) {
  request->send(200);
}

static String handle_update_processor(const String& var)
{
  // Serial.println(F("var: ") + var);
  String message;
  message = "";

  if (var == F("{Language}")) {
    message +=  String(TEXT_LANG);
  }

  if (var == F("{WEB_PAGE_CSS}")) {
    message += String(WEB_PAGE_HEADER_CSS);
  }

  if (var == F("{SMOGLY_LOGO_URL}")) {
    message += String(SMOGLY_LOGO_URL);
  }

  if (var == F("{CurrentPageTitle}")) {
    message += String(TEXT_UPDATE_PAGE);
  }
  if (var == F("{IndexPageTitle}")) {
    message += String(TEXT_INDEX_PAGE);
  }
  if (var == F("{ConfigPageTitle}")) {
    message += String(TEXT_CONFIG_PAGE);
  }
  if (var == F("{UpdatePageTitle}")) {
    message += String(TEXT_UPDATE_PAGE);
  }

  if (var == F("{TEXT_UPDATE_PAGE}")) {
    message += String(TEXT_UPDATE_PAGE);
  }

  if (!deviceSettings.autoUpdate) {
    if (need_update) {
      if (var == F("{WEB_UPDATE_INFO_WARNING}")) {
        message += String(WEB_UPDATE_INFO_WARNING);
      }

      if (var == F("{TEXT_FWUPDATEAVALIBLE}")) {
        message += String(TEXT_FWUPDATEAVALIBLE);
      }
      if (var == F("{MANUALUPDATEBUTTON}")) {
        message += "";
      }

      if (var == F("{FWUPDATEBUTTON}")) {
        message += String(WEB_UPDATE_BUTTON_FWUPDATE);
      }
      if (var == F("{TEXT_FWUPDATEBUTTON}")) {
        message += String(TEXT_FWUPDATEBUTTON);
      }

      if (var == F("{AUTOUPDATEONBUTTON}")) {
        message += String(WEB_UPDATE_BUTTON_AUTOUPDATEON);
      }
      if (var == F("{TEXT_AUTOUPDATEONBUTTON}")) {
        message += String(TEXT_AUTOUPDATEONBUTTON);
      }

      if (var == F("{TEXT_AUTOUPDATEWARNING}")) {
        message += String(TEXT_AUTOUPDATEWARNING);
      }

      if (var == F("{TEXT_FWUPDATEBUTTON}")) {
        message += String(TEXT_FWUPDATEBUTTON);
      }

    }
    if (var == F("{WEB_UPDATE_INFO_WARNING}")) {
      message += "";
    }
  } else {
    if (var == F("{WEB_UPDATE_INFO_WARNING}")) {
      message += "";
    }
  }

  if (var == F("{TEXT_SUBMITUPDATE}")) {
    message += String(TEXT_SUBMITUPDATE);
  }

  if (var == F("{TEXT_AUTOUPDATEON}")) {
    message += String(TEXT_AUTOUPDATEON);
  }

  if (deviceSettings.autoUpdate) {
    if (var == F("{AUTOUPDATEONSTATUS}")) {
      message += String(TEXT_YES);
    }
  } else {
    if (var == F("{AUTOUPDATEONSTATUS}")) {
      message += String(TEXT_NO);
    }
  }

  if (var == F("{TEXT_CURRENTSOFTVERSION}")) {
    message += String(TEXT_CURRENTSOFTVERSION);
  }
  if (var == F("{SOFTWAREVERSION}")) {
    message += String(String(currentSoftwareVersion) + " " + String(pmSensorVersion));
  }

  if (var == F("{TEXT_SERVERSOFTWAREVERSION}")) {
    message += String(TEXT_SERVERSOFTWAREVERSION);
  }
  if (var == F("{SERVERSOFTWAREVERSION}")) {
    message += String(String(serverSoftwareVersion) + " " + String(pmSensorVersion));
  }

  if (var == F("{TEXT_LATESTAVAILABLESOFT}")) {
    message += String(TEXT_LATESTAVAILABLESOFT);
    message.replace(F("{SMOGOMIERZRELEASES_LINK}"), String(SMOGOMIERZRELEASES_LINK));
    message.replace(F("{TEXT_HERE}"), String(TEXT_HERE));
  }

  if (var == F("{TEXT_CONNECTEDWIFI}")) {
    message += String(TEXT_CONNECTEDWIFI);
  }
  if (var == F("{WiFiSSID}")) {
    message += String(WiFi.SSID());
  }

  if (var == F("{TEXT_WIFIRSSI}")) {
    message += String(TEXT_WIFIRSSI);
  }
  if (var == F("{WiFiRSSI}")) {
    message += String(String(int(WiFi.RSSI())) + F(" dBm"));
  }

  if (var == F("{TEXT_WIFIQUALITY}")) {
    message += String(TEXT_WIFIQUALITY);
  }
  if (var == "{WiFiQuality}") {
    short WiFiRSSI = WiFi.RSSI();
    // https://stackoverflow.com/a/15798024
    unsigned char WiFiQuality;
    if (WiFiRSSI <= -100) {
      WiFiQuality = 0;
    } else if (WiFiRSSI >= -50) {
      WiFiQuality = 100;
    } else {
      WiFiQuality = 2 * (WiFiRSSI + 100);
    }
    message += String(String(WiFiQuality) + F(" %"));
  }

  return message;
  message = "";
}

static void handle_update(AsyncWebServerRequest *request) {
  if (authSettings.enabled == true) {
    if (!request->authenticate(authSettings.username, authSettings.password))
      return request->requestAuthentication();
  }

  if (deviceSettings.debug) {
    Serial.print(F("sizeof(WEB_UPDATE_PAGE_ALL): "));
    Serial.println(sizeof(WEB_UPDATE_PAGE_ALL)); // sizeof(WEB_UPDATE_PAGE_ALL): 2558
    Serial.print(F("\n"));
  }

  request->send_P(200, "text/html", WEB_UPDATE_PAGE_ALL, handle_update_processor);
}

static void erase_wifi(AsyncWebServerRequest *request) {
  if (authSettings.enabled == true) {
    if (!request->authenticate(authSettings.username, authSettings.password))
      return request->requestAuthentication();
  }
  Serial.println(F("Erasing Config..."));
#ifdef ARDUINO_ARCH_ESP8266
  ESP.eraseConfig();
#elif defined ARDUINO_ARCH_ESP32
  WiFi.disconnect(false, true);
#endif

  request->redirect("/");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

static void restore_config(AsyncWebServerRequest *request) {
  if (authSettings.enabled == true) {
    if (!request->authenticate(authSettings.username, authSettings.password))
      return request->requestAuthentication();
  }
  Serial.println(F("Restoring default settings..."));
  deleteConfig();

  request->redirect("/");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

static void fwupdate(AsyncWebServerRequest *request) {
  if (authSettings.enabled == true) {
    if (!request->authenticate(authSettings.username, authSettings.password))
      return request->requestAuthentication();
  }
  unsigned char x = 0;
  doUpdate(x);
  delay(1000);

  request->redirect("/");
  delay(1000);
}

#ifdef ARDUINO_ARCH_ESP32
static void handle_adv_mqtt_config_save(AsyncWebServerRequest *request) {
  if (deviceSettings.debug) {
    Serial.println(F("handle_adv_mqtt_config_save!"));
  }

  if (request->hasParam("MQTT_ON")) {
    mqttSettings.enabled = _parseAsBool(request->getParam("MQTT_ON")->value());
  }

  if (request->hasParam("MQTT_HOST")) {
    _parseAsCString(mqttSettings.host, request->getParam("MQTT_HOST")->value(), 128);
  }

  if (request->hasParam("MQTT_PORT")) {
    mqttSettings.port = (request->getParam("MQTT_PORT")->value()).toInt();
  }

  if (request->hasParam("MQTT_USER")) {
    _parseAsCString(mqttSettings.user, request->getParam("MQTT_USER")->value(), 64);
  }

  if (request->hasParam("MQTT_PASSWORD")) {
    _parseAsCString(mqttSettings.password, request->getParam("MQTT_PASSWORD")->value(), 64);
  }

  if (request->hasParam("MQTT_IP_IN_TOPIC")) {
    mqttSettings.ipInTopic = _parseAsBool(request->getParam("MQTT_IP_IN_TOPIC")->value());
  }

  if (request->hasParam("MQTT_DEVICENAME_IN_TOPIC")) {
    mqttSettings.deviceNameInTopic = _parseAsBool(request->getParam("MQTT_DEVICENAME_IN_TOPIC")->value());
  }

  if (request->hasParam("MQTT_SLASH_AT_THE_BEGINNING")) {
    mqttSettings.slashAtBeginning = _parseAsBool(request->getParam("MQTT_SLASH_AT_THE_BEGINNING")->value());
  }

  if (request->hasParam("MQTT_SLASH_AT_THE_END")) {
    mqttSettings.slashAtEnd = _parseAsBool(request->getParam("MQTT_SLASH_AT_THE_END")->value());
  }

  if (request->hasParam("MQTT_TOPIC_TEMP")) {
    _parseAsCString(mqttSettings.tempTopic, request->getParam("MQTT_TOPIC_TEMP")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_HUMI")) {
    _parseAsCString(mqttSettings.humiTopic, request->getParam("MQTT_TOPIC_HUMI")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_PRESS")) {
    _parseAsCString(mqttSettings.pressTopic, request->getParam("MQTT_TOPIC_PRESS")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_PM1")) {
    _parseAsCString(mqttSettings.pm1Topic, request->getParam("MQTT_TOPIC_PM1")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_PM25")) {
    _parseAsCString(mqttSettings.pm25Topic, request->getParam("MQTT_TOPIC_PM25")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_PM10")) {
    _parseAsCString(mqttSettings.pm10Topic, request->getParam("MQTT_TOPIC_PM10")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_AIRQUALITY")) {
    _parseAsCString(mqttSettings.airQualityTopic, request->getParam("MQTT_TOPIC_AIRQUALITY")->value(), 128);
  }

  if (deviceSettings.debug) {
    Serial.println(F("handle_adv_mqtt_config_save CONFIG END!!"));
  }

  saveConfig();
  // delay(250);
  // yield();

  request->redirect("/");
  delay(1000);
  // Serial.println(F("Restart"));
  // ESP.restart();
}
#endif

static void autoupdate_on(AsyncWebServerRequest *request) {
  if (authSettings.enabled == true) {
    if (!request->authenticate(authSettings.username, authSettings.password))
      return request->requestAuthentication();
  }
  deviceSettings.autoUpdate = true;
  saveConfig();

  request->redirect("/");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

static void handle_api(AsyncWebServerRequest *request) {
  String message;
  StaticJsonDocument<800> jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json[F("device_name")] = device_name;
  if (strcmp(sensorsSettings.dustModel, "Non")) {
    json[F("pm1")] = measurementsData.averagePM1;
    json[F("pm25")] = measurementsData.averagePM25;
    if (!strcmp(sensorsSettings.dustModel, "SPS30")) {
      json[F("pm4")] = measurementsData.averagePM4;
    }
    json[F("pm10")] = measurementsData.averagePM10;
  }
  if (strcmp(sensorsSettings.thpModel, "Non")) {
    takeTHPMeasurements();
  }
  if (!strcmp(sensorsSettings.thpModel, "BME280")) {
    if (checkBmeStatus()) {
      json[F("temperature")] = float(measurementsData.temperature);
      json[F("pressure")] = int(measurementsData.pressure);
      json[F("humidity")] = int(measurementsData.humidity);
      json[F("dewpoint")] = float(pow((measurementsData.humidity) / 100, 0.125) * (112 + 0.9 * (measurementsData.temperature)) + 0.1 * (measurementsData.temperature) - 112);
    }
  }
  if (!strcmp(sensorsSettings.thpModel, "BMP280")) {
    if (checkBmpStatus()) {
      json[F("temperature")] = float(measurementsData.temperature);
      json[F("pressure")] = int(measurementsData.pressure);
    }
  }
  if (!strcmp(sensorsSettings.thpModel, "HTU21")) {
    if (checkHTU21DStatus()) {
      json[F("temperature")] = float(measurementsData.temperature);
      json[F("humidity")] = int(measurementsData.humidity);
      json[F("dewpoint")] = float(pow((measurementsData.humidity) / 100, 0.125) * (112 + 0.9 * (measurementsData.temperature)) + 0.1 * (measurementsData.temperature) - 112);
    }
  }
  if (!strcmp(sensorsSettings.thpModel, "DHT22")) {
    if (checkDHT22Status()) {
      json[F("temperature")] = float(measurementsData.temperature);
      json[F("humidity")] = int(measurementsData.humidity);
      json[F("dewpoint")] = float(pow((measurementsData.humidity) / 100, 0.125) * (112 + 0.9 * (measurementsData.temperature)) + 0.1 * (measurementsData.temperature) - 112);
    }
  }
  if (!strcmp(sensorsSettings.thpModel, "SHT1x")) {
    if (checkSHT1xStatus()) {
      json[F("temperature")] = float(measurementsData.temperature);
      json[F("humidity")] = int(measurementsData.humidity);
      json[F("dewpoint")] = float(pow((measurementsData.humidity) / 100, 0.125) * (112 + 0.9 * (measurementsData.temperature)) + 0.1 * (measurementsData.temperature) - 112);
    }
  }

  serializeJsonPretty(json, message);
  request->send(200, "text/json", message);
}

static void homekit_reset(AsyncWebServerRequest *request) {
  if (authSettings.enabled == true) {
    if (!request->authenticate(authSettings.username, authSettings.password))
      return request->requestAuthentication();
  }
  Serial.println(F("reset homekit..."));

  String pair_file_name = F("/homekit_pair.dat");
  SPIFFS.remove(pair_file_name);

  request->redirect("/");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

#ifdef ARDUINO_ARCH_ESP32
static void homekit_on(AsyncWebServerRequest *request) {
  if (authSettings.enabled == true) {
    if (!request->authenticate(authSettings.username, authSettings.password))
      return request->requestAuthentication();
  }
  Serial.println(F("homekit on..."));

  homeKitSettings.enabled = true;
  saveConfig();

  request->redirect("/");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}
#endif

#ifdef ARDUINO_ARCH_ESP32
static void homekit_off(AsyncWebServerRequest *request) {
  if (authSettings.enabled == true) {
    if (!request->authenticate(authSettings.username, authSettings.password))
      return request->requestAuthentication();
  }
  Serial.println(F("homekit off..."));

  homeKitSettings.enabled = false;
  saveConfig();

  request->redirect("/");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}
#endif
/*
 static void logout(AsyncWebServerRequest *request) {
    if (authSettings.enabled == true) {
        if(!request->authenticate(authSettings.username, authSettings.password))
            return request->requestAuthentication();
    }

  Serial.println(F("logout..."));
  request->send(401);
  request->redirect("/");
  }
*/