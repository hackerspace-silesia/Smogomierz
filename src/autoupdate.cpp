#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include "config.h"

// ********************** Config **********************

const char* host1 = "api.github.com";
const char* host2 = "raw.githubusercontent.com";
const char* host3 = "github.com";
const int httpsPort = 443;
const int httpPort = 80;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
// Last update: 4.11.2018
const char* fingerprint1 = "5F F1 60 31 09 04 3E F2 90 D2 B0 8A 50 38 04 E8 37 9F BC 76"; // api.github.com
const char* fingerprint2 = "CC AA 48 48 66 46 0E 91 53 2C 9C 7C 23 2A B1 74 4D 29 9D 33"; // raw.githubusercontent.com
const char* fingerprint3 = "CA 06 F5 6B 25 8B 7A 0D 4F 2B 05 47 09 39 47 86 51 15 19 84"; // github.com

// ******************** Config End ********************

String payload;
String Data[3];

void doUpdate() {
	/*
	WiFiClientSecure client;
    if (DEBUG) {
      Serial.print("connecting to ");
      Serial.println(host2);
    }
    if (!client.connect(host2, httpsPort)) {
      if (DEBUG) {
        Serial.println("connection failed");
      }
      return;
    }
    if (client.verify(fingerprint2, host2)) {
      if (DEBUG) {
        Serial.println("certificate matches");
      }
    } else {
      if (DEBUG) {
        Serial.println("certificate doesn't match");
      }
    }
	*/
	//WiFiClient client;
	
	//String originalfwURL = Data[1] + Data[3]; // https://github.com/hackerspace-silesia/Smogomierz/releases/download/v2.4.3/Smogomierz_2.4.3_build_19.00_26.08.2018.bin
	
    /*
      String masterfwURL = "https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/master/"+Data[3];
      t_httpUpdate_return ret = ESPhttpUpdate.update(masterfwURL, "", fingerprint2);

      String devfwURL = "https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/dev/Smogomierz_2.4.9_build_13.00_3.11.2018.bin";
    */
    String masterfwURL = "https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/dev/" + Data[3];
    String devfwURL = "https://raw.githubusercontent.com/hackerspace-silesia/Smogomierz/dev/Smogomierz_2.5.0_build_12.00_5.11.2018.bin";

	ESP.getFreeHeap();
    delay(10);
	ESP.getFreeSketchSpace();
	delay(10);
	
    //t_httpUpdate_return ret = ESPhttpUpdate.update(HTTPdevfwURL); // dziala!!!
	//t_httpUpdate_return ret = ESPhttpUpdate.update(HTTPmasterfwURL); // dziala!!!
	
	//t_httpUpdate_return ret = ESPhttpUpdate.update(devfwURL, "", fingerprint2);
	//t_httpUpdate_return ret = ESPhttpUpdate.update(masterfwURL, "", fingerprint2);
	
   	//t_httpUpdate_return ret = ESPhttpUpdate.update(devfwURL, "", fingerprint2);
   	//t_httpUpdate_return ret = ESPhttpUpdate.update(devfwURL);
   	//t_httpUpdate_return ret = ESPhttpUpdate.update(devfwURL, fingerprint2);
	//t_httpUpdate_return ret = ESPhttpUpdate.update(String(HTTPdevfwURL));
    //t_httpUpdate_return ret = ESPhttpUpdate.update(httpURL);
	
	
    if (DEBUG) {
		/*
      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
          break;
        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("HTTP_UPDATE_NO_UPDATES");
          break;
        case HTTP_UPDATE_OK:
          Serial.println("HTTP_UPDATE_OK");
          break;
        default:
          Serial.printf("Unexpected response code %d from ESPhttpUpdate.update\n", (int)ret);
          break;
      } 
	  */
    }	
}
bool checkUpdate() {
  // Use WiFiClientSecure class to create TLS connection to api.github.com
  WiFiClientSecure client;  
  // /*
  if (DEBUG) {
    Serial.print("connecting to ");
    Serial.println(host1);
  }
  if (!client.connect(host1, httpsPort)) {
    if (DEBUG) {
      Serial.println("connection failed");
    }
    return false;
  }

  if (client.verify(fingerprint1, host1)) {
    if (DEBUG) {
      Serial.println("certificate matches");
    }
  } else {
    if (DEBUG) {
      Serial.println("certificate doesn't match");
    }
  }
  
  String url1 = "/repos/hackerspace-silesia/Smogomierz/releases/latest";
  if (DEBUG) {
    Serial.print("requesting URL: ");
    Serial.println(url1);
  }
  client.print(String("GET ") + url1 + " HTTP/1.1\r\n" +
               "Host: " + host1 + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  if (DEBUG) {
    Serial.println("request sent");
  }
  while (client.connected()) {
    payload = client.readStringUntil('\n');
    if (payload == "\r") {
      if (DEBUG) {
        Serial.println("headers received");
      }
      break;
    }
  }
  payload = client.readStringUntil('\n');

  client.stop();
  
  if (DEBUG) {
    Serial.println("reply was:");
    Serial.println("==========");
    Serial.println(payload);
    Serial.println("==========\n");
    Serial.println("closing connection");
  }

//*/
  // exaple payload String ;)
  //payload = {"{'url':'https://api.github.com/repos/hackerspace-silesia/Smogomierz/releases/12686028','assets_url':'https://api.github.com/repos/hackerspace-silesia/Smogomierz/releases/12686028/assets','upload_url':'https://uploads.github.com/repos/hackerspace-silesia/Smogomierz/releases/12686028/assets{?name,label}','html_url':'https://github.com/hackerspace-silesia/Smogomierz/releases/tag/v2.4.3','id':12686028,'node_id':'MDc6UmVsZWFzZTEyNjg2MDI4','tag_name':'v2.4.3','target_commitish':'master','name':'Smogomierz_2.4.3_build_19.00_26.08.2018','draft':false,'author':{'login':'bfaliszek','id':1752244,'node_id':'MDQ6VXNlcjE3NTIyNDQ=','avatar_url':'https://avatars3.githubusercontent.com/u/1752244?v=4','gravatar_id':'','url':'https://api.github.com/users/bfaliszek','html_url':'https://github.com/bfaliszek','followers_url':'https://api.github.com/users/bfaliszek/followers','following_url':'https://api.github.com/users/bfaliszek/following{/other_user}','gists_url':'https://api.github.com/users/bfaliszek/gists{/gist_id}','starred_url':'https://api.github.com/users/bfaliszek/starred{/owner}{/repo}','subscriptions_url':'https://api.github.com/users/bfaliszek/subscriptions','organizations_url':'https://api.github.com/users/bfaliszek/orgs','repos_url':'https://api.github.com/users/bfaliszek/repos','events_url':'https://api.github.com/users/bfaliszek/events{/privacy}','received_events_url':'https://api.github.com/users/bfaliszek/received_events','type':'User','site_admin':false},'prerelease':false,'created_at':'2018-09-01T09:22:52Z','published_at':'2018-09-01T09:28:38Z','assets':[{'url':'https://api.github.com/repos/hackerspace-silesia/Smogomierz/releases/assets/8478575','id':8478575,'node_id':'MDEyOlJlbGVhc2VBc3NldDg0Nzg1NzU=','name':'Smogomierz_2.4.3_build_19.00_26.08.2018.bin','label':null,'uploader':{'login':'bfaliszek','id':1752244,'node_id':'MDQ6VXNlcjE3NTIyNDQ=','avatar_url':'https://avatars3.githubusercontent.com/u/1752244?v=4','gravatar_id':'','url':'https://api.github.com/users/bfaliszek','html_url':'https://github.com/bfaliszek','followers_url':'https://api.github.com/users/bfaliszek/followers','following_url':'https://api.github.com/users/bfaliszek/following{/other_user}','gists_url':'https://api.github.com/users/bfaliszek/gists{/gist_id}','starred_url':'https://api.github.com/users/bfaliszek/starred{/owner}{/repo}','subscriptions_url':'https://api.github.com/users/bfaliszek/subscriptions','organizations_url':'https://api.github.com/users/bfaliszek/orgs','repos_url':'https://api.github.com/users/bfaliszek/repos','events_url':'https://api.github.com/users/bfaliszek/events{/privacy}','received_events_url':'https://api.github.com/users/bfaliszek/received_events','type':'User','site_admin':false},'content_type':'application/macbinary','state':'uploaded','size':409216,'download_count':15,'created_at':'2018-09-01T09:24:11Z','updated_at':'2018-09-01T09:24:13Z','browser_download_url':'https://github.com/hackerspace-silesia/Smogomierz/releases/download/v2.4.3/Smogomierz_2.4.3_build_19.00_26.08.2018.bin'}],'tarball_url':'https://api.github.com/repos/hackerspace-silesia/Smogomierz/tarball/v2.4.3','zipball_url':'https://api.github.com/repos/hackerspace-silesia/Smogomierz/zipball/v2.4.3','body':'PL:\r\n\r\n+ Możliwość zmiany miernika temperatury/wilgotności/ciśnienia na inny model: BMP280, SHT21 lub HTU21D\r\n+ Możliwość wyłączenia pomiaru zanieczyszczeń powietrza\r\n+ Aktualizacja części używanych bibliotek\r\n\r\nEN:\r\n\r\n+ Now it's possible to change the temperature/humidity/ pressure meter to a different model: BMP280, SHT21 or HTU21D\r\n+ Possibility to turn off the measurement of air pollution\r\n+ Update of some used libraries'}"};

  // exaple data to compare to Data[0]
  //const char SOFTWAREVERSION[40] = "2.4.1 build 13:00_3.11.2018";
  //const char SOFTWAREVERSION[40] = "2.4.3 build 13:00_3.11.2018";
  //const char SOFTWAREVERSION[40] = "2.5.0 build 13:00_3.11.2018";

  int tag_nameIndex = payload.indexOf("tag_name");
  int target_commitishIndex = payload.indexOf("target_commitish");
  int browser_download_urlIndex = payload.indexOf("browser_download_url");
  int end_of_urlIndex = payload.indexOf("}],");
  int SFbuildIndex = String(SOFTWAREVERSION).indexOf("build");

  Data[0] = payload.substring(tag_nameIndex + 12, target_commitishIndex - 3);
  Data[2] = String(SOFTWAREVERSION).substring(0, SFbuildIndex - 1);

  int fileNameIndex = (payload.indexOf("/releases/download/")) + 20 + (Data[0].length() + 1);
  Data[1] = payload.substring(browser_download_urlIndex + 23, fileNameIndex);
  Data[3] = payload.substring(fileNameIndex, end_of_urlIndex - 1);

  Serial.println(" ");
  if (DEBUG) {
    Serial.println("Wersja w repo: " + Data[0]);
    Serial.println("Zainstalowana wersja: " + Data[2]);
    Serial.println("\n");
  }

  int dotIndexRepoSoftVer1 = Data[0].indexOf('.');
  int dotIndexRepoSoftVer2 = Data[0].indexOf('.', dotIndexRepoSoftVer1 + 1);

  int dotIndexCurrentSoftVer1 = Data[2].indexOf('.');
  int dotIndexCurrentSoftVer2 = Data[2].indexOf('.', dotIndexCurrentSoftVer1 + 1);

  String RepoSoftVer = Data[0].substring(0, dotIndexRepoSoftVer1) + Data[0].substring(dotIndexRepoSoftVer1 + 1, dotIndexRepoSoftVer2) + Data[0].substring(dotIndexRepoSoftVer2 + 1);
  String CurrentSoftVer = Data[2].substring(0, dotIndexCurrentSoftVer1) + Data[2].substring(dotIndexCurrentSoftVer1 + 1, dotIndexCurrentSoftVer2) + Data[2].substring(dotIndexCurrentSoftVer2 + 1);

  if (RepoSoftVer.toInt() > CurrentSoftVer.toInt()) {
	//for (int i = 0; i < 5 ; i++) {
    if (DEBUG) {
      Serial.println("Wymagana aktualizacja\n");
    }
	/*
	  // Use WiFiClientSecure class to create TLS connection to raw.githubusercontent.com
	WiFiClientSecure client;
	  if (DEBUG) {
	    Serial.print("connecting to ");
	    Serial.println(host2);
	  }
	  if (!client.connect(host2, httpsPort)) {
	    if (DEBUG) {
	      Serial.println("connection failed");
	    }
	    return false;
	  }
	  if (client.verify(fingerprint2, host2)) {
	    if (DEBUG) {
	      Serial.println("certificate matches");
	    }
	  } else {
	    if (DEBUG) {
	      Serial.println("certificate doesn't match");
	    }
	  }
	
	*/
	return true;
	//doUpdate();
    //delay(100);
	}//}
  if (RepoSoftVer.toInt() == CurrentSoftVer.toInt()) {
    if (DEBUG) {
      Serial.println("Oprogramowanie jest aktualne\n");
    }
	return false;
  }
  if (RepoSoftVer.toInt() < CurrentSoftVer.toInt()) {
    if (DEBUG) {
      Serial.println("Zainstalowano nowsze oprogramowanie niż dostępne w repozytorium\n");
    }
	return false;
  }
}
