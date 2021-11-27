// CONFIG DEVICE PAGE HTML
/*
% -> &#37;
*/
static const char WEB_CONFIG_DEVICE_PAGE_ALL[] PROGMEM = R"rawliteral(<html lang='%{Language}%'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogly - %{CurrentPageTitle}%</title>%{WEB_PAGE_CSS}%</head><body>
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
</div></nav><form method='GET' action='/config_device_save'>
					<main role='main' class='container'><div class='jumbotron'>
					<center><h1>Smogly - %{TEXT_CONFIG_DEVICE_PAGE}%</h1></center><br>%{TEXT_INSTRUCIONSLINK}%<br><br>
					<b>%{TEXT_DEVICENAME}%: </b>%{device_name}% <br>
					<b>%{TEXT_DEVICENAMEAUTO}%: </b>%{DEVICENAME_AUTO}%
					<b>%{TEXT_SELECTEDLANGUAGE}%: </b>%{LanguageSelect}%
					<hr>
					<b>%{TEXT_MY_COORDINATESINFO}%:
					<br>%{TEXT_MY_LATITUDE}%: </b>%{LATITUDE}%
					<b>%{TEXT_MY_LONGITUDE}%: </b>%{LONGITUDE}%
					<b>%{TEXT_ALTITUDEINFO}%: </b>%{MYALTITUDE}%
					<hr>
					<b>%{TEXT_TEMPHUMIPRESSSENSOR}%: </b>%{THP_MODELSelect}%
					<b>%{TEXT_PMSENSOR}%: </b>%{DUST_MODELSelect}%
					<hr>%{WEB_CONFIG_DEVICE_PAGE_FIRST_THP_PINS}%
					%{WEB_CONFIG_DEVICE_PAGE_SECOND_THP_PINS}%
					%{WEB_CONFIG_DEVICE_PAGE_DUST_PINS}%
					<hr>
					<b>%{TEXT_FREQUENTMEASUREMENTONOFF}%: </b>%{FREQUENTMEASUREMENT_Select}% %{TEXT_FREQUENTMEASUREMENTINFO}%<br>
					<b>%{TEXT_MEASUREMENTFREQUENCY}%: </b>%{FREQUENTMEASUREMENT_time}%
					<b>%{TEXT_AVERAGELASTRESULT}%: </b>%{NUMBEROFMEASUREMENTS}%
					<hr><b>DeepSleep: </b>%{DEEPSLEEP_ON}% %{TEXT_DEEPSLEEPINFO}%
					<hr>
					%{WEB_CONFIG_DEVICE_SHOWING_PM1}%
					<hr>
					<b>%{TEXT_SECURECONFIGUPDATEPAGE}%: </b>%{CONFIG_AUTH}%
					<b>%{TEXT_SECURELOGIN}%: </b>%{CONFIG_USERNAME}%
					<b>%{TEXT_SECUREPASSWD}%: </b>%{CONFIG_PASSWORD}%
					%{TEXT_SECURELOGOUTINFO}%
					<hr>
					<b>Debug: </b>%{DEBUG}%
					<b>%{TEXT_CALIBMETHOD}%: </b>%{CalibrationModelSelect}%
					<b>%{TEXT_CALIB1}%: </b>%{calib1}%
					<br>
					<!-- <b>%{TEXT_CALIB2}%: </b>%{calib2}% -->
					<b>%{TEXT_SOFTWATEVERSION}%: </b>%{SOFTWAREVERSION}%
					<br><b>%{TEXT_AUTOUPDATEON}%: </b>%{AUTOUPDATEON}% %{TEXT_UPDATEWARNING}%
					%{WEB_CONFIG_DEVICE_HOMEKIT}%
					<hr><center><br>
					%{WiFiEraseButton}%  %{RestoreConfigButton}%
					<br><br></center><hr><br><center>
					%{SubmitButton}%
	</center>
		<br><hr><center><a href='http://smogly.org/' target='_blank'>Smogly</a> &#9830; <a href='https://hs-silesia.pl' target='_blank'>Hackerspace Silesia</a> &#9830;
			<script type='text/javascript'>
		  		document.write(new Date().getFullYear());
			</script>
			</center></div></main></form>
					<script src='https://code.jquery.com/jquery-3.4.1.slim.min.js' integrity='sha384-J6qa4849blE2+poT4WnyKhv5vZF5SrPo0iEjwBvKU7imGFAV0wwj1yYfoRSJoZ+n' crossorigin='anonymous'></script>
					<script src='https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js' integrity='sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo' crossorigin='anonymous'></script>
					<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js' integrity='sha384-wfSDF2E50Y2D1uUdj0O3uMBJnjuUD4Ih7YwaYd1iqfktj0Uod8GCExl3Og8ifwB6' crossorigin='anonymous'></script>
					</body></html>)rawliteral";


static const char WEB_CONFIG_DEVICE_PAGE_FIRST_THP_PINS[] = R"rawliteral(%{TEXT_FIRST_THP_SDA_SCL}% - %{THP_MODEL}%<br>
					<b>%{TEXT_FIRST_THP_SDA}%: </b>%{FIRST_THP_SDA}%
					<b>%{TEXT_FIRST_THP_SCL}%: </b>%{FIRST_THP_SCL}%)rawliteral";

static const char WEB_CONFIG_DEVICE_PAGE_SECOND_THP_PINS[] = R"rawliteral(<br><b>%{TEXT_SECOND_THP}%: </b>%{SECOND_THP}% %{SECOND_THP_PINS_CONFIG}%)rawliteral";

static const char WEB_CONFIG_DEVICE_PAGE_SECOND_THP_PINS_CONFIG[] = R"rawliteral(<br><b>%{TEXT_SECOND_THP_SDA}%: </b>%{SECOND_THP_SDA}%
	<b>%{TEXT_SECOND_THP_SCL}%: </b>%{SECOND_THP_SCL}%)rawliteral";

static const char WEB_CONFIG_DEVICE_PAGE_DUST_PINS[] = R"rawliteral(<br>%{TEXT_DUST_TX_RX}% - %{DUST_MODEL}%<br>
<b>%{TEXT_DUST_TX}%: </b>%{DUST_TX}%
<b>%{TEXT_DUST_RX}%: </b>%{DUST_RX}%)rawliteral";

static const char WEB_CONFIG_DEVICE_SHOWING_PM1[] = R"rawliteral(<b>%{TEXT_DISPLAYPM1}%: </b> %{DISPLAY_PM1}%)rawliteral";

static const char WEB_CONFIG_DEVICE_HOMEKIT[] = R"rawliteral(<br><b>%{TEXT_HOMEKIT_SUPPORT}%: </b>%{HOMEKIT_SUPPORT_ON}%
%{WEB_CONFIG_DEVICE_HOMEKIT_RESET_PAIR}%)rawliteral";

static const char WEB_CONFIG_DEVICE_HOMEKIT_RESET_PAIR[] = "<b>%{TEXT_HOMEKIT_IS_PAIRED}%: </b>%{HOMEKIT_PAIRED_RESET}%";
