// CONFIG SERVICES PAGE HTML
/*
% -> &#37;
*/

static const char WEB_CONFIG_SERVICES_PAGE_ALL[] PROGMEM = R"rawliteral(<html lang='%{Language}%'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogly - %{CurrentPageTitle}%</title>%{WEB_PAGE_CSS}%</head><body>
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
</div></nav><form method='GET' action='/config_services_save'>
	<main role='main' class='container'><div class='jumbotron'>
	<center><h1>Smogly - %{TEXT_CONFIG_SERVICES_PAGE}%</h1></center><br>%{TEXT_INSTRUCIONSLINK}%<br><br>
						<b>%{TEXT_SENDINGINTERVAL}%: </b>%{SENDING_FREQUENCY}%
							<b>%{TEXT_DBSENDINGINTERVAL}%: </b>%{SENDING_DB_FREQUENCY}%
							<hr>
							%{SMOGLIST_COMMENT_BEGIN}%<b>%{TEXT_SMOGLISTSENDING}%: </b>%{SMOGLIST_ON}% %{TEXT_SMOGLISTINFO}%
							<hr>%{SMOGLIST_COMMENT_END}%
							<b>%{TEXT_LUFTDATENSENDING}%: </b>%{LUFTDATEN_ON}%
							<b>ChipID: </b>%{LUFTDATEN_ChipID}%
								%{Sensors_Pins_html}%	
							<br><b>%{TEXT_LUFTDATEN_GRAPH_ON}%: </b>%{LUFTDATEN_GRAPH_ON}%			
							<hr>
							<b>%{TEXT_AQIECOSENDING}%: </b>%{AQI_ECO_ON}%
							<b>%{TEXT_AQIECOSERVER}%: </b>%{AQI_ECO_HOST}%
							<b>%{TEXT_AQIECOPATH}%: </b>%{AQI_ECO_PATH}%
							<b>%{ESP_MODEL}% ID: </b>%{AQI_ECO_ChipID}%
							<hr>
							%{AIRMONITOR_COMMENT_BEGIN}%<b>%{TEXT_AIRMONITORSENDING}%: </b>%{AIRMONITOR_ON}%
							<b>%{TEXT_AIRMONITORCHARTS}%: </b>%{AIRMONITOR_GRAPH_ON}%
							<b>%{TEXT_AIRMONITOR_API_KEY}%: </b>%{AIRMONITOR_API_KEY}%<hr>%{AIRMONITOR_COMMENT_END}%
							<b>%{TEXT_THINGSPEAKSENDING}%: </b>%{THINGSPEAK_ON}%
							<b>%{TEXT_THINGSPEAKCHARTS}%: </b>%{THINGSPEAK_GRAPH_ON}%
							<b>%{TEXT_THINGSPEAK_WRITE_API_KEY}%: </b>%{THINGSPEAK_API_KEY}%
							<b>%{TEXT_THINGSPEAKCHANNELID}%: </b>%{THINGSPEAK_CHANNEL_ID}%
							<b>%{TEXT_THINGSPEAK_READ_API_KEY}%: </b>%{THINGSPEAK_READ_API_KEY}%
							<hr>
							<b>%{TEXT_INFLUXDBSENDING}%: </b>%{INFLUXDB_ON}%
							<b>%{TEXT_INFLUXDBVERSION}%: </b>%{INFLUXDB_VERSION}%
							<b>%{TEXT_INFLUXDBSERVER}%: </b>%{INFLUXDB_HOST}%
							<b>%{TEXT_INFLUXDBPORT}%: </b>%{INFLUXDB_PORT}%
							<b>%{TEXT_INFLUXDBNAME}%: </b>%{INFLUXDB_DATABASE}%
							<b>%{TEXT_INFLUXDBUSER}%: </b>%{INFLUXDB_USER}%
							<b>%{TEXT_INFLUXDBPASSWD}%: </b>%{INFLUXDB_PASSWORD}%
							%{INFLUXDB_V2}%
							<hr>
							<br><center>%{AdvancedMQTTConfigButton}%</center><br>
							<hr><center><br>
							%{WiFiEraseButton}% %{RestoreConfigButton}%
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
			</body></html>
	)rawliteral";
			
			// %{WEB_CONFIG_SERVICES_INFLUXDB_VERSION_V2}% %
			
static const char WEB_CONFIG_SERVICES_THP_SENSOR_PIN[] = R"rawliteral(<br><b>%{THPSENSOR}%</b> Sensor PIN: <b>%{THPXPIN}%</b>)rawliteral";
static const char WEB_CONFIG_SERVICES_DUST_MODEL_SENSOR_PIN[] = R"rawliteral(<br><b>%{DUSTSENSOR}%</b> Sensor PIN: <b>%{DUSTXPIN}%</b>)rawliteral";

static const char WEB_CONFIG_SERVICES_INFLUXDB_VERSION_V2[] = R"rawliteral(<b>{TEXT_INFLUXDBORG}: </b>{INFLUXDB_ORG}
	<b>{TEXT_INFLUXDBBUCKET}: </b>{INFLUXDB_BUCKET}
	<b>{TEXT_INFLUXDBTOKEN}: </b>{INFLUXDB_TOKEN})rawliteral";

static const char ASW_WEB_GOTO_CONFIG_ADVANCED_MQTT_PAGE_BUTTON[]  = "<a href='/config_adv_mqtt' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_CONFIG_ADV_MQTT}</a>";
