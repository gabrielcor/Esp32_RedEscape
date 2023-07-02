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

