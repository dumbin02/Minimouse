#include "MouseMain.h"
#include <Adafruit_NeoPixel.h>

// Definition of Global Wall Flags
bool GlobalLeftWall = true; // Initialize to a default state (e.g., no wall)
bool GlobalFrontWall = false;
bool GlobalRightWall = true;

// --- Configuration Notes ---
// LED Configuration is in LED-Control.ino
// Motor Configuration is in MotorControl.ino
// PID Configuration is in PID-Control.ino
// Pin Configuration is in Pins-Layout.ino
// Sensor Configuration is in Sensors.ino
// Timing Configuration is in Timing.ino


void setup(){
  pinSETUP();
  delay(1000);
  //cycleLEDColor();
  initializeMazeAndMouse(); // Initialize maze data structures and mouse state
  runFloodFill(); // Initial flood fill before first move decision
  addWallsAtCurrentPosition(GlobalLeftWall, GlobalFrontWall, GlobalRightWall);
  runFloodFill();
  moveF();
  delay(initialStepInterval);
  //cycleLEDColor();
  previousTimeMS = millis();

}

void loop(){
  FloodMove_MainLogic();


}


