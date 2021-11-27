// CONFIG SERVICES PAGE HTML
/*
% -> &#37;
*/
static const char WEB_CONFIG_ADV_MQTT_PAGE_ALL[] PROGMEM = R"rawliteral(<html lang='%{Language}%'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogly - %{CurrentPageTitle}%</title>%{WEB_PAGE_CSS}%</head><body>
	<nav class='navbar navbar-expand-lg navbar-dark bg-dark' style='padding-left:15&#37;'>
	%{SMOGLY_LOGO_URL}%
	  <button class='navbar-toggler' type='button' data-toggle='collapse' data-target='#navbarColor02' aria-controls='navbarColor02' aria-expanded='false' aria-label='Toggle navigation'>
	  <span class='navbar-toggler-icon'></span>
	  </button>
	  <div class='collapse navbar-collapse' id='navbarColor02' style='padding-left:2rem'>
	    <ul class='navbar-nav mr-auto'>
	      <li class='nav-item'>
	        <a class='nav-link' href='/'>%{IndexPageTitle}%</a>
	      </li>
	      <li class='nav-item'>
	        <a class='nav-link' href='/config'>%{ConfigPageTitle}%</a>
	      </li>
		  <li class='nav-item'>
		  	<a class='nav-link' href='/update'>%{UpdatePageTitle}%</a>
		  </li>
	    </ul>
</div></nav><form method='GET' action='/config_adv_mqtt_save'>
					<main role='main' class='container'><div class='jumbotron'>
					<center><h1>Smogly - %{TEXT_ADVANCED_MQTT_PAGE}%</h1></center><br>%{TEXT_INSTRUCIONSLINK}%<br><br>
						        <b>%{TEXT_MQTTSENDING}%: </b>%{MQTT_ON}%
								<b>%{TEXT_MQTTSERVER}%: </b>%{MQTT_HOST}%
								<b>%{TEXT_MQTTPORT}%: </b>%{MQTT_PORT}%
								<b>%{TEXT_MQTTUSER}%: </b>%{MQTT_USER}%
								<b>%{TEXT_MQTTPASSWD}%: </b>%{MQTT_PASSWORD}%
								<br>%{TEXT_MQTT_TOPIC_INFO}%
								<br><br>
								<b>%{TEXT_MQTT_IP_IN_TOPIC}%: </b>%{MQTT_IP_IN_TOPIC}%
								<b>%{TEXT_MQTT_DEVICENAME_IN_TOPIC}%: </b>%{MQTT_DEVICENAME_IN_TOPIC}%
								<b>%{TEXT_MQTT_SLASH_AT_THE_BEGINNING}%: </b>%{MQTT_SLASH_AT_THE_BEGINNING}%
								<b>%{TEXT_MQTT_SLASH_AT_THE_END}%: </b>%{MQTT_SLASH_AT_THE_END}%
								<br>%{WEB_CONFIG_ADV_MQTT_PAGE_CONFIG}%<br>
								<hr><center><br>
								%{RestoreConfigButton}%
								<br><br></center><hr><br><center>
								%{SubmitButton}%
										</form></center>
<br><hr><center><a href='http://smogly.org/' target='_blank'>Smogly</a> &#9830; <a href='https://hs-silesia.pl' target='_blank'>Hackerspace Silesia</a> &#9830;
	<script type='text/javascript'>
  		document.write(new Date().getFullYear());
	</script>
	</center></div></main></form>
			<script src='https://code.jquery.com/jquery-3.4.1.slim.min.js' integrity='sha384-J6qa4849blE2+poT4WnyKhv5vZF5SrPo0iEjwBvKU7imGFAV0wwj1yYfoRSJoZ+n' crossorigin='anonymous'></script>
			<script src='https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js' integrity='sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo' crossorigin='anonymous'></script>
			<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js' integrity='sha384-wfSDF2E50Y2D1uUdj0O3uMBJnjuUD4Ih7YwaYd1iqfktj0Uod8GCExl3Og8ifwB6' crossorigin='anonymous'></script>
			</body></html>)rawliteral";


static const char WEB_CONFIG_ADV_MQTT_PAGE_CONFIG[] PROGMEM = R"rawliteral(<b>%{TEXT_TEMP_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_TEMP}%/%{MQTT_TEMP}%<br />
											<b>%{TEXT_HUMI_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_HUMI}%/%{MQTT_HUMI}%<br />
											<b>%{TEXT_PRESS_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PRESS}%/%{MQTT_PRESS}%<br />
											<b>%{TEXT_PM1_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM1}%/%{MQTT_PM1}%<br />
											<b>%{TEXT_PM25_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM25}%/%{MQTT_PM25}%<br />
											<b>%{TEXT_PM10_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM10}%/%{MQTT_PM10}%<br />
											<b>%{TEXT_AIRQUALITY_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_AIRQUALITY}%/%{MQTT_AIRQUALITY}%<br />)rawliteral";
				
static const char WEB_CONFIG_ADV_MQTT_PAGE_CONFIG1[] PROGMEM = R"rawliteral(<b>%{TEXT_TEMP_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_TEMP}%/%{MQTT_TEMP}%<br />
								<b>%{TEXT_HUMI_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_HUMI}%/%{MQTT_HUMI}%<br />
								<b>%{TEXT_PRESS_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PRESS}%/%{MQTT_PRESS}%<br />
								<b>%{TEXT_PM1_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM1}%/%{MQTT_PM1}%<br />
								<b>%{TEXT_PM25_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM25}%/%{MQTT_PM25}%<br />
								<b>%{TEXT_PM10_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM10}%/%{MQTT_PM10}%<br />
								<b>%{TEXT_AIRQUALITY_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_AIRQUALITY}%/%{MQTT_AIRQUALITY}%<br />)rawliteral";
	
static const char WEB_CONFIG_ADV_MQTT_PAGE_CONFIG2[] PROGMEM = R"rawliteral(<b>%{TEXT_TEMP_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_TEMP}%=%{MQTT_TEMP}%<br />
								<b>%{TEXT_HUMI_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_HUMI}%=%{MQTT_HUMI}%<br />
								<b>%{TEXT_PRESS_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PRESS}%=%{MQTT_PRESS}%<br />
								<b>%{TEXT_PM1_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM1}%=%{MQTT_PM1}%<br />
								<b>%{TEXT_PM25_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM25}%=%{MQTT_PM25}%<br />
								<b>%{TEXT_PM10_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM10}%=%{MQTT_PM10}%<br />
								<b>%{TEXT_AIRQUALITY_TOPIC}%: </b>/%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_AIRQUALITY}%=%{MQTT_AIRQUALITY}%<br />)rawliteral";

static const char WEB_CONFIG_ADV_MQTT_PAGE_CONFIG3[] PROGMEM = R"rawliteral(<b>%{TEXT_TEMP_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_TEMP}%=%{MQTT_TEMP}%<br />
								<b>%{TEXT_HUMI_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_HUMI}%=%{MQTT_HUMI}%<br />
								<b>%{TEXT_PRESS_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PRESS}%=%{MQTT_PRESS}%<br />
								<b>%{TEXT_PM1_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM1}%=%{MQTT_PM1}%<br />
								<b>%{TEXT_PM25_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM25}%=%{MQTT_PM25}%<br />
								<b>%{TEXT_PM10_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_PM10}%=%{MQTT_PM10}%<br />
								<b>%{TEXT_AIRQUALITY_TOPIC}%: </b>%{MQTT_IP}%%{MQTT_DEVICENAME}%%{MQTT_TOPIC_AIRQUALITY}%=%{MQTT_AIRQUALITY}%<br />)rawliteral";