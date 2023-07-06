// CONSTANTS
// how many reed Switches
const byte numSensors = 1;

// which pins are the reed switches connected to
// const byte sensorPins[numSensors] = {3, 4, 5, 7};
const byte sensorPins[numSensors] = {5};

// Which is the in-correct value for the reed switches
const int wrongValue = 1; // All must be open
// const int wrongValue = 0; // All must be closed

// GLOBALS
// Create an array to keep track of the last state of the reed switches
byte lastState[numSensors] = {0};
// Are all the wheels in the correct position?
bool allReedCorrect = false;

