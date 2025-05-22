#include "MouseMain.h"

// --- Timing Configuration ---
const unsigned long initialStepInterval = 375;
const unsigned long stepInterval = 920;
const unsigned long halfStepInterval = stepInterval/2;
unsigned long previousTimeMS = 0;
unsigned long currentTimeMS = 0;


// --- Movement Timing Constants ---
const int delayBeforeTurning = 300;
const int delayBeforeTurningTwice = 500;
const int delayAfterTurning  = 300;

const int timeTurnRight = 695;
const int timeTurnLeft = 675;
const int timeTurnRightW = 825;
const int timeTurnLeftW = 825;

// --- Timing Functions ---
void updateTiming() {
  currentTimeMS = millis();
}

bool isTimeForNextStep() {
  if (currentTimeMS - previousTimeMS >= stepInterval) {
    previousTimeMS = currentTimeMS;
    return true;
  }
  return false;
}
