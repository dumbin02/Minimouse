#include "MouseMain.h"
#include <Adafruit_NeoPixel.h>


// --- Configuration Notes ---
// LED Configuration is in LED-Control.ino
// Motor Configuration is in MotorControl.ino
// PID Configuration is in PID-Control.ino
// Pin Configuration is in Pins-Layout.ino
// Sensor Configuration is in Sensors.ino
// Timing Configuration is in Timing.ino


void setup(){
  // --- Original Setup (Commented out for Simulation) ---
  stop();
  initializeMazeAndMouse(); // This will be called by simulation setup
  addWallsAtCurrentPosition(GlobalLeftWall, GlobalFrontWall, GlobalRightWall); 
  runFloodFill();           // Initial flood fill, from MazeState.ino
  pinSETUP();
  delay(3000);        //Delay para que no avanze de inmediato
  initSensors();
  moveF();
  delay(initialStepInterval);
  colorLED(255,0,0);
  lastSquareTime = millis();
  // --- Simulation Setup ---
  //Serial.begin(115200);  // Initialize Serial for debugging and simulation
  //while (!Serial) { delay(10); } // Wait for Serial to connect
  //Serial.println("Micromouse Maze State Simulator Initialized (from MouseMain.ino).");
  //Serial.println(F("--- Initial State (Before any move) ---"));
  //printManhattanDistances(); // From MazeState.ino

  
}

void loop(){
  FloodMove_MainLogic();
  
}
