//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: ERSCheckTimerToSendUpdate.h
// Functions to check time and send update to MQTT
// //////////////////////////

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
