<<<<<<< Updated upstream
=======
#include <ArduinoJson.h> // 6.9.0 or later
#include <ESP8266httpUpdate.h>
#include "spiffs.h"

#ifdef ARDUINO_ARCH_ESP32
#include <Update.h>
#endif

#include <FS.h>

const char* www_realm = "Custom Auth Realm";
String authFailResponse = "<meta http-equiv='refresh' content='0; url=/' /> Authentication Failed! <p><a href='/'>Redirect</a></p>";

void handle_root() {
  String message = FPSTR(WEB_PAGE_HEADER);
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

      message.replace("{Temperature}", String(currentTemperature));
      message.replace("{Pressure}", String(currentPressure));
      message.replace("{Humidity}", String(currentHumidity));
      message.replace("{Dewpoint}", String(float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
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

      message.replace("{Temperature}", String(currentTemperature));
      message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
      message.replace("{Humidity}", String(currentHumidity));
      message.replace("{Dewpoint}", String(float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
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

      message.replace("{Temperature}", String(currentTemperature));
      message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
      message.replace("{Humidity}", String(currentHumidity));
      message.replace("{Dewpoint}", String(float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
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

      message.replace("{Temperature}", String(currentTemperature));
      message.replace("{Pressure}", String(currentPressure));
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

      message.replace("{Temperature}", String(currentTemperature));
      message.replace("{TEXT_PRESSURE}: {Pressure} hPa", "");
      message.replace("{Humidity}", String(float(currentHumidity)));
      message.replace("{Dewpoint}", String(float(pow((currentHumidity) / 100, 0.125) * (112 + 0.9 * (currentTemperature)) + 0.1 * (currentTemperature) - 112)));
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
>>>>>>> Stashed changes

void handle_root () {
	String message;
	message += "<!DOCTYPE html>";
	if(SELECTED_LANGUAGE == 1){
		message += "<html lang='en'>";
	} else if(SELECTED_LANGUAGE == 2){
		message += "<html lang='pl'>";
	} else {
		message += "<html lang='en'>";
	}
	message += "<head><title>Smogomierz</title> ";
	message += "<meta name='viewport' content='width=device-width, initial-scale=1' />";
	message += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />";
	if(SELECTED_LANGUAGE == 1){
		message += "<meta name='keywords' content='Smog, Air Quality, Air Dust' />";
	} else if(SELECTED_LANGUAGE == 2){
		message += "<meta name='keywords' content='Smog, Zanieczyszczenia powietrza, Pył Zawieszony, Smogomierz, Pomiar Zanieczyszczeń' />";
	} else {
		message += "<meta name='keywords' content='Smog, Air Quality, Air Dust' />";
	}
	message += "<script type='application/x-javascript'> addEventListener('load', function() { setTimeout(hideURLbar, 0); }, false); function hideURLbar(){ window.scrollTo(0,1); } </script>";
	message += "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css' integrity='sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO' crossorigin='anonymous'>";
	message += "<link rel='stylesheet' type='text/css' href='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/css/style11.css' />";
	message += "<link href='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/css/style.css' type='text/css' rel='stylesheet' media='all'> ";
	message += "<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.4.2/css/all.css' integrity='sha384-/rXc/GQVaYpyDdyxK+ecHPVYJSN9bmVFBvjA/9eOB+pb3F2w2N6fc5qB9Ew5yIns' crossorigin='anonymous'> ";
	message += "<link href='https://fonts.googleapis.com/css?family=Poppins:300,400,500,600,700' rel='stylesheet'>";
	message += "<link href='https://fonts.googleapis.com/css?family=Roboto:400,100,100italic,300,300italic,400italic,500,500italic,700,700italic,900,900italic' rel='stylesheet' type='text/css'></head>";
	message += "<body class='bg'><div class='overlay overlay-simplegenie'><button type='button' class='overlay-close'><i class='fas fa-times' ></i></button><nav><ul>";
	message += "<li><i class='fas fa-clock'></i><a href='/'>Pomiary</a></li>";
	message += "<li><i class='fas fa-cog'></i><a href='/config'>Ustawienia</a></li>";
	message += "<li><i class='fas fa-arrow-circle-up'></i><a href='/update'>Aktualizacje</a></li>";
	message += "<li><i class='fas fa-question-circle'></i><a href='/help'>Pomoc</a></li>";
	message += "<li><i class='fas fa-info-circle'></i><a href='/contact'>Kontakt</a></li>";
	message += "</ul></nav></div>";
	message += "<section class='header-w3ls'><button id='trigger-overlay' type='button'><i class='fas fa-bars' ></i></button><div class='bottons-agileits-w3layouts'>";
	message += "<!-- <a class='page-scroll' href='#myModal2' data-toggle='modal'><i class='fas fa-sign-in-alt' ></i>Login</a> -->";
	message += "<!-- <a class='page-scroll' href='#myModal3' data-toggle='modal'><i class='fas fa-pencil-alt' ></i>Register</a> -->";
	message += "</div>";
	message += "<h1><a href='/'>Smogomierz</a></h1><div class='clearfix'></div></section>";
	message += "<div class='banner-bottom'>";
	if (!strcmp(THP_MODEL, "BME280")) {
        if (checkBmeStatus()) {
			message += "<div class='bnr-btm-grids-agileits'><div class='bnr-btm-icon'><i class='fas fa-cloud-sun'></i></div><div class='bnr-btm-info'>";
			if(SELECTED_LANGUAGE == 1){
				message += "<h3>Weather</h3>";
	        	message += "<p>Temperature: ";
	        	message += (BMESensor.temperature);
	        	message += " °C</p>";
				message += "<p>Pressure: ";
				message += (BMESensor.seaLevelForAltitude(MYALTITUDE));
				message += " hPa<p>";
	        	message += "<p>Humidity: ";
	        	message += (BMESensor.humidity);
	        	message += " %</p>";
	        	message += "<p>Dew point: ";
	        	message += (BMESensor.temperature-((100-BMESensor.humidity)/5));
	        	message += " °C</p></div>";
			} else if(SELECTED_LANGUAGE == 2){
				message += "<h3>Pogoda</h3>";
	        	message += "<p>Temperatura: ";
	        	message += (BMESensor.temperature);
	        	message += " °C</p>";
				message += "<p>Ciśnienie: ";
				message += (BMESensor.seaLevelForAltitude(MYALTITUDE));
				message += " hPa<p>";
	        	message += "<p>Wilgotność: ";
	        	message += (BMESensor.humidity);
	        	message += " %</p>";
	        	message += "<p>Punkt rosy: ";
	        	message += (BMESensor.temperature-((100-BMESensor.humidity)/5));
	        	message += " °C</p></div>";
			} else {
				message += "<h3>Weather</h3>";
	        	message += "<p>Temperature: ";
	        	message += (BMESensor.temperature);
	        	message += " °C</p>";
				message += "<p>Pressure: ";
				message += (BMESensor.seaLevelForAltitude(MYALTITUDE));
				message += " hPa<p>";
	        	message += "<p>Humidity: ";
	        	message += (BMESensor.humidity);
	        	message += " %</p>";
	        	message += "<p>Dew point: ";
	        	message += (BMESensor.temperature-((100-BMESensor.humidity)/5));
	        	message += " °C</p></div>";
			}
		}
	} else if (!strcmp(THP_MODEL, "HTU21")) {
        if (checkHTU21DStatus()) {
			if(SELECTED_LANGUAGE == 1){
				message += "<div class='bnr-btm-grids-agileits'><div class='bnr-btm-icon'><i class='fas fa-cloud-sun'></i></div><div class='bnr-btm-info'>";
				message += "<h3>Weather</h3>";
				message += "<p>Temperature: ";
	        	message += (myHTU21D.readTemperature());
	        	message += " °C</p>";
				message += "<p>Humidity: ";
				message += (myHTU21D.readCompensatedHumidity());
				message += " %</p>";
				message += "<p>Dew point: ";
				message += (myHTU21D.readTemperature()-((100-myHTU21D.readCompensatedHumidity())/5));
				message += " °C</p></div>";
			} else if(SELECTED_LANGUAGE == 2){
				message += "<div class='bnr-btm-grids-agileits'><div class='bnr-btm-icon'><i class='fas fa-cloud-sun'></i></div><div class='bnr-btm-info'>";
				message += "<h3>Pogoda</h3>";
				message += "<p>Temperatura: ";
	        	message += (myHTU21D.readTemperature());
	        	message += " °C</p>";
				message += "<p>Wilgotność: ";
				message += (myHTU21D.readCompensatedHumidity());
				message += " %</p>";
				message += "<p>Punkt rosy: ";
				message += (myHTU21D.readTemperature()-((100-myHTU21D.readCompensatedHumidity())/5));
				message += " °C</p></div>";
			} else {
				message += "<div class='bnr-btm-grids-agileits'><div class='bnr-btm-icon'><i class='fas fa-cloud-sun'></i></div><div class='bnr-btm-info'>";
				message += "<h3>Weather</h3>";
				message += "<p>Temperature: ";
	        	message += (myHTU21D.readTemperature());
	        	message += " °C</p>";
				message += "<p>Humidity: ";
				message += (myHTU21D.readCompensatedHumidity());
				message += " %</p>";
				message += "<p>Dew point: ";
				message += (myHTU21D.readTemperature()-((100-myHTU21D.readCompensatedHumidity())/5));
				message += " °C</p></div>";
			}
        }
	} else if (!strcmp(THP_MODEL, "BMP280")) {
		if (checkBmpStatus()) {
			if(SELECTED_LANGUAGE == 1){
				message += "<div class='bnr-btm-grids-agileits'><div class='bnr-btm-icon'><i class='fas fa-cloud-sun'></i></div><div class='bnr-btm-info'>";
				message += "<h3>Weather</h3>";
				message += "<p>Temperature: ";
	        	message += (bmp.readTemperature());
	        	message += " °C</p>";
				message += "<p>Pressure: ";
				message += ((bmp.readPressure())/100);
				message += " hPa</p></div>";
			} else if(SELECTED_LANGUAGE == 2){
				message += "<div class='bnr-btm-grids-agileits'><div class='bnr-btm-icon'><i class='fas fa-cloud-sun'></i></div><div class='bnr-btm-info'>";
				message += "<h3>Pogoda</h3>";
				message += "<p>Temperatura: ";
	        	message += (bmp.readTemperature());
	        	message += " °C</p>";
				message += "<p>Ciśnienie: ";
				message += ((bmp.readPressure())/100);
				message += " hPa</p></div>";
			} else {
				message += "<div class='bnr-btm-grids-agileits'><div class='bnr-btm-icon'><i class='fas fa-cloud-sun'></i></div><div class='bnr-btm-info'>";
				message += "<h3>Weather</h3>";
				message += "<p>Temperature: ";
	        	message += (bmp.readTemperature());
	        	message += " °C</p>";
				message += "<p>Pressure: ";
				message += ((bmp.readPressure())/100);
				message += " hPa</p></div>";
			}
		}
	}
	message += "<div class='clearfix'> </div></div>";
	if (strcmp(DUST_MODEL, "Non") != 0){
		message += "<div class='bnr-btm-grids-agileits'><div class='bnr-btm-icon'><i class='fas fa-industry'></i></div><div class='bnr-btm-info'>";
		  if(SELECTED_LANGUAGE == 1){
			  message += "<h3>Air pollution</h3>";
		  } else if(SELECTED_LANGUAGE == 2){
			  message += "<h3>Pomiary zanieczyszczeń</h3>";
		  } else {
			  message += "<h3>Air pollution</h3>";
		  }
	      if (DISPLAY_PM1){
	          message += "<p>PM1: ";
	          message += (averagePM1);
	          message += " µg/m³</p>";
	        }
	        message += "<p>PM2.5: ";
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
	        message += " µg/m³</p></font>";
        
	        message += "<p>PM10: ";
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
	        message += " µg/m³</p></font>";
			message += "</div>";
		}
	message += "<div class='clearfix'></div></div>";
	message += "<div class='bnr-btm-grids-agileits'>";
	message += "<div class='bnr-btm-icon'>";
	message += "<i class='fas fa-info-circle'></i>";
	message += "</div><div class='bnr-btm-info'>";
	
	if(SELECTED_LANGUAGE == 1){
		message += "<h3>Info</h3>";
		message += "<p>Additional info 1.</p>";
		message += "<p>Additional info 2.</p>";
		message += "<p>Additional info 3.</p>";
	} else if(SELECTED_LANGUAGE == 2){
		message += "<h3>Info</h3>";
		message += "<p>";
		message += device_name;
		message += "</p>";
		message += "<p>Miernik Temp: ";
		message += THP_MODEL;	
		message += "</p>";
		message += "<p>Miernik PM: ";
		message += DUST_MODEL;	
		message += "</p>";
	} else {
		message += "<h3>Info</h3>";
		message += "<p>Additional info 1.</p>";
		message += "<p>Additional info 2.</p>";
		message += "<p>Additional info 3.</p>";
	}
	
	message += "</div><div class='clearfix'></div></div><div class='clearfix'></div></div><div class='options-wthree'><div class='container'><ul>";
	if(AIRMONITOR_GRAPH_ON){
		message += "<li>";
		//message += "<div class='icon-agileits-w3layouts'>";
    	message += ("<iframe src='http://metrics.airmonitor.pl:3000/dashboard-solo/db/airmonitor?orgId=1&var-latitude=");
    	message += (String(LATITUDE, 4));
    	message += ("&var-longitude=");
    	message += (String(LONGITUDE, 4));
    	message += ("&refresh=1m&panelId=14' width='auto' height='510' frameborder='0'></iframe>");
		//message += "</div><div class='opt-text-w3layouts'><h6></h6></div>";
		message += "</li><li>";
		//message += "<div class='icon-agileits-w3layouts'>";
        message += ("<iframe src='http://metrics.airmonitor.pl:3000/dashboard-solo/db/airmonitor?orgId=1&var-latitude=");
        message += (String(LATITUDE, 4));
        message += ("&var-longitude=");
        message += (String(LONGITUDE, 4));
        message += ("&refresh=1m&panelId=13' width='auto' height='510' frameborder='0'></iframe>");
		//message += "</div><div class='opt-text-w3layouts'><h6></h6></div>";
		message += "</li>";
	}
	if(THINGSPEAK_GRAPH_ON){
		message += "<li>";
		//message += "<div class='icon-agileits-w3layouts'>";
        message += ("<iframe width='auto' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
        message += (THINGSPEAK_CHANNEL_ID);
        message += ("/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=PM2.5&type=line&yaxis=ug%2Fm3&update=15'></iframe>");
		//message += "</div><div class='opt-text-w3layouts'><h6></h6></div>";
		message += "</li><li>";
		//message += "<div class='icon-agileits-w3layouts'>";
        message += ("<iframe width='auto' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/");
        message += (THINGSPEAK_CHANNEL_ID);
        message += ("/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=PM10&type=line&yaxis=ug%2Fm3&update=15'></iframe>");
		//message += "</div><div class='opt-text-w3layouts'><h6></h6></div>";
		message += "</li>";
	}
	message += "</ul></div></div>";
	
	message += "<div class='clearfix'></div></div><div class='clearfix'></div>";
	message += "<br></br><br></br><br></br>";
	message += "<p></p><p></p><p></p>";
	message += "<div class='clearfix'></div></div><div class='clearfix'></div>";
	
	message += "<footer class='fixed-bottom'><div class='agileinfo_footer_bottom1'><div class='container'>";
	message += "<p>Hackerspace Silesia &#9830; 2018</p>";
	message += "<div class='clearfix'> </div></div></div></footer>";
	message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script>";
	message += "<script src='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/js/scripts.js'></script>";
	message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/ResponsiveSlides.js/1.55/responsiveslides.js'></script>";
	message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/modernizr/2.8.3/modernizr.min.js'></script>";
	message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/classie/1.0.1/classie.js'></script>";
	message += "<script src='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/js/demo1.js'></script>";
	message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.4.1/jquery.easing.compatibility.js'></script> ";
	message += "<script type='text/javascript'>";
	message += "jQuery(document).ready(function($) {";
	message += "$('.scroll').click(function(event){		";
	message += "event.preventDefault();";
	message += "$('html,body').animate({scrollTop:$(this.hash).offset().top},1000);";
	message += "});";
	message += "});";
	message += "</script>";
	message += "<script src='https://code.jquery.com/jquery-3.3.1.slim.min.js' integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo' crossorigin='anonymous'></script>";
	message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.3/umd/popper.min.js' integrity='sha384-ZMP7rVo3mIykV+2+9J3UJ46jBk0WLaUAdn689aCwoqbBJiSnjAK/l8WvCWPIPm49' crossorigin='anonymous'></script>";
	message += "<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js' integrity='sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy' crossorigin='anonymous'></script>";
	message += "</body></html>";
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
	if(SELECTED_LANGUAGE == 1){
    	input += _addOption("yes", "Yes", selectValue);
    	input += _addOption("no", "No", selectValue);
	} else if(SELECTED_LANGUAGE == 2){
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
	if(SELECTED_LANGUAGE == 1){
    	//input += _addOption("black", "Case 86x86/120x80x50", value);
    	input += _addOption("red", "Without calibration", value);
		if (!strcmp(THP_MODEL, "BME280")){
		input += _addOption("white", "Automatic calibration", value);
		}
		if (!strcmp(THP_MODEL, "HTU21")){
		input += _addOption("white", "Automatic calibration", value);
		}
	} else if(SELECTED_LANGUAGE == 2){
	    input += _addOption("red", "Bez kalibracji", value);
		if (!strcmp(THP_MODEL, "BME280")){
		input += _addOption("white", "Automatyczna kalibracja", value);
		}
		if (!strcmp(THP_MODEL, "HTU21")){
		input += _addOption("white", "Automatyczna kalibracja", value);
		}
	} else {
    	input += _addOption("red", "Without calibration", value);
		if (!strcmp(THP_MODEL, "BME280")){
		input += _addOption("white", "Automatic calibration", value);
		}
		if (!strcmp(THP_MODEL, "HTU21")){
		input += _addOption("white", "Automatic calibration", value);
		}
	}
    input += "</select><br />";
    return input;
}

String _addTHP_MODELSelect(const String &key, const String &value) {
    String input = "<select name='";
    input += key;
    input += "'>";
	if(SELECTED_LANGUAGE == 1){
    	input += _addOption("BME280", "BME280", value);
		input += _addOption("HTU21", "SHT21/HTU21D", value);
		input += _addOption("BMP280", "BMP280", value);
		input += _addOption("Non", "Without sensor", value);
	} else if(SELECTED_LANGUAGE == 2){
    	input += _addOption("BME280", "BME280", value);
		input += _addOption("HTU21", "SHT21/HTU21D", value);
		input += _addOption("BMP280", "BMP280", value);
		input += _addOption("Non", "Bez miernika", value);
	} else {
    	input += _addOption("BME280", "BME280", value);
		input += _addOption("HTU21", "SHT21/HTU21D", value);
		input += _addOption("BMP280", "BMP280", value);
		input += _addOption("Non", "Without sensor", value);
	}
    input += "</select><br />";
    return input;
}

String _addDUST_MODELSelect(const String &key, const String &value) {
    String input = "<select name='";
    input += key;
    input += "'>";
	if(SELECTED_LANGUAGE == 1){
    	input += _addOption("PMS7003", "PMS7003", value);
		input += _addOption("SDS011/21", "SDS011/21", value);
		input += _addOption("HPMA115S0", "HPMA115S0", value);
		input += _addOption("Non", "Without sensor", value);
	} else if(SELECTED_LANGUAGE == 2){
	    input += _addOption("PMS7003", "PMS7003", value);
		input += _addOption("SDS011/21", "SDS011/21", value);
		input += _addOption("HPMA115S0", "HPMA115S0", value);
		input += _addOption("Non", "Bez miernika", value);
	} else {
    	input += _addOption("PMS7003", "PMS7003", value);
		input += _addOption("SDS011/21", "SDS011/21", value);
		input += _addOption("HPMA115S0", "HPMA115S0", value);
		input += _addOption("Non", "Without sensor", value);
	}
    input += "</select><br />";
    return input;
}

String _addLanguageSelect(const String &key, const String &value) {
    String input = "<select name='";
    input += key;
    input += "'>";
	if(SELECTED_LANGUAGE == 1){
	    input += _addOption("polish", "polish", value);
	    input += _addOption("english", "english", value);
	} else if(SELECTED_LANGUAGE == 2){
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
	if(SELECTED_LANGUAGE == 1){
		return "<input type='submit' class='btn btn-outline-danger' value='Save' /><br /><br />";
	} else if(SELECTED_LANGUAGE == 2){
		return "<input type='submit' class='btn btn-outline-danger' value='Zapisz' /><br /><br />";
	} else {
		return "<input type='submit' class='btn btn-outline-danger' value='Save' /><br /><br />";
	}
}

void _handle_config(bool is_success) {
<<<<<<< Updated upstream
	String message;
	
	message += "<!DOCTYPE html>";
	if(SELECTED_LANGUAGE == 1){
		message += "<html lang='en'>";
	} else if(SELECTED_LANGUAGE == 2){
		message += "<html lang='pl'>";
	} else {
		message += "<html lang='en'>";
	}
	message += "<head><title>Smogomierz</title> ";
	message += "<meta name='viewport' content='width=device-width, initial-scale=1' />";
	message += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />";
	if(SELECTED_LANGUAGE == 1){
		message += "<meta name='keywords' content='Smog, Air Quality, Air Dust' />";
	} else if(SELECTED_LANGUAGE == 2){
		message += "<meta name='keywords' content='Smog, Zanieczyszczenia powietrza, Pył Zawieszony, Smogomierz, Pomiar Zanieczyszczeń' />";
	} else {
		message += "<meta name='keywords' content='Smog, Air Quality, Air Dust' />";
	}
	message += "<script type='application/x-javascript'> addEventListener('load', function() { setTimeout(hideURLbar, 0); }, false); function hideURLbar(){ window.scrollTo(0,1); } </script>";
	message += "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css' integrity='sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO' crossorigin='anonymous'>";
	message += "<link rel='stylesheet' type='text/css' href='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/css/style11.css' />";
	message += "<link href='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/css/style.css' type='text/css' rel='stylesheet' media='all'> ";
	message += "<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.4.2/css/all.css' integrity='sha384-/rXc/GQVaYpyDdyxK+ecHPVYJSN9bmVFBvjA/9eOB+pb3F2w2N6fc5qB9Ew5yIns' crossorigin='anonymous'> ";
	message += "<link href='https://fonts.googleapis.com/css?family=Poppins:300,400,500,600,700' rel='stylesheet'>";
	message += "<link href='https://fonts.googleapis.com/css?family=Roboto:400,100,100italic,300,300italic,400italic,500,500italic,700,700italic,900,900italic' rel='stylesheet' type='text/css'></head>";
	message += "<body class='bg'><form method='POST' action='/config'><div class='overlay overlay-simplegenie'><button type='button' class='overlay-close'><i class='fas fa-times' ></i></button><nav><ul>";
	message += "<li><i class='fas fa-clock'></i><a href='/'>Pomiary</a></li>";
	message += "<li><i class='fas fa-cog'></i><a href='/config'>Ustawienia</a></li>";
	message += "<li><i class='fas fa-arrow-circle-up'></i><a href='/update'>Aktualizacje</a></li>";
	message += "<li><i class='fas fa-question-circle'></i><a href='/help'>Pomoc</a></li>";
	message += "<li><i class='fas fa-info-circle'></i><a href='/contact'>Kontakt</a></li>";
	message += "</ul></nav></div>";
	message += "<section class='header-w3ls'><button id='trigger-overlay' type='button'><i class='fas fa-bars' ></i></button><div class='bottons-agileits-w3layouts'>";
	message += "<!-- <a class='page-scroll' href='#myModal2' data-toggle='modal'><i class='fas fa-sign-in-alt' ></i>Login</a> -->";
	message += "<!-- <a class='page-scroll' href='#myModal3' data-toggle='modal'><i class='fas fa-pencil-alt' ></i>Register</a> -->";
	message += "</div>";
	message += "<h1><a href='/'>Smogomierz</a></h1><div class='clearfix'></div></section>";
	
	//message += "	<div class='inner-banner-agileits-w3layouts'>"; // Zmienic na grafike zwiazana z HSem! 
	//message += "	</div>";
	
	message += "<!-- locations -->";
	message += "	<div class='contact w3-agileits'>";
	message += "	<div class='container'>";
	message += "	<h4 class='tittle-w3layouts'>Ustawienia<h4>";
	if (is_success) {
		        message += "<br><div style='color: #2f7a2d'> <strong>ZAPISANO!</strong> - wszystko wygląda OK, za chwilę nastąpi restart Smogomierza </div><br>";
		    }
	message += "	</div>";
	message += "<div class='locations-w3-agileits'>";
	message += "	<div class='container'>";
	
	message += "	<div class='location-agileits'>";
	message += "		<div class='loc-left'>";
	// z lewej
	message += "			<h4>Nazwa i język</h4>";
    message += "<p>Nazwa urządzenia: ";
   	if (DEVICENAME_AUTO) {
		message += device_name;
		message += "</p>";
	} else {
		message += _addTextInput("DEVICENAME", DEVICENAME);
		message += "</p>";
	}	
   	message += "<p>Automatyczne generowanie nazwy: ";
	message += _addBoolSelect("DEVICENAME_AUTO", DEVICENAME_AUTO);
	message += "</p>";
	message += "<p>Język: ";
	message += _addLanguageSelect("LANGUAGE", LANGUAGE);
	message += "</p>";
	
	//message += "<p>Opcje zaawansowane: ";
	//message += _addBoolSelect("ADVANCED_OPTIONS_ON", ADVANCED_OPTIONS_ON);
	//message += "</p>";
	message += "		</div>";
	
	message += "		<div class='loc-right'>";
	//z prawej
	message += "		<br><br><h4>Używane mierniki</h4>";
    message += "<p>Miernik Temp/Wilgotności/Ciśnienia: ";
   	message += _addTHP_MODELSelect("THP_MODEL", THP_MODEL);
	message += "</p>";
   	message += "Miernik PM2.5/PM10: ";
    message += _addDUST_MODELSelect("DUST_MODEL", DUST_MODEL);
	message += "</p>";
	if (!strcmp(DUST_MODEL, "PMS7003")){
		message += "<p>Wyświetlanie pomiarów PM1: ";
		message += _addBoolSelect("DISPLAY_PM1", DISPLAY_PM1);
		message += "</p>";
	}
	message += "		</div>";
	message += "		<div class='clearfix'> </div>";

	message += "	<div class='location-agileits'>";
	message += "		<div class='loc-left'>";
	// z lewej
	message += "			<h4>AirMonitor</h4>";
	message += "<p>Wysyłanie danych do serwisu <a title='AirMonitor' href='http://mapa.airmonitor.pl' target='_blank'>AirMonitor</a>(wymaga wypełnienia <a title='Formularz AirMonitor' href='https://docs.google.com/forms/d/e/1FAIpQLSdw72_DggyrK7xnSQ1nR11Y-YK4FYWk_MF9QbecpOERql-T2w/viewform' target='_blank'>formularza</a>; Sensor: PMS7003): ";
	message += _addBoolSelect("AIRMONITOR_ON", AIRMONITOR_ON);
	message += "</p>";
	message += "<p>Wyświetlanie wykresów z serwisu AirMonitor: ";
	message += _addBoolSelect("AIRMONITOR_GRAPH_ON", AIRMONITOR_GRAPH_ON);
	message += "</p>";
	message += "		</div>";
	
	message += "		<div class='loc-right'>";
	// z prawej
	message += "		<br><br><h4>Współrzędne geograficzne</h4>";
    message += "<p>Współrzędne geograficzne miernika(można sprawdzić <a title='wspolrzedne-gps.pl' href='https://www.wspolrzedne-gps.pl' target='_blank'>Tutaj</a>):<br>Szerokość(latitude): ";
	message += _addFloatInput("LATITUDE", LATITUDE, 6, "°");
	message += "</p>";
	message += "<p>Długość(longitude): ";
	message += _addFloatInput("LONGITUDE", LONGITUDE, 6, "°");
	message += "</p>";
	message += "<p>Wysokość: ";
	message += _addIntInput("MYALTITUDE", MYALTITUDE, "m.n.p.m");
	message += "</p>";
	message += "		</div>";
	message += "		</div>";
	
	message += "		<div class='clearfix'> </div>";
	
	message += "	<div class='location-agileits'>";
	message += "		<div class='loc-left'>";
	// z lewej
	message += "			<h4>ThingSpeak</h4>";
	message += "<p>Wysyłanie danych do serwisu <a title='ThingSpeak' href='https://thingspeak.com' target='_blank'>ThingSpeak</a>: ";
	message += _addBoolSelect("THINGSPEAK_ON", THINGSPEAK_ON);
	message += "</p>";
	message += "<p>Wyświetlanie wykresów z serwisu ThingSpeak: ";
	message += _addBoolSelect("THINGSPEAK_GRAPH_ON", THINGSPEAK_GRAPH_ON);
	message += "</p>";
	message += "<p>ThingSpeak API_KEY: ";
	message += _addTextInput("THINGSPEAK_API_KEY", THINGSPEAK_API_KEY);
	message += "</p>";
	message += "<p>ThingSpeak Channel ID: ";
	message += _addIntInput("THINGSPEAK_CHANNEL_ID", THINGSPEAK_CHANNEL_ID);
	message += "</p>";
	message += "		</div>";
	
	message += "		<div class='loc-right'>";
	// z prawej
	message += "		<br><br><h4>InfluxDB</h4>";
	message += "<p>Wysyłanie danych do InfluxDB: ";
	message += _addBoolSelect("INFLUXDB_ON", INFLUXDB_ON);
	message += "</p>";
	message += "<p>Adres bazy danych InfluxDB: ";
	message += _addTextInput("INFLUXDB_HOST", INFLUXDB_HOST);
	message += "</p>";
	message += "<p>Port InfluxDB: ";
	message += _addIntInput("INFLUXDB_PORT", INFLUXDB_PORT);
	message += "</p>";
	message += "<p>Nazwa bazy danych: ";
	message += _addTextInput("DATABASE", DATABASE);
	message += "</p>";
	message += "<p>Użytkownik bazy danych: ";
	message += _addTextInput("DB_USER", DB_USER);
	message += "</p>";
	message += "<p>Hasło do bazy danych: ";
	message += _addPasswdInput("DB_PASSWORD", DB_PASSWORD);
	message += "</p>";
	message += "		</div>";
	message += "		</div>";
	message += "		<div class='clearfix'> </div>";
	
	//if (ADVANCED_OPTIONS_ON) {
		//message += "<!-- ";
		//}
		message += "	<div class='location-agileits'>";
		message += "		<div class='loc-left'>";
		// z lewej
		message += "			<h4>Zaawansowane</h4>";
		
	    message += "<p>Wykonywanie pomiarów PM co: ";
	    message += _addIntInput("DUST_TIME", DUST_TIME, "sekund");
		message += "</p>";
		
	    message += "<p>Uśredniaj wynik z ostatnich: ";
	    message += _addIntInput("NUMBEROFMEASUREMENTS", NUMBEROFMEASUREMENTS, "pomiarów PM");
		message += "</p>";
  
		message += "<p>Wysyłanie pomiarów do AirMonitor co: ";
		message += _addIntInput("AIRMONITOR_TIME", AIRMONITOR_TIME, "minut");
		message += "</p>";
  
		message += "<p>Wysyłanie pomiarów do ThingSpeak co: ";
		message += _addIntInput("THINGSPEAK_TIME", THINGSPEAK_TIME, "minut");
		message += "</p>";	

		message += "<p>Wysyłanie pomiarów do InfluxDB co: ";
		message += _addIntInput("INFLUXDB_TIME", INFLUXDB_TIME, "sekund");
		message += "</p>";

		message += "<p>Debug: ";
	    message += _addBoolSelect("DEBUG", DEBUG);
		message += "</p>";

	    message += "<p>Sposób kalibracji: ";
	    message += _addModelSelect("MODEL", MODEL);
		message += "</p>";
		
	    message += "<p>Zmienna kalibracyjna calib1: ";
	    message += (calib1);
		message += "</p>";
	    message += "<p>Zmienna kalibracyjna calib2: ";
	    message += (calib2);
		message += "</p>";
		
	    message += "<p>Wersja oprogramowania: <b>";
	    message += (SOFTWAREVERSION);
		message += "</b></p>";
		
	    message += "<p>Automatyczne aktualizacje: <b>";
		message += _addBoolSelect("AUTOUPDATE_ON", AUTOUPDATE_ON);
		message += "</b></p>";
		
	    message += "<p>Platforma sprzętowa: <b>";
	    message += (HARDWAREVERSION);
		message += "</b></p>";
		message += "		</div>";
		message += "		</div>";
		message += "		<div class='clearfix'> </div>";	
		//if (!ADVANCED_OPTIONS_ON) {
		//	message += " --> ";
		//}
	
	message += "</div>";
	
	message += "<br><center>";
	message += _addSubmit();
	message += "</center>";
	
	message += "		</div>";
	message += "		<div class='clearfix'> </div>";
	message += "	</div>";
	message += "		<div class='clearfix'> </div>";
	message += "	</div>";
	message += "	</div>";
	message += "<!-- /locations --> ";
	
	message += "<br></br>";
	message += "<div class='clearfix'></div></div><div class='clearfix'></div>";
	message += "<p></p><p></p>";
	message += "<div class='clearfix'></div></div><div class='clearfix'></div>";
	message += "<footer class='fixed-bottom'><div class='agileinfo_footer_bottom1'><div class='container'>";
	message += "<p>Hackerspace Silesia &#9830; 2018</p>";
	message += "<div class='clearfix'> </div></div></div></footer>";
	message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script>";
	message += "<script src='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/js/scripts.js'></script>";
	message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/ResponsiveSlides.js/1.55/responsiveslides.js'></script>";
	message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/modernizr/2.8.3/modernizr.min.js'></script>";
	message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/classie/1.0.1/classie.js'></script>";
	message += "<script src='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/js/demo1.js'></script>";
	message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.4.1/jquery.easing.compatibility.js'></script> ";
	message += "<script type='text/javascript'>";
	message += "jQuery(document).ready(function($) {";
	message += "$('.scroll').click(function(event){		";
	message += "event.preventDefault();";
	message += "$('html,body').animate({scrollTop:$(this.hash).offset().top},1000);";
	message += "});";
	message += "});";
	message += "</script>";
	message += "<script src='https://code.jquery.com/jquery-3.3.1.slim.min.js' integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo' crossorigin='anonymous'></script>";
	message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.3/umd/popper.min.js' integrity='sha384-ZMP7rVo3mIykV+2+9J3UJ46jBk0WLaUAdn689aCwoqbBJiSnjAK/l8WvCWPIPm49' crossorigin='anonymous'></script>";
	message += "<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js' integrity='sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy' crossorigin='anonymous'></script>";
	message += "</form></body></html>";
    WebServer.send(200, "text/html", message);
	
	
	/*
	if(SELECTED_LANGUAGE == 1){
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
=======
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
>>>>>>> Stashed changes
    }
	
	message += "All instructions and descriptions[in polish] are available <a title='Instructions' href='https://github.com/hackerspace-silesia/Smogomierz#instrukcje' target='_blank'>here</a>.<br><br>";

<<<<<<< Updated upstream
    message += "<b>Device Name: </b>";
    if (DEVICENAME_AUTO) {
        message += device_name;
	    message += "<br>";
=======
  message += FPSTR(WEB_CONFIG_PAGE_CONFIG);

  message.replace("{TEXT_DEVICENAME}", (TEXT_DEVICENAME));
  if (DEVICENAME_AUTO) {
    message.replace("{device_name}", (device_name));
  } else {
    message.replace("{device_name}", _addTextInput("DEVICENAME", DEVICENAME));
  }
  message.replace("{TEXT_DEVICENAMEAUTO}", (TEXT_DEVICENAMEAUTO));
  message.replace("{DEVICENAME_AUTO}", _addBoolSelect("DEVICENAME_AUTO", DEVICENAME_AUTO));
  message.replace("{TEXT_SELECTEDLANGUAGE}", (TEXT_SELECTEDLANGUAGE));
  message.replace("{LanguageSelect}", _addLanguageSelect("LANGUAGE", LANGUAGE));
  message.replace("{TEXT_TEMPHUMIPRESSSENSOR}", (TEXT_TEMPHUMIPRESSSENSOR));
  message.replace("{THP_MODELSelect}", _addTHP_MODELSelect("THP_MODEL", THP_MODEL));
  message.replace("{TEXT_PMSENSOR}", (TEXT_PMSENSOR));
  message.replace("{DUST_MODELSelect}", _addDUST_MODELSelect("DUST_MODEL", DUST_MODEL));

  message.replace("{TEXT_FREQUENTMEASUREMENTONOFF}", (TEXT_FREQUENTMEASUREMENTONOFF));
  message.replace("{TEXT_FREQUENTMEASUREMENTINFO}", (TEXT_FREQUENTMEASUREMENTINFO));
  message.replace("{TEXT_MEASUREMENTFREQUENCY}", (TEXT_MEASUREMENTFREQUENCY));

  message.replace("{FREQUENTMEASUREMENT_Select}", _addBoolSelect("FREQUENTMEASUREMENT", FREQUENTMEASUREMENT));
  
  if (FREQUENTMEASUREMENT == true) {
    message.replace("{FREQUENTMEASUREMENT_time}", _addIntInput("DUST_TIME", DUST_TIME, "{TEXT_SECONDS}"));
    message.replace("{TEXT_SECONDS}", (TEXT_SECONDS));
  } else {
    message.replace("{FREQUENTMEASUREMENT_time}", _addIntInput("DUST_TIME", DUST_TIME, "{TEXT_MINUTES}"));
    message.replace("{TEXT_MINUTES}", (TEXT_MINUTES));
  }

  message.replace("{TEXT_AVERAGELASTRESULT}", (TEXT_AVERAGELASTRESULT));
  message.replace("{NUMBEROFMEASUREMENTS}", _addIntInput("NUMBEROFMEASUREMENTS", NUMBEROFMEASUREMENTS, "{TEXT_PMMEASUREMENTS}"));
  message.replace("{TEXT_PMMEASUREMENTS}", (TEXT_PMMEASUREMENTS));

  if (FREQUENTMEASUREMENT == true) {
    message.replace("{TEXT_SENDINGINTERVAL}", (TEXT_SERVICESSENDINGINTERVAL));
    message.replace("{SENDING_FREQUENCY}", _addIntInput("SENDING_FREQUENCY", SENDING_FREQUENCY, "{TEXT_SECONDS}"));
    message.replace("{TEXT_SECONDS}", (TEXT_SECONDS));

    message.replace("{TEXT_DBSENDINGINTERVAL}", (TEXT_DBSENDINGINTERVAL));
    message.replace("{SENDING_DB_FREQUENCY}", _addIntInput("SENDING_DB_FREQUENCY", SENDING_DB_FREQUENCY, "{TEXT_SECONDS}"));
    message.replace("{TEXT_SECONDS}", (TEXT_SECONDS));

    message.replace("<hr><b>DeepSleep: </b>{DEEPSLEEP_ON} {TEXT_DEEPSLEEPINFO}", "");
  } else {
    if (DEEPSLEEP_ON == true) {
      message.replace("{TEXT_SENDINGINTERVAL}", (TEXT_SENDINGINTERVAL));
      message.replace("{SENDING_FREQUENCY}", _addIntInput("SENDING_FREQUENCY", SENDING_FREQUENCY, "{TEXT_MINUTES}"));
      message.replace("{TEXT_MINUTES}", (TEXT_MINUTES));

      message.replace("<b>{TEXT_DBSENDINGINTERVAL}: </b>{SENDING_DB_FREQUENCY}", "");
>>>>>>> Stashed changes
    } else {
        message += _addTextInput("DEVICENAME", DEVICENAME);
    }
<<<<<<< Updated upstream
    message += "<b>Automatic name generation: </b>";
    message += _addBoolSelect("DEVICENAME_AUTO", DEVICENAME_AUTO);
	
    message += "<b>Language: </b>";
    message += _addLanguageSelect("LANGUAGE", LANGUAGE);
	
	message += "<b>Opcje zaawansowane: </b>";
	message += _addBoolSelect("ADVANCED_OPTIONS_ON", ADVANCED_OPTIONS_ON);
	
	message += "<hr>";
	
    message += "<b>Temp/Humi/Press Sensor: </b>";
    message += _addTHP_MODELSelect("THP_MODEL", THP_MODEL);

    message += "<b>PM2.5/PM10 Sensor: </b>";
    message += _addDUST_MODELSelect("DUST_MODEL", DUST_MODEL);
	
	if (!strcmp(DUST_MODEL, "PMS7003")){
    	message += "<b>Display of PM1 measurements: </b>";
    	message += _addBoolSelect("DISPLAY_PM1", DISPLAY_PM1);
	}
	
    message += "<b>Read PM measurements every: </b>";
    message += _addIntInput("DUST_TIME", DUST_TIME, "seconds");
	
    message += "<b>Number of measurements for averaging: </b>";
    message += _addIntInput("NUMBEROFMEASUREMENTS", NUMBEROFMEASUREMENTS);
	
	message += "<hr>";	
  
    message += "<b>Sending data to the <a title='AirMonitor' href='http://mapa.airmonitor.pl' target='_blank'>AirMonitor</a> service(requires filling out <a title='AirMonitor Form' href='https://docs.google.com/forms/d/e/1FAIpQLSdw72_DggyrK7xnSQ1nR11Y-YK4FYWk_MF9QbecpOERql-T2w/viewform' target='_blank'>the form</a>; Sensor: PMS7003): </b>";
    message += _addBoolSelect("AIRMONITOR_ON", AIRMONITOR_ON);
	
    message += "<b>Send measurements every: </b>";
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
	
    message += "<b>Send measurements every: </b>";
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
    message += "<b>Send measurements every: </b>";
    message += _addIntInput("INFLUXDB_TIME", INFLUXDB_TIME, "seconds");
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
	
    //message += "<b>calib2: </b>";
    //message += (calib2);
    //message += "<br>";
	
    message += "<b>Software version: </b>";
    message += (SOFTWAREVERSION);
	message += "<br>";
    message += "<b>Hardware version: </b>";
    message += (HARDWAREVERSION);
	message += "<hr><br><center>";
    message += _addSubmit();
	message += "</center>";
	} else if(SELECTED_LANGUAGE == 2){
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
		
	    message += "<b>Język: </b>";
	    message += _addLanguageSelect("LANGUAGE", LANGUAGE);
		
		message += "<hr>";
		
	    message += "<b>Miernik Temp/Wilgotności/Ciśnienia: </b>";
	    message += _addTHP_MODELSelect("THP_MODEL", THP_MODEL);

	    message += "<b>Miernik PM2.5/PM10: </b>";
	    message += _addDUST_MODELSelect("DUST_MODEL", DUST_MODEL);
		
		if (!strcmp(DUST_MODEL, "PMS7003")){
		    message += "<b>Wyświetlanie pomiarów PM1: </b>";
		    message += _addBoolSelect("DISPLAY_PM1", DISPLAY_PM1);
		}
		
	    message += "<b>Wykonywanie pomiarów PM co: </b>";
	    message += _addIntInput("DUST_TIME", DUST_TIME, "sekund");
		
	    message += "<b>Uśredniaj wynik z ostatnich: </b>";
	    message += _addIntInput("NUMBEROFMEASUREMENTS", NUMBEROFMEASUREMENTS, "pomiarów");
		
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
	    message += _addIntInput("INFLUXDB_TIME", INFLUXDB_TIME, "sekund");
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
		
	    //message += "<b>Zmienna kalibracyjna calib2: </b>";
	    //message += (calib2);
	    //message += "<br>";
		
	    message += "<b>Wersja oprogramowania: </b>";
	    message += (SOFTWAREVERSION);
		message += "<br>";
	    message += "<b>Platforma sprzętowa: </b>";
	    message += (HARDWAREVERSION);
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
		
	    message += "<b>Language: </b>";
	    message += _addLanguageSelect("LANGUAGE", LANGUAGE);
		
		message += "<hr>";
		
	    message += "<b>Temp/Humi/Press Sensor: </b>";
	    message += _addTHP_MODELSelect("THP_MODEL", THP_MODEL);

	    message += "<b>PM2.5/PM10 Sensor: </b>";
	    message += _addDUST_MODELSelect("DUST_MODEL", DUST_MODEL);
		
		if (!strcmp(DUST_MODEL, "PMS7003")){
	    	message += "<b>Display of PM1 measurements: </b>";
	    	message += _addBoolSelect("DISPLAY_PM1", DISPLAY_PM1);
		}
		
	    message += "<b>Read PM measurements every: </b>";
	    message += _addIntInput("DUST_TIME", DUST_TIME, "seconds");
		
	    message += "<b>Number of measurements for averaging: </b>";
	    message += _addIntInput("NUMBEROFMEASUREMENTS", NUMBEROFMEASUREMENTS);
		
		message += "<hr>";	
  
	    message += "<b>Sending data to the <a title='AirMonitor' href='http://mapa.airmonitor.pl' target='_blank'>AirMonitor</a> service(requires filling out <a title='AirMonitor Form' href='https://docs.google.com/forms/d/e/1FAIpQLSdw72_DggyrK7xnSQ1nR11Y-YK4FYWk_MF9QbecpOERql-T2w/viewform' target='_blank'>the form</a>; Sensor: PMS7003): </b>";
	    message += _addBoolSelect("AIRMONITOR_ON", AIRMONITOR_ON);
	
	    message += "<b>Send measurements every: </b>";
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
	
	    message += "<b>Send measurements every: </b>";
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
	    message += "<b>Send measurements every: </b>";
	    message += _addIntInput("INFLUXDB_TIME", INFLUXDB_TIME, "seconds");
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
		
	    //message += "<b>calib2: </b>";
	    //message += (calib2);
	    //message += "<br>";
		
	    message += "<b>Software version: </b>";
	    message += (SOFTWAREVERSION);
		message += "<br>";
	    message += "<b>Hardware version: </b>";
	    message += (HARDWAREVERSION);
		message += "<hr><br><center>";
	    message += _addSubmit();
		message += "</center>";	
	}
    message += "<hr><center>Hackerspace Silesia &#9830; 2018</center></div></main></form></body></html>";
    WebServer.send(200, "text/html", message);
	*/
=======

    message.replace("{TEXT_DEEPSLEEPINFO}", TEXT_DEEPSLEEPINFO);
    message.replace("{INTERFACEWWWONTIME}", String(int(NUMBEROFMEASUREMENTS) * 2 + 10 ));
    message.replace("{SENDING_FREQUENCY}", String(SENDING_FREQUENCY));
    message.replace("{DEEPSLEEP_ON}", _addBoolSelect("DEEPSLEEP_ON", DEEPSLEEP_ON));
  }
  
  if (!strcmp(DUST_MODEL, "PMS7003")) {
    message.replace("{DISPLAY_PM1}", _addBoolSelect("DISPLAY_PM1", DISPLAY_PM1));
    message.replace("{TEXT_DISPLAYPM1}", (TEXT_DISPLAYPM1));
  } else {
    message.replace("<b>{TEXT_DISPLAYPM1}: </b> {DISPLAY_PM1}", "");
  }
  message.replace("{TEXT_ALTITUDEINFO}", (TEXT_ALTITUDEINFO));
  message.replace("{WSPOLRZEDNE_GPS_LINK}", (WSPOLRZEDNE_GPS_LINK));
  message.replace("{TEXT_HERE}", (TEXT_HERE));
  message.replace("{MYALTITUDE}", _addIntInput("MYALTITUDE", MYALTITUDE, "m.n.p.m"));

  message.replace("{TEXT_SECURECONFIGUPDATEPAGE}", (TEXT_SECURECONFIGUPDATEPAGE));
  message.replace("{CONFIG_AUTH}", _addBoolSelect("CONFIG_AUTH", CONFIG_AUTH));
  message.replace("{TEXT_SECURELOGIN}", (TEXT_SECURELOGIN));
  
  message.replace("{CONFIG_USERNAME}", _addTextInput("CONFIG_USERNAME", CONFIG_USERNAME));
  message.replace("{TEXT_SECUREPASSWD}", (TEXT_SECUREPASSWD));
  
Serial.println("CRASH - 1 - _addPasswdInput");
  //message.replace("{CONFIG_PASSWORD}", _addPasswdInput("CONFIG_PASSWORD", CONFIG_PASSWORD)); // CRASH!!!
  
  if (!CONFIG_AUTH) {
    message.replace("{TEXT_SECURELOGOUTINFO}", "");
  } else {
    message.replace("{TEXT_SECURELOGOUTINFO}", (TEXT_SECURELOGOUTINFO));
  }
  
  message.replace("{TEXT_SMOGLISTSENDING}", (TEXT_SMOGLISTSENDING));
  message.replace("{SMOGLIST_LINK}", (SMOGLIST_LINK));
  message.replace("{SMOGLIST_ON}", _addBoolSelect("SMOGLIST_ON", SMOGLIST_ON));
  message.replace("{TEXT_SMOGLISTINFO}", (TEXT_SMOGLISTINFO));

  message.replace("{TEXT_LUFTDATENSENDING}", (TEXT_LUFTDATENSENDING));
  
Serial.println("CRASH - 2 - LINK");
  //message.replace("{LUFTDATEN_LINK}", (LUFTDATEN_LINK)); // CRASH!!!
  //message.replace("{LUFTDATENFORM_LINK}", (LUFTDATENFORM_LINK)); // CRASH!!!

    message.replace("{LUFTDATEN_ON}", _addBoolSelect("LUFTDATEN_ON", LUFTDATEN_ON));
  #ifdef ARDUINO_ARCH_ESP8266
      message.replace("{ChipID}", "smogomierz-" + String(ESP.getChipId()));
  #elif defined ARDUINO_ARCH_ESP32
  	message.replace("{ChipID}", "smogomierz-" + String(ESP.getEfuseMac()));
  #endif
    
  if (!strcmp(THP_MODEL, "BME280")) {
    message.replace("{THPSENSOR}", "BME280");
    message.replace("{THPXPIN}", "11");
  } else if (!strcmp(THP_MODEL, "BMP280")) {
    message.replace("{THPSENSOR}", "BMP280");
    message.replace("{THPXPIN}", "3");
  } else if (!strcmp(THP_MODEL, "HTU21")) {
    message.replace("{THPSENSOR}", "HTU21");
    message.replace("{THPXPIN}", "7");
  } else if (!strcmp(THP_MODEL, "DHT22")) {
    message.replace("{THPSENSOR}", "DHT22");
    message.replace("{THPXPIN}", "7");
  } else if (!strcmp(THP_MODEL, "SHT1x")) {
    message.replace("{THPSENSOR}", "SHT1x");
    message.replace("{THPXPIN}", "12");
  } else {
    message.replace("<br><b>{THPSENSOR}</b> Sensor PIN: <b>{THPXPIN}</b>", "");
  }

  if (!strcmp(DUST_MODEL, "PMS7003")) {
    message.replace("{DUSTSENSOR}", "PMS5003/7003");
    message.replace("{DUSTXPIN}", "1");
  } else if (!strcmp(DUST_MODEL, "SDS011/21")) {
    message.replace("{DUSTSENSOR}", "SDS011/21");
    message.replace("{DUSTXPIN}", "1");
  } else if (!strcmp(DUST_MODEL, "HPMA115S0")) {
    message.replace("{DUSTSENSOR}", "HPMA115S0");
    message.replace("{DUSTXPIN}", "1");
  } else {
    message.replace("<br><b>{DUSTSENSOR}</b> Sensor PIN: <b>{DUSTXPIN}</b>", "");
  }

  message.replace("{TEXT_AIRMONITORSENDING}", (TEXT_AIRMONITORSENDING));
  
  char PMSENSORMODEL[16];
  if (!strcmp(DUST_MODEL, "PMS7003") or !strcmp(DUST_MODEL, "Non")) {
    strcpy(PMSENSORMODEL, "PMS7003");
  } else if (!strcmp(DUST_MODEL, "SDS011/21")) {
    strcpy(PMSENSORMODEL, "SDS011");
  } else if (!strcmp(DUST_MODEL, "HPMA115S0")) {
    strcpy(PMSENSORMODEL, "HPMA115S0");
  }
  message.replace("{PMSENSORMODEL}", PMSENSORMODEL);
  
Serial.println("CRASH - 3 - LINK"); // CRASH!!! 
/*
  message.replace("{AIRMONITOR_LINK}", (AIRMONITOR_LINK));
  message.replace("{AIRMONITORFORM_LINK}", (AIRMONITORFORM_LINK));
  message.replace("{TEXT_THEFORM}", (TEXT_THEFORM));
*/

  message.replace("{TEXT_AIRMONITORCHARTS}", (TEXT_AIRMONITORCHARTS));
  message.replace("{AIRMONITOR_ON}", _addBoolSelect("AIRMONITOR_ON", AIRMONITOR_ON));
  message.replace("{TEXT_AIRMONITORCOORDINATESINFO}", (TEXT_AIRMONITORCOORDINATESINFO));

  message.replace("{LATLONG_LINK}", (LATLONG_LINK));
  message.replace("{TEXT_HERE}", (TEXT_HERE));
  message.replace("{AIRMONITOR_GRAPH_ON}", _addBoolSelect("AIRMONITOR_GRAPH_ON", AIRMONITOR_GRAPH_ON));
  message.replace("{TEXT_AIRMONITORLATITUDE}", (TEXT_AIRMONITORLATITUDE));
Serial.println("CRASH - 4 - _addFloatInput");
  //message.replace("{LATITUDE}", _addFloatInput("LATITUDE", LATITUDE, 6, "°")); // CRASH!!! 
  message.replace("{TEXT_AIRMONITORLONGITUDE}", (TEXT_AIRMONITORLONGITUDE));
Serial.println("CRASH - 5 - _addFloatInput");
  //message.replace("{LONGITUDE}", _addFloatInput("LONGITUDE", LONGITUDE, 6, "°")); // CRASH!!!

  message.replace("{TEXT_THINGSPEAKSENDING}", (TEXT_THINGSPEAKSENDING));
Serial.println("CRASH - 6 - LINK");
  //message.replace("{THINGSPEAK_LINK}", (THINGSPEAK_LINK)); // CRASH!!!
  message.replace("{THINGSPEAK_ON}", _addBoolSelect("THINGSPEAK_ON", THINGSPEAK_ON));
  message.replace("{TEXT_THINGSPEAKCHARTS}", (TEXT_THINGSPEAKCHARTS));
  message.replace("{THINGSPEAK_GRAPH_ON}", _addBoolSelect("THINGSPEAK_GRAPH_ON", THINGSPEAK_GRAPH_ON));
  message.replace("{TEXT_THINGSPEAKAPIKEY}", (TEXT_THINGSPEAKAPIKEY));
  message.replace("{THINGSPEAK_API_KEY}", _addTextInput("THINGSPEAK_API_KEY", THINGSPEAK_API_KEY));
  message.replace("{TEXT_THINGSPEAKCHANNELID}", (TEXT_THINGSPEAKCHANNELID));
  message.replace("{THINGSPEAK_CHANNEL_ID}", _addIntInput("THINGSPEAK_CHANNEL_ID", THINGSPEAK_CHANNEL_ID));
  
  message.replace("{TEXT_INFLUXDBSENDING}", (TEXT_INFLUXDBSENDING));
  message.replace("{INFLUXDB_ON}", _addBoolSelect("INFLUXDB_ON", INFLUXDB_ON));
  message.replace("{TEXT_INFLUXDBSERVER}", (TEXT_INFLUXDBSERVER));
  message.replace("{INFLUXDB_HOST}", _addTextInput("INFLUXDB_HOST", INFLUXDB_HOST));
  message.replace("{TEXT_INFLUXDBPORT}", (TEXT_INFLUXDBPORT));
  message.replace("{INFLUXDB_PORT}", _addIntInput("INFLUXDB_PORT", INFLUXDB_PORT));
  message.replace("{TEXT_INFLUXDBNAME}", (TEXT_INFLUXDBNAME));
  message.replace("{INFLUXDB_DATABASE}", _addTextInput("INFLUXDB_DATABASE", INFLUXDB_DATABASE));
  message.replace("{TEXT_INFLUXDBUSER}", (TEXT_INFLUXDBUSER));
  message.replace("{DB_USER}", _addTextInput("DB_USER", DB_USER));
  message.replace("{TEXT_INFLUXDBPASSWD}", (TEXT_INFLUXDBPASSWD));
Serial.println("CRASH - 7 - _addPasswdInput");
  //message.replace("{DB_PASSWORD}", _addPasswdInput("DB_PASSWORD", DB_PASSWORD)); // CRASH!!!

  message.replace("{TEXT_MQTTSENDING}", (TEXT_MQTTSENDING));
  message.replace("{MQTT_ON}", _addBoolSelect("MQTT_ON", MQTT_ON));
  message.replace("{TEXT_MQTTSERVER}", (TEXT_MQTTSERVER));
  message.replace("{MQTT_HOST}", _addTextInput("MQTT_HOST", MQTT_HOST));
  message.replace("{TEXT_MQTTPORT}", (TEXT_MQTTPORT));
  message.replace("{MQTT_PORT}", _addIntInput("MQTT_PORT", MQTT_PORT));
  message.replace("{TEXT_MQTTUSER}", (TEXT_MQTTUSER));
  message.replace("{MQTT_USER}", _addTextInput("MQTT_USER", MQTT_USER));
  message.replace("{TEXT_MQTTPASSWD}", (TEXT_MQTTPASSWD));
Serial.println("CRASH - 8 - _addPasswdInput");
  //message.replace("{MQTT_PASSWORD}", _addPasswdInput("MQTT_PASSWORD", MQTT_PASSWORD)); // CRASH!!!

  message.replace("{DEBUG}", _addBoolSelect("DEBUG", DEBUG));
  message.replace("{TEXT_CALIBMETHOD}", (TEXT_CALIBMETHOD));
  message.replace("{CalibrationModelSelect}", _addModelSelect("MODEL", MODEL));
  message.replace("{TEXT_CALIB1}", (TEXT_CALIB1));
  message.replace("{calib1}", String(calib1));
  message.replace("{TEXT_CALIB2}", (TEXT_CALIB2));
  message.replace("{calib2}", String(calib2));

  message.replace("{TEXT_SOFTWATEVERSION}", (TEXT_SOFTWATEVERSION));
  message.replace("{SOFTWAREVERSION}", SOFTWAREVERSION);

  message.replace("{TEXT_AUTOUPDATEON}", TEXT_AUTOUPDATEON);
  message.replace("{AUTOUPDATEON}", _addBoolSelect("AUTOUPDATE_ON", AUTOUPDATE_ON));
  
#ifdef ARDUINO_ARCH_ESP8266
  message.replace("{TEXT_UPDATEPAGEAUTOUPDATEWARNING}", TEXT_UPDATEPAGEAUTOUPDATEWARNING);
#elif defined ARDUINO_ARCH_ESP32
  message.replace("{TEXT_UPDATEPAGEAUTOUPDATEWARNING}<br>", "");
#endif
  
  message.replace("{WiFiEraseButton}", _addWiFiErase());
  message.replace("{RestoreConfigButton}", _addRestoreConfig());
  message.replace("{SubmitButton}", _addSubmit());
  message += FPSTR(WEB_PAGE_FOOTER);

  WebServer.send(200, "text/html", message);
>>>>>>> Stashed changes
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
		if (!strcmp(THP_MODEL, "BME280")) {
			if (checkBmeStatus() == true) {
        		calib1 = float((100-(BMESensor.humidity)+100)/150);
			}}
		if (!strcmp(THP_MODEL, "HTU21D")) {
			if (checkHTU21DStatus() == true) {
				calib1 = float((100-(myHTU21D.readCompensatedHumidity())+100)/150);
			}}
		calib1 = calib1;
        calib2 = calib1/2;
    } else {
        calib1 = 1.0;
        calib2 = 1.0;
    }
}

void _set_language() {
    if (!strcmp(LANGUAGE, "english")) {
        SELECTED_LANGUAGE = 1;
    } else if (!strcmp(LANGUAGE, "polish")) {
        SELECTED_LANGUAGE = 2;
    } else {
        SELECTED_LANGUAGE = 3;
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
	_parseAsCString(THP_MODEL, WebServer.arg("THP_MODEL"));
	_parseAsCString(DUST_MODEL, WebServer.arg("DUST_MODEL"));
	DUST_TIME = WebServer.arg("DUST_TIME").toInt();
	NUMBEROFMEASUREMENTS = WebServer.arg("NUMBEROFMEASUREMENTS").toInt();
	//ADVANCED_OPTIONS_ON = _parseAsBool(WebServer.arg("ADVANCED_OPTIONS_ON"));
	AUTOUPDATE_ON = _parseAsBool(WebServer.arg("AUTOUPDATE_ON"));
	
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
	message += "<!DOCTYPE html>";
	if(SELECTED_LANGUAGE == 1){
		message += "<html lang='en'>";
	} else if(SELECTED_LANGUAGE == 2){
		message += "<html lang='pl'>";
	} else {
		message += "<html lang='en'>";
	}
	message += "<head><title>Smogomierz</title> ";
	message += "<meta name='viewport' content='width=device-width, initial-scale=1' />";
	message += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />";
	if(SELECTED_LANGUAGE == 1){
		message += "<meta name='keywords' content='Smog, Air Quality, Air Dust' />";
	} else if(SELECTED_LANGUAGE == 2){
		message += "<meta name='keywords' content='Smog, Zanieczyszczenia powietrza, Pył Zawieszony, Smogomierz, Pomiar Zanieczyszczeń' />";
	} else {
		message += "<meta name='keywords' content='Smog, Air Quality, Air Dust' />";
	}
	message += "<script type='application/x-javascript'> addEventListener('load', function() { setTimeout(hideURLbar, 0); }, false); function hideURLbar(){ window.scrollTo(0,1); } </script>";
	message += "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css' integrity='sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO' crossorigin='anonymous'>";
	message += "<link rel='stylesheet' type='text/css' href='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/css/style11.css' />";
	message += "<link href='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/css/style.css' type='text/css' rel='stylesheet' media='all'> ";
	message += "<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.4.2/css/all.css' integrity='sha384-/rXc/GQVaYpyDdyxK+ecHPVYJSN9bmVFBvjA/9eOB+pb3F2w2N6fc5qB9Ew5yIns' crossorigin='anonymous'> ";
	message += "<link href='https://fonts.googleapis.com/css?family=Poppins:300,400,500,600,700' rel='stylesheet'>";
	message += "<link href='https://fonts.googleapis.com/css?family=Roboto:400,100,100italic,300,300italic,400italic,500,500italic,700,700italic,900,900italic' rel='stylesheet' type='text/css'></head>";
	message += "<body class='bg'><div class='overlay overlay-simplegenie'><button type='button' class='overlay-close'><i class='fas fa-times' ></i></button><nav><ul>";
	message += "<li><i class='fas fa-clock'></i><a href='/'>Pomiary</a></li>";
	message += "<li><i class='fas fa-cog'></i><a href='/config'>Ustawienia</a></li>";
	message += "<li><i class='fas fa-arrow-circle-up'></i><a href='/update'>Aktualizacje</a></li>";
	message += "<li><i class='fas fa-question-circle'></i><a href='/help'>Pomoc</a></li>";
	message += "<li><i class='fas fa-info-circle'></i><a href='/contact'>Kontakt</a></li>";
	message += "</ul></nav></div>";
	message += "<section class='header-w3ls'><button id='trigger-overlay' type='button'><i class='fas fa-bars' ></i></button><div class='bottons-agileits-w3layouts'>";
	message += "<!-- <a class='page-scroll' href='#myModal2' data-toggle='modal'><i class='fas fa-sign-in-alt' ></i>Login</a> -->";
	message += "<!-- <a class='page-scroll' href='#myModal3' data-toggle='modal'><i class='fas fa-pencil-alt' ></i>Register</a> -->";
	message += "</div>";
	message += "<h1><a href='/'>Smogomierz</a></h1><div class='clearfix'></div></section>";
	/*
	message += "	<div class='inner-banner-agileits-w3layouts'>"; // Zmienic na grafike zwiazana z HSem! 
	message += "	</div>";
	*/
	message += "<!-- locations -->";
	message += "	<div class='contact w3-agileits'>";
	message += "	<div class='container'>";
	message += "	<h4 class='tittle-w3layouts'>Aktualizacje<h4>";
	message += "	</div>";
	message += "<div class='locations-w3-agileits'>";
	message += "	<div class='container'>";
	message += "	<div class='location-agileits'>";
	message += "		<div class='loc-left'>";
	message += "			<h4>Ręczna aktualizacja</h4>";
	message += "			<p>Aktualna wersja oprogramowania: <b></p>";
	message += SOFTWAREVERSION;
	message += "			<p></b></p>";
	message += "			<p> </p>";
    message += "<p>Najnowszą wersję oprogramowania znajdziesz zawsze <b><a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/releases'>tutaj</a></b>.</p>";
	message += "		</div>";
	message += "		<div class='loc-right'>";
	
    message += "<form id='data' action='/update' method='POST' enctype='multipart/form-data'>";
    message += "<br><br><div class='input-group mb-3'><div class='custom-file'><input type='file' accept='.bin' class='custom-file-input' id='inputGroupFile04' name='update'><label class='custom-file-label' for='inputGroupFile04'>Wybierz plik .bin</label></div><div class='input-group-append'><button class='btn btn-danger' type='submit'>Update!</button></div></div>";
    message += "</form>";
	
	message += "		</div>";
	message += "		<div class='clearfix'> </div>";
	message += "	</div>";
	message += "	<div class='location-agileits'>";
	message += "		<div class='loc-right'>";
	message += "		<br><br><button type='button' class='btn btn-danger btn-lg btn-block'>Wyszukaj aktualizacje</button>";
	message += "		</div>";
	message += "		<div class='loc-left'>";
	message += "			<h4>Automatyczna aktualizacja</h4>";
	message += "			<p>Aktualna wersja oprogramowania: <b></p>";
	message += SOFTWAREVERSION;
	message += "			<p></b></p>";
	message += "			<p> </p>";
	message += "			<p> Smogomierz po każdym restarcie i co 24 godziny, sprawdza dostępnośc nowych, stabilnych wersji oprogramowania. </p>";
	message += "			<p> Po znalezieniu nowej wersji, automatycznie ją pobierze i zanstaluje na mierniku. </p>";
	message += "			<p> Automatyczne aktualizacje można wyłączyć lub właczyć w <a href='config'>Ustawieniach</a>. </p>";
	message += "		</div>";
	message += "		<div class='clearfix'> </div>";
	message += "	</div>";
	message += "		<div class='clearfix'> </div>";
	message += "	</div>";
	message += "	</div>";
	message += "</div>";
	message += "<!-- /locations --> ";
	
	message += "<br></br>";
	message += "<div class='clearfix'></div></div><div class='clearfix'></div>";
	message += "<p></p><p></p>";
	message += "<div class='clearfix'></div></div><div class='clearfix'></div>";
	message += "<footer class='fixed-bottom'><div class='agileinfo_footer_bottom1'><div class='container'>";
	message += "<p>Hackerspace Silesia &#9830; 2018</p>";
	message += "<div class='clearfix'> </div></div></div></footer>";
	message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script>";
	message += "<script src='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/js/scripts.js'></script>";
	message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/ResponsiveSlides.js/1.55/responsiveslides.js'></script>";
	message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/modernizr/2.8.3/modernizr.min.js'></script>";
	message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/classie/1.0.1/classie.js'></script>";
	message += "<script src='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/js/demo1.js'></script>";
	message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.4.1/jquery.easing.compatibility.js'></script> ";
	message += "<script type='text/javascript'>";
	message += "jQuery(document).ready(function($) {";
	message += "$('.scroll').click(function(event){		";
	message += "event.preventDefault();";
	message += "$('html,body').animate({scrollTop:$(this.hash).offset().top},1000);";
	message += "});";
	message += "});";
	message += "</script>";
	message += "<script src='https://code.jquery.com/jquery-3.3.1.slim.min.js' integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo' crossorigin='anonymous'></script>";
	message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.3/umd/popper.min.js' integrity='sha384-ZMP7rVo3mIykV+2+9J3UJ46jBk0WLaUAdn689aCwoqbBJiSnjAK/l8WvCWPIPm49' crossorigin='anonymous'></script>";
	message += "<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js' integrity='sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy' crossorigin='anonymous'></script>";
	message += "</body></html>";
    WebServer.send(200, "text/html", message);
	
	/*
	String message;
	if(SELECTED_LANGUAGE == 1){
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
	} else if(SELECTED_LANGUAGE == 2){
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
    WebServer.send(200, "text/html", message);*/
  }
<<<<<<< Updated upstream
	
	void handle_contact() {
		String message;
		message += "<!DOCTYPE html>";
		if(SELECTED_LANGUAGE == 1){
			message += "<html lang='en'>";
		} else if(SELECTED_LANGUAGE == 2){
			message += "<html lang='pl'>";
		} else {
			message += "<html lang='en'>";
		}
		message += "<head><title>Smogomierz</title> ";
		message += "<meta name='viewport' content='width=device-width, initial-scale=1' />";
		message += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />";
		if(SELECTED_LANGUAGE == 1){
			message += "<meta name='keywords' content='Smog, Air Quality, Air Dust' />";
		} else if(SELECTED_LANGUAGE == 2){
			message += "<meta name='keywords' content='Smog, Zanieczyszczenia powietrza, Pył Zawieszony, Smogomierz, Pomiar Zanieczyszczeń' />";
		} else {
			message += "<meta name='keywords' content='Smog, Air Quality, Air Dust' />";
		}
		message += "<script type='application/x-javascript'> addEventListener('load', function() { setTimeout(hideURLbar, 0); }, false); function hideURLbar(){ window.scrollTo(0,1); } </script>";
		message += "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css' integrity='sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO' crossorigin='anonymous'>";
		message += "<link rel='stylesheet' type='text/css' href='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/css/style11.css' />";
		message += "<link href='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/css/style.css' type='text/css' rel='stylesheet' media='all'> ";
		message += "<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.4.2/css/all.css' integrity='sha384-/rXc/GQVaYpyDdyxK+ecHPVYJSN9bmVFBvjA/9eOB+pb3F2w2N6fc5qB9Ew5yIns' crossorigin='anonymous'> ";
		message += "<link href='https://fonts.googleapis.com/css?family=Poppins:300,400,500,600,700' rel='stylesheet'>";
		message += "<link href='https://fonts.googleapis.com/css?family=Roboto:400,100,100italic,300,300italic,400italic,500,500italic,700,700italic,900,900italic' rel='stylesheet' type='text/css'></head>";
		message += "<body class='bg'><div class='overlay overlay-simplegenie'><button type='button' class='overlay-close'><i class='fas fa-times' ></i></button><nav><ul>";
		message += "<li><i class='fas fa-clock'></i><a href='/'>Pomiary</a></li>";
		message += "<li><i class='fas fa-cog'></i><a href='/config'>Ustawienia</a></li>";
		message += "<li><i class='fas fa-arrow-circle-up'></i><a href='/update'>Aktualizacje</a></li>";
		message += "<li><i class='fas fa-question-circle'></i><a href='/help'>Pomoc</a></li>";
		message += "<li><i class='fas fa-info-circle'></i><a href='/contact'>Kontakt</a></li>";
		message += "</ul></nav></div>";
		message += "<section class='header-w3ls'><button id='trigger-overlay' type='button'><i class='fas fa-bars' ></i></button><div class='bottons-agileits-w3layouts'>";
		message += "<!-- <a class='page-scroll' href='#myModal2' data-toggle='modal'><i class='fas fa-sign-in-alt' ></i>Login</a> -->";
		message += "<!-- <a class='page-scroll' href='#myModal3' data-toggle='modal'><i class='fas fa-pencil-alt' ></i>Register</a> -->";
		message += "</div>";
		message += "<h1><a href='/'>Smogomierz</a></h1><div class='clearfix'></div></section>";
		/*
		message += "	<div class='inner-banner-agileits-w3layouts'>"; // Zmienic na grafike zwiazana z HSem! 
		message += "	</div>";
		*/
		message += "	<!-- contact -->";
		message += "		<div class='contact w3-agileits'>";
		message += "			<div class='container'>";
		message += "				<h4 class='tittle-w3layouts'>Hackerspace Silesia</h4>";
		message += "				<div class='contact-w3lsrow'>";
		message += "					<div class='contact-right'>";
		message += "						<H4>Nagłówek</h4><br>";
		message += "						<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent ac lorem porta, lobortis ipsum eget, feugiat neque. Praesent nec nulla at mauris posuere facilisis quis bibendum sem. Cras a bibendum ipsum, quis mattis neque. Proin eget erat elit. Praesent facilisis massa id lectus hendrerit, ac malesuada est ultrices. Curabitur nec nibh ut nisi bibendum gravida ut a massa. Nam faucibus eros sit amet malesuada porta. Suspendisse quis imperdiet felis. Vestibulum vitae magna in felis feugiat faucibus. Quisque dictum magna fermentum, scelerisque magna vitae, malesuada ligula. Curabitur posuere at elit in elementum.</p>";
		message += "					</div>";
		message += "					<div class='contact-left wthree'>";
		message += "						<iframe src='https://www.openstreetmap.org/export/embed.html?bbox=18.989964723587036%2C50.26337315454736%2C18.998183012008667%2C50.26771439397775&amp;layer=mapnik&amp;marker=50.26385,18.99572' allowfullscreen></iframe>";
		message += "						<div class='timings-w3ls'>";
		message += "							<h5>Odwiedź nas!</h5>";
		message += "							<ul>";
		message += "								<li>Każdy Czwartek <span> 18:00-20:00 </span></li>";
		message += "								<li>Lista spotkań <span> <a target='_blank' href='https://wiki.hs-silesia.pl/wiki/Planowane_spotkania' style='color:#C31A14'>Sprawdź</a> </span></li>";
		message += "							</ul>";
		message += "						</div>";
		message += "					</div>";
		message += "					<div class='clearfix'> </div>";
		message += "				</div>";
		message += "			</div>";
		message += "			<!-- Address -->  ";
		message += "	<div class='container'>";
		message += "				<div class='w3agile_footer_grid'>";
		message += "					<div class='w3agile_footer_grid_left'>";
		message += "						<div class='w3agile_footer_grid_left1'>";
		message += "							<span class='fas fa-map-marked-alt' aria-hidden='true'></span>";
		message += "						</div>";
		message += "						<a target='_blank' href='https://www.google.com/maps/place/Hackerspace+Silesia/@50.264104,18.9936806,17z/data=!3m1!4b1!4m5!3m4!1s0x4716ce1320bf55f9:0xb1ae316b317ecc2f!8m2!3d50.2641006!4d18.9958693'><h4>Ondraszka 17, 40-857 Katowice</h4></a>";
		message += "					</div>";
		message += "					<div class='w3agile_footer_grid_left'>";
		message += "						<div class='w3agile_footer_grid_left1'>";
		message += "							<span class='fas fa-envelope' aria-hidden='true'></span>";
		message += "						</div>";
		message += "						<a href='mailto:info@hs-silesia.pl'><h4>info@hs-silesia.pl</h4></a>";
		message += "					</div>";
		message += "					<div class='w3agile_footer_grid_left'>";
		message += "						<div class='w3agile_footer_grid_left1'>";
		message += "							<span class='fab fa-facebook-f' aria-hidden='true'></span>";
		message += "						</div>";
		message += "						<a target='_blank' href='https://www.facebook.com/HackerspaceSilesia/'><h4>fb.com/HackerspaceSilesia</h4></a>";
		message += "					</div>";
		message += "					<div class='clearfix'> </div>";
		message += "				</div>";
		message += "			</div>";
		message += "		<!-- /Address --> ";
		message += "		</div>";
		message += "		<!-- /contact --> ";
		
		message += "<br></br>";
		message += "<div class='clearfix'></div></div><div class='clearfix'></div>";
		message += "<p></p><p></p>";
		message += "<div class='clearfix'></div></div><div class='clearfix'></div>";
		message += "<footer class='fixed-bottom'><div class='agileinfo_footer_bottom1'><div class='container'>";
		message += "<p>Hackerspace Silesia &#9830; 2018</p>";
		message += "<div class='clearfix'> </div></div></div></footer>";
		message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script>";
		message += "<script src='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/js/scripts.js'></script>";
		message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/ResponsiveSlides.js/1.55/responsiveslides.js'></script>";
		message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/modernizr/2.8.3/modernizr.min.js'></script>";
		message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/classie/1.0.1/classie.js'></script>";
		message += "<script src='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/js/demo1.js'></script>";
		message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.4.1/jquery.easing.compatibility.js'></script> ";
		message += "<script type='text/javascript'>";
		message += "jQuery(document).ready(function($) {";
		message += "$('.scroll').click(function(event){		";
		message += "event.preventDefault();";
		message += "$('html,body').animate({scrollTop:$(this.hash).offset().top},1000);";
		message += "});";
		message += "});";
		message += "</script>";
		message += "<script src='https://code.jquery.com/jquery-3.3.1.slim.min.js' integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo' crossorigin='anonymous'></script>";
		message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.3/umd/popper.min.js' integrity='sha384-ZMP7rVo3mIykV+2+9J3UJ46jBk0WLaUAdn689aCwoqbBJiSnjAK/l8WvCWPIPm49' crossorigin='anonymous'></script>";
		message += "<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js' integrity='sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy' crossorigin='anonymous'></script>";
		message += "</body></html>";
	    WebServer.send(200, "text/html", message);
	  }

	void handle_help() {
		String message;
		message += "<!DOCTYPE html>";
		if(SELECTED_LANGUAGE == 1){
			message += "<html lang='en'>";
		} else if(SELECTED_LANGUAGE == 2){
			message += "<html lang='pl'>";
		} else {
			message += "<html lang='en'>";
		}
		message += "<head><title>Smogomierz</title> ";
		message += "<meta name='viewport' content='width=device-width, initial-scale=1' />";
		message += "<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />";
		if(SELECTED_LANGUAGE == 1){
			message += "<meta name='keywords' content='Smog, Air Quality, Air Dust' />";
		} else if(SELECTED_LANGUAGE == 2){
			message += "<meta name='keywords' content='Smog, Zanieczyszczenia powietrza, Pył Zawieszony, Smogomierz, Pomiar Zanieczyszczeń' />";
		} else {
			message += "<meta name='keywords' content='Smog, Air Quality, Air Dust' />";
		}
		message += "<script type='application/x-javascript'> addEventListener('load', function() { setTimeout(hideURLbar, 0); }, false); function hideURLbar(){ window.scrollTo(0,1); } </script>";
		message += "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css' integrity='sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO' crossorigin='anonymous'>";
		message += "<link rel='stylesheet' type='text/css' href='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/css/style11.css' />";
		message += "<link href='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/css/style.css' type='text/css' rel='stylesheet' media='all'> ";
		message += "<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.4.2/css/all.css' integrity='sha384-/rXc/GQVaYpyDdyxK+ecHPVYJSN9bmVFBvjA/9eOB+pb3F2w2N6fc5qB9Ew5yIns' crossorigin='anonymous'> ";
		message += "<link href='https://fonts.googleapis.com/css?family=Poppins:300,400,500,600,700' rel='stylesheet'>";
		message += "<link href='https://fonts.googleapis.com/css?family=Roboto:400,100,100italic,300,300italic,400italic,500,500italic,700,700italic,900,900italic' rel='stylesheet' type='text/css'></head>";
		message += "<body class='bg'><div class='overlay overlay-simplegenie'><button type='button' class='overlay-close'><i class='fas fa-times' ></i></button><nav><ul>";
		message += "<li><i class='fas fa-clock'></i><a href='/'>Pomiary</a></li>";
		message += "<li><i class='fas fa-cog'></i><a href='/config'>Ustawienia</a></li>";
		message += "<li><i class='fas fa-arrow-circle-up'></i><a href='/update'>Aktualizacje</a></li>";
		message += "<li><i class='fas fa-question-circle'></i><a href='/help'>Pomoc</a></li>";
		message += "<li><i class='fas fa-info-circle'></i><a href='/contact'>Kontakt</a></li>";
		message += "</ul></nav></div>";
		message += "<section class='header-w3ls'><button id='trigger-overlay' type='button'><i class='fas fa-bars' ></i></button><div class='bottons-agileits-w3layouts'>";
		message += "<!-- <a class='page-scroll' href='#myModal2' data-toggle='modal'><i class='fas fa-sign-in-alt' ></i>Login</a> -->";
		message += "<!-- <a class='page-scroll' href='#myModal3' data-toggle='modal'><i class='fas fa-pencil-alt' ></i>Register</a> -->";
		message += "</div>";
		message += "<h1><a href='/'>Smogomierz</a></h1><div class='clearfix'></div></section>";
		/*
		message += "<!-- banner -->";
		message += "<div class='inner-banner-agileits-w3layouts'>"; // Zmienic na grafike zwiazana z HSem! 
		message += "</div>";
		message += "<!-- /banner -->";
		*/
		message += "<!-- services -->";
		message += "<div class='services-w3-agileits'>";
		message += "	<div class='container'>";
		message += "	<h4 class='tittle-w3layouts'>Pomoc<h4>";
		message += "			<div class='services-grids'>";
		message += "			<a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz'>";
		message += "				<i class='fab fa-github' aria-hidden='true'></i>";
		message += "				<h4>Github</h4></a>";
		message += "				<p>Repozytorium z kodem Smogomierza.</p>";
		message += "			</a>";
		message += "			</div>";
		message += "		<div class='services-grids'>";
		message += "			<a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/software.md'>";
		message += "			<i class='fas fa-code' aria-hidden='true'></i>";
		message += "			<h4>Oprogramowanie</h4>";
		message += "			<p>Opis instalacji wymaganego oprogramowania.</p>";
		message += "			</a>";
		message += "		</div>";
		message += "		<div class='services-grids'>";
		message += "			<a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/components.md'>";
		message += "			<i class='fas fa-list-ol' aria-hidden='true'></i>";
		message += "			<h4>Lista cześci</h4>";
		message += "			<p>Spis części potrzebnych do budowy.</p>";
		message += "			</a>";
		message += "		</div>";
		message += "		<div class='services-grids'>";
		message += "			<a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/hardware.md'>";
		message += "			<i class='fas fa-wrench' aria-hidden='true'></i>";
		message += "			<h4>Sprzęt</h4>";
		message += "			<p>Opis budowy Smogomierza.</p>";
		message += "			</a>";
		message += "		</div>";
		message += "		<div class='services-grids'>";
		message += "			<a target='_blank' href='https://github.com/hackerspace-silesia/Smogomierz/blob/master/instrukcje/thingspeak.md'>";
		message += "			<i class='far fa-comment-alt' aria-hidden='true'></i>";
		message += "			<h4>ThingSpeak</h4>";
		message += "			<p>Konfiguracja serwisu ThingSpeak.</p>";
		message += "			</a>";
		message += "		</div>";
		message += "		<div class='services-grids'>";
		message += "			<a href='#'>";
		message += "			<i class='fas fa-ambulance' aria-hidden='true'></i>";
		message += "			<h4>Coś jeszcze?</h4>";
		message += "			<p>Tylko nie wiem jeszcze co.</p>";
		message += "			</a>";
		message += "		</div>";
		message += "		<div class='clearfix'> </div>";
		message += "	</div>";
		message += "</div>";
		message += "<!-- /services -->";
		
		message += "<br></br>";
		message += "<div class='clearfix'></div></div><div class='clearfix'></div>";
		message += "<p></p><p></p>";
		message += "<div class='clearfix'></div></div><div class='clearfix'></div>";
		message += "<footer class='fixed-bottom'><div class='agileinfo_footer_bottom1'><div class='container'>";
		message += "<p>Hackerspace Silesia &#9830; 2018</p>";
		message += "<div class='clearfix'> </div></div></div></footer>";
		message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script>";
		message += "<script src='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/js/scripts.js'></script>";
		message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/ResponsiveSlides.js/1.55/responsiveslides.js'></script>";
		message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/modernizr/2.8.3/modernizr.min.js'></script>";
		message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/classie/1.0.1/classie.js'></script>";
		message += "<script src='https://p.w3layouts.com/demos_new/07-03-2017/clinical_care-demo_Free/264767743/web/js/demo1.js'></script>";
		message += "<script type='text/javascript' src='https://cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.4.1/jquery.easing.compatibility.js'></script> ";
		message += "<script type='text/javascript'>";
		message += "jQuery(document).ready(function($) {";
		message += "$('.scroll').click(function(event){		";
		message += "event.preventDefault();";
		message += "$('html,body').animate({scrollTop:$(this.hash).offset().top},1000);";
		message += "});";
		message += "});";
		message += "</script>";
		message += "<script src='https://code.jquery.com/jquery-3.3.1.slim.min.js' integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo' crossorigin='anonymous'></script>";
		message += "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.3/umd/popper.min.js' integrity='sha384-ZMP7rVo3mIykV+2+9J3UJ46jBk0WLaUAdn689aCwoqbBJiSnjAK/l8WvCWPIPm49' crossorigin='anonymous'></script>";
		message += "<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js' integrity='sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy' crossorigin='anonymous'></script>";
		message += "</body></html>";
	    WebServer.send(200, "text/html", message);
	  }
=======
  String message = FPSTR(WEB_PAGE_HEADER);
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
  WebServer.send(200, "text/html", message);
}

void erase_wifi() {
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
  }
  Serial.println("Erasing Config...");
#ifdef ARDUINO_ARCH_ESP8266
  ESP.eraseConfig();
#elif defined ARDUINO_ARCH_ESP32
  WiFi.disconnect(false, true);
#endif
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  delay(1000);
  Serial.println("Restart");
  ESP.restart();
}

void restore_config() {
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
  }
  Serial.println("Restoring default settings...");
  deleteConfig();
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  delay(1000);
  Serial.println("Restart");
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

void autoupdateon() {
  if (CONFIG_AUTH == true) {
    if (!WebServer.authenticate(CONFIG_USERNAME, CONFIG_PASSWORD)) {
      //return WebServer.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      return WebServer.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
  }
  AUTOUPDATE_ON = true;
  saveConfig();
  delay(300);
  WebServer.sendHeader("Location", "/", true);
  WebServer.send ( 302, "text/plain", "");
  delay(1000);
  Serial.println("Restart");
  ESP.restart();
}
>>>>>>> Stashed changes

void handle_api() {
    String message;
    //StaticJsonBuffer<800> jsonBuffer;
      //JsonObject& json = jsonBuffer.createObject();
	StaticJsonDocument<800> jsonBuffer;
	JsonObject json = jsonBuffer.to<JsonObject>();

        json["device_name"] = device_name;
		if (strcmp(DUST_MODEL, "Non") != 0){
        	json["pm1"] = averagePM1;
        	json["pm25"] = averagePM25;
        	json["pm10"] = averagePM10;
		}
		if (!strcmp(THP_MODEL, "BME280")) {
	        if (checkBmeStatus()) {
				json["temperature"] = float(BMESensor.temperature);
				json["pressure"] = int(BMESensor.seaLevelForAltitude(MYALTITUDE));
				json["humidity"] = int(BMESensor.humidity);
				json["dewpoint"] = float(BMESensor.temperature-((100-BMESensor.humidity)/5));  
	        }
		}
       	if (!strcmp(THP_MODEL, "BMP280")) {
			if (checkBmpStatus()) {
				json["temperature"] = float(bmp.readTemperature());
  		  		json["pressure"] = int((bmp.readPressure())/100);
			}
		}
		if (!strcmp(THP_MODEL, "HTU21")) {
			if (checkHTU21DStatus()) {
				json["temperature"] = float(myHTU21D.readTemperature());
  		  		json["humidity"] = int(myHTU21D.readCompensatedHumidity());
		  		json["dewpoint"] = float(myHTU21D.readTemperature()-((100-myHTU21D.readCompensatedHumidity())/5));
			}
		}
		serializeJsonPretty(json, message);
      //json.prettyPrintTo(message);
      WebServer.send(200, "text/json", message);
  }