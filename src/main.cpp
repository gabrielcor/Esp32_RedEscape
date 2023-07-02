// Includes all the necessary libraries and defines the main functions for Escape Room System (ERS)
#include "ERSHeader.h"

// BLE

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>

int scanTime = 1;               // duration in seconds
const int delayBLEScan = 100;   // delay between scans
const int RSSI_1M = -53;        // RSSI at 1 meter distance
const int SIGNAL_LOSS = 2;      // Signal loss factor per meter
const int RSSI_THRESHOLD = -70; // RSSI threshold for detection

BLEScan *pBLEScan;

double rssiToDistance(int rssi)
{
  double signalLoss = rssi - RSSI_1M;
  double distance = pow(10.0, signalLoss / (-10.0 * SIGNAL_LOSS));
  return distance;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    if (advertisedDevice.haveManufacturerData() == true)
    {
      int rssi = advertisedDevice.getRSSI();
      if (rssi < RSSI_THRESHOLD)
      {
        return;
      }

      // check that advertised name is "78-beacon-01" and if its not return
      if (advertisedDevice.getName() != "78-beacon-01")
      {
        return;
      }
      Serial.print("RSSI: ");
      Serial.println(rssi);

      std::string strManufacturerData = advertisedDevice.getManufacturerData();

      uint8_t cManufacturerData[100];
      strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

      if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
      {
        if (advertisedDevice.haveName())
        {
          Serial.print("Device name: ");
          Serial.println(advertisedDevice.getName().c_str());
        }
        BLEBeacon oBeacon = BLEBeacon();
        oBeacon.setData(strManufacturerData);
        Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n", oBeacon.getManufacturerId(), ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
        Serial.print("iBeacon RSSI: ");
        Serial.println(rssi);
        // Convert RSSI to distance in centimeters
        // Convert RSSI to distance in centimeters
        double distance = rssiToDistance(rssi);
        Serial.print("Distance: ");
        Serial.println(distance);
        Serial.println();
      }
    }
  }
};

// WLED
#include <FastLED.h>
int fadeAmount = 5; // Set the amount to fade I usually do 5, 10, 15, 20, 25 etc even up to 255.
int brightness = 0;
int speedDelay = 25;
int glow_red = 0;
int glow_green = 0;
int glow_blue = 0;

#define LED_PIN 15
#define NUM_LEDS 12
#define BRIGHTNESS 64
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

// Connect to MQTT Server
void connectToMQTT()
{
  // Loop until we're reconnected
  int retries = 0;
  while (!pc.connected())
  {
    if (pc.connect(deviceID))
    {
      // Subscribe to topics meant for this device
      char topic[32];
      snprintf(topic, 32, "ToDevice/%s/#", deviceID);
      pc.subscribe(topic);
      Serial.println(F("Connected to MQTT server"));
    }
    else
    {
      Serial.println(F("Could not connect to MQTT server, rc="));
      Serial.print(pc.state());
      retries++;
      if (retries == 3)
      {
        break;
      }
      // Wait 1 seconds before retrying
      delay(1000);
    }
    Serial.print("loop() running on core ");
    Serial.println(xPortGetCoreID());
  }
}

//////////////////////////
// Actions we perform on received messages and commands
//////////////////////////

// Act upon message received via MQTT
void receiveUpdate(const JsonDocument &jsonDoc)
{

  // Act upon command received
  if (jsonDoc["command"] == "SOLVE")
  {
    state = Solved;
  }
  else if (jsonDoc["command"] == "RESET")
  {
    state = Running;
  }

  // Act upon color received on rgb node
  if (jsonDoc["rgb"] != nullptr)
  {
    String rgb = jsonDoc["rgb"];
    int pos = rgb.indexOf(',');
    String red = rgb.substring(0, pos);
    rgb = rgb.substring(pos + 1);
    pos = rgb.indexOf(',');
    String green = rgb.substring(0, pos);
    String blue = rgb.substring(pos + 1);
    glow_red = red.toInt();
    glow_green = green.toInt();
    glow_blue = blue.toInt();
  }
}

// MQTT Callback
// Gets called every time we get a message for this subscription
void processMessage(char *topic, byte *payload, unsigned int length)
{
  // Copy the payload received to a character string
  char msg[200];
  memcpy(msg, payload, length);
  msg[length] = '\0';
  Serial.print(F("Received over MQTT:"));
  Serial.println(msg);
  String msgString = String(msg);

  // Create a JSON document from the MQTT message received. Note best practice is NOT to have a reusable
  // JSON document, but create a new one each time it is needed.  https://arduinojson.org/v6/assistant/
  StaticJsonDocument<128> jsonDoc;
  deserializeJson(jsonDoc, payload, length);
  // Do whatever we need to do with the message we received
  receiveUpdate(jsonDoc);
}

//////////////////////////
// This function is where we send updates to the MQTT server
// Anything that we need to inform the server of, we send here
//////////////////////////
void sendUpdate(char locTime[9])
{
  // Create JSON document
  // General data for any device: id, state, time
  StaticJsonDocument<128> jsonDoc;
  jsonDoc["id"] = deviceID;
  jsonDoc["state"] = (state == State::Solved) ? "SOLVED" : "UNSOLVED";
  jsonDoc["time"] = locTime;

  // WLED - add leading zeroes to glow values and send the actual values for the WLED
  char rgb_values[11];
  sprintf(rgb_values, "%03d,%03d,%03d", glow_red, glow_green, glow_blue);
  jsonDoc["rgb"] = rgb_values;

  // Send to MQTT
  if (pc.connected())
  {
    char mqttMsg[128];
    serializeJson(jsonDoc, mqttMsg);
    pc.publish(topicpub, mqttMsg);
  }
  // print jsondoc to Serial
  serializeJson(jsonDoc, Serial);
  Serial.println();
}

// Show local time and send to MQTT
void printLocalTime()
{
  struct tm timeinfo;
  char locTime[9];
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("No time available (yet)");
    snprintf(locTime, 9, "00:00:00");
  }
  else
  {
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    sprintf(locTime, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  }
  sendUpdate(locTime);
}

// Function that receives seconds as input and a number as input and returns seconds rounded to that number
int roundSeconds(int seconds, int roundTo)
{
  int remainder = seconds % roundTo;

  // update below to return only seconds up to 59 (not 60)
  if (remainder > (roundTo / 2))
  {
    seconds = seconds + (roundTo - remainder);
  }
  else
  {
    seconds = seconds - remainder;
  }
  if (seconds > 59)
  {
    seconds = 0;
  }
  return seconds;
}

// Update time
void updateTime()
{
  uint8_t secondHandT = (millis() / 1000) % 60;
  secondHandT = roundSeconds(secondHandT, secondsToUpdateServer);
  if (lastSecondT != secondHandT)
  {
    lastSecondT = secondHandT;
    printLocalTime(); // it will take some time to sync time :)
  }
}

// Glow Pixels WLED
void movepixelsGlow()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].setRGB(glow_red, glow_green, glow_blue);
    leds[i].fadeLightBy(brightness);
  }
  FastLED.show();
  brightness = brightness + fadeAmount;
  // reverse the direction of the fading at the ends of the fade:
  if (brightness == 0 || brightness == 255)
  {
    fadeAmount = -fadeAmount;
  }
  delay(speedDelay);
}

// Execute BLE Scan
void Task1code(void *parameter)
{
  Serial.print("Task1code() running on core ");
  Serial.println(xPortGetCoreID());

  for (;;)
  {
    // put your main code here, to run repeatedly:
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
    delay(delayBLEScan);
  }
}

void setup()
{
  // setup MQTT
  snprintf(topicpub, 32, "FromDevice/%s", deviceID);
  snprintf(topicsub, 32, "ToDevice/%s/#", deviceID);

  Serial.begin(115200);
  Serial.println("Starting ...");

  // SETUP NTP
  Serial.println("Setting up NTP");
  sntp_servermode_dhcp(1); // (optional)
  configTzTime(time_zone, ntpServer1, ntpServer2);

  // setup Wifi
  Serial.printf("Connecting to %s ", ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());
  
  // Get Mac Address and assign it to deviceMacAddress
  Serial.print(F("MAC Address: "));
  String macAddress = WiFi.macAddress();
  macAddress.toCharArray(deviceMacAddress, 32);
  Serial.println(macAddress);

  // Setup BLE
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); // create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); // active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value

  // Multitask setup
  // Things that will run on other Cores
  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1",   /* Name of the task */
      10000,     /* Stack size in words */
      NULL,      /* Task input parameter */
      0,         /* Priority of the task */
      NULL,      /* Task handle. */
      0);        /* Core where the task should run */

  // Setup WLED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  // Specify the callback that processes any messages received on an MQTT topic to which we are subscribed
  pc.setCallback([](char *topic, byte *payload, unsigned int length)
                 {
                   // Create function to process message based on the topic and payload
                   processMessage(topic, payload, length); });
}

void loop()
{
  updateTime();
  connectToMQTT();
  // Call the main MQTT loop to check for and publish messages
  if (pc.connected())
  {
    pc.loop();
  }
  // Move pixels
  movepixelsGlow();
}