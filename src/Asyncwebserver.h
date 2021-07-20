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

// const char* www_realm = "Custom Auth Realm";
// String authFailResponse = "<meta http-equiv='refresh' content='0; url=/' /> Authentication Failed! <p><a href='/'>Redirect</a></p>";

String handle_root_processor(const String& var)
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

  if (!AUTOUPDATE_ON) {
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

  if (!strcmp(THP_MODEL, "Non")) {
    if (var == F("{WEB_ROOT_PAGE_MEASUREMENTS_THP1")) {
      message += "";
    }
  } else {
    if (var == F("{WEB_ROOT_PAGE_MEASUREMENTS_THP1")) {
      takeTHPMeasurements();
      message += String(WEB_ROOT_PAGE_MEASUREMENTS_THP1);
      message.replace(F("{TEXT_WEATHER}"), String(TEXT_WEATHER));

      if (!strcmp(THP_MODEL, "BME280")) {
        if (checkBmeStatus()) {
            message.replace(F("{TEXT_TEMPERATURE}"), String(TEXT_TEMPERATURE));
            message.replace(F("{TEXT_HUMIDITY}"), String(TEXT_HUMIDITY));
            message.replace(F("{TEXT_PRESSURE}"), String(TEXT_PRESSURE));
            message.replace(F("{TEXT_DEWPOINT}"), String(TEXT_DEWPOINT));

            message.replace(F("{Temperature}"), String(int(currentTemperature)));
            message.replace(F("{Pressure}"), String(int(currentPressure)));
            message.replace(F("{Humidity}"), String(int(currentHumidity)));
            message.replace(F("{Dewpoint}"), String(int(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
        } else {
          message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
        }
      } else if (!strcmp(THP_MODEL, "HTU21")) {
        if (checkHTU21DStatus()) {
          message.replace(F("{TEXT_TEMPERATURE}"), String(TEXT_TEMPERATURE));
          message.replace(F("{TEXT_HUMIDITY}"), String(TEXT_HUMIDITY));
          message.replace(F("{TEXT_DEWPOINT}"), String(TEXT_DEWPOINT));

          message.replace(F("{Temperature}"), String(int(currentTemperature)));
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{Humidity}"), String(int(currentHumidity)));
          message.replace(F("{Dewpoint}"), String(int(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
        } else {
          message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
        }
      } else if (!strcmp(THP_MODEL, "DHT22")) {
        if (checkDHT22Status()) {
          message.replace(F("{TEXT_TEMPERATURE}"), String(TEXT_TEMPERATURE));
          message.replace(F("{TEXT_HUMIDITY}"), String(TEXT_HUMIDITY));
          message.replace(F("{TEXT_DEWPOINT}"), String(TEXT_DEWPOINT));

          message.replace(F("{Temperature}"), String(int(currentTemperature)));
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{Humidity}"), String(int(currentHumidity)));
          message.replace(F("{Dewpoint}"), String(int(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
        } else {
          message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
        }
      } else if (!strcmp(THP_MODEL, "BMP280")) {
        if (checkBmpStatus()) {
          message.replace(F("{TEXT_TEMPERATURE}"), String(TEXT_TEMPERATURE));
          message.replace(F("{TEXT_PRESSURE}"), String(TEXT_PRESSURE));

          message.replace(F("{Temperature}"), String(int(currentTemperature)));
          message.replace(F("{Pressure}"), String(int(currentPressure)));
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Pressure} °C"), "");
        } else {
          message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
        }
      } else if (!strcmp(THP_MODEL, "SHT1x")) {
        if (checkSHT1xStatus()) {
          message.replace(F("{TEXT_TEMPERATURE}"), String(TEXT_TEMPERATURE));
          message.replace(F("{TEXT_HUMIDITY}"), String(TEXT_HUMIDITY));
          message.replace(F("{TEXT_DEWPOINT}"), String(TEXT_DEWPOINT));

          message.replace(F("{Temperature}"), String(int(currentTemperature)));
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{Humidity}"), String(float(currentHumidity)));
          message.replace(F("{Dewpoint}"), String(int(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
        } else {
          message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
          message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
          message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
          message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
        }
      }

    }
  }

  if (strcmp(DUST_MODEL, "Non")) {
    if (var == F("{WEB_ROOT_PAGE_MEASUREMENTS_AIR}")) {
      message += String(WEB_ROOT_PAGE_MEASUREMENTS_AIR);
      message.replace(F("{TEXT_AIRPOLLUTION}"), String(TEXT_AIRPOLLUTION));

      if (DISPLAY_PM1) {
        message.replace(F("{averagePM1}"), String(averagePM1));
      } else {
        message.replace(F("PM1: {averagePM1} µg/m³"), "");
      }

      if (averagePM25 <= 10) {
        message.replace(F("{colorAveragePM25}"), F("<font color='#61EEE4'>"));
      } else if (averagePM25 > 10 && averagePM25 <= 20) {
        message.replace(F("{colorAveragePM25}"), F("<font color='#5BCAAA'>"));
      } else if (averagePM25 > 20 && averagePM25 <= 25) {
        message.replace(F("{colorAveragePM25}"), F("<font color='#EEE25D'>"));
      } else if (averagePM25 > 25 && averagePM25 <= 50) {
        message.replace(F("{colorAveragePM25}"), F("<font color='#F95459'>"));
      } else if (averagePM25 > 50) {
        message.replace(F("{colorAveragePM25}"), F("<font color='#920736'>"));
      } else {
        message.replace(F("{colorAveragePM25}"), F("<font>"));
      }

      message.replace(F("{averagePM25}"), String(averagePM25) + F("</font>"));

      if (averagePM10 <= 20) {
        message.replace(F("{colorAveragePM10}"), F("<font color='#61EEE4'>"));
      } else if (averagePM10 > 20 && averagePM10 <= 35) {
        message.replace(F("{colorAveragePM10}"), F("<font color='#5BCAAA'>"));
      } else if (averagePM10 > 35 && averagePM10 <= 50) {
        message.replace(F("{colorAveragePM10}"), F("<font color='#EEE25D'>"));
      } else if (averagePM10 > 50 && averagePM10 <= 100) {
        message.replace(F("{colorAveragePM10}"), F("<font color='#F95459'>"));
      } else if (averagePM10 > 100) {
        message.replace(F("{colorAveragePM10}"), F("<font color='#920736'>"));
      } else {
        message.replace(F("{colorAveragePM10}"), F("<font>"));
      }
      message.replace(F("{averagePM10}"), String(averagePM10) + F("</font>"));
    }
  } else {
    if (var == F("{WEB_ROOT_PAGE_MEASUREMENTS_AIR}")) {
      message += "";
    }
  }

  if (AIRMONITOR_GRAPH_ON) {
    if (var == F("{WEB_ROOT_PAGE_AIRMONITOR_GRAPH}")) {
      message += String(WEB_ROOT_PAGE_AIRMONITOR_GRAPH);
      message.replace(F("{LATITUDE}"), String(LATITUDE));
      message.replace(F("{LONGITUDE}"), String(LONGITUDE));
    }
  } else {
    if (var == F("{WEB_ROOT_PAGE_AIRMONITOR_GRAPH}")) {
      message += "";
    }
  }

  if (AIRMONITOR_GRAPH_ON) {
    if (var == F("{WEB_ROOT_PAGE_THINGSPEAK_GRAPH}")) {
      message += String(WEB_ROOT_PAGE_THINGSPEAK_GRAPH);
      message.replace(F("{THINGSPEAK_CHANNEL_ID}"), String(THINGSPEAK_CHANNEL_ID));
      message.replace(F("{THINGSPEAK_READ_API_KEY}"), String(THINGSPEAK_READ_API_KEY));
    }
  } else {
    if (var == F("{WEB_ROOT_PAGE_THINGSPEAK_GRAPH}")) {
      message += "";
    }
  }

  return message;
  message = "";
}

//void handle_root() {
void handle_root(AsyncWebServerRequest *request) {
  if (DEBUG) {
    Serial.print(F("sizeof(WEB_ROOT_PAGE_ALL): "));
    Serial.println(sizeof(WEB_ROOT_PAGE_ALL)); // sizeof(WEB_ROOT_PAGE_ALL): ~2255
    Serial.print(F("\n"));
  }

  request->send_P(200, "text/html", WEB_ROOT_PAGE_ALL, handle_root_processor);
}

String _addOption(const String &value, const String &label, const String &srslyValue) {
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

String _add_FIRST_THP_Option(const String &value, const String &label, const String &srslyValue) {
  String option = FPSTR(WEB_CONFIG_PAGE_ADDOPTION);
  option.replace(F("{value}"), value);

  if (strcmp(DUST_MODEL, "Non")) {
    if (String(CONFIG_DUST_TX) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (String(CONFIG_DUST_RX) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (SECOND_THP) {
      if (String(CONFIG_SECOND_THP_SDA) == value) {
        option.replace(F("{srslyValue}"), F("disabled>"));
      }
      if (String(CONFIG_SECOND_THP_SCL) == value) {
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

String _add_SECOND_THP_Option(const String &value, const String &label, const String &srslyValue) {
  String option = FPSTR(WEB_CONFIG_PAGE_ADDOPTION);
  option.replace(F("{value}"), value);

  if (strcmp(DUST_MODEL, "Non")) {
    if (String(CONFIG_DUST_TX) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (String(CONFIG_DUST_RX) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (strcmp(DUST_MODEL, "Non")) {
      if (String(CONFIG_FIRST_THP_SDA) == value) {
        option.replace(F("{srslyValue}"), F("disabled>"));
      }
      if (String(CONFIG_FIRST_THP_SCL) == value) {
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

String _add_DUST_Option(const String &value, const String &label, const String &srslyValue) {
  String option = FPSTR(WEB_CONFIG_PAGE_ADDOPTION);
  option.replace(F("{value}"), value);

  if (strcmp(THP_MODEL, "Non")) {
    if (String(CONFIG_FIRST_THP_SDA) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (String(CONFIG_FIRST_THP_SCL) == value) {
      option.replace(F("{srslyValue}"), F("disabled>"));
    }
    if (SECOND_THP) {
      if (String(CONFIG_SECOND_THP_SDA) == value) {
        option.replace(F("{srslyValue}"), F("disabled>"));
      }
      if (String(CONFIG_SECOND_THP_SCL) == value) {
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

String _addBoolSelect(const String &key, const bool &value) {
  String selectValue = value ? "yes" : "no";
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  input += _addOption("yes", (TEXT_YES), selectValue);
  input += _addOption("no", (TEXT_NO), selectValue);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _addModelSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  input += _addOption("red", (TEXT_WITHOUTCALIBRATION), value);
  if (!strcmp(THP_MODEL, "BME280")) {
    input += _addOption("white", (TEXT_AUTOMATICCALIBRATION), value);
  }
  if (!strcmp(THP_MODEL, "HTU21")) {
    input += _addOption("white", (TEXT_AUTOMATICCALIBRATION), value);
  }
  if (!strcmp(THP_MODEL, "DHT22")) {
    input += _addOption("white", (TEXT_AUTOMATICCALIBRATION), value);
  }
  if (!strcmp(THP_MODEL, "SHT1x")) {
    input += _addOption("white", (TEXT_AUTOMATICCALIBRATION), value);
  }
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _addTHP_MODELSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  input += _addOption(F("BME280"), F("BME280"), value);
  if (strcmp(PMSENSORVERSION, "PMS-SparkFunBME280")) {
    if (!strcmp(DUST_MODEL, "PMS7003") or !strcmp(DUST_MODEL, "Non")) {
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

String _addDUST_MODELSelect(const String &key, const String &value) {
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

String _add_FIRST_THP_SDA_SCL_Select(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  if (!strcmp(THP_MODEL, "DS18B20")) {
    input += _add_FIRST_THP_Option(F("D5"), F("D5/GPIO14"), value);
  } else {
    input += _add_FIRST_THP_Option(F("D1"), F("D1/GPIO05"), value);
    input += _add_FIRST_THP_Option(F("D2"), F("D2/GPIO04"), value);
    input += _add_FIRST_THP_Option(F("D3"), F("D3/GPIO00"), value);
    input += _add_FIRST_THP_Option(F("D4"), F("D4/GPIO02"), value);
    input += _add_FIRST_THP_Option(F("D5"), F("D5/GPIO14"), value);
    input += _add_FIRST_THP_Option(F("D6"), F("D6/GPIO12"), value);
    input += _add_FIRST_THP_Option(F("D7"), F("D7/GPIO13"), value);
    //input += _addOption(F("D8"), F("D8/GPIO15"), value);
#ifdef ARDUINO_ARCH_ESP32
    input += _add_FIRST_THP_Option(F("D16"), F("D16/GPIO16"), value);
    input += _add_FIRST_THP_Option(F("D17"), F("D17/GPIO17"), value);
#endif
  }
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _add_SECOND_THP_SDA_SCL_Select(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  input += _add_SECOND_THP_Option(F("D1"), F("D1/GPIO05"), value);
  input += _add_SECOND_THP_Option(F("D2"), F("D2/GPIO04"), value);
  input += _add_SECOND_THP_Option(F("D3"), F("D3/GPIO00"), value);
  input += _add_SECOND_THP_Option(F("D4"), F("D4/GPIO02"), value);
  input += _add_SECOND_THP_Option(F("D5"), F("D5/GPIO14"), value);
  input += _add_SECOND_THP_Option(F("D6"), F("D6/GPIO12"), value);
  input += _add_SECOND_THP_Option(F("D7"), F("D7/GPIO13"), value);
  //input += _addOption(F("D8"), F("D8/GPIO15"), value);
#ifdef ARDUINO_ARCH_ESP32
  input += _add_SECOND_THP_Option(F("D16"), F("D16/GPIO16"), value);
  input += _add_SECOND_THP_Option(F("D17"), F("D17/GPIO17"), value);
#endif
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _add_DUST_TX_RX_Select(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  if (!strcmp(DUST_MODEL, "SPS30")) {
    input += _add_DUST_Option(F("D1"), F("D1/GPIO05"), value);
    input += _add_DUST_Option(F("D2"), F("D2/GPIO04"), value);
  } else {
    input += _add_DUST_Option(F("D1"), F("D1/GPIO05"), value);
    input += _add_DUST_Option(F("D2"), F("D2/GPIO04"), value);
    input += _add_DUST_Option(F("D3"), F("D3/GPIO00"), value);
    input += _add_DUST_Option(F("D4"), F("D4/GPIO02"), value);
    input += _add_DUST_Option(F("D5"), F("D5/GPIO14"), value);
    input += _add_DUST_Option(F("D6"), F("D6/GPIO12"), value);
    input += _add_DUST_Option(F("D7"), F("D7/GPIO13"), value);
    //input += _addOption(F("D8"), F("D8/GPIO15"), value);
#ifdef ARDUINO_ARCH_ESP32
    input += _add_DUST_Option(F("D16"), F("D16/GPIO16"), value);
    input += _add_DUST_Option(F("D17"), F("D17/GPIO17"), value);
#endif
  }
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _addINFLUXDB_VERSIONSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  input += _addOption(F("1"), F("1.x"), value);
  input += _addOption(F("2"), F("2.x"), value);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _addLanguageSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace(F("{key}"), key);
  input += _addOption(F("polish"), (TEXT_INTL_PL), value);
  input += _addOption(F("english"), (TEXT_INTL_EN), value);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _escapeString (const String &value) {
  String trimmed = value;
  trimmed.trim();
  trimmed.replace(F("'"), F("&#39;"));
  return trimmed;
}

String _addTextInput(const String &key, const String &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_TEXTIMPUT);
  input.replace(F("{key}"), key);
  input.replace(F("{value}"), _escapeString(value));
  input.replace(F("{postfix}"), postfix);
  return input;
}

String _addMQTTTextInput(const String &key, const String &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_MQTT_TEXTIMPUT);
  input.replace(F("{key}"), key);
  input.replace(F("{value}"), _escapeString(value));
  input.replace(F("{postfix}"), postfix);
  return input;
}

String _addPasswdInput(const String &key, const String &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_PASSWDINPUT);
  input.replace(F("{key}"), key);
  input.replace(F("{value}"), _escapeString(value));
  input.replace(F("{postfix}"), postfix);
  return input;
}

String _addIntInput(const String &key, const int &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_INTINPUT);
  input.replace(F("{key}"), key);
  input.replace(F("{value}"), String(value));
  input.replace(F("{postfix}"), postfix);
  return input;
}

String _addFloatInput(const String &key, const double &value, const int &precision = 6, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_FLOATINPUT);
  input.replace(F("{key}"), key);
  input.replace(F("{value}"), String(value, precision));
  input.replace(F("{postfix}"), postfix);
  return input;
}

String _addSubmitDevice() {
  String submitDevice = FPSTR(WEB_CONFIG_PAGE_SUBMIT_DEVICE_BUTTON);
  submitDevice.replace(F("{TEXT_SAVE}"), (TEXT_SAVE));
  return submitDevice;
}

String _addSubmitServices() {
  String submitServices = FPSTR(WEB_CONFIG_PAGE_SUBMIT_SERVICES_BUTTON);
  submitServices.replace(F("{TEXT_SAVE}"), (TEXT_SAVE));
  return submitServices;
}

String _addSubmitAdvMQTT() {
  String submitAdvMQTT = FPSTR(WEB_CONFIG_ADVANCED_MQTT_PAGE_SUBMIT_SERVICES_BUTTON);
  submitAdvMQTT.replace(F("{TEXT_SAVE}"), (TEXT_SAVE));
  return submitAdvMQTT;
}

String _addWiFiErase() {
  String WiFiErase = FPSTR(WEB_CONFIG_PAGE_WIFIERASE);
  WiFiErase.replace(F("{TEXT_ERASEWIFICONFIG}"), (TEXT_ERASEWIFICONFIG));
  return WiFiErase;
}

String _addRestoreConfig() {
  String RestoreConfig = FPSTR(WEB_CONFIG_PAGE_RESTORECONFIG);
  RestoreConfig.replace(F("{TEXT_RESTORESETTINGS}"), (TEXT_RESTORESETTINGS));
  return RestoreConfig;
}

String _add_homekit_reset() {
  String homekit_reset = FPSTR(WEB_CONFIG_PAGE_HOMEKIT_RESET);
  homekit_reset.replace(F("{TEXT_PAGE_HOMEKIT_RESET}"), (TEXT_PAGE_HOMEKIT_RESET));
  return homekit_reset;
}

String handle_config_processor(const String& var)
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

//void handle_config() {
void handle_config(AsyncWebServerRequest *request) {
  if (DEBUG) {
    Serial.print(F("sizeof(WEB_CONFIG_PAGE_ALL): "));
    Serial.println(sizeof(WEB_CONFIG_PAGE_ALL)); // sizeof(WEB_CONFIG_PAGE_ALL): ~3282
    Serial.print(F("\n"));
  }

  request->send_P(200, "text/html", WEB_CONFIG_PAGE_ALL, handle_config_processor);
}

String handle_config_device_processor(const String& var)
{
  //Serial.println(F("var: ") + var);
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
    if (DEVICENAME_AUTO) {
      message += String(device_name);
    } else {
      message += (_addTextInput(F("DEVICENAME"), DEVICENAME));
    }
  }

  if (var == F("{TEXT_DEVICENAMEAUTO}")) {
    message += String(TEXT_DEVICENAMEAUTO);
  }
  if (var == F("{DEVICENAME_AUTO}")) {
    message += (_addBoolSelect(F("DEVICENAME_AUTO"), DEVICENAME_AUTO));
  }
  if (var == F("{TEXT_SELECTEDLANGUAGE}")) {
    message += String(TEXT_SELECTEDLANGUAGE);
  }
  if (var == F("{LanguageSelect}")) {
    message += (_addLanguageSelect(F("LANGUAGE"), LANGUAGE));
  }
  if (var == F("{TEXT_TEMPHUMIPRESSSENSOR}")) {
    message += String(TEXT_TEMPHUMIPRESSSENSOR);
  }
  if (var == F("{THP_MODELSelect}")) {
    message += (_addTHP_MODELSelect(F("THP_MODEL"), THP_MODEL));
  }
  if (var == F("{TEXT_PMSENSOR}")) {
    message += String(TEXT_PMSENSOR);
  }
  if (var == F("{DUST_MODELSelect}")) {
    message += (_addDUST_MODELSelect(F("DUST_MODEL"), DUST_MODEL));
  }

  if (var == F("{WEB_CONFIG_DEVICE_PAGE_FIRST_TH")) {
    if (strcmp(THP_MODEL, "Non")) {
      message += String(WEB_CONFIG_DEVICE_PAGE_FIRST_THP_PINS);
    } else {
      message += "";
    }
  }

  if (var == F("{THP_MODEL}")) {
    message += String(THP_MODEL);
  }
  if (var == F("{TEXT_FIRST_THP_SDA_SCL}")) {
    message += String(TEXT_FIRST_THP_SDA_SCL);
  }
  if (var == F("{TEXT_FIRST_THP_SDA}")) {
    message += String(TEXT_FIRST_THP_SDA);
  }
  if (var == F("{FIRST_THP_SDA}")) {
    message += (_add_FIRST_THP_SDA_SCL_Select(F("CONFIG_FIRST_THP_SDA"), CONFIG_FIRST_THP_SDA));
  }
  if (var == F("{TEXT_FIRST_THP_SCL}")) {
    if (!strcmp(THP_MODEL, "DHT22") or !strcmp(THP_MODEL, "DS18B20")) {
      message += ("");
    } else {
      message += String(TEXT_FIRST_THP_SCL);
    }
  }
  if (var == F("{FIRST_THP_SCL}")) {
    if (!strcmp(THP_MODEL, "DHT22") or !strcmp(THP_MODEL, "DS18B20")) {
      message += ("");
    } else {
      message += (_add_FIRST_THP_SDA_SCL_Select(F("CONFIG_FIRST_THP_SCL"), CONFIG_FIRST_THP_SCL));
    }
  }

  if (var == F("{WEB_CONFIG_DEVICE_PAGE_SECOND_T")) {
    if (strcmp(THP_MODEL, "Non")) {
      message += String(WEB_CONFIG_DEVICE_PAGE_SECOND_THP_PINS);
    } else {
      message += ("");
    }
  }
  if (var == F("{TEXT_SECOND_THP}")) {
    message += String(TEXT_SECOND_THP);
  }
  if (var == F("{SECOND_THP}")) {
    message += (_addBoolSelect("SECOND_THP", SECOND_THP));
  }
  if (var == F("{TEXT_SECOND_THP_SDA_SCL}")) {
    message += String(TEXT_SECOND_THP_SDA_SCL);
  }
  if (var == F("{TEXT_SECOND_THP_SDA}")) {
    message += String(TEXT_SECOND_THP_SDA);
  }
  if (var == F("{SECOND_THP_SDA}")) {
    message += (_add_SECOND_THP_SDA_SCL_Select(F("CONFIG_SECOND_THP_SDA"), CONFIG_SECOND_THP_SDA));
  }
  if (var == F("{TEXT_SECOND_THP_SCL}")) {
    message += String(TEXT_SECOND_THP_SCL);
  }
  if (var == F("{SECOND_THP_SCL}")) {
    message += (_add_SECOND_THP_SDA_SCL_Select(F("CONFIG_SECOND_THP_SCL"), CONFIG_SECOND_THP_SCL));
  }


  if (var == F("{WEB_CONFIG_DEVICE_PAGE_DUST_PIN")) {
    if (strcmp(DUST_MODEL, "Non")) {
      message += String(WEB_CONFIG_DEVICE_PAGE_DUST_PINS);
    } else {
      message += "";
    }
  }

  if (var == F("{DUST_MODEL}")) {
    message += String(DUST_MODEL);
  }
  if (var == F("{TEXT_DUST_TX_RX}")) {
    message += String(TEXT_DUST_TX_RX);
  }
  if (var == F("{TEXT_DUST_TX}")) {
    message += String(TEXT_DUST_TX);
  }
  if (var == F("{DUST_TX}")) {
    message += (_add_DUST_TX_RX_Select(F("CONFIG_DUST_TX"), CONFIG_DUST_TX));
  }
  if (var == F("{TEXT_DUST_RX}")) {
    message += String(TEXT_DUST_RX);
  }
  if (var == F("{DUST_RX}")) {
    message += (_add_DUST_TX_RX_Select(F("CONFIG_DUST_RX"), CONFIG_DUST_RX));
  }

  if (var == F("{TEXT_FREQUENTMEASUREMENTONOFF}")) {
    message += String(TEXT_FREQUENTMEASUREMENTONOFF);
  }
  if (var == F("{FREQUENTMEASUREMENT_Select}")) {
    message += (_addBoolSelect(F("FREQUENTMEASUREMENT"), FREQUENTMEASUREMENT));
  }
  if (var == F("{TEXT_FREQUENTMEASUREMENTINFO}")) {
    message += String(TEXT_FREQUENTMEASUREMENTINFO);
  }
  if (var == F("{TEXT_MEASUREMENTFREQUENCY}")) {
    message += String(TEXT_MEASUREMENTFREQUENCY);
  }
  if (var == F("{FREQUENTMEASUREMENT_time}")) {
    if (FREQUENTMEASUREMENT == true) {
      message += (_addIntInput(F("DUST_TIME"), DUST_TIME, F("{TEXT_SECONDS}")));
      message.replace(F("{TEXT_SECONDS}"), String(TEXT_SECONDS));
    } else {
      message += (_addIntInput(F("DUST_TIME"), DUST_TIME, F("{TEXT_MINUTES}")));
      message.replace(F("{TEXT_MINUTES}"), String(TEXT_MINUTES));
    }
  }

  if (var == F("{TEXT_AVERAGELASTRESULT}")) {
    message += String(TEXT_AVERAGELASTRESULT);
  }
  if (var == F("{NUMBEROFMEASUREMENTS}")) {
    message += (_addIntInput(F("NUMBEROFMEASUREMENTS"), NUMBEROFMEASUREMENTS, F("{TEXT_PMMEASUREMENTS}")));
    message.replace(F("{TEXT_PMMEASUREMENTS}"), String(TEXT_PMMEASUREMENTS));
  }

  if (var == F("{TEXT_DEEPSLEEPINFO}")) {
#ifdef ARDUINO_ARCH_ESP8266
    message += String(TEXT_DEEPSLEEPINFO);
#elif defined ARDUINO_ARCH_ESP32
    message += String(TEXT_DEEPSLEEPINFO_ESP32);
#endif
    message.replace(F("{INTERFACEWWWONTIME}"), String(int(NUMBEROFMEASUREMENTS) * 2 + 10 ));
    message.replace(F("{SENDING_FREQUENCY}"), String(SENDING_FREQUENCY));
  }
  if (var == F("{DEEPSLEEP_ON}")) {
    message += (_addBoolSelect(F("DEEPSLEEP_ON"), DEEPSLEEP_ON));
  }

  if (var == F("{WEB_CONFIG_DEVICE_SHOWING_PM1}")) {
    if (!strcmp(DUST_MODEL, "PMS7003") or !strcmp(DUST_MODEL, "SPS30")) {
      message += String(WEB_CONFIG_DEVICE_SHOWING_PM1);
    } else {
      message += ("");
    }
  }
  if (var == F("{TEXT_DISPLAYPM1}")) {
    message += String(TEXT_DISPLAYPM1);
  }
  if (var == F("{DISPLAY_PM1}")) {
    message += (_addBoolSelect(F("DISPLAY_PM1"), DISPLAY_PM1));
    message.replace(F("{TEXT_DISPLAYPM1}"), String(TEXT_DISPLAYPM1));
  }
  if (var == F("{TEXT_ALTITUDEINFO}")) {
    message += String(TEXT_ALTITUDEINFO);
    message.replace(F("{WSPOLRZEDNE_GPS_LINK}"), String(WSPOLRZEDNE_GPS_LINK));
    message.replace(F("{TEXT_HERE}"), String(TEXT_HERE));
  }
  if (var == F("{MYALTITUDE}")) {
    message += (_addIntInput(F("MYALTITUDE"), MYALTITUDE, F("m.n.p.m")));
  }

  if (var == F("{TEXT_SECURECONFIGUPDATEPAGE}")) {
    message += String(TEXT_SECURECONFIGUPDATEPAGE);
  }
  if (var == F("{CONFIG_AUTH}")) {
    message += (_addBoolSelect(F("CONFIG_AUTH"), CONFIG_AUTH));
  }
  if (var == F("{TEXT_SECURELOGIN}")) {
    message += String(TEXT_SECURELOGIN);
  }
  if (var == F("{CONFIG_USERNAME}")) {
    message += (_addTextInput(F("CONFIG_USERNAME"), CONFIG_USERNAME));
  }
  if (var == F("{TEXT_SECUREPASSWD}")) {
    message += String(TEXT_SECUREPASSWD);
  }
  if (var == F("{CONFIG_PASSWORD}")) {
    message += (_addPasswdInput(F("CONFIG_PASSWORD"), CONFIG_PASSWORD));
  }

  if (var == F("{TEXT_SECURELOGOUTINFO}")) {
    if (!CONFIG_AUTH) {
      message += ("");
    } else {
      message += String(TEXT_SECURELOGOUTINFO);
    }
  }

  if (var == F("{DEBUG}")) {
    message += (_addBoolSelect(F("DEBUG"), DEBUG));
  }
  if (var == F("{TEXT_CALIBMETHOD}")) {
    message += String(TEXT_CALIBMETHOD);
  }
  if (var == F("{CalibrationModelSelect}")) {
    message += (_addModelSelect(F("MODEL"), MODEL));
  }
  if (var == F("{TEXT_CALIB1}")) {
    message += String(TEXT_CALIB1);
  }
  if (var == F("{calib1}")) {
    message += (String(calib1));
  }
  if (var == F("{TEXT_CALIB2}")) {
    message += String(TEXT_CALIB2);
  }
  if (var == F("{calib2}")) {
    message += (String(calib2));
  }

  if (var == F("{TEXT_SOFTWATEVERSION}")) {
    message += String(TEXT_SOFTWATEVERSION);
  }
  if (var == F("{SOFTWAREVERSION}")) {
    message += String(SOFTWAREVERSION);
  }

  if (var == F("{TEXT_AUTOUPDATEON}")) {
    message += String(TEXT_AUTOUPDATEON);
  }
  if (var == F("{AUTOUPDATEON}")) {
    message += (_addBoolSelect(F("AUTOUPDATE_ON"), AUTOUPDATE_ON));
  }

#ifdef ARDUINO_ARCH_ESP8266
  if (var == F("{WEB_CONFIG_DEVICE_HOMEKIT}")) {
    message += ("");
  }
#elif defined ARDUINO_ARCH_ESP32
  if (var == F("{WEB_CONFIG_DEVICE_HOMEKIT}")) {
    message += String(WEB_CONFIG_DEVICE_HOMEKIT);
  }

  if (var == F("{TEXT_HOMEKIT_SUPPORT}")) {
    message += String(TEXT_HOMEKIT_SUPPORT);
  }
  if (var == F("{HOMEKIT_SUPPORT_ON}")) {
    message += (_addBoolSelect(F("HOMEKIT_SUPPORT"), HOMEKIT_SUPPORT));
  }
  if (var == F("{TEXT_HOMEKIT_IS_PAIRED}")) {
    if (HOMEKIT_SUPPORT == true) {
      // Serial.println(F("homekit_is_paired: ") + String(homekit_is_paired()));
      if (String(homekit_is_paired()) == "1") {
        message += String(TEXT_HOMEKIT_IS_PAIRED);
      }
    }
  }
  if (var == F("{HOMEKIT_PAIRED_RESET}")) {
    if (HOMEKIT_SUPPORT == true) {
      // Serial.println(F("homekit_is_paired: ") + String(homekit_is_paired()));
      if (String(homekit_is_paired()) == "1") {
        message += (_add_homekit_reset());
      }
    }
  }
#endif

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

//void _handle_config_device(bool is_success) {
void handle_config_device(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
      return request->requestAuthentication();
  }

  if (DEBUG) {
    Serial.print(F("sizeof(WEB_CONFIG_DEVICE_PAGE_ALL): "));
    Serial.println(sizeof(WEB_CONFIG_DEVICE_PAGE_ALL)); // sizeof(WEB_CONFIG_DEVICE_PAGE_ALL): ~3781
    Serial.print(F("\n"));
  }

  request->send_P(200, "text/html", WEB_CONFIG_DEVICE_PAGE_ALL, handle_config_device_processor);
}

String handle_config_services_processor(const String& var)
{
  //Serial.println(F("var: ") + var);
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
    if (FREQUENTMEASUREMENT == true) {
      message += _addIntInput(F("SENDING_FREQUENCY"), SENDING_FREQUENCY, F("{TEXT_SECONDS}"));
      message.replace(F("{TEXT_SECONDS}"), String(TEXT_SECONDS));
    } else {
      message += _addIntInput(F("SENDING_DB_FREQUENCY"), SENDING_DB_FREQUENCY, F("{TEXT_MINUTES}"));
      message.replace(F("{TEXT_MINUTES}"), String(TEXT_MINUTES));
    }
  }

  if (var == F("{TEXT_DBSENDINGINTERVAL}")) {
    message += String(TEXT_DBSENDINGINTERVAL);
  }
  if (var == F("{SENDING_DB_FREQUENCY}")) {
    if (FREQUENTMEASUREMENT == true) {
      message += _addIntInput(F("SENDING_DB_FREQUENCY"), SENDING_DB_FREQUENCY, F("{TEXT_SECONDS}"));
      message.replace(F("{TEXT_SECONDS}"), String(TEXT_SECONDS));
    } else {
      message += _addIntInput(F("SENDING_DB_FREQUENCY"), SENDING_DB_FREQUENCY, F("{TEXT_MINUTES}"));
      message.replace(F("{TEXT_MINUTES}"), String(TEXT_MINUTES));
    }
  }

  if (var == F("{TEXT_SMOGLISTSENDING}")) {
    message += String(TEXT_SMOGLISTSENDING);
    message.replace(F("{SMOGLIST_LINK}"), String(SMOGLIST_LINK));

  }

  if (var == F("{SMOGLIST_ON}")) {
    message += (_addBoolSelect(F("SMOGLIST_ON"), SMOGLIST_ON));
  }
  if (var == F("{TEXT_SMOGLISTINFO}")) {
    message += String(TEXT_SMOGLISTINFO);
  }

  if (var == F("{TEXT_LUFTDATENSENDING}")) {
    message += String(TEXT_LUFTDATENSENDING);
    message.replace(F("{LUFTDATEN_LINK}"), String(LUFTDATEN_LINK));
    message.replace(F("{LUFTDATENFORM_LINK}"), String(LUFTDATENFORM_LINK));
  }
  if (var == F("{LUFTDATEN_ON}")) {
    message += (_addBoolSelect(F("LUFTDATEN_ON"), LUFTDATEN_ON));
  }

  if (var == F("{LUFTDATEN_ChipID}")) {
#ifdef ARDUINO_ARCH_ESP8266
    message += (F("smogomierz-") + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
    message += (F("smogomierz-") + String((uint32_t)(ESP.getEfuseMac())));
#endif
  }

  if (var == F("{Sensors_Pins_html}")) {
    if (strcmp(THP_MODEL, "Non")) {
      message += String(WEB_CONFIG_SERVICES_THP_SENSOR_PIN);
    } else {
      message += "";
    }
  }
  if (!strcmp(THP_MODEL, "BME280")) {
    if (var == F("{THPSENSOR}")) {
      message += F("BME280");
    }
    if (var == F("{THPXPIN}")) {
      message += F("11");
    }
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    if (var == F("{THPSENSOR}")) {
      message += F("BMP280");
    }
    if (var == F("{THPXPIN}")) {
      message += F("3");
    }
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    if (var == F("{THPSENSOR}")) {
      message += F("HTU21");
    }
    if (var == F("{THPXPIN}")) {
      message += F("7");
    }
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    if (var == F("{THPSENSOR}")) {
      message += F("DHT22");
    }
    if (var == F("{THPXPIN}")) {
      message += F("7");
    }
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
    if (var == F("{THPSENSOR}")) {
      message += F("SHT1x");
    }
    if (var == F("{THPXPIN}")) {
      message += F("12");
    }
  } else if (!strcmp(THP_MODEL, "DS18B20")) {
    if (var == F("{THPSENSOR}")) {
      message += F("DS18B20");
    }
    if (var == F("{THPXPIN}")) {
      message += F("13");
    }
  }

  if (var == F("{Sensors_Pins_html}")) {
    if (strcmp(DUST_MODEL, "Non")) {
      message += String(WEB_CONFIG_SERVICES_DUST_MODEL_SENSOR_PIN);
    } else {
      message += "";
    }
  }
  if (!strcmp(DUST_MODEL, "PMS7003")) {
    if (var == F("{DUSTSENSOR}")) {
      message += F("PMS5003/7003");
    }
    if (var == F("{DUSTXPIN}")) {
      message += F("1");
    }
  } else if (!strcmp(DUST_MODEL, "SDS011/21")) {
    if (var == F("{DUSTSENSOR}")) {
      message += F("SDS011/21");
    }
    if (var == F("{DUSTXPIN}")) {
      message += F("1");
    }
  } else if (!strcmp(DUST_MODEL, "HPMA115S0")) {
    if (var == F("{DUSTSENSOR}")) {
      message += F("HPMA115S0");
    }
    if (var == F("{DUSTXPIN}")) {
      message += F("1");
    }
  } else if (!strcmp(DUST_MODEL, "SPS30")) {
    if (var == F("{DUSTSENSOR}")) {
      message += F("SPS30");
    }
    if (var == F("{DUSTXPIN}")) {
      message += F("1");
    }
  }

  if (var == F("{TEXT_AQIECOSENDING}")) {
    message += String(TEXT_AQIECOSENDING);
  }
  if (var == F("{AQI_ECO_ON}")) {
    message += (_addBoolSelect(F("AQI_ECO_ON"), AQI_ECO_ON));
  }
  if (var == F("{TEXT_AQIECOSERVER}")) {
    message += String(TEXT_AQIECOSERVER);
  }
  if (var == F("{AQI_ECO_HOST}")) {
    message += (_addTextInput(F("AQI_ECO_HOST"), AQI_ECO_HOST));
  }
  if (var == F("{TEXT_AQIECOPATH}")) {
    message += String(TEXT_AQIECOPATH);
  }
  if (var == F("{AQI_ECO_PATH}")) {
    message += (_addTextInput(F("AQI_ECO_PATH"), AQI_ECO_PATH));
  }

#ifdef ARDUINO_ARCH_ESP8266
  if (var == F("{ESP_MODEL}")) {
    message += F("ESP8266");
  }
  if (var == F("{AQI_ECO_ChipID}")) {
    message += (String(ESP.getChipId()));
  }
#elif defined ARDUINO_ARCH_ESP32
  if (var == F("{ESP_MODEL}")) {
    message += F("ESP32");
  }
  if (var == F("{AQI_ECO_ChipID}")) {
    message += (String((uint32_t)(ESP.getEfuseMac())));
  }
#endif

  if (var == F("{TEXT_AIRMONITORSENDING}")) {
    message += String(TEXT_AIRMONITORSENDING);
    char PMSENSORMODEL[16];
    if (!strcmp(DUST_MODEL, "PMS7003") or !strcmp(DUST_MODEL, "Non")) {
      strcpy(PMSENSORMODEL, "PMS7003");
    } else if (!strcmp(DUST_MODEL, "SDS011/21")) {
      strcpy(PMSENSORMODEL, "SDS011");
    } else if (!strcmp(DUST_MODEL, "HPMA115S0")) {
      strcpy(PMSENSORMODEL, "HPMA115S0");
    } else if (!strcmp(DUST_MODEL, "SPS30")) {
      strcpy(PMSENSORMODEL, "SPS30");
    }
    message.replace(F("{PMSENSORMODEL}"), String(PMSENSORMODEL));
    message.replace(F("{AIRMONITOR_LINK}"), String(AIRMONITOR_LINK));
    message.replace(F("{AIRMONITORFORM_LINK}"), String(AIRMONITORFORM_LINK));
    message.replace(F("{TEXT_THEFORM}"), String(TEXT_THEFORM));
    message.replace(F("{TEXT_AIRMONITORCHARTS}"), String(TEXT_AIRMONITORCHARTS));
  }
  if (var == F("{AIRMONITOR_ON}")) {
    message += (_addBoolSelect(F("AIRMONITOR_ON"), AIRMONITOR_ON));
  }
  if (var == F("{TEXT_AIRMONITORCHARTS}")) {
    message += String(TEXT_AIRMONITORCHARTS);
  }
  if (var == F("{AIRMONITOR_GRAPH_ON}")) {
    message += (_addBoolSelect(F("AIRMONITOR_GRAPH_ON"), AIRMONITOR_GRAPH_ON));
  }
  if (var == F("{TEXT_AIRMONITOR_API_KEY}")) {
    message += String(TEXT_AIRMONITOR_API_KEY);
  }
  if (var == F("{AIRMONITOR_API_KEY}")) {
    message += (_addTextInput(F("AIRMONITOR_API_KEY"), AIRMONITOR_API_KEY));
  }
  if (var == F("{TEXT_AIRMONITORCOORDINATESINFO}")) {
    message += String(TEXT_AIRMONITORCOORDINATESINFO);
    message.replace(F("{LATLONG_LINK}"), String(LATLONG_LINK));
    message.replace(F("{TEXT_HERE}"), String(TEXT_HERE));
  }
  if (var == F("{TEXT_AIRMONITORLATITUDE}")) {
    message += String(TEXT_AIRMONITORLATITUDE);
  }
  if (var == F("{LATITUDE}")) {
    message += (_addFloatInput(F("LATITUDE"), atof(LATITUDE), 6, F("°")));
  }
  if (var == F("{TEXT_AIRMONITORLONGITUDE}")) {
    message += String(TEXT_AIRMONITORLONGITUDE);
  }
  if (var == F("{LONGITUDE}")) {
    message += (_addFloatInput(F("LONGITUDE"), atof(LONGITUDE), 6, F("°")));
  }

  if (var == F("{TEXT_THINGSPEAKSENDING}")) {
    message += String(TEXT_THINGSPEAKSENDING);
    message.replace(F("{THINGSPEAK_LINK}"), String(THINGSPEAK_LINK));
  }
  if (var == F("{THINGSPEAK_ON}")) {
    message += (_addBoolSelect(F("THINGSPEAK_ON"), THINGSPEAK_ON));
  }
  if (var == F("{TEXT_THINGSPEAKCHARTS}")) {
    message += String(TEXT_THINGSPEAKCHARTS);
  }
  if (var == F("{THINGSPEAK_GRAPH_ON}")) {
    message += (_addBoolSelect(F("THINGSPEAK_GRAPH_ON"), THINGSPEAK_GRAPH_ON));
  }
  if (var == F("{TEXT_THINGSPEAK_WRITE_API_KEY}")) {
    message += String(TEXT_THINGSPEAK_WRITE_API_KEY);
  }
  if (var == F("{THINGSPEAK_API_KEY}")) {
    message += (_addTextInput(F("THINGSPEAK_API_KEY"), THINGSPEAK_API_KEY));
  }
  if (var == F("{TEXT_THINGSPEAKCHANNELID}")) {
    message += String(TEXT_THINGSPEAKCHANNELID);
  }
  if (var == F("{THINGSPEAK_CHANNEL_ID}")) {
    message += (_addIntInput(F("THINGSPEAK_CHANNEL_ID"), THINGSPEAK_CHANNEL_ID));
  }
  if (var == F("{TEXT_THINGSPEAK_READ_API_KEY}")) {
    message += String(TEXT_THINGSPEAK_READ_API_KEY);
  }
  if (var == F("{THINGSPEAK_READ_API_KEY}")) {
    message += (_addTextInput(F("THINGSPEAK_READ_API_KEY"), THINGSPEAK_READ_API_KEY));
  }

  if (var == F("{TEXT_INFLUXDBSENDING}")) {
    message += String(TEXT_INFLUXDBSENDING);
  }
  if (var == F("{INFLUXDB_ON}")) {
    message += (_addBoolSelect(F("INFLUXDB_ON"), INFLUXDB_ON));
  }
  if (var == F("{TEXT_INFLUXDBVERSION}")) {
    message += String(TEXT_INFLUXDBVERSION);
  }
  if (var == F("{INFLUXDB_VERSION}")) {
    message += (_addINFLUXDB_VERSIONSelect(F("INFLUXDB_VERSION"), INFLUXDB_VERSION));
  }
  if (var == F("{TEXT_INFLUXDBSERVER}")) {
    message += String(TEXT_INFLUXDBSERVER);
  }
  if (var == F("{INFLUXDB_HOST}")) {
    message += (_addTextInput(F("INFLUXDB_HOST"), INFLUXDB_HOST));
  }
  if (var == F("{TEXT_INFLUXDBPORT}")) {
    message += String(TEXT_INFLUXDBPORT);
  }
  if (var == F("{INFLUXDB_PORT}")) {
    message += (_addIntInput(F("INFLUXDB_PORT"), INFLUXDB_PORT));
  }
  if (var == F("{TEXT_INFLUXDBNAME}")) {
    message += String(TEXT_INFLUXDBNAME);
  }
  if (var == F("{INFLUXDB_DATABASE}")) {
    message += ( _addTextInput(F("INFLUXDB_DATABASE"), INFLUXDB_DATABASE));
  }
  if (var == F("{TEXT_INFLUXDBUSER}")) {
    message += String(TEXT_INFLUXDBUSER);
  }
  if (var == F("{INFLUXDB_USER}")) {
    message += (_addTextInput(F("INFLUXDB_USER"), INFLUXDB_USER));
  }
  if (var == F("{TEXT_INFLUXDBPASSWD}")) {
    message += String(TEXT_INFLUXDBPASSWD);
  }
  if (var == F("{INFLUXDB_PASSWORD}")) {
    message += (_addPasswdInput(F("INFLUXDB_PASSWORD"), INFLUXDB_PASSWORD));
  }
  if (var == F("{WEB_CONFIG_SERVICES_INFLUXDB_VE}")) {
    if (!strcmp(INFLUXDB_VERSION, "2")) {
      message += String(WEB_CONFIG_SERVICES_INFLUXDB_VERSION_V2);
      message.replace(F("{TEXT_INFLUXDBORG}"), String(TEXT_INFLUXDBORG));
      message.replace(F("{INFLUXDB_ORG}"), _addTextInput(F("INFLUXDB_ORG"), INFLUXDB_ORG));
      message.replace(F("{TEXT_INFLUXDBBUCKET}"), String(TEXT_INFLUXDBBUCKET));
      message.replace(F("{INFLUXDB_BUCKET}"), _addTextInput(F("INFLUXDB_BUCKET"), INFLUXDB_BUCKET));
      message.replace(F("{TEXT_INFLUXDBTOKEN}"), String(TEXT_INFLUXDBTOKEN));
      message.replace(F("{INFLUXDB_TOKEN}"), _addTextInput(F("INFLUXDB_TOKEN"), INFLUXDB_TOKEN));
    } else {
      message += "";
    }
  }
  /*
    if (var == F("{TEXT_MQTTSENDING}")) {
     message += (TEXT_MQTTSENDING);
    }
    if (var == F("{MQTT_ON}")) {
     message += (_addBoolSelect(F("MQTT_ON"), MQTT_ON));
    }
    if (var == F("{TEXT_MQTTSERVER}")) {
    message += (TEXT_MQTTSERVER);
    }
    if (var == F("{MQTT_HOST}")) {
    message += (_addTextInput(F("MQTT_HOST"), MQTT_HOST));
    }
    if (var == F("{TEXT_MQTTPORT}")) {
    message += (TEXT_MQTTPORT);
    }
    if (var == F("{MQTT_PORT}")) {
    message += (_addIntInput(F("MQTT_PORT"), MQTT_PORT));
    }
    if (var == F("{TEXT_MQTTUSER}")) {
    message += (TEXT_MQTTUSER);
    }
    if (var == F("{MQTT_USER}")) {
    message += (_addTextInput(F("MQTT_USER"), MQTT_USER));
    }
    if (var == F("{TEXT_MQTTPASSWD}")) {
    message += (TEXT_MQTTPASSWD);
    }
    if (var == F("{MQTT_PASSWORD}")) {
    message += (_addPasswdInput(F("MQTT_PASSWORD"), MQTT_PASSWORD));
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
    message += (_addBoolSelect(F("MQTT_IP_IN_TOPIC"), MQTT_IP_IN_TOPIC));
    }
    if (var == F("{TEXT_MQTT_DEVICENAME_IN_TOPIC}")) {
    message += (TEXT_MQTT_DEVICENAME_IN_TOPIC);
    }
    if (var == F("{MQTT_DEVICENAME_IN_TOPIC}")) {
    message += (_addBoolSelect(F("MQTT_DEVICENAME_IN_TOPIC"), MQTT_DEVICENAME_IN_TOPIC));
    }
    if (var == F("{TEXT_MQTT_SLASH_AT_THE_BEGINNIN")) {
    message += (TEXT_MQTT_SLASH_AT_THE_BEGINNING);
    }
    if (var == F("{MQTT_SLASH_AT_THE_BEGINNING}")) {
    message += (_addBoolSelect(F("MQTT_SLASH_AT_THE_BEGINNING"), MQTT_SLASH_AT_THE_BEGINNING));
    }
    if (var == F("{TEXT_MQTT_SLASH_AT_THE_END}")) {
    message += (TEXT_MQTT_SLASH_AT_THE_END);
    }
    if (var == F("{MQTT_SLASH_AT_THE_END}")) {
    message += (_addBoolSelect(F("MQTT_SLASH_AT_THE_END"), MQTT_SLASH_AT_THE_END));
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
    message += (_addMQTTTextInput(F("MQTT_TOPIC_TEMP"), MQTT_TOPIC_TEMP));
    }
    if (var == F("{MQTT_TEMP}")) {
    message += (String(int(currentTemperature)));
    }
  */

  if (var == F("{AdvancedMQTTConfigButton}")) {
    message += String(ASW_WEB_GOTO_CONFIG_ADVANCED_MQTT_PAGE_BUTTON);
    message.replace(F("{TEXT_CONFIG_ADV_MQTT}"), String(TEXT_CONFIG_ADV_MQTT));
  }

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

  return message;
  message = "";
}

//void _handle_config_services(bool is_success) {
void handle_config_services(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
      return request->requestAuthentication();
  }

  if (DEBUG) {
    Serial.print(F("sizeof(WEB_CONFIG_SERVICES_PAGE_ALL): "));
    Serial.println(sizeof(WEB_CONFIG_SERVICES_PAGE_ALL)); // sizeof(WEB_CONFIG_SERVICES_PAGE_ALL): 4178
    Serial.print(F("\n"));
  }

  request->send_P(200, "text/html", WEB_CONFIG_SERVICES_PAGE_ALL, handle_config_services_processor);
}

String handle_adv_mqtt_config_processor(const String& var)
{
  //Serial.println(F("var: ") + var);
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
    message += (_addBoolSelect(F("MQTT_ON"), MQTT_ON));
  }
  if (var == F("{TEXT_MQTTSERVER}")) {
    message += String(TEXT_MQTTSERVER);
  }
  if (var == F("{MQTT_HOST}")) {
    message += (_addTextInput(F("MQTT_HOST"), MQTT_HOST));
  }
  if (var == F("{TEXT_MQTTPORT}")) {
    message += String(TEXT_MQTTPORT);
  }
  if (var == F("{MQTT_PORT}")) {
    message += (_addIntInput(F("MQTT_PORT"), MQTT_PORT));
  }
  if (var == F("{TEXT_MQTTUSER}")) {
    message += String(TEXT_MQTTUSER);
  }
  if (var == F("{MQTT_USER}")) {
    message += (_addTextInput(F("MQTT_USER"), MQTT_USER));
  }
  if (var == F("{TEXT_MQTTPASSWD}")) {
    message += String(TEXT_MQTTPASSWD);
  }
  if (var == F("{MQTT_PASSWORD}")) {
    message += (_addPasswdInput(F("MQTT_PASSWORD"), MQTT_PASSWORD));
  }

  if (var == F("{TEXT_MQTT_TOPIC_INFO}")) {
    message += String(TEXT_MQTT_TOPIC_INFO);
  }
  if (var == F("{TEXT_MQTT_IP_IN_TOPIC}")) {
    message += String(TEXT_MQTT_IP_IN_TOPIC);
  }
  if (var == F("{MQTT_IP_IN_TOPIC}")) {
    message += (_addBoolSelect(F("MQTT_IP_IN_TOPIC"), MQTT_IP_IN_TOPIC));
  }
  if (var == F("{TEXT_MQTT_DEVICENAME_IN_TOPIC}")) {
    message += String(TEXT_MQTT_DEVICENAME_IN_TOPIC);
  }
  if (var == F("{MQTT_DEVICENAME_IN_TOPIC}")) {
    message += (_addBoolSelect(F("MQTT_DEVICENAME_IN_TOPIC"), MQTT_DEVICENAME_IN_TOPIC));
  }
  if (var == F("{TEXT_MQTT_SLASH_AT_THE_BEGINNIN")) {
    message += String(TEXT_MQTT_SLASH_AT_THE_BEGINNING);
  }
  if (var == F("{MQTT_SLASH_AT_THE_BEGINNING}")) {
    message += (_addBoolSelect(F("MQTT_SLASH_AT_THE_BEGINNING"), MQTT_SLASH_AT_THE_BEGINNING));
  }
  if (var == F("{TEXT_MQTT_SLASH_AT_THE_END}")) {
    message += String(TEXT_MQTT_SLASH_AT_THE_END);
  }
  if (var == F("{MQTT_SLASH_AT_THE_END}")) {
    message += (_addBoolSelect(F("MQTT_SLASH_AT_THE_END"), MQTT_SLASH_AT_THE_END));
  }

  if (var == F("{WEB_CONFIG_ADV_MQTT_PAGE_CONFIG")) {
    if (!MQTT_SLASH_AT_THE_BEGINNING and MQTT_SLASH_AT_THE_END) {
      message += String(WEB_CONFIG_ADV_MQTT_PAGE_CONFIG1);
    } else if (!MQTT_SLASH_AT_THE_END and MQTT_SLASH_AT_THE_BEGINNING) {
      message += String(WEB_CONFIG_ADV_MQTT_PAGE_CONFIG2);
    } else if (!MQTT_SLASH_AT_THE_END and !MQTT_SLASH_AT_THE_BEGINNING) {
      message += String(WEB_CONFIG_ADV_MQTT_PAGE_CONFIG3);
    } else {
      message += String(WEB_CONFIG_ADV_MQTT_PAGE_CONFIG);
    }
  }

  if (strcmp(THP_MODEL, "Non")) {
    // takeTHPMeasurements();
    if (var == F("{MQTT_TEMP}")) {
      message += (String(int(currentTemperature)));
    }
    if (var == F("{MQTT_TOPIC_TEMP}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_TEMP"), MQTT_TOPIC_TEMP));
    }

    if (var == F("{MQTT_HUMI}")) {
      message += (String(int(currentHumidity)));
    }
    if (var == F("{MQTT_TOPIC_HUMI}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_HUMI"), MQTT_TOPIC_HUMI));
    }
    if (var == F("{MQTT_PRESS}")) {
      message += (String(int(currentPressure)));
    }
    if (var == F("{MQTT_TOPIC_PRESS}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_PRESS"), MQTT_TOPIC_PRESS));
    }
  }

  if (strcmp(DUST_MODEL, "Non")) {
    if (var == F("{MQTT_PM1}")) {
      message += (String(int(averagePM1)));
    }
    if (var == F("{MQTT_TOPIC_PM1}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_PM1"), MQTT_TOPIC_PM1));
    }
    if (var == F("{MQTT_PM25}")) {
      message += (String(int(averagePM25)));
    }
    if (var == F("{MQTT_TOPIC_PM25}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_PM25"), MQTT_TOPIC_PM25));
    }
    if (var == F("{MQTT_PM10}")) {
      message += (String(int(averagePM10)));
    }
    if (var == F("{MQTT_TOPIC_PM10}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_PM10"), MQTT_TOPIC_PM10));
    }

    if (var == F("{MQTT_AIRQUALITY}")) {
      if (averagePM25 <= 10) {
        message += F("EXCELLENT");
      } else if (averagePM25 > 10 && averagePM25 <= 20) {
        message += F("GOOD");
      } else if (averagePM25 > 20 && averagePM25 <= 25) {
        message += F("FAIR");
      } else if (averagePM25 > 25 && averagePM25 <= 50) {
        message += F("INFERIOR");
      } else if (averagePM25 > 50) {
        message += F("POOR");
      } else {
        message += F("UNKNOWN");
      }
    }
    if (var == F("{MQTT_TOPIC_AIRQUALITY}")) {
      message += (_addMQTTTextInput(F("MQTT_TOPIC_AIRQUALITY"), MQTT_TOPIC_AIRQUALITY));
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
    if (MQTT_DEVICENAME_IN_TOPIC) {
      message += ((String(device_name) + F("/")));
    } else {
      message += F("");
    }
  }
  if (var == F("{MQTT_IP_IN_TOPIC}")) {
    if (MQTT_IP_IN_TOPIC) {
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

//#ifdef ARDUINO_ARCH_ESP32
void handle_adv_mqtt_config(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
      return request->requestAuthentication();
  }

  if (DEBUG) {
    Serial.print(F("sizeof(WEB_CONFIG_ADV_MQTT_PAGE_ALL): "));
    Serial.println(sizeof(WEB_CONFIG_ADV_MQTT_PAGE_ALL)); // sizeof(WEB_CONFIG_ADV_MQTT_PAGE_ALL): 2998
    Serial.print(F("\n"));
  }

  request->send_P(200, "text/html", WEB_CONFIG_ADV_MQTT_PAGE_ALL, handle_adv_mqtt_config_processor);
}
//#endif
/*
  #ifdef ARDUINO_ARCH_ESP8266
  void handle_adv_mqtt_config(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
      return request->requestAuthentication();
  }
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
  message.replace(F("{MQTT_ON}"), _addBoolSelect("MQTT_ON", MQTT_ON));
  message.replace(F("{TEXT_MQTTSERVER}"), (TEXT_MQTTSERVER));
  message.replace(F("{MQTT_HOST}"), _addTextInput("MQTT_HOST", MQTT_HOST));
  message.replace(F("{TEXT_MQTTPORT}"), (TEXT_MQTTPORT));
  message.replace(F("{MQTT_PORT}"), _addIntInput("MQTT_PORT", MQTT_PORT));
  message.replace(F("{TEXT_MQTTUSER}"), (TEXT_MQTTUSER));
  message.replace(F("{MQTT_USER}"), _addTextInput("MQTT_USER", MQTT_USER));
  message.replace(F("{TEXT_MQTTPASSWD}"), (TEXT_MQTTPASSWD));
  message.replace(F("{MQTT_PASSWORD}"), _addPasswdInput("MQTT_PASSWORD", MQTT_PASSWORD));

  message.replace(F("{TEXT_MQTT_TOPIC_INFO}"), (TEXT_MQTT_TOPIC_INFO));

  message.replace(F("{TEXT_MQTT_IP_IN_TOPIC}"), (TEXT_MQTT_IP_IN_TOPIC));
  message.replace(F("{MQTT_IP_IN_TOPIC}"), _addBoolSelect("MQTT_IP_IN_TOPIC", MQTT_IP_IN_TOPIC));

  message.replace(F("{TEXT_MQTT_DEVICENAME_IN_TOPIC}"), (TEXT_MQTT_DEVICENAME_IN_TOPIC));
  message.replace(F("{MQTT_DEVICENAME_IN_TOPIC}"), _addBoolSelect("MQTT_DEVICENAME_IN_TOPIC", MQTT_DEVICENAME_IN_TOPIC));

  message.replace(F("{TEXT_MQTT_SLASH_AT_THE_BEGINNING}"), (TEXT_MQTT_SLASH_AT_THE_BEGINNING));
  message.replace(F("{MQTT_SLASH_AT_THE_BEGINNING}"), _addBoolSelect("MQTT_SLASH_AT_THE_BEGINNING", MQTT_SLASH_AT_THE_BEGINNING));

  message.replace(F("{TEXT_MQTT_SLASH_AT_THE_END}"), (TEXT_MQTT_SLASH_AT_THE_END));
  message.replace(F("{MQTT_SLASH_AT_THE_END}"), _addBoolSelect("MQTT_SLASH_AT_THE_END", MQTT_SLASH_AT_THE_END));

  if (!MQTT_SLASH_AT_THE_BEGINNING) {
    message.replace(F("<b>{TEXT_TEMP_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), F("<b>{TEXT_TEMP_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"));
    message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"));
    message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"));
    message.replace(F("<b>{TEXT_PM1_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM1}/{MQTT_PM1}<br />"), F("<b>{TEXT_PM1_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM1}/{MQTT_PM1}<br />"));
    message.replace(F("<b>{TEXT_PM25_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM25}/{MQTT_PM25}<br />"), F("<b>{TEXT_PM25_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM25}/{MQTT_PM25}<br />"));
    message.replace(F("<b>{TEXT_PM10_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM10}/{MQTT_PM10}<br />"), F("<b>{TEXT_PM10_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PM10}/{MQTT_PM10}<br />"));
    message.replace(F("<b>{TEXT_AIRQUALITY_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_AIRQUALITY}/{MQTT_AIRQUALITY}<br />"), F("<b>{TEXT_AIRQUALITY_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_AIRQUALITY}/{MQTT_AIRQUALITY}<br />"));
  }

  if (!MQTT_SLASH_AT_THE_END) {
    message.replace(F("{MQTT_TOPIC_TEMP}/{MQTT_TEMP}<br />"), F("{MQTT_TOPIC_TEMP}={MQTT_TEMP}<br />"));
    message.replace(F("{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), F("{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"));
    message.replace(F("{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), F("{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"));
    message.replace(F("{MQTT_TOPIC_PM1}/{MQTT_PM1}<br />"), F("{MQTT_TOPIC_PM1}={MQTT_PM1}<br />"));
    message.replace(F("{MQTT_TOPIC_PM25}/{MQTT_PM25}<br />"), F("{MQTT_TOPIC_PM25}={MQTT_PM25}<br />"));
    message.replace(F("{MQTT_TOPIC_PM10}/{MQTT_PM10}<br />"), F("{MQTT_TOPIC_PM10}={MQTT_PM10}<br />"));
    message.replace(F("{MQTT_TOPIC_AIRQUALITY}/{MQTT_AIRQUALITY}<br />"), F("{MQTT_TOPIC_AIRQUALITY}={MQTT_AIRQUALITY}<br />"));
  }

  if (strcmp(THP_MODEL, "Non")) {
    takeTHPMeasurements();
    if (!strcmp(THP_MODEL, "BME280")) {
      if (checkBmeStatus() == true) {
        message.replace(F("{MQTT_TEMP}"), String(int(currentTemperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", MQTT_TOPIC_TEMP));

        message.replace(F("{MQTT_HUMI}"), String(int(currentHumidity)));
        message.replace(F("{MQTT_TOPIC_HUMI}"), _addMQTTTextInput("MQTT_TOPIC_HUMI", MQTT_TOPIC_HUMI));

        message.replace(F("{MQTT_PRESS}"), String(int(currentPressure)));
        message.replace(F("{MQTT_TOPIC_PRESS}"), _addMQTTTextInput("MQTT_TOPIC_PRESS", MQTT_TOPIC_PRESS));
      } else {
        if (DEBUG) {
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

    if (!strcmp(THP_MODEL, "BMP280")) {
      if (checkBmpStatus() == true) {
        message.replace(F("{MQTT_TEMP}"), String(int(currentTemperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", MQTT_TOPIC_TEMP));

        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");

        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");

        message.replace(F("{MQTT_PRESS}"), String(int(currentPressure)));
        message.replace(F("{MQTT_TOPIC_PRESS}"), _addMQTTTextInput("MQTT_TOPIC_PRESS", MQTT_TOPIC_PRESS));
      } else {
        if (DEBUG) {
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

    if (!strcmp(THP_MODEL, "HTU21")) {
      if (checkHTU21DStatus() == true) {
        message.replace(F("{MQTT_TEMP}"), String(int(currentTemperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", MQTT_TOPIC_TEMP));

        message.replace(F("{MQTT_HUMI}"), String(int(currentHumidity)));
        message.replace(F("{MQTT_TOPIC_HUMI}"), _addMQTTTextInput("MQTT_TOPIC_HUMI", MQTT_TOPIC_HUMI));

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      } else {
        if (DEBUG) {
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

    if (!strcmp(THP_MODEL, "DHT22")) {
      if (checkDHT22Status() == true) {
        message.replace(F("{MQTT_TEMP}"), String(int(currentTemperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", MQTT_TOPIC_TEMP));

        message.replace(F("{MQTT_HUMI}"), String(int(currentHumidity)));
        message.replace(F("{MQTT_TOPIC_HUMI}"), _addMQTTTextInput("MQTT_TOPIC_HUMI", MQTT_TOPIC_HUMI));

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      } else {
        if (DEBUG) {
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

    if (!strcmp(THP_MODEL, "SHT1x")) {
      if (checkDHT22Status() == true) {
        message.replace(F("{MQTT_TEMP}"), String(int(currentTemperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", MQTT_TOPIC_TEMP));

        message.replace(F("{MQTT_HUMI}"), String(int(currentHumidity)));
        message.replace(F("{MQTT_TOPIC_HUMI}"), _addMQTTTextInput("MQTT_TOPIC_HUMI", MQTT_TOPIC_HUMI));

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      } else {
        if (DEBUG) {
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
    if (!strcmp(THP_MODEL, "DS18B20")) {
      if (checkDS18B20Status()) {

        message.replace(F("{MQTT_TEMP}"), String(int(currentTemperature)));
        message.replace(F("{MQTT_TOPIC_TEMP}"), _addMQTTTextInput("MQTT_TOPIC_TEMP", MQTT_TOPIC_TEMP));

        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}/{MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}/{MQTT_PRESS}<br />"), "");

        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>/{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
        message.replace(F("<b>{TEXT_HUMI_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_HUMI}={MQTT_HUMI}<br />"), "");
        message.replace(F("<b>{TEXT_PRESS_TOPIC}: </b>{MQTT_IP}{MQTT_DEVICENAME}{MQTT_TOPIC_PRESS}={MQTT_PRESS}<br />"), "");
      } else {
        if (DEBUG) {
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

  if (strcmp(DUST_MODEL, "Non")) {
    message.replace(F("{MQTT_PM1}"), String(int(averagePM1)));
    message.replace(F("{MQTT_TOPIC_PM1}"), _addMQTTTextInput("MQTT_TOPIC_PM1", MQTT_TOPIC_PM1));
    message.replace(F("{MQTT_PM25}"), String(int(averagePM25)));
    message.replace(F("{MQTT_TOPIC_PM25}"), _addMQTTTextInput("MQTT_TOPIC_PM25", MQTT_TOPIC_PM25));
    message.replace(F("{MQTT_PM10}"), String(int(averagePM10)));
    message.replace(F("{MQTT_TOPIC_PM10}"), _addMQTTTextInput("MQTT_TOPIC_PM10", MQTT_TOPIC_PM10));

    if (averagePM25 <= 10) {
      message.replace(F("{MQTT_AIRQUALITY}"), "EXCELLENT");
    } else if (averagePM25 > 10 && averagePM25 <= 20) {
      message.replace(F("{MQTT_AIRQUALITY}"), "GOOD");
    } else if (averagePM25 > 20 && averagePM25 <= 25) {
      message.replace(F("{MQTT_AIRQUALITY}"), "FAIR");
    } else if (averagePM25 > 25 && averagePM25 <= 50) {
      message.replace(F("{MQTT_AIRQUALITY}"), "INFERIOR");
    } else if (averagePM25 > 50) {
      message.replace(F("{MQTT_AIRQUALITY}"), "POOR");
    } else {
      message.replace(F("{MQTT_AIRQUALITY}"), "UNKNOWN");
    }
    message.replace(F("{MQTT_TOPIC_AIRQUALITY}"), _addMQTTTextInput("MQTT_TOPIC_AIRQUALITY", MQTT_TOPIC_AIRQUALITY));
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

  if (MQTT_DEVICENAME_IN_TOPIC) {
    message.replace(F("{MQTT_DEVICENAME}"), (String(device_name) + "/"));
  } else {
    message.replace(F("{MQTT_DEVICENAME}"), "");
  }
  if (MQTT_IP_IN_TOPIC) {
    message.replace(F("{MQTT_IP}"), (String(WiFi.localIP().toString()) + "/"));
  } else {
    message.replace(F("{MQTT_IP}"), "");
  }

  message.replace(F("{RestoreConfigButton}"), _addRestoreConfig());
  message.replace(F("{SubmitButton}"), _addSubmitAdvMQTT());
  message += FPSTR(WEB_PAGE_FOOTER);

  if (DEBUG) {
    Serial.print(F("handle_adv_mqtt_config - message.length(): ")); // 18754
    Serial.println(message.length()); // keep it under 20000!
    Serial.print(F("\n"));
  }
  request->send(200, "text/html", message);
  }
  #endif
*/

bool _parseAsBool(String value) {
  return value == "yes";
}

void _set_language() {
  if (!strcmp(LANGUAGE, "english")) {
    SELECTED_LANGUAGE = 1;
  } else if (!strcmp(LANGUAGE, "polish")) {
    SELECTED_LANGUAGE = 2;
  } else {
    SELECTED_LANGUAGE = 1;
  }
}

void _parseAsCString(char* dest, String value, int CStringSize = 255) {
  strncpy(dest, value.c_str(), CStringSize);
}

/*
  void handle_config_device() {
  _handle_config_device(false);
  }

  void handle_config_services() {
  _handle_config_services(false);
  }
*/

//void handle_config_device_post() {
void handle_config_device_save(AsyncWebServerRequest *request) {
  unsigned char need_update = 0;
  // REMEMBER TO ADD/EDIT KEYS IN config.h AND spiffs.cpp!!
  /*
    int paramsNr = request->params();
    if (DEBUG) {
    for(int i=0;i<paramsNr;i++){

     AsyncWebParameter* p = request->getParam(i);

     Serial.print(F("Param name: "));
     Serial.println(p->name());

     Serial.print(F("Param value: "));
     Serial.println(p->value());

     Serial.println(F("------"));
    }
    }
  */

  if (request->hasParam("DEVICENAME_AUTO")) {
    DEVICENAME_AUTO = _parseAsBool(request->getParam("DEVICENAME_AUTO")->value());
  }

  if (request->hasParam("DEVICENAME")) {
    _parseAsCString(DEVICENAME, request->getParam("DEVICENAME")->value(), 32);
  }

  if (request->hasParam("DISPLAY_PM1")) {
    DISPLAY_PM1 = _parseAsBool(request->getParam("DISPLAY_PM1")->value());
  }

  if (request->hasParam("LANGUAGE")) {
    _parseAsCString(LANGUAGE, request->getParam("LANGUAGE")->value(), 32);
    _set_language();
  }

  char oldTHP_MODEL[32];
  strcpy(oldTHP_MODEL, THP_MODEL);
  if (request->hasParam("THP_MODEL")) {
    _parseAsCString(THP_MODEL, request->getParam("THP_MODEL")->value(), 32);
  }
  if (strcmp(THP_MODEL, oldTHP_MODEL) and !strcmp(THP_MODEL, "BME280-SparkFun")) {
    need_update = 1;
  }

  char oldDUST_MODEL[32];
  strcpy(oldDUST_MODEL, DUST_MODEL);
  if (request->hasParam("DUST_MODEL")) {
    _parseAsCString(DUST_MODEL, request->getParam("DUST_MODEL")->value(), 32);
  }

  // DUST Sensor config - START
  if (!strcmp(PMSENSORVERSION, "PMS")) {
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "SDS011/21")) {
      need_update = 2;
    }
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "HPMA115S0")) {
      need_update = 3;
    }
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "SPS30")) {
      need_update = 5;
    }
  } else if (!strcmp(PMSENSORVERSION, "SDS")) {
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "HPMA115S0")) {
      need_update = 3;
    }
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "PMS7003")) {
      need_update = 4;
    }
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "SPS30")) {
      need_update = 5;
    }
  } else if (!strcmp(PMSENSORVERSION, "HPMA115S0")) {
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "SDS011/21")) {
      need_update = 2;
    }
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "PMS7003")) {
      need_update = 4;
    }
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "SPS30")) {
      need_update = 5;
    }
  } else if (!strcmp(PMSENSORVERSION, "SPS30")) {
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "SDS011/21")) {
      need_update = 2;
    }
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "HPMA115S0")) {
      need_update = 3;
    }
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "PMS7003")) {
      need_update = 4;
    }
  } else if (!strcmp(PMSENSORVERSION, "PMS-SparkFunBME280")) {
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "SDS011/21")) {
      need_update = 2;
    }
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "HPMA115S0")) {
      need_update = 3;
    }
    if (strcmp(DUST_MODEL, oldDUST_MODEL) and !strcmp(DUST_MODEL, "SPS30")) {
      need_update = 5;
    }
  }
  // DUST Sensor config - END

  if (request->hasParam("FREQUENTMEASUREMENT")) {
    FREQUENTMEASUREMENT = _parseAsBool(request->getParam("FREQUENTMEASUREMENT")->value());
  }

  if (request->hasParam("DUST_TIME")) {
    DUST_TIME = (request->getParam("DUST_TIME")->value()).toInt();
  }

  if (request->hasParam("NUMBEROFMEASUREMENTS")) {
    NUMBEROFMEASUREMENTS = (request->getParam("NUMBEROFMEASUREMENTS")->value()).toInt();
  }

  if (request->hasParam("DEEPSLEEP_ON")) {
    if (FREQUENTMEASUREMENT == false) {
      DEEPSLEEP_ON = _parseAsBool(request->getParam("DEEPSLEEP_ON")->value());
    }
  }

  if (request->hasParam("MYALTITUDE")) {
    MYALTITUDE = (request->getParam("MYALTITUDE")->value()).toInt();
  }

  if (request->hasParam("CONFIG_AUTH")) {
    CONFIG_AUTH = _parseAsBool(request->getParam("CONFIG_AUTH")->value());
  }

  if (request->hasParam("CONFIG_USERNAME")) {
    _parseAsCString(CONFIG_USERNAME, request->getParam("CONFIG_USERNAME")->value(), 64);
  }

  if (request->hasParam("CONFIG_PASSWORD")) {
    _parseAsCString(CONFIG_PASSWORD, request->getParam("CONFIG_PASSWORD")->value(), 256);
  }

  if (request->hasParam("DEBUG")) {
    DEBUG = _parseAsBool(request->getParam("DEBUG")->value());
  }

  if (request->hasParam("MODEL")) {
    _parseAsCString(MODEL, request->getParam("MODEL")->value(), 32);
  }

  if (request->hasParam("AUTOUPDATE_ON")) {
    AUTOUPDATE_ON = _parseAsBool(request->getParam("AUTOUPDATE_ON")->value());
  }

  if (need_update != 0) {
    strcpy(THP_MODEL, "Non");
    strcpy(DUST_MODEL, "Non");
    saveConfig();
    //_handle_config_device(true);
	unsigned char x = 0;
    if (need_update == 1) {
      x = 1; // BME280-SparkFun
    }
    if (need_update == 2) {
      x = 2; // SDS011
    }
    if (need_update == 3) {
      x = 3; // HPMA115S0
    }
    if (need_update == 4) {
      x = 4; // PMSx003
    }
    if (need_update >= 5) {
      x = 5; // SPS30
    }
    if (need_update >= 6) {
      x = 0; // CURRENT SERVERSOFTWARE VERSION
    }
	doUpdate(x);
  }

  if (DEBUG) {
    Serial.println(F("POST DEVICE CONFIG END!!"));
  }

  saveConfig();
  //delay(250);
  //_handle_config_device(true);
  // https://github.com/esp8266/Arduino/issues/1722
  //ESP.reset();
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
  delay(1000);
  ESP.restart();
}

//void handle_config_services_post() {
void handle_config_services_save(AsyncWebServerRequest *request) {
  /*
    if (DEBUG) {
    Serial.println(F("POST CONFIG START!!"));
    int argsLen = WebServer.args();
    for (int i = 0; i < argsLen; i++) {
      String argName = WebServer.argName(i);
      String arg = WebServer.arg(i);
      String ss = "** ";
      ss += argName;
      ss += " = ";
      ss += arg;
      Serial.println(ss);
    }
    }

    // REMEMBER TO ADD/EDIT KEYS IN config.h AND spiffs.cpp!!
  */
  if (request->hasParam("SENDING_FREQUENCY")) {
    SENDING_FREQUENCY = (request->getParam("SENDING_FREQUENCY")->value()).toInt();
  }

  if (request->hasParam("SENDING_DB_FREQUENCY")) {
    SENDING_DB_FREQUENCY = (request->getParam("SENDING_DB_FREQUENCY")->value()).toInt();
  }

  if (request->hasParam("SMOGLIST_ON")) {
    SMOGLIST_ON = _parseAsBool(request->getParam("SMOGLIST_ON")->value());
  }

  if (request->hasParam("LUFTDATEN_ON")) {
    LUFTDATEN_ON = _parseAsBool(request->getParam("LUFTDATEN_ON")->value());
  }

  if (request->hasParam("AQI_ECO_ON")) {
    AQI_ECO_ON = _parseAsBool(request->getParam("AQI_ECO_ON")->value());
  }

  if (request->hasParam("AQI_ECO_HOST")) {
    _parseAsCString(AQI_ECO_HOST, request->getParam("AQI_ECO_HOST")->value(), 128);
  }

  if (request->hasParam("AQI_ECO_PATH")) {
    _parseAsCString(AQI_ECO_PATH, request->getParam("AQI_ECO_PATH")->value(), 64);
  }

  if (request->hasParam("AIRMONITOR_ON")) {
    AIRMONITOR_ON = _parseAsBool(request->getParam("AIRMONITOR_ON")->value());
  }

  if (request->hasParam("AIRMONITOR_GRAPH_ON")) {
    AIRMONITOR_GRAPH_ON = _parseAsBool(request->getParam("AIRMONITOR_GRAPH_ON")->value());
  }

  if (request->hasParam("AIRMONITOR_API_KEY")) {
    _parseAsCString(AIRMONITOR_API_KEY, request->getParam("AIRMONITOR_API_KEY")->value(), 64);
  }

  if (request->hasParam("LATITUDE")) {
    _parseAsCString(LATITUDE, request->getParam("LATITUDE")->value(), 16);
  }

  if (request->hasParam("LONGITUDE")) {
    _parseAsCString(LONGITUDE, request->getParam("LONGITUDE")->value(), 16);
  }

  if (request->hasParam("THINGSPEAK_ON")) {
    THINGSPEAK_ON = _parseAsBool(request->getParam("THINGSPEAK_ON")->value());
  }

  if (request->hasParam("THINGSPEAK_GRAPH_ON")) {
    THINGSPEAK_GRAPH_ON = _parseAsBool(request->getParam("THINGSPEAK_GRAPH_ON")->value());
  }

  if (request->hasParam("THINGSPEAK_API_KEY")) {
    _parseAsCString(THINGSPEAK_API_KEY, request->getParam("THINGSPEAK_API_KEY")->value(), 32);
  }

  if (request->hasParam("THINGSPEAK_CHANNEL_ID")) {
    THINGSPEAK_CHANNEL_ID = (request->getParam("THINGSPEAK_CHANNEL_ID")->value()).toInt();
  }

  if (request->hasParam("THINGSPEAK_READ_API_KEY")) {
    _parseAsCString(THINGSPEAK_READ_API_KEY, request->getParam("THINGSPEAK_READ_API_KEY")->value(), 32);
  }

  if (request->hasParam("INFLUXDB_ON")) {
    INFLUXDB_ON = _parseAsBool(request->getParam("INFLUXDB_ON")->value());
  }

  if (request->hasParam("INFLUXDB_VERSION")) {
    _parseAsCString(INFLUXDB_VERSION, request->getParam("INFLUXDB_VERSION")->value(), 16);
  }

  if (request->hasParam("INFLUXDB_HOST")) {
    _parseAsCString(INFLUXDB_HOST, request->getParam("INFLUXDB_HOST")->value(), 128);
  }

  if (request->hasParam("INFLUXDB_PORT")) {
    INFLUXDB_PORT = (request->getParam("INFLUXDB_PORT")->value()).toInt();
  }

  if (request->hasParam("INFLUXDB_DATABASE")) {
    _parseAsCString(INFLUXDB_DATABASE, request->getParam("INFLUXDB_DATABASE")->value(), 64);
  }

  if (request->hasParam("INFLUXDB_USER")) {
    _parseAsCString(INFLUXDB_USER, request->getParam("INFLUXDB_USER")->value(), 64);
  }

  if (request->hasParam("INFLUXDB_PASSWORD")) {
    _parseAsCString(INFLUXDB_PASSWORD, request->getParam("INFLUXDB_PASSWORD")->value(), 64);
  }

  if (request->hasParam("INFLUXDB_ORG")) {
    _parseAsCString(INFLUXDB_ORG, request->getParam("INFLUXDB_ORG")->value(), 64);
  }
  if (request->hasParam("INFLUXDB_BUCKET")) {
    _parseAsCString(INFLUXDB_BUCKET, request->getParam("INFLUXDB_BUCKET")->value(), 64);
  }
  if (request->hasParam("INFLUXDB_TOKEN")) {
    _parseAsCString(INFLUXDB_TOKEN, request->getParam("INFLUXDB_TOKEN")->value(), 128);
  }

  if (DEBUG) {
    Serial.println(F("POST SERVICES CONFIG END!!"));
  }

  saveConfig();
  //delay(250);
  //_handle_config_services(true);
  // https://github.com/esp8266/Arduino/issues/1722
  //ESP.reset();
  delay(300);
  request->redirect("/");
  delay(1000);
  ESP.restart();
}

static void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
#ifdef ARDUINO_ARCH_ESP8266
  uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
#elif defined ARDUINO_ARCH_ESP32

#endif
  if (!index) {
    Serial.println(F("Update"));
#ifdef ARDUINO_ARCH_ESP8266
    Update.runAsync(true);
    if (!Update.begin(free_space)) {
      Update.printError(Serial);
    }
#elif defined ARDUINO_ARCH_ESP32

#endif
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

void handle_update_done(AsyncWebServerRequest *request) {
  request->send(200);
}

String handle_update_processor(const String& var)
{
  //Serial.println(F("var: ") + var);
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

  if (!AUTOUPDATE_ON) {
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

  if (AUTOUPDATE_ON) {
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
    message += String(String(CURRENTSOFTWAREVERSION) + " " + String(PMSENSORVERSION));
  }

  if (var == F("{TEXT_SERVERSOFTWAREVERSION}")) {
    message += String(TEXT_SERVERSOFTWAREVERSION);
  }
  if (var == F("{SERVERSOFTWAREVERSION}")) {
    message += String(String(SERVERSOFTWAREVERSION) + " " + String(PMSENSORVERSION));
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

//void handle_update() {            //Handler for the handle_update
void handle_update(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
      return request->requestAuthentication();
  }

  if (DEBUG) {
    Serial.print(F("sizeof(WEB_UPDATE_PAGE_ALL): "));
    Serial.println(sizeof(WEB_UPDATE_PAGE_ALL)); // sizeof(WEB_UPDATE_PAGE_ALL): 2558
    Serial.print(F("\n"));
  }

  request->send_P(200, "text/html", WEB_UPDATE_PAGE_ALL, handle_update_processor);
}

//void erase_wifi() {
void erase_wifi(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
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

//void restore_config() {
void restore_config(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
      return request->requestAuthentication();
  }
  Serial.println(F("Restoring default settings..."));
  deleteConfig();

  request->redirect("/");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

//void fwupdate() {
void fwupdate(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
      return request->requestAuthentication();
  }
  unsigned char x = 0;
  doUpdate(x);
  delay(1000);

  request->redirect("/");
  delay(1000);
}

void handle_adv_mqtt_config_save(AsyncWebServerRequest *request) {
  if (DEBUG) {
    Serial.println(F("handle_adv_mqtt_config_save!"));
  }

  if (request->hasParam("MQTT_ON")) {
    MQTT_ON = _parseAsBool(request->getParam("MQTT_ON")->value());
  }

  if (request->hasParam("MQTT_HOST")) {
    _parseAsCString(MQTT_HOST, request->getParam("MQTT_HOST")->value(), 128);
  }

  if (request->hasParam("MQTT_PORT")) {
    MQTT_PORT = (request->getParam("MQTT_PORT")->value()).toInt();
  }

  if (request->hasParam("MQTT_USER")) {
    _parseAsCString(MQTT_USER, request->getParam("MQTT_USER")->value(), 64);
  }

  if (request->hasParam("MQTT_PASSWORD")) {
    _parseAsCString(MQTT_PASSWORD, request->getParam("MQTT_PASSWORD")->value(), 64);
  }

  if (request->hasParam("MQTT_IP_IN_TOPIC")) {
    MQTT_IP_IN_TOPIC = _parseAsBool(request->getParam("MQTT_IP_IN_TOPIC")->value());
  }

  if (request->hasParam("MQTT_DEVICENAME_IN_TOPIC")) {
    MQTT_DEVICENAME_IN_TOPIC = _parseAsBool(request->getParam("MQTT_DEVICENAME_IN_TOPIC")->value());
  }

  if (request->hasParam("MQTT_SLASH_AT_THE_BEGINNING")) {
    MQTT_SLASH_AT_THE_BEGINNING = _parseAsBool(request->getParam("MQTT_SLASH_AT_THE_BEGINNING")->value());
  }

  if (request->hasParam("MQTT_SLASH_AT_THE_END")) {
    MQTT_SLASH_AT_THE_END = _parseAsBool(request->getParam("MQTT_SLASH_AT_THE_END")->value());
  }

  if (request->hasParam("MQTT_TOPIC_TEMP")) {
    _parseAsCString(MQTT_TOPIC_TEMP, request->getParam("MQTT_TOPIC_TEMP")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_HUMI")) {
    _parseAsCString(MQTT_TOPIC_HUMI, request->getParam("MQTT_TOPIC_HUMI")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_PRESS")) {
    _parseAsCString(MQTT_TOPIC_PRESS, request->getParam("MQTT_TOPIC_PRESS")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_PM1")) {
    _parseAsCString(MQTT_TOPIC_PM1, request->getParam("MQTT_TOPIC_PM1")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_PM25")) {
    _parseAsCString(MQTT_TOPIC_PM25, request->getParam("MQTT_TOPIC_PM25")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_PM10")) {
    _parseAsCString(MQTT_TOPIC_PM10, request->getParam("MQTT_TOPIC_PM10")->value(), 128);
  }

  if (request->hasParam("MQTT_TOPIC_AIRQUALITY")) {
    _parseAsCString(MQTT_TOPIC_AIRQUALITY, request->getParam("MQTT_TOPIC_AIRQUALITY")->value(), 128);
  }

  if (DEBUG) {
    Serial.println(F("handle_adv_mqtt_config_save CONFIG END!!"));
  }

  saveConfig();
  //delay(250);
  // https://github.com/esp8266/Arduino/issues/1722
  //ESP.reset();
  //yield();

  request->redirect("/");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

//void autoupdate_on() {
void autoupdate_on(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
      return request->requestAuthentication();
  }
  AUTOUPDATE_ON = true;
  saveConfig();

  request->redirect("/");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

//void handle_api() {
void handle_api(AsyncWebServerRequest *request) {
  String message;
  StaticJsonDocument<800> jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json[F("device_name")] = device_name;
  if (strcmp(DUST_MODEL, "Non")) {
    json[F("pm1")] = averagePM1;
    json[F("pm25")] = averagePM25;
    if (!strcmp(DUST_MODEL, "SPS30")) {
      json[F("pm4")] = averagePM4;
    }
    json[F("pm10")] = averagePM10;
  }
  if (strcmp(THP_MODEL, "Non")) {
    takeTHPMeasurements();
  }
  if (!strcmp(THP_MODEL, "BME280")) {
    if (checkBmeStatus()) {
      json[F("temperature")] = float(currentTemperature);
      json[F("pressure")] = int(currentPressure);
      json[F("humidity")] = int(currentHumidity);
      json[F("dewpoint")] = float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112);
    }
  }
  if (!strcmp(THP_MODEL, "BMP280")) {
    if (checkBmpStatus()) {
      json[F("temperature")] = float(currentTemperature);
      json[F("pressure")] = int(currentPressure);
    }
  }
  if (!strcmp(THP_MODEL, "HTU21")) {
    if (checkHTU21DStatus()) {
      json[F("temperature")] = float(currentTemperature);
      json[F("humidity")] = int(currentHumidity);
      json[F("dewpoint")] = float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112);
    }
  }
  if (!strcmp(THP_MODEL, "DHT22")) {
    if (checkDHT22Status()) {
      json[F("temperature")] = float(currentTemperature);
      json[F("humidity")] = int(currentHumidity);
      json[F("dewpoint")] = float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112);
    }
  }
  if (!strcmp(THP_MODEL, "SHT1x")) {
    if (checkSHT1xStatus()) {
      json[F("temperature")] = float(currentTemperature);
      json[F("humidity")] = int(currentHumidity);
      json[F("dewpoint")] = float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112);
    }
  }

  serializeJsonPretty(json, message);
  //WebServer.send(200, "text/json", message);
  request->send(200, "text/json", message);
}

void homekit_reset(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
      return request->requestAuthentication();
  }
  Serial.println(F("reset homekit..."));

  String pair_file_name = F("/homekit_pair.dat");
  SPIFFS.remove(pair_file_name);

  request->redirect("/");

  //request->redirect("/");
  delay(1000);
  //Serial.println(F("Restart"));
  //ESP.restart();
}

void homekit_on(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
      return request->requestAuthentication();
  }
  Serial.println(F("homekit on..."));

  HOMEKIT_SUPPORT = true;
  saveConfig();

  request->redirect("/");

  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

void homekit_off(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
      return request->requestAuthentication();
  }
  Serial.println(F("homekit off..."));

  HOMEKIT_SUPPORT = false;
  saveConfig();

  request->redirect("/");

  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}
/*
  void logout(AsyncWebServerRequest *request) {
    if (CONFIG_AUTH == true) {
        if(!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD))
            return request->requestAuthentication();
    }

  Serial.println(F("logout..."));
  request->send(401);
  request->redirect("/");
  }
*/