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
