
#include <DHT.h>
#include <SdsDustSensor.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <CronAlarms.h>
#include <sys/time.h> 
#include <ArduinoHttpClient.h>
#include "DHT22Data.h"
#include "BMP280Data.h"
#include "SDS011Data.h"
#include "CCS811Data.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include "Adafruit_CCS811.h"
#include <Adafruit_BMP280.h>

Adafruit_CCS811 ccs;
Adafruit_BMP280 bmp;

#define DHTPIN D4 // for DHT22
#define RX_PIN D7 // for TXD from SDS011
#define TX_PIN D8 // for RXD from SDS011


#define DHTTYPE DHT22 

DHT dht(D4, DHT22);
SoftwareSerial mySerial(RX_PIN, TX_PIN);
SdsDustSensor sensor(mySerial);

const char* ssid = "MY_SSID"; // WLAN NETWORK NAME 
const char* password = "MY_PASSWORD"; // WLAN PASSWORD

char serverAddress[] = "MY_SERVER_ADDRESS"; // e.g., "example.com"
int port = MY_PORT; // e.g., 8080 or 443 for HTTPS
WiFiClient wifiClient;
HttpClient httpClient = HttpClient(wifiClient, serverAddress, port);
CronId id;

void setup() {
  Serial.begin(115200);
  
  while (!Serial);
  struct tm tm_newtime; // set time to Saturday 8:29:00am Jan 1 2011
  tm_newtime.tm_year = 2011 - 1900;
  tm_newtime.tm_mon = 1 - 1;
  tm_newtime.tm_mday = 1;
  tm_newtime.tm_hour = 6;
  tm_newtime.tm_min = 29;
  tm_newtime.tm_sec = 0;
  tm_newtime.tm_isdst = 0;
  #if defined(ESP8266) || defined(ESP32)
    timeval tv = { mktime(&tm_newtime), 0 };
    settimeofday(&tv, nullptr);
  #elif defined(__AVR__)
    set_zone(2 * 60);
    set_dst(0);
    set_system_time( mktime(&tm_newtime) );
  #endif

  connectToWiFi(ssid, password);

  // BMP280
  Wire.begin(12, 14);
  if (!bmp.begin(0x77)) { 
    Serial.println("BMP280 nicht gefunden!");
    while (1);
  }
  
  // DHT22
  dht.begin();

  // CCS811
  Wire.begin(D2, D1);
  if(!ccs.begin()){
    Serial.println("CCS811 not found!");
    while(1);
  }
  while(!ccs.available()){
    Serial.println("Waiting for CCS811 to be ready...");
    delay(1000);
  }

  // SDS011
  sensor.begin();        
  sensor.wakeup();

  Serial.println(sensor.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sensor.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sensor.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended

  // create the alarms, to trigger at specific times
  id = Cron.create("0 */1 * * * *", collectData, false);  //every 1 min
  
  Serial.println("Setup Done!");
  
}

void loop() {
  #ifdef __AVR__
    system_tick(); // must be implemented at 1Hz
  #endif
  time_t tnow = time(nullptr);
  struct tm *utcTime = gmtime(&tnow);
  utcTime->tm_hour += 2;
  
  // Handle overflow in case adding 2 hours goes beyond 24
  if (utcTime->tm_hour >= 24) {
    utcTime->tm_hour -= 24;
  }
  Serial.println(asctime(utcTime));
  Cron.delay();// if the loop has nothing else to do, delay in ms
               // should be provided as argument to ensure immediate
               // trigger when the time is reached
  delay(1000);
}

