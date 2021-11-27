// CONFIG PAGE HTML
/*
% -> &#37;
*/
static const char WEB_CONFIG_PAGE_ALL[] PROGMEM = R"rawliteral(<html lang='%{Language}%'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><title>Smogly - %{CurrentPageTitle}%</title>%{WEB_PAGE_CSS}%</head><body>
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
					<center><h1>Smogly - %{TEXT_CONFIG_PAGE}%</h1></center><br><br>
										<center>
											<table class='table table-borderless' style='table-layout:fixed;'>
											    <thead>
											      <tr align='center'>
											      <th scope='col' width='50&#37;'><h4><b>%{TEXT_CONFIG_DEVICE_PAGE}%</b></h4></th>
											      <th scope='col' width='50&#37;'><h4><b>%{TEXT_CONFIG_SERVICES_PAGE}%</b></h4></th>
											      </tr>
												  <tr align='center'>
												  <th scope='col'><hr id='left_hr' style='border-color:black;'></th>
												  <th scope='col'><hr id='right_hr' style='border-color:black;'></th>
												  </tr>
												  <tr>
												  <th scope='col'><h5>%{TEXT_CONFIG_INFO_DEVICE_PAGE}%</h5></th>
												  <th scope='col'><h5>%{TEXT_CONFIG_INFO_SERVICES_PAGE}%</h5></th>
												  </tr>
												  <tr align='center'>
												  <th scope='col'>%{ConfigDeviceButton}%</th>
												  <th scope='col'>%{ConfigServicesButton}%</th>
												  </tr>
											    </thead>
											</table>
											</center>
											<hr><center><br>
											%{WiFiEraseButton}%    %{RestoreConfigButton}%
											<br></center>
		<br><hr><center><a href='http://smogly.org/' target='_blank'>Smogly</a> &#9830; <a href='https://hs-silesia.pl' target='_blank'>Hackerspace Silesia</a> &#9830;
			<script type='text/javascript'>
		  		document.write(new Date().getFullYear());
			</script>
			</center></div></main></form>
					<script src='https://code.jquery.com/jquery-3.4.1.slim.min.js' integrity='sha384-J6qa4849blE2+poT4WnyKhv5vZF5SrPo0iEjwBvKU7imGFAV0wwj1yYfoRSJoZ+n' crossorigin='anonymous'></script>
					<script src='https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js' integrity='sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo' crossorigin='anonymous'></script>
					<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js' integrity='sha384-wfSDF2E50Y2D1uUdj0O3uMBJnjuUD4Ih7YwaYd1iqfktj0Uod8GCExl3Og8ifwB6' crossorigin='anonymous'></script>
					</body></html>)rawliteral";

					
static const char WEB_CONFIG_DEVICE_PAGE_BUTTON[] = R"rawliteral(<a href='/config_device' class='btn btn-outline-primary btn-sm' role='button'>%{TEXT_CONFIG_DEVICE_PAGE}%</a>)rawliteral";

static const char WEB_CONFIG_SERVICES_PAGE_BUTTON[] = R"rawliteral(<a href='/config_services' class='btn btn-outline-primary btn-sm' role='button'>%{TEXT_CONFIG_SERVICES_PAGE}%</a>)rawliteral";
