
void handle_root () {
      String message = "<html lang='pl'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Pomiary</title>";
      message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
      message += "</head><body>";
      message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Pomiary</a>";
      message += "<a href='/config' class='navbar-brand'>Konfiguracja</a>";
      message += "<a href='/update' class='navbar-brand'>Update</a>";
      message += "</div></nav>";
      message += "<main role='main' class='container'><div class='jumbotron'>";
      message += "<center><h1>Smogomierz</h1><br>";

      if (checkBmeStatus()) {
      message += "<h2>Pogoda:</h2>";
      message += "<h3>Temperatura: ";
      message += (BMESensor.temperature);
      message += " °C</h3>";
      message += "<h3>Ciśnienie: ";
      message += (BMESensor.pressure  / 100.0F);
      message += " hPa</h3>";
      message += "<h3>Wilgotność: ";
      message += (BMESensor.humidity);
      message += " %</h3>";
      message += "<h3>Punkt rosy: ";
      message += (BMESensor.temperature-((100-BMESensor.humidity)/5));
      message += " °C</h3>";
      }
      message += "<p><h2>Pomiary zanieczyszczeń:</h2>";
      
      if (DISPLAY_PM1){
          message += "<h3>PM1: ";
          message += (averagePM1);
          message += " µg/m³</h3>";
        }
        message += "<h3>PM2.5: ";
        if (averagePM25 <= 10){
          message += "<font color='#61EEE4'>";
        } else if (averagePM25 > 10 && averagePM25 <= 20){
          message += "<font color='#5BCAAA'>";
        } else if (averagePM25 > 20 && averagePM25 <= 25){
          message += "<font color='#EEE25D'>";
        } else if (averagePM25 > 25 && averagePM25 <= 50){
          message += "<font color='#F95459'>";
        } else if (averagePM25 > 50){
          message += "<font color='#920736'>";
        } else {
          message += "<font>";
        }
        message += averagePM25;
        message += " µg/m³</h3>";
		message += "</font>";
        
        message += "<h3>PM10: ";
        if (averagePM10 <= 20){
          message += "<font color='#61EEE4'>";
        } else if (averagePM10 > 20 && averagePM10 <= 35){
          message += "<font color='#5BCAAA'>";
        } else if (averagePM10 > 35 && averagePM10 <= 50){
          message += "<font color='#EEE25D'>";
        } else if (averagePM10 > 50 && averagePM10 <= 100){
          message += "<font color='#F95459'>";
        } else if (averagePM10 > 100){
          message += "<font color='#920736'>";
        } else {
          message += "<font>";
        }      
        message += averagePM10;
        message += " µg/m³</h3>";
		message += "</font>";

  if(AIRMONITOR_GRAPH_ON){
      message += ("<hr>");
      message += ("<iframe src='http://metrics.airmonitor.pl:3000/dashboard-solo/db/airmonitor?orgId=1&var-latitude=");
      message += (String(LATITUDE, 4));
      message += ("&var-longitude=");
      message += (String(LONGITUDE, 4));
      message += ("&refresh=1m&panelId=14' width='450' height='510' frameborder='0'></iframe>");
      message += (" ");
      message += ("<iframe src='http://metrics.airmonitor.pl:3000/dashboard-solo/db/airmonitor?orgId=1&var-latitude=");
      message += (String(LATITUDE, 4));
      message += ("&var-longitude=");
      message += (String(LONGITUDE, 4));
      message += ("&refresh=1m&panelId=13' width='450' height='510' frameborder='0'></iframe>");
  }
  if(THINGSPEAK_GRAPH_ON){
      message += ("<hr>");
      /*
      if (DISPLAY_PM1) {
        message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
        message += (THINGSPEAK_CHANNEL_ID);
        message += ("/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=PM1&type=line&yaxis=ug%2Fm3&update=15'></iframe>");
        message += (" ");
      }*/
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=PM2.5&type=line&yaxis=ug%2Fm3&update=15'></iframe>");
      message += (" ");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=PM10&type=line&yaxis=ug%2Fm3&update=15'></iframe>");
      message += (" ");
      if (int(BMESensor.temperature) == 0 && int(BMESensor.humidity) == 0 && int(BMESensor.pressure  / 100.0F) == 0){
        Serial.println("Brak pomiarow z BME280!\n");
      }else{
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Temperatura&type=line&update=15'></iframe>");
      message += (" ");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Ci%C5%9Bnienie&type=line&update=15'></iframe>");
      message += (" ");
      message += ("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
      message += (THINGSPEAK_CHANNEL_ID);
      message += ("/charts/6?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Wilgotno%C5%9B%C4%87&type=line&update=15'></iframe>");
      }
    }

    message += "</form></div>";
    message += "</center></main></body></html>";
    WebServer.send(200, "text/html", message);
  }

String _addOption(const String &value, const String &label, const String &srslyValue) {
    String option = "";
    
    option += "<option value='";
    option += value;
    if (value == srslyValue) {
        option += "' selected>";
    } else {
        option += "'>";
    }
    option += label;
    option += "</option>";
    return option;
}

String _addBoolSelect(const String &key, const bool &value) {
    String selectValue = value? "yes": "no";
    String input = "<select name='";
    input += key;
    input += "'>";
    input += _addOption("yes", "Tak", selectValue);
    input += _addOption("no", "Nie", selectValue);
    input += "</select><br />";
    return input;
}

String _addModelSelect(const String &key, const String &value) {
    String input = "<select name='";
    input += key;
    input += "'>";
    input += _addOption("black", "BLACK EDITION", value);
    input += _addOption("white", "WHITE EDITION", value);
    input += _addOption("red", "RED EDITION", value);
    input += "</select><br />";
    return input;
}

String _escapeString (const String &value) {
    String trimmed = value;
    trimmed.trim();
    trimmed.replace("'", "&#39;");
    return trimmed;
}

String _addTextInput(const String &key, const String &value, const String &postfix="") {
    String input = "<input type='text' maxlength='255' size='50' name='";
    input += key;
    input += "' value='";
    input += _escapeString(value);
    input += "'> ";
    input += postfix;
    input += "<br />";
    return input;
}

String _addPasswdInput(const String &key, const String &value, const String &postfix="") {
    String input = "<input type='password' maxlength='255' size='50' name='";
    input += key;
    input += "' value='";
    input += _escapeString(value);
    input += "'> ";
    input += postfix;
    input += "<br />";
    return input;
}

String _addIntInput(const String &key, const int &value, const String &postfix="") {
    String input = "<input type='number' step='1' maxlength='255' name='";
    input += key;
    input += "' value='";
    input += String(value);
    input += "'> ";
    input += postfix;
    input += "<br />";
    return input;
}

String _addFloatInput(const String &key, const double &value, const int &precision=6, const String &postfix="") {
    String input = "<input type='number' maxlength='255' name='";
    input += key;
    input += "' value='";
    input += String(value, precision);
    input += "'> ";
    input += postfix;
    input += "<br />";
    return input;
}

String _addSubmit() {
    return "<input type='submit' value='Zapisz' /><br /><br />";
}

void _handle_config(bool is_success) {
    String message = "<html lang='pl'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Config</title>";
    message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
    message += "</head><body>";
    message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Pomiary</a>";
    message += "<a href='/config' class='navbar-brand'>Konfiguracja</a>";
    message += "<a href='/update' class='navbar-brand'>Update</a>";
    message += "</div></nav>";
    message += "<form method='POST' action='/config'>";
    message += "<main role='main' class='container'><div class='jumbotron'>";

    message += "<center><h1>Smogomierz - Config</h1></center><br><br>";

    if (is_success) {
        message += "<div style='color: #2f7a2d'> <strong>ZAPISANO I ZRESETOWANO!</strong> - nie placz, wszystko jest OK </div>";
    }

    message += "<b>Nazwa urządzenia: </b>";
    if (DEVICENAME_AUTO) {
        message += device_name;
    } else {
        message += _addTextInput("DEVICENAME", DEVICENAME);
    }
    message += "<br>";

    message += "<b>Automatyczne generowanie nazwy: </b>";
    message += _addBoolSelect("DEVICENAME_AUTO", DEVICENAME_AUTO);
    
    message += "<b>Wyświetlanie pomiarów PM1: </b>";
    message += _addBoolSelect("DISPLAY_PM1", DISPLAY_PM1);
    message += _addSubmit();
  
    message += "<b>Wysyłanie danych do serwisu AirMonitor: </b>";
    message += _addBoolSelect("AIRMONITOR_ON", AIRMONITOR_ON);

    message += "<b>Wyświetlanie wykresów z serwisu AirMonitor: </b>";
    message += _addBoolSelect("AIRMONITOR_GRAPH_ON", AIRMONITOR_GRAPH_ON);
    message += _addSubmit();

    message += "<b>Współrzędne geograficzne miernika:<br>Szerokość(latitude): </b>";
    message += _addFloatInput("LATITUDE", LATITUDE, 6, "°");
    message += "<b>Długość(longitude): </b>";
    message += _addFloatInput("LONGITUDE", LONGITUDE, 6, "°");
    message += "<b>Wysokość: </b>";
    message += _addIntInput("MYALTITUDE", MYALTITUDE, "m.n.p.m");
    message += _addSubmit();
  
    message += "<b>Wysyłanie danych do serwisu ThingSpeak: </b>";
    message += _addBoolSelect("THINGSPEAK_ON", THINGSPEAK_ON);

    message += "<b>Wyświetlanie wykresów z serwisu ThingSpeak: </b>";
    message += _addBoolSelect("THINGSPEAK_GRAPH_ON", THINGSPEAK_GRAPH_ON);

    message += "<b>ThingSpeak API_KEY: </b>";
    message += _addTextInput("THINGSPEAK_API_KEY", THINGSPEAK_API_KEY);
    message += "<b>ThingSpeak Channel ID: </b>";
    message += _addIntInput("THINGSPEAK_CHANNEL_ID", THINGSPEAK_CHANNEL_ID);
    message += _addSubmit();

    message += "<b>Wysyłanie danych do InfluxDB: </b>";
    message += _addBoolSelect("INFLUXDB_ON", INFLUXDB_ON);
    message += "<br><b>Adres bazy danych InfluxDB: </b>";
    message += _addTextInput("INFLUXDB_HOST", INFLUXDB_HOST);
    message += "<b>Port InfluxDB: </b>";
    message += _addIntInput("INFLUXDB_PORT", INFLUXDB_PORT);
    message += "<b>Nazwa bazy danych: </b>";
    message += _addTextInput("DATABASE", DATABASE);
    message += "<b>Użytkownik bazy danych: </b>";
    message += _addTextInput("DB_USER", DB_USER);
    message += "<b>Hasło do bazy danych: </b>";
    message += _addPasswdInput("DB_PASSWORD", DB_PASSWORD);
    message += _addSubmit();

    message += "<b>Debug: </b>";
    message += _addBoolSelect("DEBUG", DEBUG);

    message += "<b>Model Smogomierza: </b>";
    message += _addModelSelect("MODEL", MODEL);

    message += "<b>Zmienna kalibracyjna calib1: </b>";
    message += (calib1);
    message += "<br>";

    message += "<b>Zmienna kalibracyjna calib2: </b>";
    message += (calib2);
    message += "<br>";

    message += "<b>Wersja oprogramowania: </b>";
    message += (SOFTWAREVERSION);
    message += "<br />";
    message += _addSubmit();
  
    message += "</main></form></body></html>";
    WebServer.send(200, "text/html", message);
}

bool _parseAsBool(String value) {
    return value == "yes";
}

void _set_calib1_and_calib2() {
    if (!strcmp(MODEL, "black")) {
        calib1 = 1;
        calib2 = 1;
    } else if (!strcmp(MODEL, "white")) {
        calib1 = 2;
        calib2 = 2;
    } else {
        calib1 = 3;
        calib2 = 3;
    }
}

void _parseAsCString(char* dest, String value) {
    strncpy(dest, value.c_str(), 255);
}

void handle_config() {
    _handle_config(false);
}

void handle_config_post() {

    if (DEBUG) {
        Serial.println("POST CONFIG START!!");
        int argsLen = WebServer.args();
        for (int i=0; i < argsLen; i++) {
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
    _parseAsCString(DEVICENAME, WebServer.arg("DEVICENAME"));
    DISPLAY_PM1 = _parseAsBool(WebServer.arg("DISPLAY_PM1"));
    AIRMONITOR_ON = _parseAsBool(WebServer.arg("AIRMONITOR_ON"));
    AIRMONITOR_GRAPH_ON = _parseAsBool(WebServer.arg("AIRMONITOR_GRAPH_ON"));
    LATITUDE = WebServer.arg("LATITUDE").toFloat();
    LONGITUDE = WebServer.arg("LONGITUDE").toFloat();
    MYALTITUDE = WebServer.arg("MYALTITUDE").toInt();
      
    THINGSPEAK_ON = _parseAsBool(WebServer.arg("THINGSPEAK_ON"));
    THINGSPEAK_GRAPH_ON = _parseAsBool(WebServer.arg("THINGSPEAK_GRAPH_ON"));
    _parseAsCString(THINGSPEAK_API_KEY, WebServer.arg("THINGSPEAK_API_KEY"));
    THINGSPEAK_CHANNEL_ID = WebServer.arg("THINGSPEAK_CHANNEL_ID").toInt();
      
    INFLUXDB_ON = _parseAsBool(WebServer.arg("INFLUXDB_ON"));
    _parseAsCString(INFLUXDB_HOST, WebServer.arg("INFLUXDB_HOST"));
    INFLUXDB_PORT = WebServer.arg("INFLUXDB_PORT").toInt();
    _parseAsCString(DATABASE, WebServer.arg("DATABASE"));
    _parseAsCString(DB_USER, WebServer.arg("DB_USER"));
    _parseAsCString(DB_PASSWORD, WebServer.arg("DB_PASSWORD"));

    _parseAsCString(MODEL, WebServer.arg("MODEL"));
    _set_calib1_and_calib2();

    if (DEBUG) {
        Serial.println("POST CONFIG END!!");
    }
    
    saveConfig();
    _handle_config(true);
    // https://github.com/esp8266/Arduino/issues/1722
    // ESP.reset();
}

void handle_update() {            //Handler for the handle_update
    String message = "<html lang='pl'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Update</title>";
    message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
    message += "</head><body>";
    message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Pomiary</a>";
    message += "<a href='/config' class='navbar-brand'>Konfiguracja</a>";
    message += "<a href='/update' class='navbar-brand'>Update</a>";
    message += "</div></nav>";
    message += "<main role='main' class='container'><div class='jumbotron'>";
    message += "<form id='data' action='/update' method='POST' enctype='multipart/form-data'>";
    message += "<center><h1>Smogomierz - Update</h1></center><br><br>";
    message += "<div class='input-group mb-3'><div class='custom-file'><input type='file' accept='.bin' class='custom-file-input' id='inputGroupFile04' name='update'><label class='custom-file-label' for='inputGroupFile04'>Wybierz plik .bin</label></div><div class='input-group-append'><button class='btn btn-danger' type='submit'>Update!</button></div></div>";
    message += "</form>";
    message += "<br><br>";
    message += "Aktualna wersja oprogramowania: <b>";
    message += SOFTWAREVERSION;
    message += "</b><br>";
    message += "Najnowszą wersję oprogramowania znajdziesz zawsze <b><a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/releases'>tutaj</a></b>.";
    message += "</div>";
    message += "</main></body></html>";
    WebServer.send(200, "text/html", message);
  }

void handle_api() {
    String message;
    StaticJsonBuffer<400> jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();

        json["device_name"] = device_name;
        json["pm1"] = averagePM1;
        json["pm25"] = averagePM25;
        json["pm10"] = averagePM10;
        if (checkBmeStatus()) {
          json["temperature"] = float(BMESensor.temperature);
          json["pressure"] = int(BMESensor.pressure  / 100.0F);
          json["humidity"] = int(BMESensor.humidity);
          json["dewpoint"] = int(BMESensor.temperature-((100-BMESensor.humidity)/5));    
        }

      json.prettyPrintTo(message);
    WebServer.send(200, "text/json", message);
  }
