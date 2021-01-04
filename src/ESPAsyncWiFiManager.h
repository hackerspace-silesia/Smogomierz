/**************************************************************
   WiFiManager is a library for the ESP8266/Arduino platform
   (https://github.com/esp8266/Arduino) to enable easy
   configuration and reconfiguration of WiFi credentials using a Captive Portal
   inspired by:
   http://www.esp8266.com/viewtopic.php?f=29&t=2520
   https://github.com/chriscook8/esp-arduino-apboot
   https://github.com/esp8266/Arduino/tree/esp8266/hardware/esp8266com/esp8266/libraries/DNSServer/examples/CaptivePortalAdvanced
   Built by AlexT https://github.com/tzapu
   Ported to Async Web Server by https://github.com/alanswx
   Licensed under MIT license
 **************************************************************/

#ifndef ESPAsyncWiFiManager_h
#define ESPAsyncWiFiManager_h

#if defined(ESP8266)
//#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>
#include "esp_wps.h"
#define ESP_WPS_MODE WPS_TYPE_PBC
#endif
#include <ESPAsyncWebServer.h>

//#define USE_EADNS               //Uncomment to use ESPAsyncDNSServer
#ifdef USE_EADNS
#include <ESPAsyncDNSServer.h>    //https://github.com/devyte/ESPAsyncDNSServer
                                  //https://github.com/me-no-dev/ESPAsyncUDP
#else
#include <DNSServer.h>
#endif
#include <memory>

// fix crash on ESP32 (see https://github.com/alanswx/ESPAsyncWiFiManager/issues/44)
#if defined(ESP8266)
typedef int wifi_ssid_count_t;
#else
typedef int16_t wifi_ssid_count_t;
#endif

#if defined(ESP8266)
extern "C" {
  #include "user_interface.h"
}
#else
#include <rom/rtc.h>
#endif

const char WFM_HTTP_HEAD[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><meta charset='UTF-8'><title>{v}</title>";
//const char HTTP_STYLE[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";

const char HTTP_STYLE[]            PROGMEM = "<style>"
".c,body{text-align:center;font-family:verdana}div,input{padding:5px;font-size:1em;margin:5px 0;box-sizing:border-box;}"
"input,button,.msg{border-radius:.3rem;width: 100%}"
"button,input[type='button'],input[type='submit']{cursor:pointer;border:0;background-color:#20BF55;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%}"
"input[type='file']{border:1px solid #20BF55}"
".wrap {text-align:left;display:inline-block;min-width:260px;max-width:500px}"
// links
"a{color:#000;font-weight:700;text-decoration:none}a:hover{color:#20BF55;text-decoration:underline}"
// quality icons
".q{height:16px;margin:0;padding:0 5px;text-align:right;min-width:38px;float:right}.q.q-0:after{background-position-x:0}.q.q-1:after{background-position-x:-16px}.q.q-2:after{background-position-x:-32px}.q.q-3:after{background-position-x:-48px}.q.q-4:after{background-position-x:-64px}.q.l:before{background-position-x:-80px;padding-right:5px}.ql .q{float:left}.q:after,.q:before{content:'';width:16px;height:16px;display:inline-block;background-repeat:no-repeat;background-position: 16px 0;"
"background-image:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGAAAAAQCAMAAADeZIrLAAAAJFBMVEX///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADHJj5lAAAAC3RSTlMAIjN3iJmqu8zd7vF8pzcAAABsSURBVHja7Y1BCsAwCASNSVo3/v+/BUEiXnIoXkoX5jAQMxTHzK9cVSnvDxwD8bFx8PhZ9q8FmghXBhqA1faxk92PsxvRc2CCCFdhQCbRkLoAQ3q/wWUBqG35ZxtVzW4Ed6LngPyBU2CobdIDQ5oPWI5nCUwAAAAASUVORK5CYII=');}"
// icons @2x media query (32px rescaled)
"@media (-webkit-min-device-pixel-ratio: 2),(min-resolution: 192dpi){.q:before,.q:after {"
"background-image:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAALwAAAAgCAMAAACfM+KhAAAALVBMVEX///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAOrOgAAAADnRSTlMAESIzRGZ3iJmqu8zd7gKjCLQAAACmSURBVHgB7dDBCoMwEEXRmKlVY3L//3NLhyzqIqSUggy8uxnhCR5Mo8xLt+14aZ7wwgsvvPA/ofv9+44334UXXngvb6XsFhO/VoC2RsSv9J7x8BnYLW+AjT56ud/uePMdb7IP8Bsc/e7h8Cfk912ghsNXWPpDC4hvN+D1560A1QPORyh84VKLjjdvfPFm++i9EWq0348XXnjhhT+4dIbCW+WjZim9AKk4UZMnnCEuAAAAAElFTkSuQmCC');"
"background-size: 95px 16px;}}"
// msg callouts
".msg{padding:20px;margin:20px 0;border:1px solid #eee;border-left-width:5px;border-left-color:#777}.msg h4{margin-top:0;margin-bottom:5px}.msg.P{border-left-color:#20BF55}.msg.P h4{color:#20BF55}.msg.D{border-left-color:#dc3630}.msg.D h4{color:#dc3630}"
// lists
"dt{font-weight:bold}dd{margin:0;padding:0 0 0.5em 0;min-height:12px}"
"td{vertical-align: top;}"
".h{display:none}"
"button.D{background-color:#dc3630}"
// invert
"body.invert,body.invert a,body.invert h1 {background-color:#060606;color:#fff;}"
"body.invert .msg{color:#fff;background-color:#282828;border-top:1px solid #555;border-right:1px solid #555;border-bottom:1px solid #555;}"
"body.invert .q[role=img]{-webkit-filter:invert(1);filter:invert(1);}"
"input:disabled {opacity: 0.5;}"
"</style>";

const char HTTP_SCRIPT[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEAD_END[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_PORTAL_OPTIONS[] PROGMEM  = "<form action=\"/wifi\" method=\"get\"><button><center>Podłącz do WiFi <br> WiFi Config</center></button></form><br/><form action=\"/r\" method=\"post\"><button><center>Uruchom ponownie <br> Restart</center></button></form>";
const char HTTP_ITEM[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_FORM_START[] PROGMEM      = "<form method='get' action='wifisave'><input id='s' name='s' length=32 placeholder='{v}'><br/><input id='p' name='p' length=64 type='password' placeholder='********'><br/>";
const char HTTP_FORM_PARAM[] PROGMEM      = "<br/><input id='{i}' name='{n}' length={l} placeholder='{p}' value='{v}' {c}>";
const char HTTP_FORM_END[] PROGMEM        = "<br/><button type='submit'><center>Zapisz <br> Save</center></button></form>";
const char HTTP_SCAN_LINK[] PROGMEM       = "<br/><div class=\"c\"><a href=\"/wifi\"><button name='refresh' value='1'><center>Odśwież <br> Refresh</center></button></div>";

//const char HTTP_SAVED[] PROGMEM           = "<div>Credentials Saved<br />Trying to connect ESP to network.<br />If it fails reconnect to AP to try again</div>";
const char HTTP_SAVED[]            PROGMEM = "<div class='msg'></center><strong>PL:</strong><br>Zapisano ustawienia<br />Za chwilę nastąpi restart Smogly. Jeśli nie uda się połączyć z siecią WiFi - spróbuj ponownie skonfigurować urządzenie.<br><br><strong>EN:</strong><br>Saving Credentials<br/>Trying to connect Smogly to network.<br/>If it fails reconnect - try again</div>";

//const char HTTP_END[] PROGMEM             = "</div></body></html>";
const char HTTP_END[]              PROGMEM = R"rawliteral(</div><br><hr><center>Smogly &#9830;
	<script type='text/javascript'>
  		document.write(new Date().getFullYear());
	</script>
	</center></body></html>)rawliteral";

#define WIFI_MANAGER_MAX_PARAMS 10

class AsyncWiFiManagerParameter {
public:
  AsyncWiFiManagerParameter(const char *custom);
  AsyncWiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length);
  AsyncWiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);

  const char *getID();
  const char *getValue();
  const char *getPlaceholder();
  int         getValueLength();
  const char *getCustomHTML();
private:
  const char *_id;
  const char *_placeholder;
  char       *_value;
  int         _length;
  const char *_customHTML;

  void init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);

  friend class AsyncWiFiManager;
};


class WiFiResult
{
public:
  bool duplicate;
  String SSID;
  uint8_t encryptionType;
  int32_t RSSI;
  uint8_t* BSSID;
  int32_t channel;
  bool isHidden;

  WiFiResult()
  {
  }


};

class AsyncWiFiManager
{
public:
  #ifdef USE_EADNS
  AsyncWiFiManager(AsyncWebServer * server, AsyncDNSServer *dns);
  #else
  AsyncWiFiManager(AsyncWebServer * server, DNSServer *dns);
  #endif

  void          scan(boolean async = false);
  String        scanModal();
  void          loop();
  void          safeLoop();
  void          criticalLoop();
  String        infoAsString();

  boolean       autoConnect(unsigned long maxConnectRetries = 1, unsigned long retryDelayMs = 1000);
  boolean       autoConnect(char const *apName, char const *apPassword = NULL, unsigned long maxConnectRetries = 1, unsigned long retryDelayMs = 1000);

  //if you want to always start the config portal, without trying to connect first
  boolean       startConfigPortal(char const *apName, char const *apPassword = NULL);
  void startConfigPortalModeless(char const *apName, char const *apPassword);

  // get the AP name of the config portal, so it can be used in the callback
  String        getConfigPortalSSID();

  void          resetSettings();

  //sets timeout before webserver loop ends and exits even if there has been no setup.
  //usefully for devices that failed to connect at some point and got stuck in a webserver loop
  //in seconds setConfigPortalTimeout is a new name for setTimeout
  void          setConfigPortalTimeout(unsigned long seconds);
  void          setTimeout(unsigned long seconds);

  //sets timeout for which to attempt connecting, usefull if you get a lot of failed connects
  void          setConnectTimeout(unsigned long seconds);

  //wether or not the wifi manager tries to connect to configured access point even when
  //configuration portal (ESP as access point) is running [default true/on]
  void          setTryConnectDuringConfigPortal(boolean v);


  void          setDebugOutput(boolean debug);
  //defaults to not showing anything under 8% signal quality if called
  void          setMinimumSignalQuality(int quality = 8);
  //sets a custom ip /gateway /subnet configuration
  void          setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
  //sets config for a static IP
  void          setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn, IPAddress dns1=(uint32_t)0x00000000, IPAddress dns2=(uint32_t)0x00000000);
  //called when AP mode and config portal is started
  void          setAPCallback( void (*func)(AsyncWiFiManager*) );
  //called when settings have been changed and connection was successful
  void          setSaveConfigCallback( void (*func)(void) );
  //adds a custom parameter
  void          addParameter(AsyncWiFiManagerParameter *p);
  //if this is set, it will exit after config, even if connection is unsucessful.
  void          setBreakAfterConfig(boolean shouldBreak);
  //if this is set, try WPS setup when starting (this will delay config portal for up to 2 mins)
  //TODO
  //if this is set, customise style
  void          setCustomHeadElement(const char* element);
  //if this is true, remove duplicated Access Points - defaut true
  void          setRemoveDuplicateAPs(boolean removeDuplicates);
  //sets a custom element to add to options page
  void          setCustomOptionsElement(const char* element);

private:
  AsyncWebServer *server;
  #ifdef USE_EADNS
  AsyncDNSServer      *dnsServer;
  #else
  DNSServer      *dnsServer;
  #endif


  boolean         _modeless;
  unsigned long   scannow;
  int             shouldscan;
  boolean         needInfo = true;

  //const int     WM_DONE                 = 0;
  //const int     WM_WAIT                 = 10;

  //const String  HTTP_HEAD = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/><title>{v}</title>";

  void          setupConfigPortal();
#ifdef NO_EXTRA_4K_HEAP
  void          startWPS();
#endif
  String        pager;
  wl_status_t   wifiStatus;
  const char*   _apName                 = "no-net";
  const char*   _apPassword             = NULL;
  String        _ssid                   = "";
  String        _pass                   = "";
  unsigned long _configPortalTimeout    = 0;
  unsigned long _connectTimeout         = 0;
  unsigned long _configPortalStart      = 0;

  IPAddress     _ap_static_ip;
  IPAddress     _ap_static_gw;
  IPAddress     _ap_static_sn;
  IPAddress     _sta_static_ip;
  IPAddress     _sta_static_gw;
  IPAddress     _sta_static_sn;
  IPAddress     _sta_static_dns1= (uint32_t)0x00000000;
  IPAddress     _sta_static_dns2= (uint32_t)0x00000000;

  int           _paramsCount            = 0;
  int           _minimumQuality         = -1;
  boolean       _removeDuplicateAPs     = true;
  boolean       _shouldBreakAfterConfig = false;
#ifdef NO_EXTRA_4K_HEAP
  boolean       _tryWPS                 = false;
#endif
  const char*   _customHeadElement      = "";
  const char*   _customOptionsElement   = "";

  //String        getEEPROMString(int start, int len);
  //void          setEEPROMString(int start, int len, String string);

  int           status = WL_IDLE_STATUS;
  int           connectWifi(String ssid, String pass);
  uint8_t       waitForConnectResult();
  void          setInfo();
  void			copySSIDInfo(wifi_ssid_count_t n);
  String networkListAsString();

  void          handleRoot(AsyncWebServerRequest *);
  void          handleWifi(AsyncWebServerRequest*,boolean scan);
  void          handleWifiSave(AsyncWebServerRequest*);
  void          handleInfo(AsyncWebServerRequest*);
  void          handleReset(AsyncWebServerRequest*);
  void          handleNotFound(AsyncWebServerRequest*);
  void          handle204(AsyncWebServerRequest*);
  boolean       captivePortal(AsyncWebServerRequest*);

  // DNS server
  const byte    DNS_PORT = 53;

  //helpers
  int           getRSSIasQuality(int RSSI);
  boolean       isIp(String str);
  String        toStringIp(IPAddress ip);

  boolean       connect;
  boolean       _debug = true;

  WiFiResult          *wifiSSIDs;
  wifi_ssid_count_t   wifiSSIDCount;
  boolean             wifiSSIDscan;

  boolean             _tryConnectDuringConfigPortal = true;

  void (*_apcallback)(AsyncWiFiManager*) = NULL;
  void (*_savecallback)(void) = NULL;

  AsyncWiFiManagerParameter* _params[WIFI_MANAGER_MAX_PARAMS];

  template <typename Generic>
  void          DEBUG_WM(Generic text);

  template <class T>
  auto optionalIPFromString(T *obj, const char *s) -> decltype(  obj->fromString(s)  ) {
    return  obj->fromString(s);
  }
  auto optionalIPFromString(...) -> bool {
    DEBUG_WM("NO fromString METHOD ON IPAddress, you need ESP8266 core 2.1.0 or newer for Custom IP configuration to work.");
    return false;
  }
};

#endif
