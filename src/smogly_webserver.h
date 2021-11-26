#pragma once

/*
Default - webserver.h is not used. Instead, we use Asyncwebserver.h.
*/

#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266httpUpdate.h>
#elif defined ARDUINO_ARCH_ESP32
#include <Update.h>
#include <SPIFFS.h>
#endif

#include <ArduinoJson.h> // 6.9.0 or later
#include "smogly_spiffs.h"
#include <FS.h>

const char* www_realm PROGMEM = "Custom Auth Realm";
String authFailResponse = "<meta http-equiv='refresh' content='0; url=/' /> Authentication Failed! <p><a href='/'>Redirect</a></p>";

void handle_root() {
	String message;
	
  message += FPSTR(WEB_PAGE_HEADER);
  message.replace(F("{WEB_PAGE_CSS}"), FPSTR(WEB_PAGE_HEADER_CSS));
  message.replace(F("{Language}"), (TEXT_LANG));
  message.replace(F("{CurrentPageTitle}"), (TEXT_INDEX_PAGE));
  message.replace(F("{IndexPageTitle}"), (TEXT_INDEX_PAGE));
  message.replace(F("{ConfigPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{UpdatePageTitle}"), (TEXT_UPDATE_PAGE));

  message += FPSTR(WEB_ROOT_PAGE_MEASUREMENTS);

  if (!AUTOUPDATE_ON) {
    if (need_update) {
        message.replace(F("{WEB_UPDATE_INFO_WARNING}"), FPSTR(WEB_UPDATE_INFO_WARNING));
        message.replace(F("{TEXT_FWUPDATEAVALIBLE}"), (TEXT_FWUPDATEAVALIBLE));
        message.replace(F("{MANUALUPDATEBUTTON}"), FPSTR(WEB_UPDATE_BUTTON_MANUALUPDATE));
        message.replace(F("{TEXT_MANUALUPDATEBUTTON}"), (TEXT_MANUALUPDATEBUTTON));
        message.replace(F("{FWUPDATEBUTTON}"), FPSTR(WEB_UPDATE_BUTTON_FWUPDATE));
        message.replace(F("{TEXT_FWUPDATEBUTTON}"), (TEXT_FWUPDATEBUTTON));
        message.replace(F("{AUTOUPDATEONBUTTON}"), FPSTR(WEB_UPDATE_BUTTON_AUTOUPDATEON));
        message.replace(F("{TEXT_AUTOUPDATEONBUTTON}"), (TEXT_AUTOUPDATEONBUTTON));
        message.replace(F("{TEXT_AUTOUPDATEWARNING}"), (TEXT_AUTOUPDATEWARNING));
        message.replace(F("{TEXT_FWUPDATEBUTTON}"), (TEXT_FWUPDATEBUTTON));
    }
    message.replace(F("{WEB_UPDATE_INFO_WARNING}"), "");
  } else {
    message.replace(F("{WEB_UPDATE_INFO_WARNING}"), "");
  }

  if (!strcmp(THP_MODEL, "Non")) {
      message.replace(F("{TEXT_WEATHER}:"), "");
      message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
      message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
      message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
      message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
  } else {
    takeTHPMeasurements();
    message.replace(F("{TEXT_WEATHER}"), (TEXT_WEATHER));
  }
  if (!strcmp(THP_MODEL, "BME280")) {
      if (checkBmeStatus()) {
        message.replace(F("{TEXT_TEMPERATURE}"), (TEXT_TEMPERATURE));
        message.replace(F("{TEXT_HUMIDITY}"), (TEXT_HUMIDITY));
        message.replace(F("{TEXT_PRESSURE}"), (TEXT_PRESSURE));
        message.replace(F("{TEXT_DEWPOINT}"), (TEXT_DEWPOINT));

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
        message.replace(F("{TEXT_TEMPERATURE}"), (TEXT_TEMPERATURE));
        message.replace(F("{TEXT_HUMIDITY}"), (TEXT_HUMIDITY));
        message.replace(F("{TEXT_DEWPOINT}"), (TEXT_DEWPOINT));

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
        message.replace(F("{TEXT_TEMPERATURE}"), (TEXT_TEMPERATURE));
        message.replace(F("{TEXT_HUMIDITY}"), (TEXT_HUMIDITY));
        message.replace(F("{TEXT_DEWPOINT}"), (TEXT_DEWPOINT));

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
        message.replace(F("{TEXT_TEMPERATURE}"), (TEXT_TEMPERATURE));
        message.replace(F("{TEXT_PRESSURE}"), (TEXT_PRESSURE));

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
        message.replace(F("{TEXT_TEMPERATURE}"), (TEXT_TEMPERATURE));
        message.replace(F("{TEXT_HUMIDITY}"), (TEXT_HUMIDITY));
        message.replace(F("{TEXT_DEWPOINT}"), (TEXT_DEWPOINT));

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
  } else if (!strcmp(THP_MODEL, "DS18B20")) {
      if (checkDS18B20Status()) {
        message.replace(F("{TEXT_TEMPERATURE}"), (TEXT_TEMPERATURE));
        message.replace(F("{Temperature}"), String(int(currentTemperature)));
        message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
        message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
        message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
      } else {
        message.replace(F("{TEXT_TEMPERATURE}: {Temperature} °C"), "");
        message.replace(F("{TEXT_HUMIDITY}: {Humidity} %"), "");
        message.replace(F("{TEXT_PRESSURE}: {Pressure} hPa"), "");
        message.replace(F("{TEXT_DEWPOINT}: {Dewpoint} °C"), "");
      }
  }

  if (strcmp(DUST_MODEL, "Non")) {
    message.replace(F("{TEXT_AIRPOLLUTION}"), (TEXT_AIRPOLLUTION));

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
  } else {
    message.replace(F("{TEXT_AIRPOLLUTION}:"), "");
    message.replace(F("PM1: {averagePM1} µg/m³"), "");
    message.replace(F("PM2.5: {colorAveragePM25} {averagePM25} µg/m³"), "");
    message.replace(F("PM10: {colorAveragePM10} {averagePM10} µg/m³"), "");
  }
  
  if (SECOND_THP) {
    if (!strcmp(SECOND_THP_MODEL, "BME280")) {
      message += FPSTR(WEB_ROOT_PAGE_MEASUREMENTS_THP2);
      message.replace(F("{TEXT_TEMPERATURE}"), (TEXT_TEMPERATURE));
      message.replace(F("{TEXT_HUMIDITY}"), (TEXT_HUMIDITY));
      message.replace(F("{TEXT_PRESSURE}"), (TEXT_PRESSURE));
      message.replace(F("{TEXT_DEWPOINT}"), (TEXT_DEWPOINT));

      message.replace(F("{Temperature}"), String(int(currentTemperature_THP2)));
      message.replace(F("{Pressure}"), String(int(currentPressure_THP2)));
      message.replace(F("{Humidity}"), String(int(currentHumidity_THP2)));
      message.replace(F("{Dewpoint}"), String(int(pow((currentHumidity_THP2) / 100, 0.125) * (112 + 0.9 * (currentTemperature_THP2)) + 0.1 * (currentTemperature_THP2) - 112)));
    }
  }

  if (AIRMONITOR_GRAPH_ON) {
    message += FPSTR(WEB_ROOT_PAGE_AIRMONITOR_GRAPH);
    message.replace(F("{LATITUDE}"), String(LATITUDE));
    message.replace(F("{LONGITUDE}"), String(LONGITUDE));
  }

  if (THINGSPEAK_GRAPH_ON) {
    message += FPSTR(WEB_ROOT_PAGE_THINGSPEAK_GRAPH);
    message.replace(F("{THINGSPEAK_CHANNEL_ID}"), String(THINGSPEAK_CHANNEL_ID));
	message.replace(F("{THINGSPEAK_READ_API_KEY}"), String(THINGSPEAK_READ_API_KEY));
  }

  message += FPSTR(WEB_PAGE_FOOTER);
  
  if (DEBUG) {
	Serial.print("ROOT - message.length(): ");
    Serial.println(message.length()); // keep it under 20000!
	Serial.print("\n");
  }
  WebServer.send(200, "text/html", message);
}

String _addOption(const String &value, const String &label, const String &srslyValue) {
  String option = FPSTR(WEB_CONFIG_PAGE_ADDOPTION);
  option.replace("{value}", value);  
  if (value == srslyValue) {
    option.replace("{srslyValue}", " selected>");
  } else {
    option.replace("{srslyValue}", ">");
  }
  option.replace("{label}", label);
  return option;
}

String _add_FIRST_THP_Option(const String &value, const String &label, const String &srslyValue) {
  String option = FPSTR(WEB_CONFIG_PAGE_ADDOPTION);
  option.replace("{value}", value);
  
  if (strcmp(DUST_MODEL, "Non")) {
	  if (String(CONFIG_DUST_TX) == value) {
		  option.replace("{srslyValue}", "disabled>");
	  }
  	  if (String(CONFIG_DUST_RX) == value) {
		  option.replace("{srslyValue}", "disabled>");
	  }
	  if (SECOND_THP) {
		  if (String(CONFIG_SECOND_THP_SDA) == value) {
			  option.replace("{srslyValue}", "disabled>");
		  }
	  	  if (String(CONFIG_SECOND_THP_SCL) == value) {
			  option.replace("{srslyValue}", "disabled>");
		  }
	  }
  }
  
  if (value == srslyValue) {
    option.replace("{srslyValue}", " selected>");
  } else {
    option.replace("{srslyValue}", ">");
  }
  option.replace("{label}", label);
  return option;
}

String _add_SECOND_THP_Option(const String &value, const String &label, const String &srslyValue) {
  String option = FPSTR(WEB_CONFIG_PAGE_ADDOPTION);
  option.replace("{value}", value);
  
  if (strcmp(DUST_MODEL, "Non")) {
	  if (String(CONFIG_DUST_TX) == value) {
		  option.replace("{srslyValue}", "disabled>");
	  }
  	  if (String(CONFIG_DUST_RX) == value) {
		  option.replace("{srslyValue}", "disabled>");
	  }
	  if (strcmp(DUST_MODEL, "Non")) {
		  if (String(CONFIG_FIRST_THP_SDA) == value) {
			  option.replace("{srslyValue}", "disabled>");
		  }
	  	  if (String(CONFIG_FIRST_THP_SCL) == value) {
			  option.replace("{srslyValue}", "disabled>");
		  }
	  }
  }
  
  if (value == srslyValue) {
    option.replace("{srslyValue}", " selected>");
  } else {
    option.replace("{srslyValue}", ">");
  }
  option.replace("{label}", label);
  return option;
}

String _add_DUST_Option(const String &value, const String &label, const String &srslyValue) {
  String option = FPSTR(WEB_CONFIG_PAGE_ADDOPTION);
  option.replace("{value}", value);
  
  if (strcmp(THP_MODEL, "Non")) {
	  if (String(CONFIG_FIRST_THP_SDA) == value) {
		  option.replace("{srslyValue}", "disabled>");
	  }
  	  if (String(CONFIG_FIRST_THP_SCL) == value) {
		  option.replace("{srslyValue}", "disabled>");
	  }
	  if (SECOND_THP) {
		  if (String(CONFIG_SECOND_THP_SDA) == value) {
			  option.replace("{srslyValue}", "disabled>");
		  }
	  	  if (String(CONFIG_SECOND_THP_SCL) == value) {
			  option.replace("{srslyValue}", "disabled>");
		  }
	  }
  }
  
  if (value == srslyValue) {
    option.replace("{srslyValue}", " selected>");
  } else {
    option.replace("{srslyValue}", ">");
  }
  option.replace("{label}", label);
  return option;
}

String _addBoolSelect(const String &key, const bool &value) {
  String selectValue = value ? "yes" : "no";
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace("{key}", key);
  input += _addOption("yes", (TEXT_YES), selectValue);
  input += _addOption("no", (TEXT_NO), selectValue);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _addModelSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace("{key}", key);
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
  input.replace("{key}", key);
  input += _addOption("BME280", "BME280", value);
  if (strcmp(PMSENSORVERSION, "PMS-SparkFunBME280")) {
    if (!strcmp(DUST_MODEL, "PMS7003") or !strcmp(DUST_MODEL, "Non")) {
      input += _addOption("BME280-SparkFun", "BME280-SparkFun", value);
    }
  }
  input += _addOption("SHT1x", "SHT1x", value);
  input += _addOption("HTU21", "SHT21/HTU21D", value);
  input += _addOption("DHT22", "DHT22", value);
  input += _addOption("BMP280", "BMP280", value);
  input += _addOption("DS18B20", "DS18B20", value);

  input += _addOption("Non", (TEXT_WITHOUTSENSOR), value);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _addSECOND_THP_MODELSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace("{key}", key);
  input += _addOption("BME280", "BME280", value);
  if (strcmp(PMSENSORVERSION, "PMS-SparkFunBME280")) {
    if (!strcmp(DUST_MODEL, "PMS7003") or !strcmp(DUST_MODEL, "Non")) {
      input += _addOption("BME280-SparkFun", "BME280-SparkFun", value);
    }
  }
  input += _addOption("SHT1x", "SHT1x", value);
  input += _addOption("HTU21", "SHT21/HTU21D", value);
  input += _addOption("DHT22", "DHT22", value);
  input += _addOption("BMP280", "BMP280", value);
  input += _addOption("DS18B20", "DS18B20", value);

  input += _addOption("Non", (TEXT_WITHOUTSENSOR), value);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _addDUST_MODELSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace("{key}", key);
  input += _addOption("PMS7003", "PMS5003/7003", value);
  input += _addOption("SDS011/21", "SDS011/21", value);
  input += _addOption("HPMA115S0", "HPMA115S0", value);
  input += _addOption("SPS30", "SPS30", value);

  input += _addOption("Non", (TEXT_WITHOUTSENSOR), value);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _add_FIRST_THP_SDA_SCL_Select(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace("{key}", key);
  if (!strcmp(THP_MODEL, "DS18B20")) {
	  input += _add_FIRST_THP_Option("D5", "D5/GPIO14", value);
  } else {
  input += _add_FIRST_THP_Option("D1", "D1/GPIO05", value);
  input += _add_FIRST_THP_Option("D2", "D2/GPIO04", value);
  input += _add_FIRST_THP_Option("D3", "D3/GPIO00", value);
  input += _add_FIRST_THP_Option("D4", "D4/GPIO02", value);
  input += _add_FIRST_THP_Option("D5", "D5/GPIO14", value);
  input += _add_FIRST_THP_Option("D6", "D6/GPIO12", value);
  input += _add_FIRST_THP_Option("D7", "D7/GPIO13", value);
  //input += _addOption("D8", "D8/GPIO15", value);
  #ifdef ARDUINO_ARCH_ESP32
  input += _add_FIRST_THP_Option("D16", "D16/GPIO16", value);
  input += _add_FIRST_THP_Option("D17", "D17/GPIO17", value);
  #endif
}
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _add_SECOND_THP_SDA_SCL_Select(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace("{key}", key);
  input += _add_SECOND_THP_Option("D1", "D1/GPIO05", value);
  input += _add_SECOND_THP_Option("D2", "D2/GPIO04", value);
  input += _add_SECOND_THP_Option("D3", "D3/GPIO00", value);
  input += _add_SECOND_THP_Option("D4", "D4/GPIO02", value);
  input += _add_SECOND_THP_Option("D5", "D5/GPIO14", value);
  input += _add_SECOND_THP_Option("D6", "D6/GPIO12", value);
  input += _add_SECOND_THP_Option("D7", "D7/GPIO13", value);
  //input += _addOption("D8", "D8/GPIO15", value);
  #ifdef ARDUINO_ARCH_ESP32
  input += _add_SECOND_THP_Option("D16", "D16/GPIO16", value);
  input += _add_SECOND_THP_Option("D17", "D17/GPIO17", value);
  #endif
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _add_DUST_TX_RX_Select(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace("{key}", key);
  if (!strcmp(DUST_MODEL, "SPS30")) {
	  input += _add_DUST_Option("D1", "D1/GPIO05", value);
	  input += _add_DUST_Option("D2", "D2/GPIO04", value);
  } else {
  input += _add_DUST_Option("D1", "D1/GPIO05", value);
  input += _add_DUST_Option("D2", "D2/GPIO04", value);
  input += _add_DUST_Option("D3", "D3/GPIO00", value);
  input += _add_DUST_Option("D4", "D4/GPIO02", value);
  input += _add_DUST_Option("D5", "D5/GPIO14", value);
  input += _add_DUST_Option("D6", "D6/GPIO12", value);
  input += _add_DUST_Option("D7", "D7/GPIO13", value);
  //input += _addOption("D8", "D8/GPIO15", value);
  #ifdef ARDUINO_ARCH_ESP32
  input += _add_DUST_Option("D16", "D16/GPIO16", value);
  input += _add_DUST_Option("D17", "D17/GPIO17", value);
  #endif
}
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _addINFLUXDB_VERSIONSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace("{key}", key);
  input += _addOption("1", "1.x", value);
  input += _addOption("2", "2.x", value);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _addLanguageSelect(const String &key, const String &value) {
  String input = FPSTR(WEB_CONFIG_PAGE_SELECT);
  input.replace("{key}", key);
  input += _addOption("polish", (TEXT_INTL_PL), value);
  input += _addOption("english", (TEXT_INTL_EN), value);
  input += FPSTR(WEB_CONFIG_PAGE_SELECTEND);
  return input;
}

String _escapeString (const String &value) {
  String trimmed = value;
  trimmed.trim();
  trimmed.replace("'", "&#39;");
  return trimmed;
}

String _addTextInput(const String &key, const String &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_TEXTIMPUT);
  input.replace("{key}", key);
  input.replace("{value}", _escapeString(value));
  input.replace("{postfix}", postfix);
  return input;
}

String _addMQTTTextInput(const String &key, const String &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_MQTT_TEXTIMPUT);
  input.replace("{key}", key);
  input.replace("{value}", _escapeString(value));
  input.replace("{postfix}", postfix);
  return input;
}

String _addPasswdInput(const String &key, const String &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_PASSWDINPUT);
  input.replace("{key}", key);
  input.replace("{value}", _escapeString(value));
  input.replace("{postfix}", postfix);
  return input;
}

String _addIntInput(const String &key, const int &value, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_INTINPUT);
  input.replace("{key}", key);
  input.replace("{value}", String(value));
  input.replace("{postfix}", postfix);
  return input;
}

String _addFloatInput(const String &key, const double &value, const int &precision = 6, const String &postfix = "") {
  String input = FPSTR(WEB_CONFIG_PAGE_FLOATINPUT);
  input.replace("{key}", key);
  input.replace("{value}", String(value, precision));
  input.replace("{postfix}", postfix);
  return input;
}

String _addSubmitDevice() {
  String submitDevice = FPSTR(WEB_CONFIG_PAGE_SUBMIT_DEVICE_BUTTON);
  submitDevice.replace("{TEXT_SAVE}", (TEXT_SAVE));
  return submitDevice;
}

String _addSubmitServices() {
  String submitServices = FPSTR(WEB_CONFIG_PAGE_SUBMIT_SERVICES_BUTTON);
  submitServices.replace("{TEXT_SAVE}", (TEXT_SAVE));
  return submitServices;
}

String _addSubmitAdvMQTT() {
  String submitAdvMQTT = FPSTR(WEB_CONFIG_ADVANCED_MQTT_PAGE_SUBMIT_SERVICES_BUTTON);
  submitAdvMQTT.replace("{TEXT_SAVE}", (TEXT_SAVE));
  return submitAdvMQTT;
}

String _addWiFiErase() {
  String WiFiErase = FPSTR(WEB_CONFIG_PAGE_WIFIERASE);
  WiFiErase.replace("{TEXT_ERASEWIFICONFIG}", (TEXT_ERASEWIFICONFIG));
  return WiFiErase;
}

String _addRestoreConfig() {
  String RestoreConfig = FPSTR(WEB_CONFIG_PAGE_RESTORECONFIG);
  RestoreConfig.replace("{TEXT_RESTORESETTINGS}", (TEXT_RESTORESETTINGS));
  return RestoreConfig;
}

String _add_homekit_reset() {
  String homekit_reset = FPSTR(WEB_CONFIG_PAGE_HOMEKIT_RESET);
  homekit_reset.replace("{TEXT_PAGE_HOMEKIT_RESET}", (TEXT_PAGE_HOMEKIT_RESET));
  return homekit_reset;
}

void handle_config() {
  String message = FPSTR(WEB_PAGE_HEADER);
  message.replace(F("{WEB_PAGE_CSS}"), FPSTR(WEB_PAGE_HEADER_CSS));
  message.replace(F("{Language}"), (TEXT_LANG));
  message.replace(F("{CurrentPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{IndexPageTitle}"), (TEXT_INDEX_PAGE));
  message.replace(F("{ConfigPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{UpdatePageTitle}"), (TEXT_UPDATE_PAGE));

message += FPSTR(WEB_CONFIG_PAGE_TOP);
message.replace(F("{TEXT_CONFIG_PAGE}"), (TEXT_CONFIG_PAGE));

message += FPSTR(WEB_CONFIG_PAGE_CONFIG);
message.replace(F("{TEXT_CONFIG_INFO_DEVICE_PAGE}"), (TEXT_CONFIG_INFO_DEVICE_PAGE));
message.replace(F("{TEXT_CONFIG_DEVICE_PAGE}"), (TEXT_CONFIG_DEVICE_PAGE));

message.replace(F("{TEXT_CONFIG_INFO_SERVICES_PAGE}"), (TEXT_CONFIG_INFO_SERVICES_PAGE));
message.replace(F("{TEXT_CONFIG_SERVICES_PAGE}"), (TEXT_CONFIG_SERVICES_PAGE));

message.replace(F("{ConfigDeviceButton}"), FPSTR(WEB_CONFIG_DEVICE_PAGE_BUTTON));
message.replace(F("{TEXT_CONFIG_DEVICE_PAGE}"), (TEXT_CONFIG_DEVICE_PAGE));
message.replace(F("{ConfigServicesButton}"), FPSTR(WEB_CONFIG_SERVICES_PAGE_BUTTON));
message.replace(F("{TEXT_CONFIG_SERVICES_PAGE}"), (TEXT_CONFIG_SERVICES_PAGE));

message.replace(F("{WiFiEraseButton}"), _addWiFiErase());
message.replace(F("{RestoreConfigButton}"), _addRestoreConfig());

message += FPSTR(WEB_PAGE_FOOTER);
  WebServer.send(200, "text/html", message);
}

void _handle_config_device(bool is_success) {
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
  }

  String message = FPSTR(WEB_PAGE_HEADER);
  message.replace(F("{WEB_PAGE_CSS}"), FPSTR(WEB_PAGE_HEADER_CSS));
  message.replace(F("{Language}"), (TEXT_LANG));
  message.replace(F("{CurrentPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{IndexPageTitle}"), (TEXT_INDEX_PAGE));
  message.replace(F("{ConfigPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{UpdatePageTitle}"), (TEXT_UPDATE_PAGE));

  message += FPSTR(WEB_CONFIG_DEVICE_PAGE_TOP);
  message.replace(F("{TEXT_CONFIG_DEVICE_PAGE}"), (TEXT_CONFIG_DEVICE_PAGE));

  if (!is_success) {
      message.replace(F("{WEB_CONFIG_TOP_PAGE_INFO}"), "");
    } else {
  	  message.replace(F("{WEB_CONFIG_TOP_PAGE_INFO}"), FPSTR(WEB_CONFIG_TOP_PAGE_INFO));
      message.replace(F("{TEXT_SAVED}"), (TEXT_SAVED));
      message.replace(F("{TEXT_POSTCONFIG_INFO}"), (TEXT_POSTCONFIG_INFO));
  }

  message.replace(F("{TEXT_INSTRUCIONSLINK}"), (TEXT_INSTRUCIONSLINK));
  message.replace(F("{GITHUB_LINK}"), String(GITHUB_LINK));
  message.replace(F("{TEXT_HERE}"), (TEXT_HERE));

  message += FPSTR(WEB_CONFIG_DEVICE_PAGE_CONFIG);

  message.replace(F("{TEXT_DEVICENAME}"), (TEXT_DEVICENAME));
  if (DEVICENAME_AUTO) {
    message.replace(F("{device_name}"), (device_name));
  } else {
    message.replace(F("{device_name}"), _addTextInput("DEVICENAME", DEVICENAME));
  }

  message.replace(F("{TEXT_DEVICENAMEAUTO}"), (TEXT_DEVICENAMEAUTO));
  message.replace(F("{DEVICENAME_AUTO}"), _addBoolSelect("DEVICENAME_AUTO", DEVICENAME_AUTO));
  message.replace(F("{TEXT_SELECTEDLANGUAGE}"), (TEXT_SELECTEDLANGUAGE));
  message.replace(F("{LanguageSelect}"), _addLanguageSelect("LANGUAGE", LANGUAGE));
  message.replace(F("{TEXT_TEMPHUMIPRESSSENSOR}"), (TEXT_TEMPHUMIPRESSSENSOR));
  message.replace(F("{THP_MODELSelect}"), _addTHP_MODELSelect("THP_MODEL", THP_MODEL));
  message.replace(F("{TEXT_PMSENSOR}"), (TEXT_PMSENSOR));
  message.replace(F("{DUST_MODELSelect}"), _addDUST_MODELSelect("DUST_MODEL", DUST_MODEL));


 if (!strcmp(THP_MODEL, "Non")) {
	message.replace(F("<hr>{TEXT_FIRST_THP_SDA_SCL} - {THP_MODEL}<br>"), "");
	message.replace(F("<b>{TEXT_FIRST_THP_SDA}: </b>{FIRST_THP_SDA}"), "");
	message.replace(F("<b>{TEXT_FIRST_THP_SCL}: </b>{FIRST_THP_SCL}"), "");
 } else {
 	message.replace(F("{THP_MODEL}"), (THP_MODEL));
 }

 if (!strcmp(DUST_MODEL, "Non")) {
 	message.replace(F("<br>{TEXT_DUST_TX_RX} - {DUST_MODEL}<br>"), "");
	message.replace(F("<b>{TEXT_DUST_TX}: </b>{DUST_TX}"), "");
	message.replace(F("<b>{TEXT_DUST_RX}: </b>{DUST_RX}"), "");
 } else {
	  if (!strcmp(THP_MODEL, "Non")) {
		 message.replace(F("<br>{TEXT_DUST_TX_RX} - {DUST_MODEL}<br>"), "<hr>{TEXT_DUST_TX_RX} - {DUST_MODEL}<br>"); 
	  }
 	message.replace(F("{DUST_MODEL}"), (DUST_MODEL));
 }


message.replace(F("{TEXT_FIRST_THP_SDA_SCL}"), (TEXT_FIRST_THP_SDA_SCL));
message.replace(F("{TEXT_FIRST_THP_SDA}"), (TEXT_FIRST_THP_SDA));
message.replace(F("{FIRST_THP_SDA}"), _add_FIRST_THP_SDA_SCL_Select("CONFIG_FIRST_THP_SDA", CONFIG_FIRST_THP_SDA));
if (!strcmp(THP_MODEL, "DHT22") or !strcmp(THP_MODEL, "DS18B20")) {
	message.replace(F("<b>{TEXT_FIRST_THP_SCL}: </b>{FIRST_THP_SCL}"), "");
} else {
	message.replace(F("{TEXT_FIRST_THP_SCL}"), (TEXT_FIRST_THP_SCL));
	message.replace(F("{FIRST_THP_SCL}"), _add_FIRST_THP_SDA_SCL_Select("CONFIG_FIRST_THP_SCL", CONFIG_FIRST_THP_SCL));
}
message.replace(F("{TEXT_SECOND_THP}"), (TEXT_SECOND_THP));
message.replace(F("{SECOND_THP}"), _addBoolSelect("SECOND_THP", SECOND_THP));

if(SECOND_THP) {
    message.replace(F("{WEB_CONFIG_DEVICE_PAGE_SECOND_THP}"), FPSTR(WEB_CONFIG_DEVICE_PAGE_SECOND_THP));
	message.replace(F("{TEXT_SECOND_THP_SDA_SCL}"), (TEXT_SECOND_THP_SDA_SCL));
	message.replace(F("{TEXT_SECOND_THP_SDA}"), (TEXT_SECOND_THP_SDA));
	message.replace(F("{SECOND_THP_SDA}"), _add_SECOND_THP_SDA_SCL_Select("CONFIG_SECOND_THP_SDA", CONFIG_SECOND_THP_SDA));
	message.replace(F("{TEXT_SECOND_THP_SCL}"), (TEXT_SECOND_THP_SCL));
	message.replace(F("{SECOND_THP_SCL}"), _add_SECOND_THP_SDA_SCL_Select("CONFIG_SECOND_THP_SCL", CONFIG_SECOND_THP_SCL));
} else {
	message.replace(F("{WEB_CONFIG_DEVICE_PAGE_SECOND_THP}"), "");
}

if(SECOND_THP) {
	message.replace(F("{}"), "");
	message.replace(F("{}"), "");
} else {
	message.replace(F("{}"), "");
}

message.replace(F("{TEXT_DUST_TX_RX}"), (TEXT_DUST_TX_RX));
message.replace(F("{TEXT_DUST_TX}"), (TEXT_DUST_TX));
message.replace(F("{DUST_TX}"), _add_DUST_TX_RX_Select("CONFIG_DUST_TX", CONFIG_DUST_TX));
message.replace(F("{TEXT_DUST_RX}"), (TEXT_DUST_RX));
message.replace(F("{DUST_RX}"), _add_DUST_TX_RX_Select("CONFIG_DUST_RX", CONFIG_DUST_RX));

  message.replace(F("{TEXT_FREQUENTMEASUREMENTONOFF}"), (TEXT_FREQUENTMEASUREMENTONOFF));
  message.replace(F("{TEXT_FREQUENTMEASUREMENTINFO}"), (TEXT_FREQUENTMEASUREMENTINFO));
  message.replace(F("{TEXT_MEASUREMENTFREQUENCY}"), (TEXT_MEASUREMENTFREQUENCY));

  message.replace(F("{FREQUENTMEASUREMENT_Select}"), _addBoolSelect("FREQUENTMEASUREMENT", FREQUENTMEASUREMENT));

  if (FREQUENTMEASUREMENT == true) {
    message.replace(F("{FREQUENTMEASUREMENT_time}"), _addIntInput("DUST_TIME", DUST_TIME, "{TEXT_SECONDS}"));
    message.replace(F("{TEXT_SECONDS}"), (TEXT_SECONDS));
  } else {
    message.replace(F("{FREQUENTMEASUREMENT_time}"), _addIntInput("DUST_TIME", DUST_TIME, "{TEXT_MINUTES}"));
    message.replace(F("{TEXT_MINUTES}"), (TEXT_MINUTES));
  }

  message.replace(F("{TEXT_AVERAGELASTRESULT}"), (TEXT_AVERAGELASTRESULT));
  message.replace(F("{NUMBEROFMEASUREMENTS}"), _addIntInput("NUMBEROFMEASUREMENTS", NUMBEROFMEASUREMENTS, "{TEXT_PMMEASUREMENTS}"));
  message.replace(F("{TEXT_PMMEASUREMENTS}"), (TEXT_PMMEASUREMENTS));

  if (FREQUENTMEASUREMENT == true) {
    message.replace(F("<hr><b>DeepSleep: </b>{DEEPSLEEP_ON} {TEXT_DEEPSLEEPINFO}"), "");
  } else {
#ifdef ARDUINO_ARCH_ESP8266
    message.replace(F("{TEXT_DEEPSLEEPINFO}"), TEXT_DEEPSLEEPINFO);
#elif defined ARDUINO_ARCH_ESP32
    message.replace(F("{TEXT_DEEPSLEEPINFO}"), TEXT_DEEPSLEEPINFO_ESP32);
#endif
    message.replace(F("{INTERFACEWWWONTIME}"), String(int(NUMBEROFMEASUREMENTS) * 2 + 10 ));
    message.replace(F("{SENDING_FREQUENCY}"), String(SENDING_FREQUENCY));
    message.replace(F("{DEEPSLEEP_ON}"), _addBoolSelect("DEEPSLEEP_ON", DEEPSLEEP_ON));
  }

  if (!strcmp(DUST_MODEL, "PMS7003") or !strcmp(DUST_MODEL, "SPS30")) {
    message.replace(F("{DISPLAY_PM1}"), _addBoolSelect("DISPLAY_PM1", DISPLAY_PM1));
    message.replace(F("{TEXT_DISPLAYPM1}"), (TEXT_DISPLAYPM1));
  } else {
    message.replace(F("<b>{TEXT_DISPLAYPM1}: </b> {DISPLAY_PM1}"), "");
  }
  message.replace(F("{TEXT_ALTITUDEINFO}"), (TEXT_ALTITUDEINFO));
  message.replace(F("{WSPOLRZEDNE_GPS_LINK}"), String(WSPOLRZEDNE_GPS_LINK));
  message.replace(F("{TEXT_HERE}"), (TEXT_HERE));
  message.replace(F("{MYALTITUDE}"), _addIntInput("MYALTITUDE", MYALTITUDE, "m.n.p.m"));

  message.replace(F("{TEXT_SECURECONFIGUPDATEPAGE}"), (TEXT_SECURECONFIGUPDATEPAGE));
  message.replace(F("{CONFIG_AUTH}"), _addBoolSelect("CONFIG_AUTH", CONFIG_AUTH));
  message.replace(F("{TEXT_SECURELOGIN}"), (TEXT_SECURELOGIN));

  message.replace(F("{CONFIG_USERNAME}"), _addTextInput("CONFIG_USERNAME", CONFIG_USERNAME));
  message.replace(F("{TEXT_SECUREPASSWD}"), (TEXT_SECUREPASSWD));
  message.replace(F("{CONFIG_PASSWORD}"), _addPasswdInput("CONFIG_PASSWORD", CONFIG_PASSWORD));

  if (!CONFIG_AUTH) {
    message.replace(F("{TEXT_SECURELOGOUTINFO}"), "");
  } else {
    message.replace(F("{TEXT_SECURELOGOUTINFO}"), (TEXT_SECURELOGOUTINFO));
  }

  message.replace(F("{DEBUG}"), _addBoolSelect("DEBUG", DEBUG));
  message.replace(F("{TEXT_CALIBMETHOD}"), (TEXT_CALIBMETHOD));
  message.replace(F("{CalibrationModelSelect}"), _addModelSelect("MODEL", MODEL));
  message.replace(F("{TEXT_CALIB1}"), (TEXT_CALIB1));
  message.replace(F("{calib1}"), String(calib1));
  message.replace(F("{TEXT_CALIB2}"), (TEXT_CALIB2));
  message.replace(F("{calib2}"), String(calib2));

  message.replace(F("{TEXT_SOFTWATEVERSION}"), (TEXT_SOFTWATEVERSION));
  message.replace(F("{SOFTWAREVERSION}"), SOFTWAREVERSION);

  message.replace(F("{TEXT_AUTOUPDATEON}"), TEXT_AUTOUPDATEON);
  message.replace(F("{AUTOUPDATEON}"), _addBoolSelect("AUTOUPDATE_ON", AUTOUPDATE_ON));

#ifdef ARDUINO_ARCH_ESP8266
  message.replace(F("{TEXT_UPDATEPAGEAUTOUPDATEWARNING}"), TEXT_UPDATEPAGEAUTOUPDATEWARNING);
#elif defined ARDUINO_ARCH_ESP32
  message.replace(F("{TEXT_UPDATEPAGEAUTOUPDATEWARNING}"), "");
#endif
  
#ifdef ARDUINO_ARCH_ESP8266
  message.replace(F("{TEXT_HOMEKIT_SUPPORT}"), "");
  message.replace(F("{HOMEKIT_SUPPORT_ON}"), "");
#elif defined ARDUINO_ARCH_ESP32
  message.replace(F("{TEXT_HOMEKIT_SUPPORT}"), TEXT_HOMEKIT_SUPPORT);
  message.replace(F("{HOMEKIT_SUPPORT_ON}"), _addBoolSelect("HOMEKIT_SUPPORT", HOMEKIT_SUPPORT));
  if (HOMEKIT_SUPPORT == true) {
      // Serial.println("homekit_is_paired: " + String(homekit_is_paired()));
	  if (String(homekit_is_paired()) == "1") {
		  message.replace(F("{TEXT_HOMEKIT_IS_PAIRED}"), (TEXT_HOMEKIT_IS_PAIRED));
		  message.replace(F("{HOMEKIT_PAIRED_RESET}"), _add_homekit_reset());  
	  } else {
		  message.replace(F("<b>{TEXT_HOMEKIT_IS_PAIRED}: </b>{HOMEKIT_PAIRED_RESET}"), "");  		
	  } 	
  } else {
	  message.replace(F("<b>{TEXT_HOMEKIT_IS_PAIRED}: </b>{HOMEKIT_PAIRED_RESET}"), "");  	
  }
	  
#endif

  message.replace(F("{WiFiEraseButton}"), _addWiFiErase());
  message.replace(F("{RestoreConfigButton}"), _addRestoreConfig());
  message.replace(F("{SubmitButton}"), _addSubmitDevice());
  message += FPSTR(WEB_PAGE_FOOTER);

  WebServer.send(200, "text/html", message);
}

void _handle_config_services(bool is_success) {
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
  }

  String message = FPSTR(WEB_PAGE_HEADER);
  message.replace(F("{WEB_PAGE_CSS}"), FPSTR(WEB_PAGE_HEADER_CSS));
  message.replace(F("{Language}"), (TEXT_LANG));
  message.replace(F("{CurrentPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{IndexPageTitle}"), (TEXT_INDEX_PAGE));
  message.replace(F("{ConfigPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{UpdatePageTitle}"), (TEXT_UPDATE_PAGE));

  message += FPSTR(WEB_CONFIG_SERVICES_PAGE_TOP);
  message.replace(F("{TEXT_CONFIG_SERVICES_PAGE}"), (TEXT_CONFIG_SERVICES_PAGE));

  if (!is_success) {
    message.replace(F("{WEB_CONFIG_TOP_PAGE_INFO}"), "");
  } else {
	message.replace(F("{WEB_CONFIG_TOP_PAGE_INFO}"), FPSTR(WEB_CONFIG_TOP_PAGE_INFO));
    message.replace(F("{TEXT_SAVED}"), (TEXT_SAVED));
    message.replace(F("{TEXT_POSTCONFIG_INFO}"), (TEXT_POSTCONFIG_INFO));
  }

  message.replace(F("{TEXT_INSTRUCIONSLINK}"), (TEXT_INSTRUCIONSLINK));
  message.replace(F("{GITHUB_LINK}"), String(GITHUB_LINK));
  message.replace(F("{TEXT_HERE}"), (TEXT_HERE));

  message += FPSTR(WEB_CONFIG_SERVICES_PAGE_CONFIG);

  if (FREQUENTMEASUREMENT == true) {
    message.replace(F("{TEXT_SENDINGINTERVAL}"), (TEXT_SERVICESSENDINGINTERVAL));
    message.replace(F("{SENDING_FREQUENCY}"), _addIntInput("SENDING_FREQUENCY", SENDING_FREQUENCY, "{TEXT_SECONDS}"));
    message.replace(F("{TEXT_SECONDS}"), (TEXT_SECONDS));

    message.replace(F("{TEXT_DBSENDINGINTERVAL}"), (TEXT_DBSENDINGINTERVAL));
    message.replace(F("{SENDING_DB_FREQUENCY}"), _addIntInput("SENDING_DB_FREQUENCY", SENDING_DB_FREQUENCY, "{TEXT_SECONDS}"));
    message.replace(F("{TEXT_SECONDS}"), (TEXT_SECONDS));
  } else {
    if (DEEPSLEEP_ON == true) {
      message.replace(F("{TEXT_SENDINGINTERVAL}"), (TEXT_SENDINGINTERVAL));
      message.replace(F("{SENDING_FREQUENCY}"), _addIntInput("SENDING_FREQUENCY", SENDING_FREQUENCY, "{TEXT_MINUTES}"));
      message.replace(F("{TEXT_MINUTES}"), (TEXT_MINUTES));

      message.replace(F("<b>{TEXT_DBSENDINGINTERVAL}: </b>{SENDING_DB_FREQUENCY}"), "");
    } else {
      message.replace(F("{TEXT_SENDINGINTERVAL}"), (TEXT_SERVICESSENDINGINTERVAL));
      message.replace(F("{SENDING_FREQUENCY}"), _addIntInput("SENDING_FREQUENCY", SENDING_FREQUENCY, "{TEXT_MINUTES}"));
      message.replace(F("{TEXT_MINUTES}"), (TEXT_MINUTES));

      message.replace(F("{TEXT_DBSENDINGINTERVAL}"), (TEXT_DBSENDINGINTERVAL));
      message.replace(F("{SENDING_DB_FREQUENCY}"), _addIntInput("SENDING_DB_FREQUENCY", SENDING_DB_FREQUENCY, "{TEXT_MINUTES}"));
      message.replace(F("{TEXT_MINUTES}"), (TEXT_MINUTES));
    }
}

  message.replace(F("{TEXT_SMOGLISTSENDING}"), (TEXT_SMOGLISTSENDING));
  message.replace(F("{SMOGLIST_LINK}"), String(SMOGLIST_LINK));
  message.replace(F("{SMOGLIST_ON}"), _addBoolSelect("SMOGLIST_ON", SMOGLIST_ON));
  message.replace(F("{TEXT_SMOGLISTINFO}"), (TEXT_SMOGLISTINFO));

  message.replace(F("{TEXT_LUFTDATENSENDING}"), (TEXT_LUFTDATENSENDING));
  message.replace(F("{LUFTDATEN_LINK}"), (LUFTDATEN_LINK));
  message.replace(F("{LUFTDATENFORM_LINK}"), (LUFTDATENFORM_LINK));
  message.replace(F("{LUFTDATEN_ON}"), _addBoolSelect("LUFTDATEN_ON", LUFTDATEN_ON));

#ifdef ARDUINO_ARCH_ESP8266
  message.replace(F("{LUFTDATEN_ChipID}"), "smogomierz-" + String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
  //message.replace("{ChipID}", "smogomierz-" + (ESP.getEfuseMac()));
  message.replace(F("{LUFTDATEN_ChipID}"), "smogomierz-" + String((uint32_t)(ESP.getEfuseMac())));
#endif

  if (!strcmp(THP_MODEL, "BME280")) {
    message.replace(F("{THPSENSOR}"), "BME280");
    message.replace(F("{THPXPIN}"), "11");
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    message.replace(F("{THPSENSOR}"), "BMP280");
    message.replace(F("{THPXPIN}"), "3");
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    message.replace(F("{THPSENSOR}"), "HTU21");
    message.replace(F("{THPXPIN}"), "7");
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    message.replace(F("{THPSENSOR}"), "DHT22");
    message.replace(F("{THPXPIN}"), "7");
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
    message.replace(F("{THPSENSOR}"), "SHT1x");
    message.replace(F("{THPXPIN}"), "12");
  } else if (!strcmp(THP_MODEL, "DS18B20")) {
    message.replace(F("{THPSENSOR}"), "DS18B20");
    message.replace(F("{THPXPIN}"), "13");
  } else {
    message.replace(F("<br><b>{THPSENSOR}</b> Sensor PIN: <b>{THPXPIN}</b>"), "");
  }

  if (!strcmp(DUST_MODEL, "PMS7003")) {
    message.replace(F("{DUSTSENSOR}"), "PMS5003/7003");
    message.replace(F("{DUSTXPIN}"), "1");
  } else if (!strcmp(DUST_MODEL, "SDS011/21")) {
    message.replace(F("{DUSTSENSOR}"), "SDS011/21");
    message.replace(F("{DUSTXPIN}"), "1");
  } else if (!strcmp(DUST_MODEL, "HPMA115S0")) {
    message.replace(F("{DUSTSENSOR}"), "HPMA115S0");
    message.replace(F("{DUSTXPIN}"), "1");
  } else if (!strcmp(DUST_MODEL, "SPS30")) {
    message.replace(F("{DUSTSENSOR}"), "SPS30");
    message.replace(F("{DUSTXPIN}"), "1");
  } else {
    message.replace(F("<br><b>{DUSTSENSOR}</b> Sensor PIN: <b>{DUSTXPIN}</b>"), "");
  }

  message.replace(F("{TEXT_AQIECOSENDING}"), (TEXT_AQIECOSENDING));
  message.replace(F("{AQI_ECO_ON}"), _addBoolSelect("AQI_ECO_ON", AQI_ECO_ON));
  message.replace(F("{TEXT_AQIECOSERVER}"), (TEXT_AQIECOSERVER));
  message.replace(F("{AQI_ECO_HOST}"), _addTextInput("AQI_ECO_HOST", AQI_ECO_HOST));
  message.replace(F("{TEXT_AQIECOPATH}"), (TEXT_AQIECOPATH));
  message.replace(F("{AQI_ECO_PATH}"), _addTextInput("AQI_ECO_PATH", AQI_ECO_PATH));

#ifdef ARDUINO_ARCH_ESP8266
  message.replace(F("{ESP_MODEL}"), "ESP8266");
  message.replace(F("{AQI_ECO_ChipID}"), String(ESP.getChipId()));
#elif defined ARDUINO_ARCH_ESP32
  //message.replace("{ChipID}", "smogomierz-" + (ESP.getEfuseMac()));
  message.replace(F("{ESP_MODEL}"), "ESP32");
  message.replace(F("{AQI_ECO_ChipID}"), String((uint32_t)(ESP.getEfuseMac())));
#endif

  message.replace(F("{TEXT_AIRMONITORSENDING}"), (TEXT_AIRMONITORSENDING));

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
  message.replace(F("{PMSENSORMODEL}"), PMSENSORMODEL);

  message.replace(F("{AIRMONITOR_LINK}"), (AIRMONITOR_LINK));
  message.replace(F("{AIRMONITORFORM_LINK}"), (AIRMONITORFORM_LINK));
  message.replace(F("{TEXT_THEFORM}"), (TEXT_THEFORM));
  message.replace(F("{TEXT_AIRMONITORCHARTS}"), (TEXT_AIRMONITORCHARTS));
  message.replace(F("{AIRMONITOR_ON}"), _addBoolSelect("AIRMONITOR_ON", AIRMONITOR_ON));

  message.replace(F("{TEXT_AIRMONITORCOORDINATESINFO}"), (TEXT_AIRMONITORCOORDINATESINFO));
  message.replace(F("{LATLONG_LINK}"), (LATLONG_LINK));
  message.replace(F("{TEXT_HERE}"), (TEXT_HERE));

  message.replace(F("{AIRMONITOR_GRAPH_ON}"), _addBoolSelect("AIRMONITOR_GRAPH_ON", AIRMONITOR_GRAPH_ON));
  message.replace(F("{TEXT_AIRMONITORLATITUDE}"), (TEXT_AIRMONITORLATITUDE));

  message.replace(F("{LATITUDE}"), _addFloatInput("LATITUDE", atof(LATITUDE), 6, "°"));
  message.replace(F("{TEXT_AIRMONITORLONGITUDE}"), (TEXT_AIRMONITORLONGITUDE));
  message.replace(F("{LONGITUDE}"), _addFloatInput("LONGITUDE", atof(LONGITUDE), 6, "°"));

  message.replace(F("{TEXT_THINGSPEAKSENDING}"), (TEXT_THINGSPEAKSENDING));
  message.replace(F("{THINGSPEAK_LINK}"), (THINGSPEAK_LINK));
  message.replace(F("{THINGSPEAK_ON}"), _addBoolSelect("THINGSPEAK_ON", THINGSPEAK_ON));
  message.replace(F("{TEXT_THINGSPEAKCHARTS}"), (TEXT_THINGSPEAKCHARTS));
  message.replace(F("{THINGSPEAK_GRAPH_ON}"), _addBoolSelect("THINGSPEAK_GRAPH_ON", THINGSPEAK_GRAPH_ON));
  message.replace(F("{TEXT_THINGSPEAK_WRITE_API_KEY}"), (TEXT_THINGSPEAK_WRITE_API_KEY));
  message.replace(F("{THINGSPEAK_API_KEY}"), _addTextInput("THINGSPEAK_API_KEY", THINGSPEAK_API_KEY));
  message.replace(F("{TEXT_THINGSPEAKCHANNELID}"), (TEXT_THINGSPEAKCHANNELID));
  message.replace(F("{THINGSPEAK_CHANNEL_ID}"), _addIntInput("THINGSPEAK_CHANNEL_ID", THINGSPEAK_CHANNEL_ID));
  message.replace(F("{TEXT_THINGSPEAK_READ_API_KEY}"), (TEXT_THINGSPEAK_READ_API_KEY));
  message.replace(F("{THINGSPEAK_READ_API_KEY}"), _addTextInput("THINGSPEAK_READ_API_KEY", THINGSPEAK_READ_API_KEY));

  message.replace(F("{TEXT_INFLUXDBSENDING}"), (TEXT_INFLUXDBSENDING));
  message.replace(F("{INFLUXDB_ON}"), _addBoolSelect("INFLUXDB_ON", INFLUXDB_ON));
  message.replace(F("{TEXT_INFLUXDBVERSION}"), (TEXT_INFLUXDBVERSION));
  message.replace(F("{INFLUXDB_VERSION}"), _addINFLUXDB_VERSIONSelect("INFLUXDB_VERSION", INFLUXDB_VERSION));
  message.replace(F("{TEXT_INFLUXDBSERVER}"), (TEXT_INFLUXDBSERVER));
  message.replace(F("{INFLUXDB_HOST}"), _addTextInput("INFLUXDB_HOST", INFLUXDB_HOST));
  message.replace(F("{TEXT_INFLUXDBPORT}"), (TEXT_INFLUXDBPORT));
  message.replace(F("{INFLUXDB_PORT}"), _addIntInput("INFLUXDB_PORT", INFLUXDB_PORT));
  message.replace(F("{TEXT_INFLUXDBNAME}"), (TEXT_INFLUXDBNAME));
  message.replace(F("{INFLUXDB_DATABASE}"), _addTextInput("INFLUXDB_DATABASE", INFLUXDB_DATABASE));
  message.replace(F("{TEXT_INFLUXDBUSER}"), (TEXT_INFLUXDBUSER));
  message.replace(F("{INFLUXDB_USER}"), _addTextInput("INFLUXDB_USER", INFLUXDB_USER));
  message.replace(F("{TEXT_INFLUXDBPASSWD}"), (TEXT_INFLUXDBPASSWD));
  message.replace(F("{INFLUXDB_PASSWORD}"), _addPasswdInput("INFLUXDB_PASSWORD", INFLUXDB_PASSWORD));

  if (!strcmp(INFLUXDB_VERSION, "2")) {
  message.replace(F("{TEXT_INFLUXDBORG}"), (TEXT_INFLUXDBORG));
  message.replace(F("{INFLUXDB_ORG}"), _addTextInput("INFLUXDB_ORG", INFLUXDB_ORG));
  message.replace(F("{TEXT_INFLUXDBBUCKET}"), (TEXT_INFLUXDBBUCKET));
  message.replace(F("{INFLUXDB_BUCKET}"), _addTextInput("INFLUXDB_BUCKET", INFLUXDB_BUCKET));
  message.replace(F("{TEXT_INFLUXDBTOKEN}"), (TEXT_INFLUXDBTOKEN));
  message.replace(F("{INFLUXDB_TOKEN}"), _addTextInput("INFLUXDB_TOKEN", INFLUXDB_TOKEN));
  } else {
	  message.replace(F("<b>{TEXT_INFLUXDBORG}: </b>{INFLUXDB_ORG}"), "");
	  message.replace(F("<b>{TEXT_INFLUXDBBUCKET}: </b>{INFLUXDB_BUCKET}"), "");
	  message.replace(F("<b>{TEXT_INFLUXDBTOKEN}: </b>{INFLUXDB_TOKEN}"), "");
  }

  message.replace(F("{AdvancedMQTTConfigButton}"), FPSTR(WEB_GOTO_CONFIG_ADVANCED_MQTT_PAGE_BUTTON));
  message.replace(F("{TEXT_CONFIG_ADV_MQTT}"), (TEXT_CONFIG_ADV_MQTT));

  message.replace(F("{WiFiEraseButton}"), _addWiFiErase());

  message.replace(F("{WiFiEraseButton}"), _addWiFiErase());
  message.replace(F("{RestoreConfigButton}"), _addRestoreConfig());
  message.replace(F("{SubmitButton}"), _addSubmitServices());
  message += FPSTR(WEB_PAGE_FOOTER);

  WebServer.send(200, "text/html", message);
}

void _handle_adv_mqtt_config(bool is_success) {
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
  }

  String message = FPSTR(WEB_PAGE_HEADER);
  message.replace(F("{WEB_PAGE_CSS}"), FPSTR(WEB_PAGE_HEADER_CSS));
  message.replace(F("{Language}"), (TEXT_LANG));
  message.replace(F("{CurrentPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{IndexPageTitle}"), (TEXT_INDEX_PAGE));
  message.replace(F("{ConfigPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{UpdatePageTitle}"), (TEXT_UPDATE_PAGE));

  message += FPSTR(WEB_CONFIG_ADVANCED_MQTT_PAGE_TOP);
  message.replace(F("{TEXT_ADVANCED_MQTT_PAGE}"), (TEXT_CONFIG_ADV_MQTT));

  if (!is_success) {
    message.replace(F("{WEB_CONFIG_TOP_PAGE_INFO}"), "");
  } else {
	message.replace(F("{WEB_CONFIG_TOP_PAGE_INFO}"), FPSTR(WEB_CONFIG_TOP_PAGE_INFO));
    message.replace(F("{TEXT_SAVED}"), (TEXT_SAVED));
    message.replace(F("{TEXT_POSTCONFIG_INFO}"), (TEXT_POSTCONFIG_INFO));
  }

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

  WebServer.send(200, "text/html", message);
}

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

void handle_config_device() {
  _handle_config_device(false);
}

void handle_config_services() {
  _handle_config_services(false);
}

void handle_adv_mqtt_config() {
  _handle_adv_mqtt_config(false);
}

void handle_config_device_post() {
  int need_update = 0;
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
  DEVICENAME_AUTO = _parseAsBool(WebServer.arg("DEVICENAME_AUTO"));
  if (!DEVICENAME_AUTO) {
    _parseAsCString(DEVICENAME, WebServer.arg("DEVICENAME"), 32);
  }
  DISPLAY_PM1 = _parseAsBool(WebServer.arg("DISPLAY_PM1"));
  _parseAsCString(LANGUAGE, WebServer.arg("LANGUAGE"), 32);
  _set_language();

  char oldTHP_MODEL[32];
  strcpy(oldTHP_MODEL, THP_MODEL);
  _parseAsCString(THP_MODEL, WebServer.arg("THP_MODEL"), 32);
  _parseAsCString(SECOND_THP_MODEL, WebServer.arg("SECOND_THP_MODEL"), 32);

  if (strcmp(THP_MODEL, oldTHP_MODEL) and !strcmp(THP_MODEL, "BME280-SparkFun")) {
    need_update = 1;
  }

  char oldDUST_MODEL[32];
  strcpy(oldDUST_MODEL, DUST_MODEL);
  _parseAsCString(DUST_MODEL, WebServer.arg("DUST_MODEL"), 32);

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
  _parseAsCString(CONFIG_FIRST_THP_SDA, WebServer.arg("CONFIG_FIRST_THP_SDA"), 8);
  _parseAsCString(CONFIG_FIRST_THP_SCL, WebServer.arg("CONFIG_FIRST_THP_SCL"), 8);

  _parseAsCString(CONFIG_SECOND_THP_SDA, WebServer.arg("CONFIG_SECOND_THP_SDA"), 8);
  _parseAsCString(CONFIG_SECOND_THP_SCL, WebServer.arg("CONFIG_SECOND_THP_SCL"), 8);

  _parseAsCString(CONFIG_DUST_TX, WebServer.arg("CONFIG_DUST_TX"), 8);
  _parseAsCString(CONFIG_DUST_RX, WebServer.arg("CONFIG_DUST_RX"), 8);

  SECOND_THP = _parseAsBool(WebServer.arg("SECOND_THP"));

  yield();
  FREQUENTMEASUREMENT = _parseAsBool(WebServer.arg("FREQUENTMEASUREMENT"));

  DUST_TIME = WebServer.arg("DUST_TIME").toInt();
  NUMBEROFMEASUREMENTS = WebServer.arg("NUMBEROFMEASUREMENTS").toInt();

  DEEPSLEEP_ON = _parseAsBool(WebServer.arg("DEEPSLEEP_ON"));
  MYALTITUDE = WebServer.arg("MYALTITUDE").toInt();

  DEBUG = _parseAsBool(WebServer.arg("DEBUG"));
  AUTOUPDATE_ON = _parseAsBool(WebServer.arg("AUTOUPDATE_ON"));

  CONFIG_AUTH = _parseAsBool(WebServer.arg("CONFIG_AUTH"));
  _parseAsCString(CONFIG_USERNAME, WebServer.arg("CONFIG_USERNAME"), 64);
  _parseAsCString(CONFIG_PASSWORD, WebServer.arg("CONFIG_PASSWORD"), 256);

  _parseAsCString(MODEL, WebServer.arg("MODEL"), 32);


  HOMEKIT_SUPPORT = _parseAsBool(WebServer.arg("HOMEKIT_SUPPORT"));


  if (DEBUG) {
    Serial.println(F("POST CONFIG END!!"));
  }

  if (need_update != 0) {
    strcpy(THP_MODEL, "Non");
    strcpy(DUST_MODEL, "Non");
    saveConfig();
    _handle_config_device(true);

    yield();
    if (need_update == 1) {
      doUpdate(1); // BME280-SparkFun
    }
    if (need_update == 2) {
      doUpdate(2); // SDS011
    }
    if (need_update == 3) {
      doUpdate(3); // HPMA115S0
    }
    if (need_update == 4) {
      doUpdate(4); // PMSx003
    }
    if (need_update >= 5) {
      doUpdate(5); // SPS30
    }
    if (need_update >= 6) {
      doUpdate(0); // CURRENT SERVERSOFTWARE VERSION
    }
  }

  saveConfig();
  _handle_config_device(true);

  // https://github.com/esp8266/Arduino/issues/1722
  //ESP.reset();
  //yield();
  ESP.restart();
}

void handle_config_services_post() {
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
  SENDING_FREQUENCY = WebServer.arg("SENDING_FREQUENCY").toInt();
  SENDING_DB_FREQUENCY = WebServer.arg("SENDING_DB_FREQUENCY").toInt();

  SMOGLIST_ON = _parseAsBool(WebServer.arg("SMOGLIST_ON"));
  LUFTDATEN_ON = _parseAsBool(WebServer.arg("LUFTDATEN_ON"));

  AQI_ECO_ON = _parseAsBool(WebServer.arg("AQI_ECO_ON"));
  _parseAsCString(AQI_ECO_HOST, WebServer.arg("AQI_ECO_HOST"), 128);
  _parseAsCString(AQI_ECO_PATH, WebServer.arg("AQI_ECO_PATH"), 64);

  AIRMONITOR_ON = _parseAsBool(WebServer.arg("AIRMONITOR_ON"));
  AIRMONITOR_GRAPH_ON = _parseAsBool(WebServer.arg("AIRMONITOR_GRAPH_ON"));

  _parseAsCString(LATITUDE, WebServer.arg("LATITUDE"), 16);
  _parseAsCString(LONGITUDE, WebServer.arg("LONGITUDE"), 16);
  _parseAsCString(EMAIL, WebServer.arg("EMAIL"), 128);
  

  THINGSPEAK_ON = _parseAsBool(WebServer.arg("THINGSPEAK_ON"));
  THINGSPEAK_GRAPH_ON = _parseAsBool(WebServer.arg("THINGSPEAK_GRAPH_ON"));
  _parseAsCString(THINGSPEAK_API_KEY, WebServer.arg("THINGSPEAK_API_KEY"), 32);
  THINGSPEAK_CHANNEL_ID = WebServer.arg("THINGSPEAK_CHANNEL_ID").toInt();
  _parseAsCString(THINGSPEAK_READ_API_KEY, WebServer.arg("THINGSPEAK_READ_API_KEY"), 32);

  INFLUXDB_ON = _parseAsBool(WebServer.arg("INFLUXDB_ON"));
  _parseAsCString(INFLUXDB_VERSION, WebServer.arg("INFLUXDB_VERSION"), 16);
  _parseAsCString(INFLUXDB_HOST, WebServer.arg("INFLUXDB_HOST"), 128);
  INFLUXDB_PORT = WebServer.arg("INFLUXDB_PORT").toInt();
  _parseAsCString(INFLUXDB_DATABASE, WebServer.arg("INFLUXDB_DATABASE"), 64);
  _parseAsCString(INFLUXDB_USER, WebServer.arg("INFLUXDB_USER"), 64);
  _parseAsCString(INFLUXDB_PASSWORD, WebServer.arg("INFLUXDB_PASSWORD"), 64);

  _parseAsCString(INFLUXDB_ORG, WebServer.arg("INFLUXDB_ORG"), 64);
  _parseAsCString(INFLUXDB_BUCKET, WebServer.arg("INFLUXDB_BUCKET"), 64);
  _parseAsCString(INFLUXDB_TOKEN, WebServer.arg("INFLUXDB_TOKEN"), 64);

  if (DEBUG) {
    Serial.println(F("POST CONFIG END!!"));
  }

  saveConfig();
  //delay(250);
  _handle_config_services(true);
  // https://github.com/esp8266/Arduino/issues/1722
  //ESP.reset();
  //yield();
  ESP.restart();
}

void handle_adv_mqtt_config_post() {
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
   MQTT_ON = _parseAsBool(WebServer.arg("MQTT_ON"));
  _parseAsCString(MQTT_HOST, WebServer.arg("MQTT_HOST"), 128);
  MQTT_PORT = WebServer.arg("MQTT_PORT").toInt();
  _parseAsCString(MQTT_USER, WebServer.arg("MQTT_USER"), 64);
  _parseAsCString(MQTT_PASSWORD, WebServer.arg("MQTT_PASSWORD"), 64);

  MQTT_IP_IN_TOPIC = _parseAsBool(WebServer.arg("MQTT_IP_IN_TOPIC"));
  MQTT_DEVICENAME_IN_TOPIC = _parseAsBool(WebServer.arg("MQTT_DEVICENAME_IN_TOPIC"));
  MQTT_SLASH_AT_THE_BEGINNING = _parseAsBool(WebServer.arg("MQTT_SLASH_AT_THE_BEGINNING"));
  MQTT_SLASH_AT_THE_END = _parseAsBool(WebServer.arg("MQTT_SLASH_AT_THE_END"));

  _parseAsCString(MQTT_TOPIC_TEMP, WebServer.arg("MQTT_TOPIC_TEMP"), 128);
  _parseAsCString(MQTT_TOPIC_HUMI, WebServer.arg("MQTT_TOPIC_HUMI"), 128);
  _parseAsCString(MQTT_TOPIC_PRESS, WebServer.arg("MQTT_TOPIC_PRESS"), 128);
  _parseAsCString(MQTT_TOPIC_PM1, WebServer.arg("MQTT_TOPIC_PM1"), 128);
  _parseAsCString(MQTT_TOPIC_PM25, WebServer.arg("MQTT_TOPIC_PM25"), 128);
  _parseAsCString(MQTT_TOPIC_PM10, WebServer.arg("MQTT_TOPIC_PM10"), 128);
  _parseAsCString(MQTT_TOPIC_AIRQUALITY, WebServer.arg("MQTT_TOPIC_AIRQUALITY"), 128);

  if (DEBUG) {
    Serial.println(F("POST CONFIG END!!"));
  }

  saveConfig();
  //delay(250);
  _handle_adv_mqtt_config(true);
  // https://github.com/esp8266/Arduino/issues/1722
  //ESP.reset();
  //yield();

  ESP.restart();
}

void handle_update() {            //Handler for the handle_update
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
  }
  String message = FPSTR(WEB_PAGE_HEADER);
  message.replace(F("{WEB_PAGE_CSS}"), FPSTR(WEB_PAGE_HEADER_CSS));
  message.replace(F("{Language}"), (TEXT_LANG));
  message.replace(F("{CurrentPageTitle}"), (TEXT_UPDATE_PAGE));
  message.replace(F("{IndexPageTitle}"), (TEXT_INDEX_PAGE));
  message.replace(F("{ConfigPageTitle}"), (TEXT_CONFIG_PAGE));
  message.replace(F("{UpdatePageTitle}"), (TEXT_UPDATE_PAGE));

  message += FPSTR(WEB_UPDATE_PAGE_UPDATE);

  if (!AUTOUPDATE_ON) {
    if (need_update) {
      message.replace(F("{WEB_UPDATE_INFO_WARNING}"), FPSTR(WEB_UPDATE_INFO_WARNING));
      message.replace(F("{TEXT_FWUPDATEAVALIBLE}"), (TEXT_FWUPDATEAVALIBLE));
      message.replace(F("{MANUALUPDATEBUTTON}"), "");
      message.replace(F("{FWUPDATEBUTTON}"), FPSTR(WEB_UPDATE_BUTTON_FWUPDATE));
      message.replace(F("{TEXT_FWUPDATEBUTTON}"), (TEXT_FWUPDATEBUTTON));
      message.replace(F("{AUTOUPDATEONBUTTON}"), FPSTR(WEB_UPDATE_BUTTON_AUTOUPDATEON));
      message.replace(F("{TEXT_AUTOUPDATEONBUTTON}"), (TEXT_AUTOUPDATEONBUTTON));
      message.replace(F("{TEXT_AUTOUPDATEWARNING}"), (TEXT_AUTOUPDATEWARNING));
      message.replace(F("{TEXT_FWUPDATEBUTTON}"), (TEXT_FWUPDATEBUTTON));
    }
    message.replace(F("{WEB_UPDATE_INFO_WARNING}"), "");
  } else {
    message.replace(F("{WEB_UPDATE_INFO_WARNING}"), "");
  }

  message.replace(F("{TEXT_UPDATE_PAGE}"), (TEXT_UPDATE_PAGE));
  message.replace(F("{TEXT_SELECTUPDATEFILE}"), (TEXT_SELECTUPDATEFILE));
  message.replace(F("{TEXT_SUBMITUPDATE}"), (TEXT_SUBMITUPDATE));

  message.replace(F("{TEXT_AUTOUPDATEON}"), (TEXT_AUTOUPDATEON));
  if (AUTOUPDATE_ON) {
    message.replace(F("{AUTOUPDATEONSTATUS}"), (TEXT_YES));
  } else {
    message.replace(F("{AUTOUPDATEONSTATUS}"), (TEXT_NO));
  }

  message.replace(F("{TEXT_CURRENTSOFTVERSION}"), (TEXT_CURRENTSOFTVERSION));
  message.replace(F("{SOFTWAREVERSION}"), String(CURRENTSOFTWAREVERSION) + " " + String(PMSENSORVERSION));

  message.replace(F("{TEXT_SERVERSOFTWAREVERSION}"), (TEXT_SERVERSOFTWAREVERSION));
  message.replace(F("{SERVERSOFTWAREVERSION}"), String(SERVERSOFTWAREVERSION) + " " + String(PMSENSORVERSION));

  message.replace(F("{TEXT_LATESTAVAILABLESOFT}"), TEXT_LATESTAVAILABLESOFT);
  message.replace(F("{SMOGOMIERZRELEASES_LINK}"), (SMOGOMIERZRELEASES_LINK));
  message.replace(F("{TEXT_HERE}"), (TEXT_HERE));

  // init WiFi signal quality info - START
  String WiFiSSID = WiFi.SSID();
  int WiFiRSSI = WiFi.RSSI();
  message.replace(F("{TEXT_CONNECTEDWIFI}"), (TEXT_CONNECTEDWIFI));
  message.replace(F("{WiFiSSID}"), (WiFiSSID));
  message.replace(F("{TEXT_WIFIRSSI}"), (TEXT_WIFIRSSI));
  message.replace(F("{WiFiRSSI}"), (String(WiFiRSSI) + " dBm"));

  // https://stackoverflow.com/a/15798024
  int WiFiQuality;
  if (WiFiRSSI <= -100) {
    WiFiQuality = 0;
  } else if (WiFiRSSI >= -50) {
    WiFiQuality = 100;
  } else {
    WiFiQuality = 2 * (WiFiRSSI + 100);
  }
  message.replace(F("{TEXT_WIFIQUALITY}"), (TEXT_WIFIQUALITY));
  message.replace(F("{WiFiQuality}"), (String(WiFiQuality) + " %"));
  // init WiFi signal quality info - END

  message += FPSTR(WEB_PAGE_FOOTER);
  WebServer.send(200, "text/html", message);
}

void erase_wifi() {
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
  }
  Serial.println(F("Erasing Config..."));
#ifdef ARDUINO_ARCH_ESP8266
  ESP.eraseConfig();
#elif defined ARDUINO_ARCH_ESP32
  WiFi.disconnect(false, true);
#endif
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

void restore_config() {
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
  }
  Serial.println(F("Restoring default settings..."));
  deleteConfig();
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

void fwupdate() {
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
  }
  doUpdate(0);
  delay(1000);
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  delay(1000);
}

void autoupdate_on() {
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
  }
  AUTOUPDATE_ON = true;
  saveConfig();
  yield();
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

void handle_api() {
  String message;
  StaticJsonDocument<800> jsonBuffer;
  JsonObject json = jsonBuffer.to<JsonObject>();

  json["device_name"] = device_name;
  if (strcmp(DUST_MODEL, "Non")) {
    json["pm1"] = averagePM1;
    json["pm25"] = averagePM25;
	if (!strcmp(DUST_MODEL, "SPS30")) {
	json["pm4"] = averagePM4;
	}
    json["pm10"] = averagePM10;
  }
  if (strcmp(THP_MODEL, "Non")) {
    takeTHPMeasurements();
  }
  if (!strcmp(THP_MODEL, "BME280")) {
    if (checkBmeStatus()) {
      json["temperature"] = float(currentTemperature);
      json["pressure"] = int(currentPressure);
      json["humidity"] = int(currentHumidity);
      json["dewpoint"] = float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112);
    }
  }
  if (!strcmp(THP_MODEL, "BMP280")) {
    if (checkBmpStatus()) {
      json["temperature"] = float(currentTemperature);
      json["pressure"] = int(currentPressure);
    }
  }
  if (!strcmp(THP_MODEL, "HTU21")) {
    if (checkHTU21DStatus()) {
      json["temperature"] = float(currentTemperature);
      json["humidity"] = int(currentHumidity);
      json["dewpoint"] = float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112);
    }
  }
  if (!strcmp(THP_MODEL, "DHT22")) {
    if (checkDHT22Status()) {
      json["temperature"] = float(currentTemperature);
      json["humidity"] = int(currentHumidity);
      json["dewpoint"] = float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112);
    }
  }
  if (!strcmp(THP_MODEL, "SHT1x")) {
    if (checkSHT1xStatus()) {
      json["temperature"] = float(currentTemperature);
      json["humidity"] = int(currentHumidity);
      json["dewpoint"] = float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112);
    }
  }
  if (!strcmp(THP_MODEL, "DS18B20")) {
    if (checkDS18B20Status()) {
      json["temperature"] = float(currentTemperature);
    }
  }

  serializeJsonPretty(json, message);
  WebServer.send(200, "text/json", message);
}

void homekit_reset() {
    if (CONFIG_AUTH == true) {
      if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
        //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
        return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
      }
    }
  Serial.println("homekit reset...");

  String pair_file_name = "/homekit_pair.dat";
  SPIFFS.remove(pair_file_name);
  
  yield();
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

void homekit_on() {
    if (CONFIG_AUTH == true) {
      if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
        //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
        return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
      }
    }
  Serial.println("homekit on...");

  HOMEKIT_SUPPORT = true;
  saveConfig();

  yield();
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}

void homekit_off() {
    if (CONFIG_AUTH == true) {
      if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
        //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
        return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
      }
    }
  Serial.println("homekit off...");

  HOMEKIT_SUPPORT = false;
  saveConfig();
  
  yield();
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  delay(1000);
  Serial.println(F("Restart"));
  ESP.restart();
}