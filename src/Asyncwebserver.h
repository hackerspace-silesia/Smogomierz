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

const char* www_realm = "Custom Auth Realm";
String authFailResponse = "<meta http-equiv='refresh' content='0; url=/' /> Authentication Failed! <p><a href='/'>Redirect</a></p>";

//void handle_root() {
void handle_root(AsyncWebServerRequest *request) {
	String message;
	
  message += FPSTR(WEB_PAGE_HEADER);
  message.replace("{WEB_PAGE_CSS}", FPSTR(WEB_PAGE_HEADER_CSS));
  message.replace("{Language}", (TEXT_LANG));
  message.replace("{CurrentPageTitle}", (TEXT_INDEX_PAGE));
  message.replace("{IndexPageTitle}", (TEXT_INDEX_PAGE));
  message.replace("{ConfigPageTitle}", (TEXT_CONFIG_PAGE));
  message.replace("{UpdatePageTitle}", (TEXT_UPDATE_PAGE));

  message += FPSTR(WEB_ROOT_PAGE_MEASUREMENTS);

  if (!AUTOUPDATE_ON) {
    if (need_update) {
      message.replace("{WEB_UPDATE_INFO_WARNING}", FPSTR(WEB_UPDATE_INFO_WARNING));
      message.replace("{TEXT_FWUPDATEAVALIBLE}", (TEXT_FWUPDATEAVALIBLE));
      message.replace("{MANUALUPDATEBUTTON}", FPSTR(WEB_UPDATE_BUTTON_MANUALUPDATE));
      message.replace("{TEXT_MANUALUPDATEBUTTON}", (TEXT_MANUALUPDATEBUTTON));
      message.replace("{FWUPDATEBUTTON}", FPSTR(WEB_UPDATE_BUTTON_FWUPDATE));
      message.replace("{TEXT_FWUPDATEBUTTON}", (TEXT_FWUPDATEBUTTON));
      message.replace("{AUTOUPDATEONBUTTON}", FPSTR(WEB_UPDATE_BUTTON_AUTOUPDATEON));
      message.replace("{TEXT_AUTOUPDATEONBUTTON}", (TEXT_AUTOUPDATEONBUTTON));
      message.replace("{TEXT_AUTOUPDATEWARNING}", (TEXT_AUTOUPDATEWARNING));
      message.replace("{TEXT_FWUPDATEBUTTON}", (TEXT_FWUPDATEBUTTON));
    }
    message.replace("{WEB_UPDATE_INFO_WARNING}", "");
  } else {
    message.replace("{WEB_UPDATE_INFO_WARNING}", "");
  }

  if (!strcmp(THP_MODEL, "Non")) {
    message.replace("{TEXT_WEATHER}:", "");
    message.replace("{TEXT_TEMPERATURE}: {Temperature} °C", "");
    message.replace("{TEXT_HUMIDITY}: {Humidity} %", "");
    message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
    message.replace("{TEXT_DEWPOINT}: {Dewpoint} °C", "");
  } else {
    takeTHPMeasurements();
    message.replace("{TEXT_WEATHER}", (TEXT_WEATHER));
  }
  if (!strcmp(THP_MODEL, "BME280")) {
    if (checkBmeStatus()) {
      message.replace("{TEXT_TEMPERATURE}", (TEXT_TEMPERATURE));
      message.replace("{TEXT_HUMIDITY}", (TEXT_HUMIDITY));
      message.replace("{TEXT_PRESSURE}", (TEXT_PRESSURE));
      message.replace("{TEXT_DEWPOINT}", (TEXT_DEWPOINT));

      message.replace("{Temperature}", String(int(currentTemperature)));
      message.replace("{Pressure}", String(int(currentPressure)));
      message.replace("{Humidity}", String(int(currentHumidity)));
      message.replace("{Dewpoint}", String(int(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
    } else {
      message.replace("{TEXT_TEMPERATURE}: {Temperature} °C", "");
      message.replace("{TEXT_HUMIDITY}: {Humidity} %", "");
      message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
      message.replace("{TEXT_DEWPOINT}: {Dewpoint} °C", "");
    }
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    if (checkHTU21DStatus()) {
      message.replace("{TEXT_TEMPERATURE}", (TEXT_TEMPERATURE));
      message.replace("{TEXT_HUMIDITY}", (TEXT_HUMIDITY));
      message.replace("{TEXT_DEWPOINT}", (TEXT_DEWPOINT));

      message.replace("{Temperature}", String(int(currentTemperature)));
      message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
      message.replace("{Humidity}", String(int(currentHumidity)));
      message.replace("{Dewpoint}", String(int(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
    } else {
      message.replace("{TEXT_TEMPERATURE}: {Temperature} °C", "");
      message.replace("{TEXT_HUMIDITY}: {Humidity} %", "");
      message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
      message.replace("{TEXT_DEWPOINT}: {Dewpoint} °C", "");
    }
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    if (checkDHT22Status()) {
      message.replace("{TEXT_TEMPERATURE}", (TEXT_TEMPERATURE));
      message.replace("{TEXT_HUMIDITY}", (TEXT_HUMIDITY));
      message.replace("{TEXT_DEWPOINT}", (TEXT_DEWPOINT));

      message.replace("{Temperature}", String(int(currentTemperature)));
      message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
      message.replace("{Humidity}", String(int(currentHumidity)));
      message.replace("{Dewpoint}", String(int(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
    } else {
      message.replace("{TEXT_TEMPERATURE}: {Temperature} °C", "");
      message.replace("{TEXT_HUMIDITY}: {Humidity} %", "");
      message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
      message.replace("{TEXT_DEWPOINT}: {Dewpoint} °C", "");
    }
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    if (checkBmpStatus()) {
      message.replace("{TEXT_TEMPERATURE}", (TEXT_TEMPERATURE));
      message.replace("{TEXT_PRESSURE}", (TEXT_PRESSURE));

      message.replace("{Temperature}", String(int(currentTemperature)));
      message.replace("{Pressure}", String(int(currentPressure)));
      message.replace("{TEXT_HUMIDITY}: {Humidity} %", "");
      message.replace("{TEXT_DEWPOINT}: {Pressure} °C", "");
    } else {
      message.replace("{TEXT_TEMPERATURE}: {Temperature} °C", "");
      message.replace("{TEXT_HUMIDITY}: {Humidity} %", "");
      message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
      message.replace("{TEXT_DEWPOINT}: {Dewpoint} °C", "");
    }
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
    if (checkSHT1xStatus()) {
      message.replace("{TEXT_TEMPERATURE}", (TEXT_TEMPERATURE));
      message.replace("{TEXT_HUMIDITY}", (TEXT_HUMIDITY));
      message.replace("{TEXT_DEWPOINT}", (TEXT_DEWPOINT));

      message.replace("{Temperature}", String(int(currentTemperature)));
      message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
      message.replace("{Humidity}", String(float(currentHumidity)));
      message.replace("{Dewpoint}", String(int(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
    } else {
      message.replace("{TEXT_TEMPERATURE}: {Temperature} °C", "");
      message.replace("{TEXT_HUMIDITY}: {Humidity} %", "");
      message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
      message.replace("{TEXT_DEWPOINT}: {Dewpoint} °C", "");
    }
  }

  if (strcmp(DUST_MODEL, "Non")) {
    message.replace("{TEXT_AIRPOLLUTION}", (TEXT_AIRPOLLUTION));

    if (DISPLAY_PM1) {
      message.replace("{averagePM1}", String(averagePM1));
    } else {
      message.replace("PM1: {averagePM1} µg/m³", "");
    }

    if (averagePM25 <= 10) {
      message.replace("{colorAveragePM25}", "<font color='#61EEE4'>");
    } else if (averagePM25 > 10 && averagePM25 <= 20) {
      message.replace("{colorAveragePM25}", "<font color='#5BCAAA'>");
    } else if (averagePM25 > 20 && averagePM25 <= 25) {
      message.replace("{colorAveragePM25}", "<font color='#EEE25D'>");
    } else if (averagePM25 > 25 && averagePM25 <= 50) {
      message.replace("{colorAveragePM25}", "<font color='#F95459'>");
    } else if (averagePM25 > 50) {
      message.replace("{colorAveragePM25}", "<font color='#920736'>");
    } else {
      message.replace("{colorAveragePM25}", "<font>");
    }

    message.replace("{averagePM25}", String(averagePM25) + "</font>");

    if (averagePM10 <= 20) {
      message.replace("{colorAveragePM10}", "<font color='#61EEE4'>");
    } else if (averagePM10 > 20 && averagePM10 <= 35) {
      message.replace("{colorAveragePM10}", "<font color='#5BCAAA'>");
    } else if (averagePM10 > 35 && averagePM10 <= 50) {
      message.replace("{colorAveragePM10}", "<font color='#EEE25D'>");
    } else if (averagePM10 > 50 && averagePM10 <= 100) {
      message.replace("{colorAveragePM10}", "<font color='#F95459'>");
    } else if (averagePM10 > 100) {
      message.replace("{colorAveragePM10}", "<font color='#920736'>");
    } else {
      message.replace("{colorAveragePM10}", "<font>");
    }
    message.replace("{averagePM10}", String(averagePM10) + "</font>");
  } else {
    message.replace("{TEXT_AIRPOLLUTION}:", "");
    message.replace("PM1: {averagePM1} µg/m³", "");
    message.replace("PM2.5: {colorAveragePM25} {averagePM25} µg/m³", "");
    message.replace("PM10: {colorAveragePM10} {averagePM10} µg/m³", "");
  }

  if (AIRMONITOR_GRAPH_ON) {
    message += FPSTR(WEB_ROOT_PAGE_AIRMONITOR_GRAPH);
    message.replace("{LATITUDE}", String(LATITUDE));
    message.replace("{LONGITUDE}", String(LONGITUDE));
  }

  if (THINGSPEAK_GRAPH_ON) {
    message += FPSTR(WEB_ROOT_PAGE_THINGSPEAK_GRAPH);
    message.replace("{THINGSPEAK_CHANNEL_ID}", String(THINGSPEAK_CHANNEL_ID));
	message.replace("{THINGSPEAK_READ_API_KEY}", String(THINGSPEAK_READ_API_KEY));
  }

  message += FPSTR(WEB_PAGE_FOOTER);
  
  if (DEBUG) {
	Serial.print("ROOT - message.length(): ");
    Serial.println(message.length()); // keep it under 20000!
	Serial.print("\n");
  }
  request->send(200, "text/html", message);
  //WebServer.send(200, "text/html", message);
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

//void handle_config() {
void handle_config(AsyncWebServerRequest *request) {
    String message;
	
  message += FPSTR(WEB_PAGE_HEADER);
  message.replace("{WEB_PAGE_CSS}", FPSTR(WEB_PAGE_HEADER_CSS));
  message.replace("{Language}", (TEXT_LANG));
  message.replace("{CurrentPageTitle}", (TEXT_CONFIG_PAGE));
  message.replace("{IndexPageTitle}", (TEXT_INDEX_PAGE));
  message.replace("{ConfigPageTitle}", (TEXT_CONFIG_PAGE));
  message.replace("{UpdatePageTitle}", (TEXT_UPDATE_PAGE));

  message += FPSTR(WEB_CONFIG_PAGE_TOP);
  message.replace("{TEXT_CONFIG_PAGE}", (TEXT_CONFIG_PAGE));

  message += FPSTR(WEB_CONFIG_PAGE_CONFIG);
  message.replace("{TEXT_CONFIG_INFO_DEVICE_PAGE}", (TEXT_CONFIG_INFO_DEVICE_PAGE));
  message.replace("{TEXT_CONFIG_DEVICE_PAGE}", (TEXT_CONFIG_DEVICE_PAGE));

  message.replace("{TEXT_CONFIG_INFO_SERVICES_PAGE}", (TEXT_CONFIG_INFO_SERVICES_PAGE));
  message.replace("{TEXT_CONFIG_SERVICES_PAGE}", (TEXT_CONFIG_SERVICES_PAGE));

  message.replace("{ConfigDeviceButton}", FPSTR(WEB_CONFIG_DEVICE_PAGE_BUTTON));
  message.replace("{TEXT_CONFIG_DEVICE_PAGE}", (TEXT_CONFIG_DEVICE_PAGE));
  message.replace("{ConfigServicesButton}", FPSTR(WEB_CONFIG_SERVICES_PAGE_BUTTON));
  message.replace("{TEXT_CONFIG_SERVICES_PAGE}", (TEXT_CONFIG_SERVICES_PAGE));

  message.replace("{WiFiEraseButton}", _addWiFiErase());
  message.replace("{RestoreConfigButton}", _addRestoreConfig());

  message += FPSTR(WEB_PAGE_FOOTER);
  
  if (DEBUG) {
  	Serial.print("CONFIG - message.length(): ");
    Serial.println(message.length()); // keep it under 20000!
  	Serial.print("\n");
}
request->send(200, "text/html", message);
  //WebServer.send(200, "text/html", message);
}

String handle_config_device_processor(const String& var)
{
	//Serial.println("var: " + var);
	String message;
	
    if (var == "{Language}") {
        message += (TEXT_LANG);    	
    }
	
    if (var == "{WEB_PAGE_CSS}") {
      message += (WEB_PAGE_HEADER_CSS);
	  } 
	
      if (var == "{SMOGLY_LOGO_URL}") {
        message += (SMOGLY_LOGO_URL);
  	  } 
	  
      if (var == "{CurrentPageTitle}") {
        message += (TEXT_CONFIG_PAGE);
    }
      if (var == "{IndexPageTitle}") {
        message += (TEXT_INDEX_PAGE);
    }
      if (var == "{ConfigPageTitle}") {
        message += (TEXT_CONFIG_PAGE);
    }
      if (var == "{UpdatePageTitle}") {
        message += (TEXT_UPDATE_PAGE);
  	  } 	
      if (var == "{TEXT_CONFIG_DEVICE_PAGE}") {	
    message += (TEXT_CONFIG_DEVICE_PAGE);
  }  
    if (var == "{TEXT_INSTRUCIONSLINK}") {	
  message += (TEXT_INSTRUCIONSLINK);
  message.replace(F("{GITHUB_LINK}"), String(GITHUB_LINK));
  message.replace(F("{TEXT_HERE}"), (TEXT_HERE));
}

    if (var == "{TEXT_DEVICENAME}") {	
  message += (TEXT_DEVICENAME);
}  

    if (var == "{device_name}") {	
		if (DEVICENAME_AUTO) {
		  message += (device_name);
		} else {
			message += (_addTextInput("DEVICENAME", DEVICENAME));
		}
}  

    if (var == "{TEXT_DEVICENAMEAUTO}") {	
  message += (TEXT_DEVICENAMEAUTO);
}  
    if (var == "{DEVICENAME_AUTO}") {	
  message += (_addBoolSelect("DEVICENAME_AUTO", DEVICENAME_AUTO));
}  
    if (var == "{TEXT_SELECTEDLANGUAGE}") {	
  message += (TEXT_SELECTEDLANGUAGE);
}  
    if (var == "{LanguageSelect}") {	
  message += (_addLanguageSelect("LANGUAGE", LANGUAGE));
}  
    if (var == "{TEXT_TEMPHUMIPRESSSENSOR}") {	
  message += (TEXT_TEMPHUMIPRESSSENSOR);
}  
    if (var == "{THP_MODELSelect}") {	
  message += (_addTHP_MODELSelect("THP_MODEL", THP_MODEL));
}  
    if (var == "{TEXT_PMSENSOR}") {	
  message += (TEXT_PMSENSOR);
}  
    if (var == "{DUST_MODELSelect}") {	
  message += (_addDUST_MODELSelect("DUST_MODEL", DUST_MODEL));
}  

if (var == "{WEB_CONFIG_DEVICE_PAGE_FIRST_TH") {
	if (strcmp(THP_MODEL, "Non")) {
	message += (WEB_CONFIG_DEVICE_PAGE_FIRST_THP_PINS);
} else {
	message += "";
}
}

if (var == "{THP_MODEL}") {	
	message += (THP_MODEL);
}
if (var == "{TEXT_FIRST_THP_SDA_SCL}") {	
	message += (TEXT_FIRST_THP_SDA_SCL);
}
if (var == "{TEXT_FIRST_THP_SDA}") {	
	message += (TEXT_FIRST_THP_SDA);
}
if (var == "{FIRST_THP_SDA}") {	
	message += (_add_FIRST_THP_SDA_SCL_Select("CONFIG_FIRST_THP_SDA", CONFIG_FIRST_THP_SDA));
}
if (var == "{TEXT_FIRST_THP_SCL}") {	
	if (!strcmp(THP_MODEL, "DHT22") or !strcmp(THP_MODEL, "DS18B20")) {
		message += ("");
	} else {
		message += (TEXT_FIRST_THP_SCL);
	}
}
if (var == "{FIRST_THP_SCL}") {	
	if (!strcmp(THP_MODEL, "DHT22") or !strcmp(THP_MODEL, "DS18B20")) {
		message += ("");
	} else {
		message += (_add_FIRST_THP_SDA_SCL_Select("CONFIG_FIRST_THP_SCL", CONFIG_FIRST_THP_SCL));
	}
}

if (var == "{WEB_CONFIG_DEVICE_PAGE_SECOND_T") {	
	if (strcmp(THP_MODEL, "Non")) {
	message += (WEB_CONFIG_DEVICE_PAGE_SECOND_THP_PINS);
} else {
	message += ("");
}
}
if (var == "{TEXT_SECOND_THP}") {	
	message += (TEXT_SECOND_THP);
}
if (var == "{SECOND_THP}") {	
	message += (_addBoolSelect("SECOND_THP", SECOND_THP));
}
if (var == "{TEXT_SECOND_THP_SDA_SCL}") {	
	message += (TEXT_SECOND_THP_SDA_SCL);
}
if (var == "{TEXT_SECOND_THP_SDA}") {	
	message += (TEXT_SECOND_THP_SDA);
}
if (var == "{SECOND_THP_SDA}") {	
	message += (_add_SECOND_THP_SDA_SCL_Select("CONFIG_SECOND_THP_SDA", CONFIG_SECOND_THP_SDA));
}
if (var == "{TEXT_SECOND_THP_SCL}") {	
	message += (TEXT_SECOND_THP_SCL);
}
if (var == "{SECOND_THP_SCL}") {	
	message += (_add_SECOND_THP_SDA_SCL_Select("CONFIG_SECOND_THP_SCL", CONFIG_SECOND_THP_SCL));
}						
						

if (var == "{WEB_CONFIG_DEVICE_PAGE_DUST_PIN") {	
if (strcmp(DUST_MODEL, "Non")) {
	message += (WEB_CONFIG_DEVICE_PAGE_DUST_PINS);
} else {
	message += "";
}
}

if (var == "{DUST_MODEL}") {	
	message += (DUST_MODEL);
}	
if (var == "{TEXT_DUST_TX_RX}") {	
	message += (TEXT_DUST_TX_RX);
}
if (var == "{TEXT_DUST_TX}") {	
	message += (TEXT_DUST_TX);
}
if (var == "{DUST_TX}") {	
	message += (_add_DUST_TX_RX_Select("CONFIG_DUST_TX", CONFIG_DUST_TX));
}
if (var == "{TEXT_DUST_RX}") {	
	message += (TEXT_DUST_RX);
}
if (var == "{DUST_RX}") {	
	message += (_add_DUST_TX_RX_Select("CONFIG_DUST_RX", CONFIG_DUST_RX));
}

if (var == "{TEXT_FREQUENTMEASUREMENTONOFF}") {	
	message += (TEXT_FREQUENTMEASUREMENTONOFF);
}
if (var == "{FREQUENTMEASUREMENT_Select}") {	
	message += (_addBoolSelect("FREQUENTMEASUREMENT", FREQUENTMEASUREMENT));
}	
if (var == "{TEXT_FREQUENTMEASUREMENTINFO}") {	
	message += (TEXT_FREQUENTMEASUREMENTINFO);
}	
if (var == "{TEXT_MEASUREMENTFREQUENCY}") {	
	message += (TEXT_MEASUREMENTFREQUENCY);
}		
if (var == "{FREQUENTMEASUREMENT_time}") {	
if (FREQUENTMEASUREMENT == true) {
  message += (_addIntInput("DUST_TIME", DUST_TIME, "{TEXT_SECONDS}"));
  message.replace(F("{TEXT_SECONDS}"), (TEXT_SECONDS));
} else {
  message += (_addIntInput("DUST_TIME", DUST_TIME, "{TEXT_MINUTES}"));
  message.replace(F("{TEXT_MINUTES}"), (TEXT_MINUTES));
}
}

if (var == "{TEXT_AVERAGELASTRESULT}") {	
	message += (TEXT_AVERAGELASTRESULT);
}
if (var == "{NUMBEROFMEASUREMENTS}") {	
	message += (_addIntInput("NUMBEROFMEASUREMENTS", NUMBEROFMEASUREMENTS, "{TEXT_PMMEASUREMENTS}"));
    message.replace(F("{TEXT_PMMEASUREMENTS}"), (TEXT_PMMEASUREMENTS));	
}

if (var == "{TEXT_DEEPSLEEPINFO}") {	
#ifdef ARDUINO_ARCH_ESP8266
    message += (TEXT_DEEPSLEEPINFO);
#elif defined ARDUINO_ARCH_ESP32
    message += (TEXT_DEEPSLEEPINFO_ESP32);
#endif
    message.replace(F("{INTERFACEWWWONTIME}"), String(int(NUMBEROFMEASUREMENTS) * 2 + 10 ));
    message.replace(F("{SENDING_FREQUENCY}"), String(SENDING_FREQUENCY));
}
if (var == "{DEEPSLEEP_ON}") {	
	message += (_addBoolSelect("DEEPSLEEP_ON", DEEPSLEEP_ON));
}

if (var == "{WEB_CONFIG_DEVICE_SHOWING_PM1}") {	
	if (!strcmp(DUST_MODEL, "PMS7003") or !strcmp(DUST_MODEL, "SPS30")) {
message += (WEB_CONFIG_DEVICE_SHOWING_PM1);
} else {
  message += ("");
}
}
if (var == "{TEXT_DISPLAYPM1}") {	
	  message += (TEXT_DISPLAYPM1);
}
if (var == "{DISPLAY_PM1}") {	
	  message += (_addBoolSelect("DISPLAY_PM1", DISPLAY_PM1));
	  message.replace(F("{TEXT_DISPLAYPM1}"), (TEXT_DISPLAYPM1));
}
if (var == "{TEXT_ALTITUDEINFO}") {	
	message += (TEXT_ALTITUDEINFO);
	message.replace(F("{WSPOLRZEDNE_GPS_LINK}"), String(WSPOLRZEDNE_GPS_LINK));
	message.replace(F("{TEXT_HERE}"), (TEXT_HERE));
}
if (var == "{MYALTITUDE}") {	
message += (_addIntInput("MYALTITUDE", MYALTITUDE, "m.n.p.m"));
}

if (var == "{TEXT_SECURECONFIGUPDATEPAGE}") {	
message += (TEXT_SECURECONFIGUPDATEPAGE);
}
if (var == "{CONFIG_AUTH}") {	
message += (_addBoolSelect("CONFIG_AUTH", CONFIG_AUTH));
}
if (var == "{TEXT_SECURELOGIN}") {	
message += (TEXT_SECURELOGIN);
}
if (var == "{CONFIG_USERNAME}") {	
message += (_addTextInput("CONFIG_USERNAME", CONFIG_USERNAME));
}
if (var == "{TEXT_SECUREPASSWD}") {	
message += (TEXT_SECUREPASSWD);
}
if (var == "{CONFIG_PASSWORD}") {	
message += (_addPasswdInput("CONFIG_PASSWORD", CONFIG_PASSWORD));
}

if (var == "{TEXT_SECURELOGOUTINFO}") {	
if (!CONFIG_AUTH) {
  message += ("");
} else {
  message += (TEXT_SECURELOGOUTINFO);
}
}

if (var == "{DEBUG}") {	
message += (_addBoolSelect("DEBUG", DEBUG));
}
if (var == "{TEXT_CALIBMETHOD}") {	
message += (TEXT_CALIBMETHOD);
}
if (var == "{CalibrationModelSelect}") {	
message += (_addModelSelect("MODEL", MODEL));
}
if (var == "{TEXT_CALIB1}") {	
message += (TEXT_CALIB1);
}
if (var == "{calib1}") {	
message += (String(calib1));
}
if (var == "{TEXT_CALIB2}") {	
message += (TEXT_CALIB2);
}
if (var == "{calib2}") {	
message += (String(calib2));
}

if (var == "{TEXT_SOFTWATEVERSION}") {	
message += (TEXT_SOFTWATEVERSION);
}
if (var == "{SOFTWAREVERSION}") {	
message += (SOFTWAREVERSION);
}

if (var == "{TEXT_AUTOUPDATEON}") {	
message += (TEXT_AUTOUPDATEON);
}
if (var == "{AUTOUPDATEON}") {	
message += (_addBoolSelect("AUTOUPDATE_ON", AUTOUPDATE_ON));
}
  /*
if (var == "{TEXT_UPDATEPAGEAUTOUPDATEWARNIN}") {	
#ifdef ARDUINO_ARCH_ESP8266
  message += (TEXT_UPDATEPAGEAUTOUPDATEWARNING);
#elif defined ARDUINO_ARCH_ESP32
  message += ("");
#endif
}
  */

#ifdef ARDUINO_ARCH_ESP8266
if (var == "{WEB_CONFIG_DEVICE_HOMEKIT}") {	
message += ("");
}
#elif defined ARDUINO_ARCH_ESP32
if (var == "{WEB_CONFIG_DEVICE_HOMEKIT}") {	
message += (WEB_CONFIG_DEVICE_HOMEKIT);
}

if (var == "{TEXT_HOMEKIT_SUPPORT}") {	
message += (TEXT_HOMEKIT_SUPPORT);
}
if (var == "{HOMEKIT_SUPPORT_ON}") {	
message += (_addBoolSelect("HOMEKIT_SUPPORT", HOMEKIT_SUPPORT));
}
if (var == "{TEXT_HOMEKIT_IS_PAIRED}") {	
if (HOMEKIT_SUPPORT == true) {
    // Serial.println("homekit_is_paired: " + String(homekit_is_paired()));
  if (String(homekit_is_paired()) == "1") {
	  message += (TEXT_HOMEKIT_IS_PAIRED);
	}
  }	
}
if (var == "{HOMEKIT_PAIRED_RESET}") {	
if (HOMEKIT_SUPPORT == true) {
    // Serial.println("homekit_is_paired: " + String(homekit_is_paired()));
  if (String(homekit_is_paired()) == "1") {
	  message += (_add_homekit_reset());  
	}
  }	
}
#endif
  
if (var == "{WiFiEraseButton}") {
	message += (_addWiFiErase());	
}
if (var == "{RestoreConfigButton}") {
	message += (_addRestoreConfig());	
}
if (var == "{SubmitButton}") {	
	message += (_addSubmitDevice());
}

    return message;	
}

//void _handle_config_device(bool is_success) {
void handle_config_device(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
	  if(!request->authenticate(CONFIG_USERNAME,CONFIG_PASSWORD,NULL,false)) {
	        request->requestAuthentication(NULL,false); // force basic auth
	      }
  }
  
  if (DEBUG) {
	Serial.print("sizeof(WEB_CONFIG_DEVICE_PAGE_ALL): ");
  Serial.println(sizeof(WEB_CONFIG_DEVICE_PAGE_ALL)); // sizeof(WEB_CONFIG_DEVICE_PAGE_ALL): 3781
	Serial.print("\n");
}

request->send_P(200, "text/html", WEB_CONFIG_DEVICE_PAGE_ALL, handle_config_device_processor);
}

String handle_config_services_processor(const String& var)
{
	//Serial.println("var: " + var);
	String message;
	
    if (var == "{Language}") {
        message +=  (TEXT_LANG);    	
    }
	
    if (var == "{WEB_PAGE_CSS}") {
      message += (WEB_PAGE_HEADER_CSS);
	  } 
	
      if (var == "{SMOGLY_LOGO_URL}") {
        message += (SMOGLY_LOGO_URL);
  	  } 
	  
      if (var == "{CurrentPageTitle}") {
        message += (TEXT_CONFIG_PAGE);
    }
      if (var == "{IndexPageTitle}") {
        message += (TEXT_INDEX_PAGE);
    }
      if (var == "{ConfigPageTitle}") {
        message += (TEXT_CONFIG_PAGE);
    }
      if (var == "{UpdatePageTitle}") {
        message += (TEXT_UPDATE_PAGE);
  	  } 
	
    if (var == "{TEXT_CONFIG_SERVICES_PAGE}") {	
  message += (TEXT_CONFIG_SERVICES_PAGE);
}
	
    if (var == "{TEXT_INSTRUCIONSLINK}") {	
  message += (TEXT_INSTRUCIONSLINK);
  message.replace("{GITHUB_LINK}", String(GITHUB_LINK));
  message.replace("{TEXT_HERE}", (TEXT_HERE));
}

    if (var == "{TEXT_SENDINGINTERVAL}") {	
  message += (TEXT_SENDINGINTERVAL);
}
    if (var == "{SENDING_FREQUENCY}") {	
		if (FREQUENTMEASUREMENT == true) {
  message += _addIntInput("SENDING_FREQUENCY", SENDING_FREQUENCY, "{TEXT_SECONDS}");
  message.replace(F("{TEXT_SECONDS}"), (TEXT_SECONDS));
} else {
    message += _addIntInput("SENDING_DB_FREQUENCY", SENDING_DB_FREQUENCY, "{TEXT_MINUTES}");
    message.replace(F("{TEXT_MINUTES}"), (TEXT_MINUTES));
}
}

    if (var == "{TEXT_DBSENDINGINTERVAL}") {	
  message += (TEXT_DBSENDINGINTERVAL);
}
    if (var == "{SENDING_DB_FREQUENCY}") {	
		if (FREQUENTMEASUREMENT == true) {
  message += _addIntInput("SENDING_DB_FREQUENCY", SENDING_DB_FREQUENCY, "{TEXT_SECONDS}");
  message.replace(F("{TEXT_SECONDS}"), (TEXT_SECONDS));
} else {
    message += _addIntInput("SENDING_DB_FREQUENCY", SENDING_DB_FREQUENCY, "{TEXT_MINUTES}");
    message.replace(F("{TEXT_MINUTES}"), (TEXT_MINUTES));
}
}

    if (var == "{TEXT_SMOGLISTSENDING}") {	
  message += (TEXT_SMOGLISTSENDING);
  message.replace(F("{SMOGLIST_LINK}"), String(SMOGLIST_LINK));
  
}

    if (var == "{SMOGLIST_ON}") {	
  message += (_addBoolSelect("SMOGLIST_ON", SMOGLIST_ON));
}
    if (var == "{TEXT_SMOGLISTINFO}") {	
  message += (TEXT_SMOGLISTINFO);
}

    if (var == "{TEXT_LUFTDATENSENDING}") {	
  message += (TEXT_LUFTDATENSENDING);
  message.replace(F("{LUFTDATEN_LINK}"), (LUFTDATEN_LINK));
  message.replace(F("{LUFTDATENFORM_LINK}"), (LUFTDATENFORM_LINK));
}
    if (var == "{LUFTDATEN_ON}") {	
  message += (_addBoolSelect("LUFTDATEN_ON", LUFTDATEN_ON));
}

    if (var == "{LUFTDATEN_ChipID}") {	
#ifdef ARDUINO_ARCH_ESP8266
  message += ("smogomierz-" + String(ESP.getChipId()));  
#elif defined ARDUINO_ARCH_ESP32
  message += ("smogomierz-" + String((uint32_t)(ESP.getEfuseMac())));
#endif  
}


if (var == "{Sensors_Pins_html}") {	
	if (strcmp(THP_MODEL, "Non")) {
	    message += (WEB_CONFIG_SERVICES_THP_SENSOR_PIN);
	} else {
		message += "";
	}
}
if (!strcmp(THP_MODEL, "BME280")) {
    if (var == "{THPSENSOR}") {	
  message += ("BME280");
 }
 if (var == "{THPXPIN}") {	
	 message += ("11");
 }
} else if (!strcmp(THP_MODEL, "BMP280")) {
    if (var == "{THPSENSOR}") {	
  message += ("BMP280");
 }
 if (var == "{THPXPIN}") {	
	 message += ("3");
 }
} else if (!strcmp(THP_MODEL, "HTU21")) {
    if (var == "{THPSENSOR}") {	
  message += ("HTU21");
 }
 if (var == "{THPXPIN}") {	
	 message += ("7");
 }
} else if (!strcmp(THP_MODEL, "DHT22")) {
    if (var == "{THPSENSOR}") {	
  message += ("DHT22");
 }
 if (var == "{THPXPIN}") {	
	 message += ("7");
 }
} else if (!strcmp(THP_MODEL, "SHT1x")) {
    if (var == "{THPSENSOR}") {	
  message += ("SHT1x");
 }
 if (var == "{THPXPIN}") {	
	 message += ("12");
 }
} else if (!strcmp(THP_MODEL, "DS18B20")) {
    if (var == "{THPSENSOR}") {	
  message += ("DS18B20");
 }
 if (var == "{THPXPIN}") {	
	 message += ("13");
 }
} 

if (var == "{Sensors_Pins_html}") {	
	if (strcmp(DUST_MODEL, "Non")) {
	    message += (WEB_CONFIG_SERVICES_DUST_MODEL_SENSOR_PIN);
	} else {
		message += "";
	}
}
if (!strcmp(DUST_MODEL, "PMS7003")) {
    if (var == "{DUSTSENSOR}") {	
  message += ("PMS5003/7003");
 }
 if (var == "{DUSTXPIN}") {	
	 message += ("1");
 }
} else if (!strcmp(DUST_MODEL, "SDS011/21")) {
    if (var == "{DUSTSENSOR}") {	
  message += ("SDS011/21");
 }
 if (var == "{DUSTXPIN}") {	
	 message += ("1");
 }
} else if (!strcmp(DUST_MODEL, "HPMA115S0")) {
    if (var == "{DUSTSENSOR}") {	
  message += ("HPMA115S0");
 }
 if (var == "{DUSTXPIN}") {	
	 message += ("1");
 }
} else if (!strcmp(DUST_MODEL, "SPS30")) {
    if (var == "{DUSTSENSOR}") {	
  message += ("SPS30");
 }
 if (var == "{DUSTXPIN}") {	
	 message += ("1");
 }
} 


   if (var == "{TEXT_AQIECOSENDING}") {	
 message += (TEXT_AQIECOSENDING);
}
if (var == "{AQI_ECO_ON}") {	
    message += (_addBoolSelect("AQI_ECO_ON", AQI_ECO_ON));
 }
   if (var == "{TEXT_AQIECOSERVER}") {	
 message += (TEXT_AQIECOSERVER);
}
   if (var == "{AQI_ECO_HOST}") {	
 message += (_addTextInput("AQI_ECO_HOST", AQI_ECO_HOST)); 
}
   if (var == "{TEXT_AQIECOPATH}") {	
	   message += (TEXT_AQIECOPATH);
}
   if (var == "{AQI_ECO_PATH}") {	
 message += (_addTextInput("AQI_ECO_PATH", AQI_ECO_PATH)); 
}

#ifdef ARDUINO_ARCH_ESP8266
if (var == "{ESP_MODEL}") {	
 message += ("ESP8266");
}
if (var == "{AQI_ECO_ChipID}") {	
  message += (String(ESP.getChipId()));
}
#elif defined ARDUINO_ARCH_ESP32
if (var == "{ESP_MODEL}") {	
 message += ("ESP32");
}
if (var == "{AQI_ECO_ChipID}") {	
  message += (String((uint32_t)(ESP.getEfuseMac())));
}
#endif
  

if (var == "{TEXT_AIRMONITORSENDING}") {	
 message += (TEXT_AIRMONITORSENDING);
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
}
if (var == "{AIRMONITOR_ON}") {	
    message += (_addBoolSelect("AIRMONITOR_ON", AIRMONITOR_ON));
 }
 if (var == "{TEXT_AIRMONITORCHARTS}") {	
  message += (TEXT_AIRMONITORCHARTS);
 }
 if (var == "{AIRMONITOR_GRAPH_ON}") {	
     message += (_addBoolSelect("AIRMONITOR_GRAPH_ON", AIRMONITOR_GRAPH_ON));
  }
  if (var == "{TEXT_AIRMONITORCOORDINATESINFO}") {	
      message += (TEXT_AIRMONITORCOORDINATESINFO);
	  message.replace(F("{LATLONG_LINK}"), (LATLONG_LINK));
	  message.replace(F("{TEXT_HERE}"), (TEXT_HERE));
   }
   if (var == "{TEXT_AIRMONITORLATITUDE}") {	
    message += (TEXT_AIRMONITORLATITUDE);
   }
   if (var == "{LATITUDE}") {	
       message += (_addFloatInput("LATITUDE", atof(LATITUDE), 6, "°"));
    }
    if (var == "{TEXT_AIRMONITORLONGITUDE}") {	
     message += (TEXT_AIRMONITORLONGITUDE);
    }
    if (var == "{LONGITUDE}") {	
        message += (_addFloatInput("LONGITUDE", atof(LONGITUDE), 6, "°"));
     }


     if (var == "{TEXT_THINGSPEAKSENDING}") {	
      message += (TEXT_THINGSPEAKSENDING);
	  message.replace(F("{THINGSPEAK_LINK}"), (THINGSPEAK_LINK));
     }
	 if (var == "{THINGSPEAK_ON}") {	
	     message += (_addBoolSelect("THINGSPEAK_ON", THINGSPEAK_ON));
	  }
      if (var == "{TEXT_THINGSPEAKCHARTS}") {	
       message += (TEXT_THINGSPEAKCHARTS);
      }
 	 if (var == "{THINGSPEAK_GRAPH_ON}") {	
 	     message += (_addBoolSelect("THINGSPEAK_GRAPH_ON", THINGSPEAK_GRAPH_ON));
 	  }
      if (var == "{TEXT_THINGSPEAK_WRITE_API_KEY}") {	
       message += (TEXT_THINGSPEAK_WRITE_API_KEY);
      }
 	 if (var == "{THINGSPEAK_API_KEY}") {	
 	     message += (_addTextInput("THINGSPEAK_API_KEY", THINGSPEAK_API_KEY));
 	  }
      if (var == "{TEXT_THINGSPEAKCHANNELID}") {	
       message += (TEXT_THINGSPEAKCHANNELID);
      }
 	 if (var == "{THINGSPEAK_CHANNEL_ID}") {	
 	     message += (_addIntInput("THINGSPEAK_CHANNEL_ID", THINGSPEAK_CHANNEL_ID));
 	  }
      if (var == "{TEXT_THINGSPEAK_READ_API_KEY}") {	
       message += (TEXT_THINGSPEAK_READ_API_KEY);
      }
 	 if (var == "{THINGSPEAK_READ_API_KEY}") {	
 	     message += (_addTextInput("THINGSPEAK_READ_API_KEY", THINGSPEAK_READ_API_KEY));
	 }
	 
     if (var == "{TEXT_INFLUXDBSENDING}") {	
      message += (TEXT_INFLUXDBSENDING);
     }
	 if (var == "{INFLUXDB_ON}") {	
	     message += (_addBoolSelect("INFLUXDB_ON", INFLUXDB_ON));
	  }
      if (var == "{TEXT_INFLUXDBVERSION}") {	
       message += (TEXT_INFLUXDBVERSION);
      }
 	 if (var == "{INFLUXDB_VERSION}") {	
 	     message += (_addINFLUXDB_VERSIONSelect("INFLUXDB_VERSION", INFLUXDB_VERSION));
 	  }
      if (var == "{TEXT_INFLUXDBSERVER}") {	
       message += (TEXT_INFLUXDBSERVER);
      }
 	 if (var == "{INFLUXDB_HOST}") {	
 	     message += (_addTextInput("INFLUXDB_HOST", INFLUXDB_HOST));
 	  }
      if (var == "{TEXT_INFLUXDBPORT}") {	
       message += (TEXT_INFLUXDBPORT);
      }
 	 if (var == "{INFLUXDB_PORT}") {	
 	     message += (_addIntInput("INFLUXDB_PORT", INFLUXDB_PORT));
 	  }
      if (var == "{TEXT_INFLUXDBNAME}") {	
       message += (TEXT_INFLUXDBNAME);
      }
 	 if (var == "{INFLUXDB_DATABASE}") {	
 	     message += ( _addTextInput("INFLUXDB_DATABASE", INFLUXDB_DATABASE));
 	  }
      if (var == "{TEXT_INFLUXDBUSER}") {	
       message += (TEXT_INFLUXDBUSER);
      }
 	 if (var == "{INFLUXDB_USER}") {	
 	     message += (_addTextInput("INFLUXDB_USER", INFLUXDB_USER));
 	  }
      if (var == "{TEXT_INFLUXDBPASSWD}") {	
       message += (TEXT_INFLUXDBPASSWD);
      }
 	 if (var == "{INFLUXDB_PASSWORD}") {	
 	     message += (_addPasswdInput("INFLUXDB_PASSWORD", INFLUXDB_PASSWORD));
 	  }
      if (var == "{WEB_CONFIG_SERVICES_INFLUXDB_VE}") {	
		  if (!strcmp(INFLUXDB_VERSION, "2")) {
  	    message += (WEB_CONFIG_SERVICES_INFLUXDB_VERSION_V2);
  	  message.replace(F("{TEXT_INFLUXDBORG}"), (TEXT_INFLUXDBORG));
  	  message.replace(F("{INFLUXDB_ORG}"), _addTextInput("INFLUXDB_ORG", INFLUXDB_ORG));
  	  message.replace(F("{TEXT_INFLUXDBBUCKET}"), (TEXT_INFLUXDBBUCKET));
  	  message.replace(F("{INFLUXDB_BUCKET}"), _addTextInput("INFLUXDB_BUCKET", INFLUXDB_BUCKET));
  	  message.replace(F("{TEXT_INFLUXDBTOKEN}"), (TEXT_INFLUXDBTOKEN));
  	  message.replace(F("{INFLUXDB_TOKEN}"), _addTextInput("INFLUXDB_TOKEN", INFLUXDB_TOKEN));
  } else {
  	message += "";
  }
  	}
	/*
    if (var == "{TEXT_MQTTSENDING}") {	
     message += (TEXT_MQTTSENDING);
    }
 if (var == "{MQTT_ON}") {	
     message += (_addBoolSelect("MQTT_ON", MQTT_ON));
  }
   if (var == "{TEXT_MQTTSERVER}") {	
    message += (TEXT_MQTTSERVER);
   }
if (var == "{MQTT_HOST}") {	
    message += (_addTextInput("MQTT_HOST", MQTT_HOST));
 }
   if (var == "{TEXT_MQTTPORT}") {	
    message += (TEXT_MQTTPORT);
   }
if (var == "{MQTT_PORT}") {	
    message += (_addIntInput("MQTT_PORT", MQTT_PORT));
 }
   if (var == "{TEXT_MQTTUSER}") {	
    message += (TEXT_MQTTUSER);
   }
if (var == "{MQTT_USER}") {	
    message += (_addTextInput("MQTT_USER", MQTT_USER));
 }
   if (var == "{TEXT_MQTTPASSWD}") {	
    message += (TEXT_MQTTPASSWD);
   }
if (var == "{MQTT_PASSWORD}") {	
    message += (_addPasswdInput("MQTT_PASSWORD", MQTT_PASSWORD));
 }
 */
 /*
 if (var == "{TEXT_MQTT_TOPIC_INFO}") {	
  message += (TEXT_MQTT_TOPIC_INFO);
 }
 if (var == "{TEXT_MQTT_IP_IN_TOPIC}") {	
  message += (TEXT_MQTT_IP_IN_TOPIC);
 }
 if (var == "{MQTT_IP_IN_TOPIC}") {	
  message += (_addBoolSelect("MQTT_IP_IN_TOPIC", MQTT_IP_IN_TOPIC));
 }
 if (var == "{TEXT_MQTT_DEVICENAME_IN_TOPIC}") {	
  message += (TEXT_MQTT_DEVICENAME_IN_TOPIC);
 }
 if (var == "{MQTT_DEVICENAME_IN_TOPIC}") {	
  message += (_addBoolSelect("MQTT_DEVICENAME_IN_TOPIC", MQTT_DEVICENAME_IN_TOPIC));
 }
 if (var == "{TEXT_MQTT_SLASH_AT_THE_BEGINNIN") {	
  message += (TEXT_MQTT_SLASH_AT_THE_BEGINNING);
 }
 if (var == "{MQTT_SLASH_AT_THE_BEGINNING}") {	
  message += (_addBoolSelect("MQTT_SLASH_AT_THE_BEGINNING", MQTT_SLASH_AT_THE_BEGINNING));
 }
 if (var == "{TEXT_MQTT_SLASH_AT_THE_END}") {	
  message += (TEXT_MQTT_SLASH_AT_THE_END);
 }
 if (var == "{MQTT_SLASH_AT_THE_END}") {	
  message += (_addBoolSelect("MQTT_SLASH_AT_THE_END", MQTT_SLASH_AT_THE_END));
 }
 */
 /*
 if (var == "{TEXT_TEMP_TOPIC}") {	
  message += (TEXT_TEMPERATURE);
 }
 if (var == "{MQTT_IP}") {	
  message += (String(WiFi.localIP().toString()));
 }
 if (var == "{MQTT_DEVICENAME}") {	
  message += (String(device_name));
 }
 if (var == "{MQTT_TOPIC_TEMP}") {	
  message += (_addMQTTTextInput("MQTT_TOPIC_TEMP", MQTT_TOPIC_TEMP));
 }
 if (var == "{MQTT_TEMP}") {	
  message += (String(int(currentTemperature)));
 }
 */
 
 if (var == "{AdvancedMQTTConfigButton}") {	
  message += (ASW_WEB_GOTO_CONFIG_ADVANCED_MQTT_PAGE_BUTTON);
  message.replace(F("{TEXT_CONFIG_ADV_MQTT}"), (TEXT_CONFIG_ADV_MQTT));
 }
 
 
 if (var == "{WiFiEraseButton}") {	
  message += (_addWiFiErase());
 }
 
 if (var == "{RestoreConfigButton}") {	
  message += (_addRestoreConfig());
 }
 
 if (var == "{SubmitButton}") {	
	message += ("<input type='submit' name='submit2' class='btn btn-outline-danger' value='{TEXT_SAVE}' />");
    message.replace("{TEXT_SAVE}", (TEXT_SAVE));
 }
 
    return message;
}

//void _handle_config_services(bool is_success) {
void handle_config_services(AsyncWebServerRequest *request) {
	  if (CONFIG_AUTH == true) {
		  if(!request->authenticate(CONFIG_USERNAME,CONFIG_PASSWORD,NULL,false)) {
		        request->requestAuthentication(NULL,false); // force basic auth
		      }
	  }
	  
	  if (DEBUG) {
    	Serial.print("sizeof(WEB_CONFIG_SERVICES_PAGE_ALL): ");
      Serial.println(sizeof(WEB_CONFIG_SERVICES_PAGE_ALL)); // sizeof(WEB_CONFIG_SERVICES_PAGE_ALL): 5518
    	Serial.print("\n");
    }

	   request->send_P(200, "text/html", WEB_CONFIG_SERVICES_PAGE_ALL, handle_config_services_processor);
  //request->send(200, "text/html", message);
}

void handle_adv_mqtt_config(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
	  if(!request->authenticate(CONFIG_USERNAME,CONFIG_PASSWORD,NULL,false)) {
	        request->requestAuthentication(NULL,false); // force basic auth
	      }
  }
  String message;
  
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
  	Serial.print("handle_adv_mqtt_config - message.length(): "); // 18754
    Serial.println(message.length()); // keep it under 20000!
  	Serial.print("\n");
}
  request->send(200, "text/html", message);
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

/*
void _parseAsCString(char* dest, String value) {
  strncpy(dest, value.c_str(), 255);
}
*/

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
  int need_update = 0;
  // REMEMBER TO ADD/EDIT KEYS IN config.h AND spiffs.cpp!!
  /*
  int paramsNr = request->params();
  if (DEBUG) {	
for(int i=0;i<paramsNr;i++){
 
     AsyncWebParameter* p = request->getParam(i);
 
     Serial.print("Param name: ");
     Serial.println(p->name());
 
     Serial.print("Param value: ");
     Serial.println(p->value());
 
     Serial.println("------");
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
  
  if (DEBUG) {
    Serial.println("POST DEVICE CONFIG END!!");
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
    Serial.println("POST CONFIG START!!");
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
		_parseAsCString(INFLUXDB_TOKEN, request->getParam("INFLUXDB_TOKEN")->value(), 64);
	}

  if (DEBUG) {
    Serial.println("POST SERVICES CONFIG END!!");
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
  if (!index){
    Serial.println("Update");
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
    if (!Update.end(true)){
      Update.printError(Serial);
    } else {
      Serial.println("Update complete");
	  Serial.println("Restart");
	  delay(1000);
	  ESP.restart();
    }
  }
}

void handle_update_done(AsyncWebServerRequest *request) {
	request->send(200);
	}

//void handle_update() {            //Handler for the handle_update
void handle_update(AsyncWebServerRequest *request) {
    if (CONFIG_AUTH == true) {
  	  /*
      if (!server.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
        //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
        return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
      }
  	*/
  	  if(!request->authenticate(CONFIG_USERNAME,CONFIG_PASSWORD,NULL,false)) {
  	        request->requestAuthentication(NULL,false); // force basic auth
  	      }
    }
    String message = FPSTR(WEB_PAGE_HEADER);
  message.replace("{WEB_PAGE_CSS}", FPSTR(WEB_PAGE_HEADER_CSS));
  message.replace("{Language}", (TEXT_LANG));
  message.replace("{CurrentPageTitle}", (TEXT_UPDATE_PAGE));
  message.replace("{IndexPageTitle}", (TEXT_INDEX_PAGE));
  message.replace("{ConfigPageTitle}", (TEXT_CONFIG_PAGE));
  message.replace("{UpdatePageTitle}", (TEXT_UPDATE_PAGE));

  message += FPSTR(WEB_UPDATE_PAGE_UPDATE);

  if (!AUTOUPDATE_ON) {
    if (need_update) {
      message.replace("{WEB_UPDATE_INFO_WARNING}", FPSTR(WEB_UPDATE_INFO_WARNING));
      message.replace("{TEXT_FWUPDATEAVALIBLE}", (TEXT_FWUPDATEAVALIBLE));
      message.replace("{MANUALUPDATEBUTTON}", "");
      message.replace("{FWUPDATEBUTTON}", FPSTR(WEB_UPDATE_BUTTON_FWUPDATE));
      message.replace("{TEXT_FWUPDATEBUTTON}", (TEXT_FWUPDATEBUTTON));
      message.replace("{AUTOUPDATEONBUTTON}", FPSTR(WEB_UPDATE_BUTTON_AUTOUPDATEON));
      message.replace("{TEXT_AUTOUPDATEONBUTTON}", (TEXT_AUTOUPDATEONBUTTON));
      message.replace("{TEXT_AUTOUPDATEWARNING}", (TEXT_AUTOUPDATEWARNING));
      message.replace("{TEXT_FWUPDATEBUTTON}", (TEXT_FWUPDATEBUTTON));
    }
    message.replace("{WEB_UPDATE_INFO_WARNING}", "");
  } else {
    message.replace("{WEB_UPDATE_INFO_WARNING}", "");
  }

  message.replace("{TEXT_UPDATE_PAGE}", (TEXT_UPDATE_PAGE));
  message.replace("{TEXT_SELECTUPDATEFILE}", (TEXT_SELECTUPDATEFILE));
  message.replace("{TEXT_SUBMITUPDATE}", (TEXT_SUBMITUPDATE));

  message.replace("{TEXT_AUTOUPDATEON}", (TEXT_AUTOUPDATEON));
  if (AUTOUPDATE_ON) {
    message.replace("{AUTOUPDATEONSTATUS}", (TEXT_YES));
  } else {
    message.replace("{AUTOUPDATEONSTATUS}", (TEXT_NO));
  }

  message.replace("{TEXT_CURRENTSOFTVERSION}", (TEXT_CURRENTSOFTVERSION));
  message.replace("{SOFTWAREVERSION}", String(CURRENTSOFTWAREVERSION) + " " + String(PMSENSORVERSION));

  message.replace("{TEXT_SERVERSOFTWAREVERSION}", (TEXT_SERVERSOFTWAREVERSION));
  message.replace("{SERVERSOFTWAREVERSION}", String(SERVERSOFTWAREVERSION) + " " + String(PMSENSORVERSION));

  message.replace("{TEXT_LATESTAVAILABLESOFT}", TEXT_LATESTAVAILABLESOFT);
  message.replace("{SMOGOMIERZRELEASES_LINK}", (SMOGOMIERZRELEASES_LINK));
  message.replace("{TEXT_HERE}", (TEXT_HERE));

  // init WiFi signal quality info - START
  String WiFiSSID = WiFi.SSID();
  int WiFiRSSI = WiFi.RSSI();
  message.replace("{TEXT_CONNECTEDWIFI}", (TEXT_CONNECTEDWIFI));
  message.replace("{WiFiSSID}", (WiFiSSID));
  message.replace("{TEXT_WIFIRSSI}", (TEXT_WIFIRSSI));
  message.replace("{WiFiRSSI}", (String(WiFiRSSI) + " dBm"));

  // https://stackoverflow.com/a/15798024
  int WiFiQuality;
  if (WiFiRSSI <= -100) {
    WiFiQuality = 0;
  } else if (WiFiRSSI >= -50) {
    WiFiQuality = 100;
  } else {
    WiFiQuality = 2 * (WiFiRSSI + 100);
  }
  message.replace("{TEXT_WIFIQUALITY}", (TEXT_WIFIQUALITY));
  message.replace("{WiFiQuality}", (String(WiFiQuality) + " %"));
  // init WiFi signal quality info - END
  
  message += FPSTR(WEB_PAGE_FOOTER);
  //WebServer.send(200, "text/html", message);
  request->send(200, "text/html", message);
}

//void erase_wifi() {
void erase_wifi(AsyncWebServerRequest *request) {
    if (CONFIG_AUTH == true) {
  	  /*
      if (!server.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
        //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
        return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
      }
  	*/
  	  if(!request->authenticate(CONFIG_USERNAME,CONFIG_PASSWORD,NULL,false)) {
  	        request->requestAuthentication(NULL,false); // force basic auth
  	      }
    }
  Serial.println("Erasing Config...");
#ifdef ARDUINO_ARCH_ESP8266
  ESP.eraseConfig();
#elif defined ARDUINO_ARCH_ESP32
  WiFi.disconnect(false, true);
#endif
  /*
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  */
  request->redirect("/");
  delay(1000);
  Serial.println("Restart");
  ESP.restart();
}

//void restore_config() {
void restore_config(AsyncWebServerRequest *request) {
    if (CONFIG_AUTH == true) {
  	  /*
      if (!server.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
        //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
        return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
      }
  	*/
  	  if(!request->authenticate(CONFIG_USERNAME,CONFIG_PASSWORD,NULL,false)) {
  	        request->requestAuthentication(NULL,false); // force basic auth
  	      }
    }
  Serial.println("Restoring default settings...");
  deleteConfig();
  /*
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  */
  request->redirect("/");
  delay(1000);
  Serial.println("Restart");
  ESP.restart();
}

//void fwupdate() {
void fwupdate(AsyncWebServerRequest *request) {
    if (CONFIG_AUTH == true) {
  	  /*
      if (!server.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
        //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
        return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
      }
  	*/
  	  if(!request->authenticate(CONFIG_USERNAME,CONFIG_PASSWORD,NULL,false)) {
  	        request->requestAuthentication(NULL,false); // force basic auth
  	      }
    }
  doUpdate(0);
  delay(1000);
  /*
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  */
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
  Serial.println("Restart");
  ESP.restart();
}

//void autoupdate_on() {
void autoupdate_on(AsyncWebServerRequest *request) {
    if (CONFIG_AUTH == true) {
  	  /*
      if (!server.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
        //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
        return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
      }
  	*/
  	  if(!request->authenticate(CONFIG_USERNAME,CONFIG_PASSWORD,NULL,false)) {
  	        request->requestAuthentication(NULL,false); // force basic auth
  	      }
    }
  AUTOUPDATE_ON = true;
  saveConfig();
  /*
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  */
  request->redirect("/");
  delay(1000);
  Serial.println("Restart");
  ESP.restart();
}

//void handle_api() {
void handle_api(AsyncWebServerRequest *request) {
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

  serializeJsonPretty(json, message);
  //WebServer.send(200, "text/json", message);
  request->send(200, "text/json", message);
}

void homekit_reset(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    /*
      if (!server.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    */
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD, NULL, false)) {
      request->requestAuthentication(NULL, false); // force basic auth
    }
  }
  Serial.println("reset homekit...");

  String pair_file_name = "/homekit_pair.dat";
  SPIFFS.remove(pair_file_name);
  
  request->redirect("/");

  //request->redirect("/");
  delay(1000);
  //Serial.println("Restart");
  //ESP.restart();
}

void homekit_on(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    /*
      if (!server.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    */
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD, NULL, false)) {
      request->requestAuthentication(NULL, false); // force basic auth
    }
  }
  Serial.println("homekit on...");

  HOMEKIT_SUPPORT = true;
  saveConfig();
  
  request->redirect("/");

  delay(1000);
  Serial.println("Restart");
  ESP.restart();
}

void homekit_off(AsyncWebServerRequest *request) {
  if (CONFIG_AUTH == true) {
    /*
      if (!server.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    */
    if (!request->authenticate(CONFIG_USERNAME, CONFIG_PASSWORD, NULL, false)) {
      request->requestAuthentication(NULL, false); // force basic auth
    }
  }
  Serial.println("homekit off...");

  HOMEKIT_SUPPORT = false;
  saveConfig();
  
  request->redirect("/");

  delay(1000);
  Serial.println("Restart");
  ESP.restart();
}