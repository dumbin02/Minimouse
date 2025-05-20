#include "MouseMain.h"

// --- Timing Variables ---
static unsigned long lastUpdate = 0;
static unsigned long currentTime = 0;
unsigned long lastSquareTime = 0;

// --- Global Initial Wall States for Simulation/Setup ---
bool GlobalLeftWall = true;  // Default: assumes side wall after first move
bool GlobalFrontWall = false; // Default: assumes path ahead is clear after first move
bool GlobalRightWall = true; // Default: assumes side wall after first move

static uint8_t bestMoveDecision = 0;

// Function prototypes (if any private to this file)
void executeDecision(int decision);

void FloodMove_MainLogic() {
    // 1. SENSE & UPDATE WALL FLAGS
    updateRawSensors();
    updateGlobalWallFlagsFromSensors();

    // 2. UPDATE MAZE MAP
    addWallsAtCurrentPosition(GlobalLeftWall, GlobalFrontWall, GlobalRightWall);

    // 3. RECALCULATE PATHS
    runFloodFill();

    // 4. GET BEST MOVE DECISION
    bestMoveDecision = getNextBestMove();

    // 5. EXECUTE THE MOVE
    executeDecision(bestMoveDecision);

    // squaresAdvanced logic would be tied to successful forward moves in executeDecision
}

// This function determines the next best move using flood fill logic
// and then commands the robot to perform the action (turn/move).
// It assumes that sensor readings have been processed, wall flags updated,
// and flood fill has been run *before* this function is called if map changed.
void executeDecision(int decision) {
    switch (decision) {
        case 0: // Go Forward
            updateFilteredSensors();
        // Check if it's time to add 1 move
            if  (currentTime - lastSquareTime >= stepInterval){
                lastSquareTime = currentTime;
                currentPosition = getNextPosition(currentPosition, currentDirection); // Update state
            }
            // Check for both walls first (dual wall mode)
            else if (filteredLeft < wallThresholdLeft && filteredRight < wallThresholdRight) {
                applyPID();
            }
            // If only one wall is detected, use single wall PID
            else if (filteredRight < wallThresholdRight) {
                // Only right wall detected - follow right wall
                applySingleWallPID(false);  // false for right wall
            }
            else if (filteredLeft < wallThresholdLeft) {
                // Only left wall detected - follow left wall
                applySingleWallPID(true);  // true for left wall
            }
            else {
                // No walls detected - go straight
                analogWrite(pwmPinA, baseSpeed);
                analogWrite(pwmPinB, baseSpeed + 8);
            }
            break;

        case 1: // Turn Right, then move if intended by overall strategy
            turnRightW();
            moveF();
            currentDirection = getNextDirection(true); // Update state
            currentPosition = getNextPosition(currentPosition, currentDirection); // Update state
            break;

        case 2: // Turn Left, then move if intended
            turnLeftW();
            moveF();
            currentDirection = getNextDirection(false); // Update state
            currentPosition = getNextPosition(currentPosition, currentDirection); // Update state
            break;

        case 3: // Turn Around, then move if intended
            stop();
            turnRight(); 
            turnRight();
            currentDirection = getNextDirection(true);
            currentDirection = getNextDirection(true);
            currentPosition = getNextPosition(currentPosition, currentDirection); // Update state
            break;

        default:
            stop(); // Physical action
            break;
    }
}
