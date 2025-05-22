#include "MouseMain.h"

// --- Timing Variables ---
static unsigned long lastUpdate = 0;
static unsigned long currentTime = 0;
unsigned long lastSquareTime = 0;

static uint8_t lastMove = 0;

// --- Global Initial Wall States for Simulation/Setup ---
// bool GlobalLeftWall = true;  // Default: assumes side wall after first move
// bool GlobalFrontWall = false; // Default: assumes path ahead is clear after first move
// bool GlobalRightWall = true; // Default: assumes side wall after first move
// These are now defined in MouseMain.ino and declared as extern in MouseMain.h
bool previousGlobalLeftWall = GlobalLeftWall; // This will use the extern definition
bool previousGlobalRightWall = GlobalRightWall; // This will use the extern definition
MousePosition previousPosition = currentPosition;

uint8_t bestMoveDecision = 0;

// Function prototypes (if any private to this file)
void executeDecision(int decision);

void FloodMove_MainLogic() {
    // Only update sensors and walls during first 1/3 of step interval for full evaluation
        updateRawSensors();
        // If significant side sensor activity, pause and re-read for stability
        if((rawLeft > wallThresholdLeft || rawRight > wallThresholdRight) ){
            delay(50);
            updateRawSensors();
        }
        // Perform main logic for the first part of the step: update all wall flags
        if(millis() - lastSquareTime < (stepInterval*2 /3)){
            updateGlobalWallFlagsFromSensors();
            addWallsAtCurrentPosition(GlobalLeftWall, GlobalFrontWall, GlobalRightWall);
        }
            runFloodFill();
            bestMoveDecision = getNextBestMove();
            executeDecision(bestMoveDecision);
        
}

void executeDecision(int decision) {
    switch (decision) {
        case 0: // Go Forward
            updateFilteredSensors(); // Keep this to have fresh sensor data for PID
            // PID or direct motor control for forward movement
            if (filteredLeft < wallThresholdLeft && filteredRight < wallThresholdRight) {
                applyPID();
            } else if (filteredRight < wallThresholdRight) {
                applySingleWallPID(false);  // false for right wall
            } else if (filteredLeft < wallThresholdLeft) {
                applySingleWallPID(true);  // true for left wall
            } else {
                // No walls detected - go straight using baseSpeed
                moveF();
            }
            if (millis() - lastSquareTime >= stepInterval) { 
                currentPosition = getNextPosition(currentPosition, currentDirection); // Update state
                lastSquareTime = millis(); // Update time for entering new square
            }
            break;

        case 1: // Turn Right
             if (millis() - lastSquareTime > (stepInterval / 2)) {
                currentPosition = getNextPosition(currentPosition, currentDirection);
                lastSquareTime = millis();
            }
            turnRightW(); 
            currentDirection = getNextDirection(true); // Update direction
            currentPosition = getNextPosition(currentPosition, currentDirection);
            lastSquareTime = millis();
            break;

        case 2: // Turn Left
            if (millis() - lastSquareTime > (stepInterval / 2)) {
                currentPosition = getNextPosition(currentPosition, currentDirection);
                lastSquareTime = millis();
            }
            turnLeftW(); 
            currentDirection = getNextDirection(false); // Update direction
            currentPosition = getNextPosition(currentPosition, currentDirection);
            lastSquareTime = millis();
            break;

        case 3: // Turn Around
            while(rawFront > wallThresholdFrontClose){
                updateFilteredSensors();
                applyPID();
                updateRawSensors();
            }
            turnRight();    
            turnRight(); 
            moveF();
            currentDirection = getNextDirection(true); 
            currentDirection = getNextDirection(true); 
            //currentPosition = getNextPosition(currentPosition, currentDirection);
            delay(100);
            lastSquareTime = millis();
            break;

        case 4: // Stuck - all adjacent cells are unreachable
            stop();
            colorLED(255,0,0);
            
            while(1) { // Outer loop for continuous operation
                if (!Serial) { // If serial is not connected
                    Serial.end(); // End current serial connection
                    delay(1000); // Give time for serial to close
                    Serial.begin(115200); // Try to initialize serial
                    delay(1000); // Wait for connection
                }
                
                if (Serial) { // If serial is connected
                    printManhattanDistances();
                    delay(10000); // Print every 10 seconds
                }
            }
            break;
    }
} 
