String sensorValues = "";

for(int i=0; i<numSensors; i++){
    // Read the current value
    byte currentState =  lastState[i] ;
    if(currentState == wrongValue)
      sensorValues += "0";
      else
        sensorValues += "1";
    }
  
jsonDoc["REEDsensorValues"] = sensorValues;
if (allReedCorrect)
  jsonDoc["REEDsensorState"] = "1";
else
  jsonDoc["REEDsensorState"] = "0";
