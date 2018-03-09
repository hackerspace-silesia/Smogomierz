#include "webserver.h"
#include "config.h"
#include "pms.h" // https://github.com/fu-hsi/PMS
#include "bme280.h" // https://github.com/zen/BME280_light/blob/master/BME280_t.h




/* Old Webserver 

#define REQ_BUF_SZ   40
char HTTP_req[REQ_BUF_SZ] = {0};
char req_index = 0;

void showHtml(WiFiClient &client, BME280<BME280_C, BME280_ADDRESS> &BMESensor, const PMS::DATA &pms) {
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\" ><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\">"
    "</head><body><p><p><p><center><h1>Smogomierz!</h1>");

    client.println("<p><h2>Pogoda:</h2>");
    client.println("<h3>Temperatura: ");
    client.println(BMESensor.temperature);
    client.println(" °C</h3>");
      
    client.println("<h3>Ciśnienie: ");
    client.println(BMESensor.seaLevelForAltitude(MYALTITUDE) / 100.0F);
    client.println(" hPa</h3>");
      
    client.println("<h3>Wilgotność: ");
    client.println(BMESensor.humidity);
    client.println(" %</h3>");

    client.println("<h3>Punkt rosy: ");
    client.println(BMESensor.temperature-((100-BMESensor.humidity)/5));
    client.println(" °C</h3>");

    client.println("<p><h2>Pomiary zanieczyszczeń:</h2>");
    if (int(BMESensor.temperature) < 5 or int(BMESensor.humidity) > 60){
    client.println("<h3>PM1: ");
    client.println(int(calib2 * pms.PM_AE_UG_1_0));
    client.println(" µg/m³</h3>");

    client.println("<h3>PM2.5: ");
    client.println(int(calib2 * pms.PM_AE_UG_2_5));
    client.println(" µg/m³</h3>");

    client.println("<h3>PM10: ");
    client.println(int(calib2 * pms.PM_AE_UG_10_0));
    client.println(" µg/m³</h3>");
    }else{
    client.println("<h3>PM1: ");
    client.println(int(calib1 * pms.PM_AE_UG_1_0));
    client.println(" µg/m³</h3>");

    client.println("<h3>PM2.5: ");
    client.println(int(calib1 * pms.PM_AE_UG_2_5));
    client.println(" µg/m³</h3>");

    client.println("<h3>PM10: ");
    client.println(int(calib1 * pms.PM_AE_UG_10_0));
    client.println(" µg/m³</h3>");
    }

if(AIRMONITOR_GRAPH_ON == true){
    client.println("<hr>");
    client.print("<iframe src=\"http://metrics.airmonitor.pl:3000/dashboard-solo/db/airmonitor?orgId=1&var-latitude=");
    client.print(String(LATITUDE, 4));
    client.print("&var-longitude=");
    client.print(String(LONGITUDE, 4));
    client.print("&refresh=1m&panelId=14\" width=\"450\" height=\"510\" frameborder=\"0\"></iframe>");
    
    client.print("<iframe src=\"http://metrics.airmonitor.pl:3000/dashboard-solo/db/airmonitor?orgId=1&var-latitude=");
    client.print(String(LATITUDE, 4));
    client.print("&var-longitude=");
    client.print(String(LONGITUDE, 4));
    client.print("&refresh=1m&panelId=13\" width=\"450\" height=\"510\" frameborder=\"0\"></iframe>");
}

if(THINGSPEAK_GRAPH_ON == true){
    client.println("<hr>");
    client.print("<iframe width=\"450\" height=\"260\" style=\"border: 1px solid #cccccc;\" src=\"https://thingspeak.com/channels/");
    client.print(THINGSPEAK_CHANNEL_ID);
    client.print("/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15\"></iframe>");    
    client.print("<iframe width=\"450\" height=\"260\" style=\"border: 1px solid #cccccc;\" src=\"https://thingspeak.com/channels/");
    client.print(THINGSPEAK_CHANNEL_ID);
    client.print("/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15\"></iframe>");
    client.print("<iframe width=\"450\" height=\"260\" style=\"border: 1px solid #cccccc;\" src=\"https://thingspeak.com/channels/");
    client.print(THINGSPEAK_CHANNEL_ID);
    client.print("/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15\"></iframe>");
    client.print("<iframe width=\"450\" height=\"260\" style=\"border: 1px solid #cccccc;\" src=\"https://thingspeak.com/channels/");
    client.print(THINGSPEAK_CHANNEL_ID);
    client.print("/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15\"></iframe>");
    client.print("<iframe width=\"450\" height=\"260\" style=\"border: 1px solid #cccccc;\" src=\"https://thingspeak.com/channels/");
    client.print(THINGSPEAK_CHANNEL_ID);
    client.print("/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15\"></iframe>");
    client.print("<iframe width=\"450\" height=\"260\" style=\"border: 1px solid #cccccc;\" src=\"https://thingspeak.com/channels/");
    client.print(THINGSPEAK_CHANNEL_ID);
    client.print("/charts/6?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15\"></iframe>");
  }
  client.println("</center></body></html>"); 
}


void webserverShowSite(WiFiServer &server, BME280<BME280_C, BME280_ADDRESS> &BMESensor, const PMS::DATA &pms) { 
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    bool blank_line = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
          if (c == '\n' && blank_line) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            showHtml(client, BMESensor, pms);
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
*/
