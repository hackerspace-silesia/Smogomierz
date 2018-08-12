
void handle_root () {
	String message;
	if(selected_language == 1){
        	message += "<html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Measurements</title>";
	        message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
	        message += "</head><body>";
	        message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Measurements</a>";
	        message += "<a href='/config' class='navbar-brand'>Config</a>";
	        message += "<a href='/update' class='navbar-brand'>Update</a>";
	        message += "</div></nav>";
	        message += "<main role='main' class='container'><div class='jumbotron'>";
	        message += "<center><h1>Smogomierz</h1><br>";
			
	        if (checkBmeStatus()) {
	        message += "<h2>Weather:</h2>";
	        message += "<h3>Temperature: ";
	        message += (BMESensor.temperature);
	        message += " °C</h3>";
	        message += "<h3>Pressure: ";
	        message += (BMESensor.seaLevelForAltitude(MYALTITUDE));
	        message += " hPa</h3>";
	        message += "<h3>Humidity: ";
	        message += (BMESensor.humidity);
	        message += " %</h3>";
	        message += "<h3>Dew point: ";
	        message += (BMESensor.temperature-((100-BMESensor.humidity)/5));
	        message += " °C</h3>";
	        }
	        message += "<p><h2>Air pollution:</h2>";
	} else if(selected_language == 2){
        	message += "<html lang='pl'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Pomiary</title>";
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
	        message += (BMESensor.seaLevelForAltitude(MYALTITUDE));
	        message += " hPa</h3>";
	        message += "<h3>Wilgotność: ";
	        message += (BMESensor.humidity);
	        message += " %</h3>";
	        message += "<h3>Punkt rosy: ";
	        message += (BMESensor.temperature-((100-BMESensor.humidity)/5));
	        message += " °C</h3>";
	        }
	        message += "<p><h2>Pomiary zanieczyszczeń:</h2>";
	}  else {
    	message += "<html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Measurements</title>";
        message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
        message += "</head><body>";
        message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Measurements</a>";
        message += "<a href='/config' class='navbar-brand'>Config</a>";
        message += "<a href='/update' class='navbar-brand'>Update</a>";
        message += "</div></nav>";
        message += "<main role='main' class='container'><div class='jumbotron'>";
        message += "<center><h1>Smogomierz</h1><br>";
		
        if (checkBmeStatus()) {
        message += "<h2>Weather:</h2>";
        message += "<h3>Temperature: ";
        message += (BMESensor.temperature);
        message += " °C</h3>";
        message += "<h3>Pressure: ";
        message += (BMESensor.seaLevelForAltitude(MYALTITUDE));
        message += " hPa</h3>";
        message += "<h3>Humidity: ";
        message += (BMESensor.humidity);
        message += " %</h3>";
        message += "<h3>Dew point: ";
        message += (BMESensor.temperature-((100-BMESensor.humidity)/5));
        message += " °C</h3>";
        }
        message += "<p><h2>Air pollution:</h2>";
	} 
      
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
      if (int(BMESensor.temperature) == 0 && int(BMESensor.humidity) == 0 && int(BMESensor.seaLevelForAltitude(MYALTITUDE)) == 0){
		  if(selected_language == 1){
		          Serial.println("No data from BME280 sensor!\n");
		        } else if(selected_language == 2){
		          Serial.println("Brak pomiarów z BME280!\n");
		        } else {
		          Serial.println("No data from BME280 sensor!\n");
		          }
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
    message += "<br><hr><center>Hackerspace Silesia &#9830; 2018</center></div></main></form></body></html>";
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
	if(selected_language == 1){
    	input += _addOption("yes", "Yes", selectValue);
    	input += _addOption("no", "No", selectValue);
	} else if(selected_language == 2){
    	input += _addOption("yes", "Tak", selectValue);
    	input += _addOption("no", "Nie", selectValue);
	} else {
    	input += _addOption("yes", "Yes", selectValue);
    	input += _addOption("no", "No", selectValue);
	}
    input += "</select><br />";
    return input;
}

String _addModelSelect(const String &key, const String &value) {
    String input = "<select name='";
    input += key;
    input += "'>";
	if(selected_language == 1){
    	//input += _addOption("black", "Case 86x86/120x80x50", value);
    	input += _addOption("red", "Without calibration", value);
		input += _addOption("white", "Automatic calibration", value);
	} else if(selected_language == 2){
	    //input += _addOption("black", "Obudowa 86x86/120x80x50", value);
	    input += _addOption("red", "Bez kalibracji", value);
		input += _addOption("white", "Automatyczna kalibracja", value);
	} else {
	    //input += _addOption("black", "Case 86x86/120x80x50", value);
    	input += _addOption("red", "Without calibration", value);
		input += _addOption("white", "Automatic calibration", value);
	}
    input += "</select><br />";
    return input;
}

String _addLanguageSelect(const String &key, const String &value) {
    String input = "<select name='";
    input += key;
    input += "'>";
	if(selected_language == 1){
    	//input += _addOption("black", "Case 86x86/120x80x50", value);
	    input += _addOption("polish", "polish", value);
	    input += _addOption("english", "english", value);
	} else if(selected_language == 2){
	    //input += _addOption("black", "Obudowa 86x86/120x80x50", value);
	    input += _addOption("polish", "polski", value);
	    input += _addOption("english", "angielski", value);
	} else {
    	input += _addOption("polish", "polish", value);
    	input += _addOption("english", "english", value);
	}
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
    String input = "<input type='text' maxlength='255' size='20' name='";
    input += key;
    input += "' value='";
    input += _escapeString(value);
    input += "'> ";
    input += postfix;
    input += "<br />";
    return input;
}

String _addPasswdInput(const String &key, const String &value, const String &postfix="") {
    String input = "<input type='password' maxlength='255' size='20' name='";
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
    String input = "<input type='number' maxlength='255' step='0.000001' name='";
    input += key;
    input += "' value='";
    input += String(value, precision);
    input += "'> ";
    input += postfix;
    input += "<br />";
    return input;
}

String _addSubmit() {
	if(selected_language == 1){
		return "<input type='submit' class='btn btn-outline-danger' value='Save' /><br /><br />";
	} else if(selected_language == 2){
		return "<input type='submit' class='btn btn-outline-danger' value='Zapisz' /><br /><br />";
	} else {
		return "<input type='submit' class='btn btn-outline-danger' value='Save' /><br /><br />";
	}
}

void _handle_config(bool is_success) {
	String message;
	if(selected_language == 1){
    message += "<html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Config</title>";
    message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
	message += "</head><body>";
    message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Measurements</a>";
    message += "<a href='/config' class='navbar-brand'>Config</a>";
    message += "<a href='/update' class='navbar-brand'>Update</a>";
    message += "</div></nav>";
    message += "<form method='POST' action='/config'>";
    message += "<main role='main' class='container'><div class='jumbotron'>";

    message += "<center><h1>Smogomierz - Config</h1></center><br><br>";

    if (is_success) {
        message += "<div style='color: #2f7a2d'> <strong>SAVED!</strong> - everything looks OK, in a moment the Smogomierz will restart </div><br><hr><br>";
    }
	
	message += "All instructions and descriptions[in polish] are available <a title='Instructions' href='https://github.com/hackerspace-silesia/Smogomierz#instrukcje' target='_blank'>here</a>.<br><br>";

    message += "<b>Device Name: </b>";
    if (DEVICENAME_AUTO) {
        message += device_name;
	    message += "<br>";
    } else {
        message += _addTextInput("DEVICENAME", DEVICENAME);
    }
    message += "<b>Automatic name generation: </b>";
    message += _addBoolSelect("DEVICENAME_AUTO", DEVICENAME_AUTO);
    
    message += "<b>Display of PM1 measurements: </b>";
    message += _addBoolSelect("DISPLAY_PM1", DISPLAY_PM1);
	
	/*
    message += "<b>Language: </b>";
    message += (LANGUAGE);
	message += "<br>";
    message += "<b>Language: </b>";
    message += (selected_language);
	message += "<br>";
	*/
	
    message += "<b>Language: </b>";
    message += _addLanguageSelect("LANGUAGE", LANGUAGE);
	message += "<hr>";	
  
    message += "<b>Sending data to the <a title='AirMonitor' href='http://mapa.airmonitor.pl' target='_blank'>AirMonitor</a> service(requires filling out <a title='AirMonitor Form' href='https://docs.google.com/forms/d/e/1FAIpQLSdw72_DggyrK7xnSQ1nR11Y-YK4FYWk_MF9QbecpOERql-T2w/viewform' target='_blank'>the form</a>; Sensor: PMS7003): </b>";
    message += _addBoolSelect("AIRMONITOR_ON", AIRMONITOR_ON);
	
    message += "<b>Sending measurements every: </b>";
    message += _addIntInput("AIRMONITOR_TIME", AIRMONITOR_TIME, "minutes");
	
    message += "<b>Displaying charts from the AirMonitor site: </b>";
    message += _addBoolSelect("AIRMONITOR_GRAPH_ON", AIRMONITOR_GRAPH_ON);

    message += "<b>Geographical coordinates(you can check it <a title='latlong.net' href='https://www.latlong.net' target='_blank'>here</a>):<br>Latitude: </b>";
    message += _addFloatInput("LATITUDE", LATITUDE, 6, "°");
    message += "<b>Longitude: </b>";
    message += _addFloatInput("LONGITUDE", LONGITUDE, 6, "°");
    message += "<b>Altitude: </b>";
    message += _addIntInput("MYALTITUDE", MYALTITUDE, "m.n.p.m");
	message += "<hr>";	
  
    message += "<b>Sending data to the <a title='ThingSpeak' href='https://thingspeak.com' target='_blank'>ThingSpeak</a> service: </b>";
    message += _addBoolSelect("THINGSPEAK_ON", THINGSPEAK_ON);
	
    message += "<b>Sending measurements every: </b>";
    message += _addIntInput("THINGSPEAK_TIME", THINGSPEAK_TIME, "minutes");

    message += "<b>Displaying charts from the ThingSpeak site: </b>";
    message += _addBoolSelect("THINGSPEAK_GRAPH_ON", THINGSPEAK_GRAPH_ON);

    message += "<b>ThingSpeak API_KEY: </b>";
    message += _addTextInput("THINGSPEAK_API_KEY", THINGSPEAK_API_KEY);
    message += "<b>ThingSpeak Channel ID: </b>";
    message += _addIntInput("THINGSPEAK_CHANNEL_ID", THINGSPEAK_CHANNEL_ID);
	message += "<hr>";	

    message += "<b>Sending data to the InfluxDB: </b>";
    message += _addBoolSelect("INFLUXDB_ON", INFLUXDB_ON);
    message += "<b>Sending measurements every: </b>";
    message += _addIntInput("INFLUXDB_TIME", INFLUXDB_TIME, "minutes");
    message += "<b>InfluxDB database address: </b>";
    message += _addTextInput("INFLUXDB_HOST", INFLUXDB_HOST);
    message += "<b>InfluxDB port: </b>";
    message += _addIntInput("INFLUXDB_PORT", INFLUXDB_PORT);
    message += "<b>Name of the database: </b>";
    message += _addTextInput("DATABASE", DATABASE);
    message += "<b>Database user: </b>";
    message += _addTextInput("DB_USER", DB_USER);
    message += "<b>Database password: </b>";
    message += _addPasswdInput("DB_PASSWORD", DB_PASSWORD);
	message += "<hr>";	

    message += "<b>Debug: </b>";
    message += _addBoolSelect("DEBUG", DEBUG);

    message += "<b>Calibration method: </b>";
    message += _addModelSelect("MODEL", MODEL);
	
    message += "<b>calib1: </b>";
    message += (calib1);
    message += "<br>";
	/*
    message += "<b>calib2: </b>";
    message += (calib2);
    message += "<br>";
	*/
    message += "<b>Software version: </b>";
    message += (SOFTWAREVERSION);
	message += "<hr><br><center>";
    message += _addSubmit();
	message += "</center>";
	} else if(selected_language == 2){
	    message += "<html lang='pl'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Config</title>";
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
	        message += "<div style='color: #2f7a2d'> <strong>ZAPISANO!</strong> - wszystko wygląda OK, za chwilę nastąpi restart Smogomierza </div><br><hr><br>";
	    }
	
		message += "Wszystkie instrukcje i opisy dostępne są <a title='Instrukcje' href='https://github.com/hackerspace-silesia/Smogomierz#instrukcje' target='_blank'>tutaj</a>.<br><br>";

	    message += "<b>Nazwa urządzenia: </b>";
	    if (DEVICENAME_AUTO) {
	        message += device_name;
		    message += "<br>";
	    } else {
	        message += _addTextInput("DEVICENAME", DEVICENAME);
	    }
	    message += "<b>Automatyczne generowanie nazwy: </b>";
	    message += _addBoolSelect("DEVICENAME_AUTO", DEVICENAME_AUTO);
    
	    message += "<b>Wyświetlanie pomiarów PM1: </b>";
	    message += _addBoolSelect("DISPLAY_PM1", DISPLAY_PM1);
	
		/*
	    message += "<b>Język: </b>";
	    message += (LANGUAGE);
		message += "<br>";
	    message += "<b>Język: </b>";
	    message += (selected_language);
		message += "<br>";
		*/
		
	    message += "<b>Język: </b>";
	    message += _addLanguageSelect("LANGUAGE", LANGUAGE);
		message += "<hr>";	
  
	    message += "<b>Wysyłanie danych do serwisu <a title='AirMonitor' href='http://mapa.airmonitor.pl' target='_blank'>AirMonitor</a>(wymaga wypełnienia <a title='Formularz AirMonitor' href='https://docs.google.com/forms/d/e/1FAIpQLSdw72_DggyrK7xnSQ1nR11Y-YK4FYWk_MF9QbecpOERql-T2w/viewform' target='_blank'>formularza</a>; Sensor: PMS7003): </b>";
	    message += _addBoolSelect("AIRMONITOR_ON", AIRMONITOR_ON);
	
	    message += "<b>Wysyłanie pomiarów co: </b>";
	    message += _addIntInput("AIRMONITOR_TIME", AIRMONITOR_TIME, "minut");
	
	    message += "<b>Wyświetlanie wykresów z serwisu AirMonitor: </b>";
	    message += _addBoolSelect("AIRMONITOR_GRAPH_ON", AIRMONITOR_GRAPH_ON);

	    message += "<b>Współrzędne geograficzne miernika(można sprawdzić <a title='wspolrzedne-gps.pl' href='https://www.wspolrzedne-gps.pl' target='_blank'>Tutaj</a>):<br>Szerokość(latitude): </b>";
	    message += _addFloatInput("LATITUDE", LATITUDE, 6, "°");
	    message += "<b>Długość(longitude): </b>";
	    message += _addFloatInput("LONGITUDE", LONGITUDE, 6, "°");
	    message += "<b>Wysokość: </b>";
	    message += _addIntInput("MYALTITUDE", MYALTITUDE, "m.n.p.m");
		message += "<hr>";	
  
	    message += "<b>Wysyłanie danych do serwisu <a title='ThingSpeak' href='https://thingspeak.com' target='_blank'>ThingSpeak</a>: </b>";
	    message += _addBoolSelect("THINGSPEAK_ON", THINGSPEAK_ON);
	
	    message += "<b>Wysyłanie pomiarów co: </b>";
	    message += _addIntInput("THINGSPEAK_TIME", THINGSPEAK_TIME, "minut");

	    message += "<b>Wyświetlanie wykresów z serwisu ThingSpeak: </b>";
	    message += _addBoolSelect("THINGSPEAK_GRAPH_ON", THINGSPEAK_GRAPH_ON);

	    message += "<b>ThingSpeak API_KEY: </b>";
	    message += _addTextInput("THINGSPEAK_API_KEY", THINGSPEAK_API_KEY);
	    message += "<b>ThingSpeak Channel ID: </b>";
	    message += _addIntInput("THINGSPEAK_CHANNEL_ID", THINGSPEAK_CHANNEL_ID);
		message += "<hr>";	

	    message += "<b>Wysyłanie danych do InfluxDB: </b>";
	    message += _addBoolSelect("INFLUXDB_ON", INFLUXDB_ON);
	    message += "<b>Wysyłanie pomiarów co: </b>";
	    message += _addIntInput("INFLUXDB_TIME", INFLUXDB_TIME, "minut");
	    message += "<b>Adres bazy danych InfluxDB: </b>";
	    message += _addTextInput("INFLUXDB_HOST", INFLUXDB_HOST);
	    message += "<b>Port InfluxDB: </b>";
	    message += _addIntInput("INFLUXDB_PORT", INFLUXDB_PORT);
	    message += "<b>Nazwa bazy danych: </b>";
	    message += _addTextInput("DATABASE", DATABASE);
	    message += "<b>Użytkownik bazy danych: </b>";
	    message += _addTextInput("DB_USER", DB_USER);
	    message += "<b>Hasło do bazy danych: </b>";
	    message += _addPasswdInput("DB_PASSWORD", DB_PASSWORD);
		message += "<hr>";	

	    message += "<b>Debug: </b>";
	    message += _addBoolSelect("DEBUG", DEBUG);

	    message += "<b>Sposób kalibracji: </b>";
	    message += _addModelSelect("MODEL", MODEL);
		
	    message += "<b>Zmienna kalibracyjna calib1: </b>";
	    message += (calib1);
	    message += "<br>";
		/*
	    message += "<b>Zmienna kalibracyjna calib2: </b>";
	    message += (calib2);
	    message += "<br>";
		*/
	    message += "<b>Wersja oprogramowania: </b>";
	    message += (SOFTWAREVERSION);
		message += "<hr><br><center>";
	    message += _addSubmit();
		message += "</center>";
  	} else {
	    message += "<html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Config</title>";
	    message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
		message += "</head><body>";
	    message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Measurements</a>";
	    message += "<a href='/config' class='navbar-brand'>Config</a>";
	    message += "<a href='/update' class='navbar-brand'>Update</a>";
	    message += "</div></nav>";
	    message += "<form method='POST' action='/config'>";
	    message += "<main role='main' class='container'><div class='jumbotron'>";

	    message += "<center><h1>Smogomierz - Config</h1></center><br><br>";

	    if (is_success) {
	        message += "<div style='color: #2f7a2d'> <strong>SAVED!</strong> - everything looks OK, in a moment the Smogomierz will restart </div><br><hr><br>";
	    }
	
		message += "All instructions and descriptions[in polish] are available <a title='Instructions' href='https://github.com/hackerspace-silesia/Smogomierz#instrukcje' target='_blank'>here</a>.<br><br>";

	    message += "<b>Device Name: </b>";
	    if (DEVICENAME_AUTO) {
	        message += device_name;
		    message += "<br>";
	    } else {
	        message += _addTextInput("DEVICENAME", DEVICENAME);
	    }
	    message += "<b>Automatic name generation: </b>";
	    message += _addBoolSelect("DEVICENAME_AUTO", DEVICENAME_AUTO);
    
	    message += "<b>Display of PM1 measurements: </b>";
	    message += _addBoolSelect("DISPLAY_PM1", DISPLAY_PM1);
	
		/*
	    message += "<b>Language: </b>";
	    message += (LANGUAGE);
		message += "<br>";
	    message += "<b>Language: </b>";
	    message += (selected_language);
		message += "<br>";
		*/
		
	    message += "<b>Language: </b>";
	    message += _addLanguageSelect("LANGUAGE", LANGUAGE);
		message += "<hr>";	
  
	    message += "<b>Sending data to the <a title='AirMonitor' href='http://mapa.airmonitor.pl' target='_blank'>AirMonitor</a> service(requires filling out <a title='AirMonitor Form' href='https://docs.google.com/forms/d/e/1FAIpQLSdw72_DggyrK7xnSQ1nR11Y-YK4FYWk_MF9QbecpOERql-T2w/viewform' target='_blank'>the form</a>; Sensor: PMS7003): </b>";
	    message += _addBoolSelect("AIRMONITOR_ON", AIRMONITOR_ON);
	
	    message += "<b>Sending measurements every: </b>";
	    message += _addIntInput("AIRMONITOR_TIME", AIRMONITOR_TIME, "minutes");
	
	    message += "<b>Displaying charts from the AirMonitor site: </b>";
	    message += _addBoolSelect("AIRMONITOR_GRAPH_ON", AIRMONITOR_GRAPH_ON);

	    message += "<b>Geographical coordinates(you can check it <a title='latlong.net' href='https://www.latlong.net' target='_blank'>here</a>):<br>Latitude: </b>";
	    message += _addFloatInput("LATITUDE", LATITUDE, 6, "°");
	    message += "<b>Longitude: </b>";
	    message += _addFloatInput("LONGITUDE", LONGITUDE, 6, "°");
	    message += "<b>Altitude: </b>";
	    message += _addIntInput("MYALTITUDE", MYALTITUDE, "m.n.p.m");
		message += "<hr>";	
  
	    message += "<b>Sending data to the <a title='ThingSpeak' href='https://thingspeak.com' target='_blank'>ThingSpeak</a> service: </b>";
	    message += _addBoolSelect("THINGSPEAK_ON", THINGSPEAK_ON);
	
	    message += "<b>Sending measurements every: </b>";
	    message += _addIntInput("THINGSPEAK_TIME", THINGSPEAK_TIME, "minutes");

	    message += "<b>Displaying charts from the ThingSpeak site: </b>";
	    message += _addBoolSelect("THINGSPEAK_GRAPH_ON", THINGSPEAK_GRAPH_ON);

	    message += "<b>ThingSpeak API_KEY: </b>";
	    message += _addTextInput("THINGSPEAK_API_KEY", THINGSPEAK_API_KEY);
	    message += "<b>ThingSpeak Channel ID: </b>";
	    message += _addIntInput("THINGSPEAK_CHANNEL_ID", THINGSPEAK_CHANNEL_ID);
		message += "<hr>";	

	    message += "<b>Sending data to the InfluxDB: </b>";
	    message += _addBoolSelect("INFLUXDB_ON", INFLUXDB_ON);
	    message += "<b>Sending measurements every: </b>";
	    message += _addIntInput("INFLUXDB_TIME", INFLUXDB_TIME, "minutes");
	    message += "<b>InfluxDB database address: </b>";
	    message += _addTextInput("INFLUXDB_HOST", INFLUXDB_HOST);
	    message += "<b>InfluxDB port: </b>";
	    message += _addIntInput("INFLUXDB_PORT", INFLUXDB_PORT);
	    message += "<b>Name of the database: </b>";
	    message += _addTextInput("DATABASE", DATABASE);
	    message += "<b>Database user: </b>";
	    message += _addTextInput("DB_USER", DB_USER);
	    message += "<b>Database password: </b>";
	    message += _addPasswdInput("DB_PASSWORD", DB_PASSWORD);
		message += "<hr>";	

	    message += "<b>Debug: </b>";
	    message += _addBoolSelect("DEBUG", DEBUG);

	    message += "<b>Calibration method: </b>";
	    message += _addModelSelect("MODEL", MODEL);
		
	    message += "<b>calib1: </b>";
	    message += (calib1);
	    message += "<br>";
		/*
	    message += "<b>calib2: </b>";
	    message += (calib2);
	    message += "<br>";
		*/
	    message += "<b>Software version: </b>";
	    message += (SOFTWAREVERSION);
		message += "<hr><br><center>";
	    message += _addSubmit();
		message += "</center>";	
	}
    message += "<hr><center>Hackerspace Silesia &#9830; 2018</center></div></main></form></body></html>";
    WebServer.send(200, "text/html", message);
}

bool _parseAsBool(String value) {
    return value == "yes";
}

void _set_calib1_and_calib2() {
    if (!strcmp(MODEL, "black")) {
        calib1 = 1.6;
        calib2 = 0.55;
    } else if (!strcmp(MODEL, "red")) {
        calib1 = 1.0;
        calib2 = 1.0;
    } else if (!strcmp(MODEL, "white")) {
        calib1 = float((100-(BMESensor.humidity)+100)/150);
        calib2 = calib1/2;
    } else {
        calib1 = 1.0;
        calib2 = 1.0;
    }
}

void _set_language() {
    if (!strcmp(LANGUAGE, "english")) {
        selected_language = 1;
    } else if (!strcmp(LANGUAGE, "polish")) {
        selected_language = 2;
    } else {
        selected_language = 3;
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
    if (!DEVICENAME_AUTO) {
        _parseAsCString(DEVICENAME, WebServer.arg("DEVICENAME"));
    }
    DISPLAY_PM1 = _parseAsBool(WebServer.arg("DISPLAY_PM1"));
	_parseAsCString(LANGUAGE, WebServer.arg("LANGUAGE"));
	_set_language();
	
    AIRMONITOR_ON = _parseAsBool(WebServer.arg("AIRMONITOR_ON"));
	AIRMONITOR_TIME = WebServer.arg("AIRMONITOR_TIME").toInt();
    AIRMONITOR_GRAPH_ON = _parseAsBool(WebServer.arg("AIRMONITOR_GRAPH_ON"));
    LATITUDE = WebServer.arg("LATITUDE").toFloat();
    LONGITUDE = WebServer.arg("LONGITUDE").toFloat();
    MYALTITUDE = WebServer.arg("MYALTITUDE").toInt();
      
    THINGSPEAK_ON = _parseAsBool(WebServer.arg("THINGSPEAK_ON"));
	THINGSPEAK_TIME = WebServer.arg("THINGSPEAK_TIME").toInt();
    THINGSPEAK_GRAPH_ON = _parseAsBool(WebServer.arg("THINGSPEAK_GRAPH_ON"));
    _parseAsCString(THINGSPEAK_API_KEY, WebServer.arg("THINGSPEAK_API_KEY"));
    THINGSPEAK_CHANNEL_ID = WebServer.arg("THINGSPEAK_CHANNEL_ID").toInt();
      
    INFLUXDB_ON = _parseAsBool(WebServer.arg("INFLUXDB_ON"));
	INFLUXDB_TIME = WebServer.arg("INFLUXDB_TIME").toInt();
    _parseAsCString(INFLUXDB_HOST, WebServer.arg("INFLUXDB_HOST"));
    INFLUXDB_PORT = WebServer.arg("INFLUXDB_PORT").toInt();
    _parseAsCString(DATABASE, WebServer.arg("DATABASE"));
    _parseAsCString(DB_USER, WebServer.arg("DB_USER"));
    _parseAsCString(DB_PASSWORD, WebServer.arg("DB_PASSWORD"));

    DEBUG = _parseAsBool(WebServer.arg("DEBUG"));
    _parseAsCString(MODEL, WebServer.arg("MODEL"));
    _set_calib1_and_calib2();

    if (DEBUG) {
        Serial.println("POST CONFIG END!!");
    }
    
    saveConfig();
    _handle_config(true);
    // https://github.com/esp8266/Arduino/issues/1722
    //ESP.reset();
	delay(300);
	ESP.restart();
}

void handle_update() {            //Handler for the handle_update
	String message;
	if(selected_language == 1){
	    message = "<html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Update</title>";
	    message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
	    message += "</head><body>";
	    message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Measurements</a>";
	    message += "<a href='/config' class='navbar-brand'>Config</a>";
	    message += "<a href='/update' class='navbar-brand'>Update</a>";
	    message += "</div></nav>";
	    message += "<main role='main' class='container'><div class='jumbotron'>";
	    message += "<form id='data' action='/update' method='POST' enctype='multipart/form-data'>";
	    message += "<center><h1>Smogomierz - Update</h1></center><br><br>";
	    message += "<div class='input-group mb-3'><div class='custom-file'><input type='file' accept='.bin' class='custom-file-input' id='inputGroupFile04' name='update'><label class='custom-file-label' for='inputGroupFile04'>Select .bin file</label></div><div class='input-group-append'><button class='btn btn-danger' type='submit'>Update!</button></div></div>";
	    message += "</form>";
	    message += "<br><br>";
	    message += "The currently used version of the software: <b>";
	    message += SOFTWAREVERSION;
	    message += "</b><br>";
	    message += "Latest version of the software available <b><a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/releases'>here</a></b>.";
	} else if(selected_language == 2){
	    message = "<html lang='pl'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Update</title>";
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
	} else {
	    message = "<html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Update</title>";
	    message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
	    message += "</head><body>";
	    message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Measurements</a>";
	    message += "<a href='/config' class='navbar-brand'>Config</a>";
	    message += "<a href='/update' class='navbar-brand'>Update</a>";
	    message += "</div></nav>";
	    message += "<main role='main' class='container'><div class='jumbotron'>";
	    message += "<form id='data' action='/update' method='POST' enctype='multipart/form-data'>";
	    message += "<center><h1>Smogomierz - Update</h1></center><br><br>";
	    message += "<div class='input-group mb-3'><div class='custom-file'><input type='file' accept='.bin' class='custom-file-input' id='inputGroupFile04' name='update'><label class='custom-file-label' for='inputGroupFile04'>Select .bin file</label></div><div class='input-group-append'><button class='btn btn-danger' type='submit'>Update!</button></div></div>";
	    message += "</form>";
	    message += "<br><br>";
	    message += "The currently used version of the software: <b>";
	    message += SOFTWAREVERSION;
	    message += "</b><br>";
	    message += "Latest version of the software available <b><a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/releases'>here</a></b>.";
	}
    message += "<br><hr><center>Hackerspace Silesia &#9830; 2018</center></div></main></form></body></html>";
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
          json["pressure"] = int(BMESensor.seaLevelForAltitude(MYALTITUDE));
          json["humidity"] = int(BMESensor.humidity);
          json["dewpoint"] = int(BMESensor.temperature-((100-BMESensor.humidity)/5));    
        }

      json.prettyPrintTo(message);
    WebServer.send(200, "text/json", message);
  }
