#include "webserver.h"
#include "config.h"
#include "WiFiManager.h"


void showHtml(WifiClient &client, const BME280<> &bme, const PMS::DATA &pms) {
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\" ><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\">"
    "</head><body><p><p><p><center><h1>Smogomierz!</h1>");

    client.println("<p><h2>Pogoda:</h2>");
    client.println("<h3>Temperatura: ");
    client.println(BMESensor.temperature);
    client.println(" °C</h3>");
      
    client.println("<h3>Ciśnienie: ");
    client.println((BMESensor.seaLevelForAltitude(MYALTITUDE)) / 100.0F);
    client.println(" hPa</h3>");
      
    client.println("<h3>Wilgotność: ");
    client.println(BMESensor.humidity);
    client.println(" %</h3>");

    client.println("<h3>Punkt rosy: ");
    client.println(BMESensor.temperature-((100-BMESensor.humidity)/5));
    client.println(" °C</h3>");

    client.println("<p><h2>Pomiary zanieczyszczeń:</h2>");
    client.println("<h3>PM1: ");
    client.println(int(calib1 * data.PM_AE_UG_1_0));
    client.println(" µg/m³</h3>");

    client.println("<h3>PM2.5: ");
    client.println(int(calib1 * data.PM_AE_UG_2_5));
    client.println(" µg/m³</h3>");

    client.println("<h3>PM10: ");
    client.println(int(calib1 * data.PM_AE_UG_10_0));
    client.println(" µg/m3</h3>");

    client.println("</center></body></html>"); 
}


void webserverShowSite(WifiServer &server, const BME280<> &bme, const PMS::DATA &pms) { 
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    bool blank_line = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          showHtml(client, bme, pms);
          break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }     
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }  
}
