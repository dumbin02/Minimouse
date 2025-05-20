#include "MouseMain.h"

// --- Filter Configuration ---
#define FILTER_LEN 8  // Increased for better noise filtering on RP2040
int leftSensorHistory[FILTER_LEN];
int rightSensorHistory[FILTER_LEN];
int frontSensorHistory[FILTER_LEN];
uint8_t historyIndex = 0;
long leftSensorSum = 0;
long rightSensorSum = 0;
long frontSensorSum = 0;

// --- Filtered Values ---
int filteredLeft = 0;
int filteredRight = 0;
int filteredFront = 0;

// --- Raw Sensor Values ---
int rawLeft = 0;
int rawRight = 0;
int rawFront = 0;

// --- Sensor Values Calibration ---
const int wallThreshold = 400;
const int wallThresholdFront = 940;
const int wallThresholdFrontClose = 400;  // Changed from 400 to be closer to wallThresholdFront
const int wallThresholdRight = 900;
const int wallThresholdLeft = 900;

// --- Sensor History Variables ---
int currentLeft = 0;
int currentRight = 0;
int currentFront = 0;

//Iniciar Valores de sensores para PID
void initSensors(){
  resetSensorHistory();
}

void updateRawSensors() {
  rawLeft = analogRead(sensorLeftPin);
  rawRight = analogRead(sensorRightPin);
  rawFront = analogRead(sensorFrontPin);
}

void resetSensorHistory() {
  // Get current readings
  currentLeft = analogRead(sensorLeftPin);
  currentRight = analogRead(sensorRightPin);
  currentFront = analogRead(sensorFrontPin);
  
  // Reset sums
  leftSensorSum = 0;
  rightSensorSum = 0;
  frontSensorSum = 0;
  
  // Fill history with current readings
  for (uint8_t i = 0; i < FILTER_LEN; ++i) {
    leftSensorHistory[i] = currentLeft;
    rightSensorHistory[i] = currentRight;
    frontSensorHistory[i] = currentFront;
    leftSensorSum += currentLeft;
    rightSensorSum += currentRight;
    frontSensorSum += currentFront;
  }
  
  // Reset index
  historyIndex = 0;
  
  // Update filtered values
  filteredLeft = currentLeft;
  filteredRight = currentRight;
  filteredFront = currentFront;
}

void updateFilteredSensors() {
  // Subtract the oldest readings
  leftSensorSum -= leftSensorHistory[historyIndex];
  rightSensorSum -= rightSensorHistory[historyIndex];
  frontSensorSum -= frontSensorHistory[historyIndex];

  // Read new values with multiple samples for better accuracy
  currentLeft = 0;
  currentRight = 0;
  currentFront = 0;
  
  // Take 4 samples and average them
  for(uint8_t i = 0; i < 4; i++) {
    currentLeft += analogRead(sensorLeftPin);
    currentRight += analogRead(sensorRightPin);
    currentFront += analogRead(sensorFrontPin);
  }
  
  currentLeft /= 4;
  currentRight /= 4;
  currentFront /= 4;

  // Add the new readings to the sums
  leftSensorHistory[historyIndex] = currentLeft;
  rightSensorHistory[historyIndex] = currentRight;
  frontSensorHistory[historyIndex] = currentFront;
  
  leftSensorSum += currentLeft;
  rightSensorSum += currentRight;
  frontSensorSum += currentFront;

  // Advance the index
  historyIndex = (historyIndex + 1) % FILTER_LEN;

  // Calculate the filtered averages
  filteredLeft = leftSensorSum / FILTER_LEN;
  filteredRight = rightSensorSum / FILTER_LEN;
  filteredFront = frontSensorSum / FILTER_LEN;
}

// Function to update global boolean wall flags based on raw sensor readings
// Assumes raw sensor values have been recently updated (e.g., by calling updateRawSensors() or updateFilteredSensors() which should update raws)
void updateGlobalWallFlagsFromSensors() {
    // Left Wall Detection
    GlobalLeftWall = (rawLeft < wallThresholdLeft); // Assuming lower value means wall is closer/detected

    // Front Wall Detection
    GlobalFrontWall = (rawFront < wallThresholdFront); // Assuming lower value means wall is closer/detected

    // Right Wall Detection
    GlobalRightWall = (rawRight < wallThresholdRight); // Assuming lower value means wall is closer/detected
}
