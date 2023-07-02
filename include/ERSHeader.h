#include <WiFi.h>
#include <WiFiClient.h>
#include "time.h"
#include "sntp.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

//////////////////////////
// This section up to the end of section is mandatory for every device
//////////////////////////

// State of the puzzle associated to this device (if any)
enum State
{
  Initializing,
  Running,
  Solved
};
State state = Initializing;

const int secondsToUpdateServer = 10; // how many seconds between sending MQTT updates to server
static uint8_t lastSecondT = 99;      // used to detect when time has passed between updates

// WIFI Config
#include "wifi_config.h"
WiFiClient networkClient;

// MQTT Config
// The IPAddresss and port and  on which the MQTT server is running (at first, it can be overriden by the server)
const IPAddress server(192, 168, 5, 160);
const int port = 1883;

// Unique ID used to identify this device on the MQTT network (at first, it can be overriden by the server)
char deviceIDstr[] = "UNCONFIGURED";
char *deviceID = deviceIDstr;
char *deviceMacAddress; // MAC address of the device

  
PubSubClient pc(server, port, networkClient);
// MQTT publish topic
char topicpub[32];
// MQTT subscribe topic
char topicsub[32];

// NTP
// Lista timezones: https://gist.github.com/alwynallan/24d96091655391107939
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
const char *time_zone = "CLT3"; // Santiago

// Create firstTime variable to use in loop
bool firstTime = true;

//////////////////////////
// End of mandatory section
//////////////////////////
