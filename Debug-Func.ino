#include "MouseMain.h"

void sensorDebug(){
    // Update sensor readings
    
    // Print raw sensor values
    Serial.print("Raw - Front: ");
    Serial.print(rawFront);
    Serial.print(" Left: ");
    Serial.print(rawLeft);
    Serial.print(" Right: ");
    Serial.println(rawRight);
    
    // Print filtered sensor values
    Serial.print("Filtered - Front: ");
    Serial.print(filteredFront);
    Serial.print(" Left: ");
    Serial.print(filteredLeft);
    Serial.print(" Right: ");
    Serial.println(filteredRight);
    
    // Print wall detection status
    Serial.print("Walls - Front: ");
    Serial.print(rawFront < wallThresholdFront ? "Yes" : "No");
    Serial.print(" Left: ");
    Serial.print(rawLeft < wallThresholdLeft ? "Yes" : "No");
    Serial.print(" Right: ");
    Serial.println(rawRight < wallThresholdRight ? "Yes" : "No");
    
    Serial.println("-------------------");
}

void rawSensorDebug(){
    // Update sensor readings
    updateRawSensors();
    stop();
    // Print raw sensor values

    Serial.print(rawFront);
    Serial.print(" ");
    Serial.print(rawLeft);
    Serial.print(" ");
    Serial.println(rawRight);
    delay(10);
}

// --- Simulation Helper Functions ---
void printManhattanDistances() {
    Serial.println(F("Maze State (M=Mouse, Inf=Unvisited):"));
    for (int8_t r = 0; r < MAZE_HEIGHT; r++) {
        // Print cell row with values and vertical walls
        for (int8_t c = 0; c < MAZE_WIDTH; c++) {
            // Cell Content (Mouse or Distance)
            if (r == currentPosition.y && c == currentPosition.x) {
                Serial.print(F("M"));
                switch (currentDirection) {
                    case NORTH: Serial.print(F("^")); break;
                    case EAST:  Serial.print(F(">")); break;
                    case SOUTH: Serial.print(F("v")); break;
                    case WEST:  Serial.print(F("<")); break;
                }
                Serial.print(F(" ")); // Pad to 3 chars, e.g., "M^ "
            } else {
                if (manhattanDistances[r][c] == UNVISITED_DISTANCE) {
                    Serial.print(F("Inf"));
                } else {
                    if (manhattanDistances[r][c] < 10) {
                        Serial.print(F(" ")); Serial.print(manhattanDistances[r][c]); Serial.print(F(" ")); // e.g., " 9 "
                    } else if (manhattanDistances[r][c] < 100) {
                        Serial.print(manhattanDistances[r][c]); Serial.print(F(" ")); // e.g., "10 "
                    } else {
                        Serial.print(manhattanDistances[r][c]); // e.g., "123"
                    }
                }
            }

            // Vertical Wall to the right (if not the last column)
            if (c < MAZE_WIDTH - 1) {
                if (verticalWalls[r][c] == 1) {
                    Serial.print(F("|"));
                } else {
                    Serial.print(F(" ")); // Space if no vertical wall (acts as separator)
                }
            }
        }
        Serial.println(); // End of cell content row

        // Print horizontal wall row (if not the last cell row)
        if (r < MAZE_HEIGHT - 1) {
            for (int8_t c = 0; c < MAZE_WIDTH; c++) {
                if (horizontalWalls[r][c] == 1) {
                    Serial.print(F("---"));
                } else {
                    Serial.print(F("   ")); // Spaces if no horizontal wall
                }
                if (c < MAZE_WIDTH - 1) {
                    Serial.print(F(" ")); // Space between horizontal wall segments (aligns with vertical wall space)
                }
            }
            Serial.println(); // End of horizontal wall row
        }
    }
    Serial.println(F("------------------------------------"));
}

void getSimulatedSensorReadings(bool &left, bool &front, bool &right) {
    left = false; front = false; right = false;
    Serial.print("Enter detected walls (e.g., 'LFR' for Left, Front, Right, 'N' for None): ");
    while (Serial.available() == 0) { delay(10); }
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toUpperCase();
    if (input.indexOf('L') != -1) left = true;
    if (input.indexOf('F') != -1) front = true;
    if (input.indexOf('R') != -1) right = true;
    Serial.print("Simulated walls: ");
    if (left) Serial.print("Left ");
    if (front) Serial.print("Front ");
    if (right) Serial.print("Right ");
    if (!left && !front && !right) Serial.print("None");
    Serial.println();
}

void performSimulatedAction() {
    Serial.print("Enter action (F=Forward, L=Turn Left, R=Turn Right): ");
    while (Serial.available() == 0) { delay(10); } 
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toUpperCase();
    if (input == "F") {
        if (canMoveForward()) {
            currentPosition = getNextPosition(currentPosition, currentDirection);
            Serial.println("Moved Forward.");
        } else {
            Serial.println("Cannot move forward, wall detected or boundary!");
        }
    } else if (input == "L") {
        currentDirection = getNextDirection(false);
        Serial.println("Turned Left.");
    } else if (input == "R") {
        currentDirection = getNextDirection(true);
        Serial.println("Turned Right.");
    } else {
        Serial.println("Invalid action.");
    }
}

void simulateMouseMovement(){
  getSimulatedSensorReadings(GlobalLeftWall, GlobalFrontWall, GlobalRightWall);
  addWallsAtCurrentPosition(GlobalLeftWall, GlobalFrontWall, GlobalRightWall);
  runFloodFill();
  bestMoveDecision = getNextBestMove();
  Serial.print("Best Move Decision: ");
  Serial.println(bestMoveDecision);
  executeDecision(bestMoveDecision);
  printManhattanDistances();
}

void setupSimulatedMouse(){
  Serial.begin(115200);
  while(!Serial){
    delay(1000);
  }
  initializeMazeAndMouse(); // Initialize maze data structures and mouse state
  runFloodFill(); // Initial flood fill before first move decision
  addWallsAtCurrentPosition(GlobalLeftWall, GlobalFrontWall, GlobalRightWall);
  runFloodFill();
  currentPosition = getNextPosition(currentPosition, currentDirection);
  printManhattanDistances(); // Show initial maze start
}