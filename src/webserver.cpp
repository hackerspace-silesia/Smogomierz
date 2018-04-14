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

      int temperatureInt = BMESensor.temperature;
      int pressureInt = BMESensor.pressure / 100.0F;
      int humidityInt = BMESensor.humidity;
      if (temperatureInt == 0 && pressureInt == 0 && humidityInt == 0) {
        Serial.println("Brak pomiarow z BME280!\n");
      } else {
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
          message += (averagePM25);
          message += " µg/m³</h3></font>";
        } else if (averagePM25 > 10 && averagePM25 <= 20){
          message += "<font color='#5BCAAA'>";
          message += (averagePM25);
          message += " µg/m³</h3></font>";
        } else if (averagePM25 > 20 && averagePM25 <= 25){
          message += "<font color='#EEE25D'>";
          message += (averagePM25);
          message += " µg/m³</h3></font>";
        } else if (averagePM25 > 25 && averagePM25 <= 50){
          message += "<font color='#F95459'>";
          message += (averagePM25);
          message += " µg/m³</h3></font>";
        } else if (averagePM25 > 50){
          message += "<font color='#920736'>";
          message += (averagePM25);
          message += " µg/m³</h3></font>";
        } else {
          message += (averagePM25);
          message += " µg/m³</h3>";
        }
        
        message += "<h3>PM10: ";
        if (averagePM10 <= 20){
          message += "<font color='#61EEE4'>";
          message += (averagePM10);
          message += " µg/m³</h3></font>";
        } else if (averagePM10 > 20 && averagePM10 <= 35){
          message += "<font color='#5BCAAA'>";
          message += (averagePM10);
          message += " µg/m³</h3></font>";
        } else if (averagePM10 > 35 && averagePM10 <= 50){
          message += "<font color='#EEE25D'>";
          message += (averagePM10);
          message += " µg/m³</h3></font>";
        } else if (averagePM10 > 50 && averagePM10 <= 100){
          message += "<font color='#F95459'>";
          message += (averagePM10);
          message += " µg/m³</h3></font>";
        } else if (averagePM10 > 100){
          message += "<font color='#920736'>";
          message += (averagePM10);
          message += " µg/m³</h3></font>";
        } else {
          message += (averagePM10);
          message += " µg/m³</h3>";
        }      

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

void handle_config() {            //Handler for the handle_config
    String message = "<html lang='pl'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - Config</title>";
    message += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>";
    message += "</head><body>";
    message += "<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>Pomiary</a>";
    message += "<a href='/config' class='navbar-brand'>Konfiguracja</a>";
    message += "<a href='/update' class='navbar-brand'>Update</a>";
    message += "</div></nav>";
    message += "<main role='main' class='container'><div class='jumbotron'>";

    message += "<center><h1>Smogomierz - Config</h1></center><br><br>";

    message += "<b>Nazwa urządzenia: </b>";
    message += (device_name);
    message += "<br>";

    message += "<b>Automatyczne generowanie nazwy: </b>";
    message += (DEVICENAME_AUTO);
    message += "<br>";
    
    message += "<b>Wyświetlanie pomiarów PM1: </b>";
    message += (DISPLAY_PM1);
    message += "<br><br>";
  
    message += "<b>Wysyłanie danych do serwisu AirMonitor: </b>";
    message += (AIRMONITOR_ON);
    message += "<br>";

    message += "<b>Wyświetlanie wykresów z serwisu AirMonitor: </b>";
    message += (AIRMONITOR_GRAPH_ON);
    message += "<br><br>";

    message += "<b>Współrzędne geograficzne miernika:<br>Szerokość(latitude): </b>";
    message += String(LATITUDE, 4);
    message += "<br><b>Długość(longitude): </b>";
    message += String(LONGITUDE, 4);
    message += "<br><b>Wysokość: </b>";
    message += (int(MYALTITUDE));
    message += " m.n.p.m<br><br>";
  
    message += "<b>Wysyłanie danych do serwisu ThingSpeak: </b>";
    message += (THINGSPEAK_ON);
    message += "<br>";

    message += "<b>Wyświetlanie wykresów z serwisu ThingSpeak: </b>";
    message += (THINGSPEAK_GRAPH_ON);
    message += "<br>";

    message += "<b>ThingSpeak API_KEY: </b>";
    message += (THINGSPEAK_API_KEY);
    message += "<br><b>ThingSpeak Channel ID: </b>";
    message += (THINGSPEAK_CHANNEL_ID);
    message += "<br><br>";

    message += "<b>Wysyłanie danych do InfluxDB: </b>";
    message += (INFLUXDB_ON);
    message += "<br><b>Adres bazy danych InfluxDB: </b>";
    message += (INFLUXDB_HOST);
    message += "<br><b>Port InfluxDB: </b>";
    message += (INFLUXDB_PORT);
    message += "<br><b>Nazwa bazy danych: </b>";
    message += (DATABASE);
    message += "<br><b>Użytkownik bazy danych: </b>";
    message += (DB_USER);
    message += "<br><b>Hasło do bazy danych: </b>";
    message += (DB_PASSWORD);
    message += "<br><br>";

    message += "<b>Debug: </b>";
    message += (DEBUG);
    message += "<br>";

    message += "<b>Zmienna kalibracyjna calib1: </b>";
    message += (calib1);
    message += "<br>";

    message += "<b>Zmienna kalibracyjna calib2: </b>";
    message += (calib2);
    message += "<br><br>";

    message += "<b>Wersja oprogramowania: </b>";
    message += (SOFTWAREVERSION);
    message += "<br>";
  
    message += "</main></body></html>";
    WebServer.send(200, "text/html", message);
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
        if (int(BMESensor.temperature) == 0 && int(BMESensor.humidity) == 0 && int(BMESensor.pressure  / 100.0F) == 0){
          Serial.println("Brak pomiarow z BME280!\n");
        }else{
          json["temperature"] = float(BMESensor.temperature);
          json["pressure"] = int(BMESensor.pressure  / 100.0F);
          json["humidity"] = int(BMESensor.humidity);
          json["dewpoint"] = int(BMESensor.temperature-((100-BMESensor.humidity)/5));    
        }

      json.prettyPrintTo(message);
    WebServer.send(200, "text/json", message);
  }
