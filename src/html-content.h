const char WEB_PAGE_HEADER[] PROGMEM = "<html lang='{Language}'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogomierz - {CurrentPageTitle}</title>\
<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css' integrity='sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T' crossorigin='anonymous'>\
</head><body>\
<nav class='navbar navbar-expand-md navbar-dark bg-dark mb-4'><div class='container'><a href='/' class='navbar-brand'>{IndexPageTitle}</a>\
<a href='/config' class='navbar-brand'>{ConfigPageTitle}</a>\
<a href='/update' class='navbar-brand'>{UpdatePageTitle}</a>\
</div></nav> ";

const char WEB_PAGE_FOOTER[] PROGMEM = "<br><hr><center>Hackerspace Silesia &#9830; \
	<script type='text/javascript'>\
  		document.write(new Date().getFullYear());\
	</script>\
	</center></div></main></form>\
	<script src='https://code.jquery.com/jquery-3.3.1.slim.min.js' integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo' crossorigin='anonymous'></script>\
	<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js' integrity='sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1' crossorigin='anonymous'></script>\
	<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js' integrity='sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM' crossorigin='anonymous'></script>\
	</body></html>\r\n";

// MEASUREMENTS PAGE - START

const char WEB_ROOT_PAGE_MEASUREMENTS[] PROGMEM = "<main role='main' class='container'><div class='jumbotron'>\
	<center><h1>Smogomierz</h1><br>\
	{WEB_UPDATE_INFO_WARNING}\
	<h2>{TEXT_WEATHER}:</h2>\
	<h3>{TEXT_TEMPERATURE}: {Temperature} °C</h3>\
	<h3>{TEXT_HUMIDITY}: {Humidity} %</h3>\
	<h3>{TEXT_PRESSURE}: {Pressure} hPa</h3>\
	<h3>{TEXT_DEWPOINT}: {Dewpoint} °C</h3><p>\
	<h2>{TEXT_AIRPOLLUTION}:</h2>\
	<h3>PM1: {averagePM1} µg/m³</h3>\
	<h3>PM2.5: {colorAveragePM25} {averagePM25} µg/m³</h3>\
	<h3>PM10: {colorAveragePM10} {averagePM10} µg/m³</h3> ";

const char WEB_ROOT_PAGE_AIRMONITOR_GRAPH[] PROGMEM = "<hr>\
	<iframe src='http://metrics.airmonitor.pl:3000/dashboard-solo/db/airmonitor?orgId=1&var-latitude={LATITUDE}&var-longitude={LONGITUDE}&refresh=1m&panelId=14' frameborder='0' style='overflow: hidden; height: 100%; width: 100%; max-height: 510; max-width: 450;' ></iframe>\
    <iframe src='http://metrics.airmonitor.pl:3000/dashboard-solo/db/airmonitor?orgId=1&var-latitude={LATITUDE}&var-longitude={LONGITUDE}&refresh=1m&panelId=13' frameborder='0' style='overflow: hidden; height: 100%; width: 100%; max-height: 510; max-width: 450;' ></iframe> ";

const char WEB_ROOT_PAGE_THINGSPEAK_GRAPH[] PROGMEM = "<hr>\
	<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=PM2.5&type=line&yaxis=ug%2Fm3&update=15'></iframe>\
    <iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=PM10&type=line&yaxis=ug%2Fm3&update=15'></iframe>\
	<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Temperatura&type=line&update=15'></iframe>\
	<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Ci%C5%9Bnienie&type=line&update=15'></iframe>\
	<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/charts/6?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Wilgotno%C5%9B%C4%87&type=line&update=15'></iframe> ";

// MEASUREMENTS PAGE - END
// CONFIG PAGE - Start

const char WEB_CONFIG_PAGE_TOP[] PROGMEM = "<form method='POST' action='/config'>\ 
	<main role='main' class='container'><div class='jumbotron'>\
	<center><h1>Smogomierz - {TEXT_CONFIG_PAGE}</h1></center><br><br>\
	<div style='color: #2f7a2d'> <strong>{TEXT_SAVED}!</strong> - {TEXT_POSTCONFIG_INFO} </div><br><hr><br>\
	{TEXT_INSTRUCIONSLINK}<br><br> ";

const char WEB_CONFIG_PAGE_CONFIG[] PROGMEM = "<b>{TEXT_DEVICENAME}: </b>{device_name} <br>\
	<b>{TEXT_DEVICENAMEAUTO}: </b>{DEVICENAME_AUTO}\
	<b>{TEXT_SELECTEDLANGUAGE}: </b>{LanguageSelect}\
	<hr>\
	<b>{TEXT_TEMPHUMIPRESSSENSOR}: </b>{THP_MODELSelect}\
	<b>{TEXT_PMSENSOR}: </b>{DUST_MODELSelect}\
	<hr>\
	<b>{TEXT_FREQUENTMEASUREMENTONOFF}: </b>{FREQUENTMEASUREMENT_Select} {TEXT_FREQUENTMEASUREMENTINFO}<br>\
	<b>{TEXT_MEASUREMENTFREQUENCY}: </b>{FREQUENTMEASUREMENT_time}\
	<b>{TEXT_AVERAGELASTRESULT}: </b>{NUMBEROFMEASUREMENTS}\
	<b>{TEXT_SENDINGINTERVAL}: </b>{SENDING_FREQUENCY}\
	<b>{TEXT_DBSENDINGINTERVAL}: </b>{SENDING_DB_FREQUENCY}\
	<hr><b>DeepSleep: </b>{DEEPSLEEP_ON} {TEXT_DEEPSLEEPINFO}\
	<hr>\
	<b>{TEXT_DISPLAYPM1}: </b> {DISPLAY_PM1}\
	<b>{TEXT_ALTITUDEINFO}: </b>{MYALTITUDE}\
	<hr>\
	<b>{TEXT_SECURECONFIGUPDATEPAGE}: </b>{CONFIG_AUTH}\
	<b>{TEXT_SECURELOGIN}: </b>{CONFIG_USERNAME}\
	<b>{TEXT_SECUREPASSWD}: </b>{CONFIG_PASSWORD}\
	{TEXT_SECURELOGOUTINFO}\
	<hr>\
	<b>{TEXT_SMOGLISTSENDING}: </b>{SMOGLIST_ON} {TEXT_SMOGLISTINFO}\
	<hr>\	
	<b>{TEXT_LUFTDATENSENDING}: </b>{LUFTDATEN_ON}\
	<b>ChipID: </b>{ChipID}\
	<br><b>{THPSENSOR}</b> Sensor PIN: <b>{THPXPIN}</b>\
	<br><b>{DUSTSENSOR}</b> Sensor PIN: <b>{DUSTXPIN}</b>\
	<hr>\
	<b>{TEXT_AIRMONITORSENDING}: </b>{AIRMONITOR_ON}\
	<b>{TEXT_AIRMONITORCHARTS}: </b>{AIRMONITOR_GRAPH_ON}\
	<b>{TEXT_AIRMONITORCOORDINATESINFO}:\
	<br>{TEXT_AIRMONITORLATITUDE}: </b>{LATITUDE}\
	<b>{TEXT_AIRMONITORLONGITUDE}: </b>{LONGITUDE}\
	<hr>\
	<b>{TEXT_THINGSPEAKSENDING}: </b>{THINGSPEAK_ON}\
	<b>{TEXT_THINGSPEAKCHARTS}: </b>{THINGSPEAK_GRAPH_ON}\
	<b>{TEXT_THINGSPEAKAPIKEY}: </b>{THINGSPEAK_API_KEY}\
	<b>{TEXT_THINGSPEAKCHANNELID}: </b>{THINGSPEAK_CHANNEL_ID}\
	<hr>\
	<b>{TEXT_INFLUXDBSENDING}: </b>{INFLUXDB_ON}\
	<b>{TEXT_INFLUXDBSERVER}: </b>{INFLUXDB_HOST}\
	<b>{TEXT_INFLUXDBPORT}: </b>{INFLUXDB_PORT}\
	<b>{TEXT_INFLUXDBNAME}: </b>{INFLUXDB_DATABASE}\
	<b>{TEXT_INFLUXDBUSER}: </b>{DB_USER}\
	<b>{TEXT_INFLUXDBPASSWD}: </b>{DB_PASSWORD}\
	<hr>\
	<b>{TEXT_MQTTSENDING}: </b>{MQTT_ON}\
	<b>{TEXT_MQTTSERVER}: </b>{MQTT_HOST}\
	<b>{TEXT_MQTTPORT}: </b>{MQTT_PORT}\
	<b>{TEXT_MQTTUSER}: </b>{MQTT_USER}\
	<b>{TEXT_MQTTPASSWD}: </b>{MQTT_PASSWORD}\
	<hr>\
	<b>Debug: </b>{DEBUG}\
	<b>{TEXT_CALIBMETHOD}: </b>{CalibrationModelSelect}\
	<b>{TEXT_CALIB1}: </b>{calib1}\
	<br>\
	<!-- <b>{TEXT_CALIB2}: </b>{calib2}\ -->\
	<b>{TEXT_SOFTWATEVERSION}: </b>{SOFTWAREVERSION}\
	<br><b>{TEXT_AUTOUPDATEON}: </b>{AUTOUPDATEON} {TEXT_UPDATEPAGEAUTOUPDATEWARNING}\
	<hr><center><br>\
	{WiFiEraseButton}  {RestoreConfigButton}\
	<br><br></center><hr><br><center>\
	{SubmitButton}\
	</center>";

const char WEB_CONFIG_PAGE_SUBMIT_BUTTON[] PROGMEM = "<input type='submit' class='btn btn-outline-danger' value='{TEXT_SAVE}' />";

const char WEB_CONFIG_PAGE_SELECT[] PROGMEM = "<select name='{key}'>";

const char WEB_CONFIG_PAGE_SELECTEND[] PROGMEM = "</select><br />";

const char WEB_CONFIG_PAGE_ADDOPTION[] PROGMEM = "<option value='{value}' {srslyValue}{label}</option>";

const char WEB_CONFIG_PAGE_TEXTIMPUT[] PROGMEM = "<input type='text' maxlength='255' size='20' name='{key}' value='{value}'> {postfix}<br />";

const char WEB_CONFIG_PAGE_PASSWDINPUT[] = "<input type='password' maxlength='255' size='20' name='{key}' value='{value}'> {postfix}<br />";

const char WEB_CONFIG_PAGE_INTINPUT[] PROGMEM = "<input type='number' step='1' maxlength='255' name='{key}' value='{value}'> {postfix}<br />";

const char WEB_CONFIG_PAGE_FLOATINPUT[] = "<input type='number' step='0.000001' maxlength='255' name='{key}' value='{value}'> {postfix}<br />";

const char WEB_CONFIG_PAGE_WIFIERASE[] PROGMEM = "<a href='/erase_wifi' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_ERASEWIFICONFIG}</a>";

const char WEB_CONFIG_PAGE_RESTORECONFIG[] PROGMEM = "<a href='/restore_config' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_RESTORESETTINGS}</a>";

// CONFIG PAGE - END
// UPDATE BUTTONS - START

const char WEB_UPDATE_BUTTON_MANUALUPDATE[] PROGMEM = "<a href='/update' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_MANUALUPDATEBUTTON}</a>";

const char WEB_UPDATE_BUTTON_FWUPDATE[] PROGMEM = "<a href='/fwupdate' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_FWUPDATEBUTTON}</a>";

const char WEB_UPDATE_BUTTON_AUTOUPDATEON[] PROGMEM = "<a href='/autoupdateon' class='btn btn-outline-primary btn-sm' role='button'>{TEXT_AUTOUPDATEONBUTTON}</a>";

const char WEB_UPDATE_INFO_WARNING[] PROGMEM = "<center><h2><b>{TEXT_FWUPDATEAVALIBLE}</b></h2>{MANUALUPDATEBUTTON}	{FWUPDATEBUTTON}	{AUTOUPDATEONBUTTON}<br><br>{TEXT_AUTOUPDATEWARNING}<br></center>";

// UPDATE BUTTONS - END
// UPDATE PAGE - START

const char WEB_UPDATE_PAGE_UPDATE[] PROGMEM = "<main role='main' class='container'><div class='jumbotron'>\
	<form id='data' action='/update' method='POST' enctype='multipart/form-data'>\
	<center><h1>Smogomierz - {TEXT_UPDATE_PAGE}</h1></center><br><br>\
	{WEB_UPDATE_INFO_WARNING}\
	<br><br><div class='input-group mb-3'><div class='custom-file'><input type='file' accept='.bin' class='custom-file-input' id='inputGroupFile04' name='update'><label class='custom-file-label' for='inputGroupFile04'>{TEXT_SELECTUPDATEFILE}</label></div><div class='input-group-append'><button class='btn btn-danger' type='submit'>{TEXT_SUBMITUPDATE}</button></div></div></form>\
	<br>\
	{TEXT_AUTOUPDATEON}: <b>{AUTOUPDATEONSTATUS}</b>\
	<br>{TEXT_CURRENTSOFTVERSION}: <b>{SOFTWAREVERSION}</b>\
	<br>{TEXT_SERVERSOFTWAREVERSION}: <b>{SERVERSOFTWAREVERSION}</b>\
	<br>{TEXT_LATESTAVAILABLESOFT}\
	<br>\
	<br>{TEXT_CONNECTEDWIFI}: <b>{WiFiSSID}</b>\
	<br>{TEXT_WIFIRSSI}: <b>{WiFiRSSI}</b>\
	<br>{TEXT_WIFIQUALITY}: <b>{WiFiQuality}</b>";

// UPDATE PAGE - END