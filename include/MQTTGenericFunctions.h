
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
  StaticJsonDocument<512> jsonDoc;
  deserializeJson(jsonDoc, payload, length);
  // Do whatever we need to do with the message we received
  receiveUpdate(jsonDoc);
}
