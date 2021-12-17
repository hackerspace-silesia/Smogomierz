// ROOT PAGE HTML
/*
% -> &#37;
*/
static const char WEB_ROOT_PAGE_ALL[] PROGMEM = R"rawliteral(<html lang='%{Language}%'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogly - %{CurrentPageTitle}%</title>%{WEB_PAGE_CSS}%</head><body>
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
		</div></nav>
					<main role='main' class='container'><div class='jumbotron'>
						<center><h1>Smogly</h1><br>
						%{WEB_UPDATE_INFO_WARNING}%
						%{WEB_ROOT_PAGE_MEASUREMENTS_THP1}%
						%{WEB_ROOT_PAGE_MEASUREMENTS_AIR}%
						%{WEB_ROOT_PAGE_AIRMONITOR_GRAPH}%	
						%{WEB_ROOT_PAGE_LUFTDATEN_GRAPH}%	
						%{WEB_ROOT_PAGE_THINGSPEAK_GRAPH}%	
			<br><hr><center><a href='http://smogly.org/' target='_blank'>Smogly</a> &#9830; <a href='https://hs-silesia.pl' target='_blank'>Hackerspace Silesia</a> &#9830;
											<script type='text/javascript'>
										  		document.write(new Date().getFullYear());
											</script>
											</center></div></main></form>
					<script src='https://code.jquery.com/jquery-3.4.1.slim.min.js' integrity='sha384-J6qa4849blE2+poT4WnyKhv5vZF5SrPo0iEjwBvKU7imGFAV0wwj1yYfoRSJoZ+n' crossorigin='anonymous'></script>
					<script src='https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js' integrity='sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo' crossorigin='anonymous'></script>
					<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js' integrity='sha384-wfSDF2E50Y2D1uUdj0O3uMBJnjuUD4Ih7YwaYd1iqfktj0Uod8GCExl3Og8ifwB6' crossorigin='anonymous'></script>
					</body></html>)rawliteral";

			
static const char WEB_ROOT_PAGE_MEASUREMENTS_THP1[] PROGMEM = R"rawliteral(<h2>{TEXT_WEATHER}:</h2>
					<h3>{TEXT_TEMPERATURE}: {Temperature} °C</h3>
					<h3>{TEXT_HUMIDITY}: {Humidity} &#37;</h3>
					<h3>{TEXT_PRESSURE}: {Pressure} hPa</h3>
					<h3>{TEXT_DEWPOINT}: {Dewpoint} °C</h3><p>)rawliteral";

static const char WEB_ROOT_PAGE_MEASUREMENTS_THP2[] PROGMEM = R"rawliteral(
							<br><br>
							<h3>{TEXT_TEMPERATURE}: {Temperature} °C</h3>
							<h3>{TEXT_HUMIDITY}: {Humidity} %</h3>
							<h3>{TEXT_PRESSURE}: {Pressure} hPa</h3>
							<h3>{TEXT_DEWPOINT}: {Dewpoint} °C</h3><p>
							)rawliteral";
					
static const char WEB_ROOT_PAGE_MEASUREMENTS_AIR[] PROGMEM = R"rawliteral(<h2>{TEXT_AIRPOLLUTION}:</h2>
					<h3>PM1: {averagePM1} µg/m³</h3>
					<h3>PM2.5: {colorAveragePM25} {averagePM25} µg/m³</h3>
					<h3>PM10: {colorAveragePM10} {averagePM10} µg/m³</h3>)rawliteral";
		
static const char WEB_ROOT_PAGE_AIRMONITOR_GRAPH[] PROGMEM = R"rawliteral(<hr>
						<iframe src='http://metrics.airmonitor.pl/dashboard-solo/db/airmonitor?orgId=1&var-latitude={LATITUDE}&var-longitude={LONGITUDE}&refresh=1m&panelId=14' frameborder='0' style='overflow: hidden; height: 100&#37;; width: 100&#37;; max-height: 510; max-width: 450;' ></iframe>
						<iframe src='http://metrics.airmonitor.pl/dashboard-solo/db/airmonitor?orgId=1&var-latitude={LATITUDE}&var-longitude={LONGITUDE}&refresh=1m&panelId=13' frameborder='0' style='overflow: hidden; height: 100&#37;; width: 100&#37;; max-height: 510; max-width: 450;' ></iframe>
						)rawliteral";

static const char WEB_ROOT_PAGE_LUFTDATEN_GRAPH[] PROGMEM = R"rawliteral(<hr>
												<iframe src='https://maps.sensor.community/grafana/d-solo/000000004/single-sensor-view?orgId=1&panelId=2&var-node={NODE_LUFTDATEN_ID}' frameborder='0' style='overflow: hidden; height: 100&#37;; width: 100&#37;; max-height: 510; max-width: 450;' ></iframe>	
<iframe src='https://maps.sensor.community/grafana/d-solo/000000004/single-sensor-view?orgId=1&panelId=1&var-node={NODE_LUFTDATEN_ID}' frameborder='0' style='overflow: hidden; height: 100&#37;; width: 100&#37;; max-height: 510; max-width: 450;' ></iframe>
												)rawliteral";	
												
static const char WEB_ROOT_PAGE_THINGSPEAK_GRAPH[] PROGMEM = R"rawliteral(<hr>
						<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/charts/2?bgcolor=&#37;23ffffff&color=&#37;23d62020&dynamic=true&results=60&title=PM2.5&type=line&yaxis=ug&#37;2Fm3&update=15&api_key={THINGSPEAK_READ_API_KEY}'></iframe>
					    <iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/charts/3?bgcolor=&#37;23ffffff&color=&#37;23d62020&dynamic=true&results=60&title=PM10&type=line&yaxis=ug&#37;2Fm3&update=15&api_key={THINGSPEAK_READ_API_KEY}'></iframe>
						<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/charts/4?bgcolor=&#37;23ffffff&color=&#37;23d62020&dynamic=true&results=60&title=Temperatura&type=line&update=15&api_key={THINGSPEAK_READ_API_KEY}'></iframe>
						<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/charts/5?bgcolor=&#37;23ffffff&color=&#37;23d62020&dynamic=true&results=60&title=Ci&#37;C5&#37;9Bnienie&type=line&update=15&api_key={THINGSPEAK_READ_API_KEY}'></iframe>
						<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/charts/6?bgcolor=&#37;23ffffff&color=&#37;23d62020&dynamic=true&results=60&title=Wilgotno&#37;C5&#37;9B&#37;C4&#37;87&type=line&update=15&api_key={THINGSPEAK_READ_API_KEY}'></iframe>
						)rawliteral";
