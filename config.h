// config.h file contain all settings for device and sensors
#pragma once


struct DeviceSettings {
    bool autoName = true;
    char name[32] = "Smogly"; // Without Polish letters!!
    bool displayPM1 = false;
    char language[12] = "english"; // english or polish
    unsigned short selectedLanguage = 1; // en - 1; pl - 2
    unsigned short sendingServicesFrequency = 3;
    unsigned short sendingDBFrequency = 3;
    bool deepSleep = false;
    bool debug = true;
    bool autoUpdate = true;
    bool autoCalibration = false; // char model[12]
    float calib1 = 1.0;
    float calib2 = 1.0;
    char latitude[16] = "50.263911";
    char longitude[16] = "18.995711";
    short altitude = 271;
};

struct SensorsSettings {
    char thpModel[12] = "Non";
    bool secondThp = false;
    char secondThpModel[12] = "Non";
    char dustModel[12] = "Non";
    bool continuousMeasurement = false; // frequentMeasurement;
    unsigned short dustTime = 1;
    unsigned short numerOfMeasurements = 10;
};

struct I2CSettings {
    char sda[4] = ""; // TX - for dust senros
    char scl[4] = ""; // RX - for dust senros
    unsigned short address_sda = 0;
    unsigned short address_scl = 2;
};


struct SmoglistSettings {
    bool enabled = false;
};

struct LuftdatenSettings {
    bool enabled = false;
    bool graph = false;
    unsigned int apiId = 0;
};

struct AirMonitorSettings {
    bool enabled = false;
    bool graph = false;
    char apiKey[48] = "";
    char email[64] = "email@mail.pl";
};

struct ThingSpeakSettings {
    bool enabled = false;
    bool graph = false;
    char apiKey[32] = "WRITE_API_KEY";
    unsigned int channelId = 0;
    char readApiKey[32] = "READ_API_KEY";
};

struct AQIEcoSettings {
    bool enabled = false;
    char host[64] = "???.aqi.eco";
    char path[64] = "/u/???";
};

struct InfluxDBSettings {
    bool enabled = false;
    char version[4] = "1";
    char host[128] = "INFLUXDB_ADRESS";
    uint16_t port = 8086;
    char database[32] = "mydb";
    char user[32] = "username";
    char password[64] = "password";
    char org[32] = "myOrg";
    char bucket[32] = "myBucket";
    char token[128] = "myToken";
};

struct MQTTSettings {
    bool enabled = false;
    char host[128] = "MQTT_ADRESS";
    uint16_t port = 1883;
    char user[32] = "username";
    char password[64] = "password";
    bool ipInTopic = false;
    bool deviceNameInTopic = true;
    bool slashAtBeginning = true;
    bool slashAtEnd = true;
    char tempTopic[64] = "sensor/temperature";
    char humiTopic[64] = "sensor/humidity";
    char pressTopic[64] = "sensor/pressure";
    char pm1Topic[64] = "sensor/PM1";
    char pm25Topic[64] = "sensor/PM2.5";
    char pm10Topic[64] = "sensor/PM10";
    char airQualityTopic[32] = "airquality";
};

struct HomeKitSettings {
    bool enabled = false;
    char password[16] = "111-11-111";
};

struct AuthSettings {
    bool enabled = false;
    char username[64] = "admin";
    char password[128] = "admin";
};

struct MeasurementsData {
    float temperature = 0;
    unsigned short humidity = 0;
    unsigned short pressure = 0;
    float first_temperature = 0;
    unsigned short first_humidity = 0;
    unsigned short first_pressure = 0;
    float second_temperature = 0;
    unsigned short second_humidity = 0;
    unsigned short second_pressure = 0;
    unsigned short averagePM1 = 0;
    unsigned short averagePM25 = 0;
    unsigned short averagePM4 = 0;
    unsigned short averagePM10 = 0;
};

extern DeviceSettings deviceSettings;
extern SensorsSettings sensorsSettings;
extern I2CSettings firstThpSettings;
extern I2CSettings secondThpSettings;
extern I2CSettings dustSettings;
extern SmoglistSettings smoglistSettings;
extern LuftdatenSettings luftdatenSettings;
extern AirMonitorSettings airMonitorSettings;
extern ThingSpeakSettings thingSpeakSettings;
extern InfluxDBSettings influxDBSettings;
extern MQTTSettings mqttSettings;
extern AQIEcoSettings aqiEcoSettings;
extern HomeKitSettings homeKitSettings;
extern AuthSettings authSettings;
extern MeasurementsData measurementsData;


extern char serverSoftwareVersion[32];
extern char currentSoftwareVersion[32];

#ifdef DUSTSENSOR_PMS5003_7003_BME280_0x76 or DUSTSENSOR_PMS5003_7003_BME280_0x77
const char pmSensorVersion[] PROGMEM = "PMS";
#elif defined DUSTSENSOR_SDS011_21
const char pmSensorVersion[] PROGMEM = "SDS";
#elif defined DUSTSENSOR_HPMA115S0
const char pmSensorVersion[] PROGMEM = "HPMA115S0";
#elif defined DUSTSENSOR_SPS30
const char pmSensorVersion[] PROGMEM = "SPS30";
#else // If no dust sensor has been defined - use DUSTSENSOR_PMS5003_7003_BME280_0x76
const char pmSensorVersion[] PROGMEM = "PMS";
#endif

#if defined(ARDUINO_ARCH_ESP8266)
const char softwareVersion[] PROGMEM = "2.8.3 build " __TIME__ " " __DATE__;
const char hardwareVersion[] PROGMEM = "1.0 - ESP8266";
#elif defined(ARDUINO_ARCH_ESP32)
const char softwareVersion[] PROGMEM = "2.8.3 build " __TIME__ " " __DATE__;
const char hardwareVersion[] PROGMEM = "2.0 - ESP32";
#endif