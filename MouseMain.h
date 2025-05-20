#ifndef MOUSE_MAIN_H
#define MOUSE_MAIN_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// --- Maze Data Structures (moved from MazeState.ino for global access) ---
struct MousePosition {
    int8_t x; // Typically column (0-9 for 10 wide maze)
    int8_t y; // Typically row (0-9 for 10 high maze)
};

enum MouseDirection : uint8_t {
    NORTH = 0, // Towards decreasing y
    EAST = 1,  // Towards increasing x
    SOUTH = 2, // Towards increasing y
    WEST = 3   // Towards decreasing x
};

// --- Motor Driver Pins ---
extern const int pwmPinA;      // Motor A Speed (PWM)
extern const int motorA1;      // Motor A Direction 1
extern const int motorA2;      // Motor A Direction 2
extern const int motorB1;      // Motor B Direction 1
extern const int motorB2;      // Motor B Direction 2
extern const int pwmPinB;      // Motor B Speed (PWM)

// --- Sensor Pins ---
extern const int sensorFrontPin;
extern const int sensorLeftPin;
extern const int sensorRightPin;
extern const int Pin4;
extern const int Pin5;

// --- LED Pins ---
extern const int ledPower;     // Control pin for NeoPixel Power
extern const int ledPin;       // NeoPixel Data Pin

// --- Motor Speed Control ---
uint8_t baseSpeed = 235;  // Base motor speed 
uint8_t minSpeed = 130;   // Minimum speed to prevent stalling 
uint8_t maxSpeed = 255;     // Maximum speed
extern const int singleWallMinSpeed;  // Minimum speed for single wall following
extern const int singleWallMaxSpeed;  // Maximum speed for single wall following

// --- Timing Constants ---
extern const int delayBeforeTurning;
extern const int delayBeforeTurningTwice;
extern const int delayAfterTurning;
extern const int timeTurnRight;
extern const int timeTurnLeft;
extern const int timeTurnRightW;
extern const int timeTurnLeftW;
extern const unsigned long initialStepInterval;
extern const unsigned long stepInterval;
extern unsigned long previousTimeMS;
extern unsigned long currentTimeMS;
extern unsigned long lastSquareTime; // Added for global access

// --- Sensor Values ---
extern int filteredLeft;
extern int filteredRight;
extern int filteredFront;
extern int rawLeft;
extern int rawRight;
extern int rawFront;

// --- Wall Thresholds ---
extern const int wallThreshold;
extern const int wallThresholdFront;
extern const int wallThresholdFrontClose;
extern const int wallThresholdRight;
extern const int wallThresholdLeft;

// --- Global Wall Flags (defined in MouseMain.ino, updated by sensors) ---
extern bool GlobalLeftWall;
extern bool GlobalFrontWall;
extern bool GlobalRightWall;

// --- Maze State Global Variables (defined in MazeState.ino) ---
extern MousePosition currentPosition; 
extern MouseDirection currentDirection;

// --- Maze Var ---
// extern enum MouseDirection : uint8_t; // Removed as it's now defined above

// --- Function Declarations from other .ino files ---
void pinSETUP();
void moveF();
void moveB();
void turnLeft();
void turnRight();
void turnLeftW();
void turnRightW();
void stop();
void clearLED();
void colorLED(uint8_t R, uint8_t G, uint8_t B);
void cycleLEDColor();
void updateFilteredSensors();
void updateRawSensors();
void resetSensorHistory();
void initSensors();
void applyPID();
void applySingleWallPID(bool isLeftWall);
void wallFollowingControl();
void wallFollowerRight();
void straightMode();
void updateTiming();

// --- Function Declarations from Sensors.ino ---
void updateGlobalWallFlagsFromSensors(); // New function

// --- Function Declarations from FloodMove.ino ---
void executeDecision();

// --- Function Declarations from MazeState.ino (for simulation & general use) ---
void initializeMazeAndMouse();
void runFloodFill();
void printManhattanDistances(); // Simulation specific
void getSimulatedSensorReadings(bool &left, bool &front, bool &right); // Simulation specific
void addWallsAtCurrentPosition(bool leftWall, bool frontWall, bool rightWall);
void performSimulatedAction(); // Simulation specific
MousePosition getNextPosition(MousePosition current, MouseDirection dir);
MouseDirection getNextDirection(bool turnRight);
// bool canMoveForward(); // Optional: only if called directly from MouseMain.ino, currently not

#endif 