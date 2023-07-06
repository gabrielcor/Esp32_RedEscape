allReedCorrect = true;
  // Loop over every sensor
for(int i=0; i<numSensors; i++){
    // Read the current value
    byte currentState = !digitalRead(sensorPins[i]);
    // Preserve state 
    lastState[i] = currentState;
    // Is it wrong? One wrong - all wrong
    if(currentState == wrongValue){
      allReedCorrect = false;
    }
  }

