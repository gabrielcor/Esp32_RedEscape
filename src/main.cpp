// Includes all the necessary libraries and defines the main functions for Escape Room System (ERS)
#include "ERSHeader.h"

// We only have to modify in the sections that are surrouned by the following comments:
// //////////////////////////
// #pragma region "Add code ..."


// Tags to include/exclude functionalities
// #define BLETag
#define WLEDTag

#ifdef BLETag
#include "BLE\ERSBleHeader.h"
#endif

#ifdef WLEDTag
#include "WLED\ERSWledHeader.h"
#endif


#pragma region "Send and Receive from MQTT"

// Act upon message received via MQTT
void receiveUpdate(const JsonDocument &jsonDoc)
{
  // Act upon command received for basic solved / unsolved functionality
  if (jsonDoc["command"] == "SOLVE")
  {
    state = Solved;
  }
  else if (jsonDoc["command"] == "RESET")
  {
    state = Running;
  }

//////////////////////////
#pragma region "Add code to act upon message received from ERS Server for each installed functionality"
  #ifdef WLEDTag
    #include "WLED\ERSWledReceiveMessage.h"
  #endif
#pragma endregion 
//////////////////////////

}


// This function is where we send updates to the MQTT server
void sendUpdate(char locTime[9])
{
  // Create JSON document
  // General data for any device: id, state, time
  StaticJsonDocument<128> jsonDoc;
  jsonDoc["id"] = deviceID;
  jsonDoc["state"] = (state == State::Solved) ? "SOLVED" : "UNSOLVED";
  jsonDoc["time"] = locTime;

//////////////////////////
#pragma region "Add code to add data to the message for each installed functionality"
  #ifdef WLEDTag
    #include "WLED\ERSWledSendMessage.h"
  #endif  
#pragma endregion
//////////////////////////


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

// Include Generic MQTT functions for ERS (here because of dependencies)
#include "MQTTGenericFunctions.h"
// Include Generic Check Timer to send update
#include "ERSCheckTimerToSendUpdate.h"

#pragma endregion // end Send and Receive from MQTT


//////////////////////////
#pragma region "Add code for create a Task Code with functionality that you want to run on Core 0"

#ifdef BLETag
  #include "BLE\ERSBleTaskCode.h"
#endif
#pragma endregion // end Task Code for functionality that runs on Core 0
//////////////////////////

void setup()
{
#include "ERSGenericSetup.h"


//////////////////////////
#pragma region "Add code to Setup each installed functionality"

// BLE Setup includes execution on Core 0
#ifdef BLETag
  #include "BLE\ERSBleSetup.h"
#endif

// WLED Setup is just call to setup functions
#ifdef WLEDTag
  #include "WLED\ERSWledSetup.h"
#endif
//////////////////////////

#pragma endregion // end Setup for each installed functionality


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


//////////////////////////
#pragma region "Add code for Functionality to execute on main loop (Core 1)"
  #ifdef WLEDTag
    #include "WLED\ERSWledLoop.h"
  #endif
#pragma endregion // end Functionality to execute on main loop (Core 1)
//////////////////////////

}