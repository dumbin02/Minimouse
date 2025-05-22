// --- PID Configuration ---
// Dual Wall Following PID Constants
const double Kp = 0.1;  // P
const double Kd = 0.2;   // D

// Single Wall Following PID Constants
const double KpSingle = 0.1;  // P
const double KdSingle = 2;  // D

// PID Control Parameters
const double setpoint = 0;
const double singleWallSetpoint = 550;  // Target distance from wall for single wall following
const double maxWallDistance = 700;     // Maximum acceptable distance from wall
const double minWallDistance = 350;     // Minimum acceptable distance from wall
const double deadband = 5;     // Ignore small errors to prevent oscillations
const double maxOutput = 200;  // Maximum PID output to prevent aggressive corrections

// Single Wall Speed Constraints
const int singleWallMinSpeed = 160;  // Minimum speed for single wall following
const int singleWallMaxSpeed = 235;  // Maximum speed for single wall following

// --- PID Variables ---
double pTerm = 0;
double dTerm = 0;
double derivative = 0;
double pidOutput = 0;
int pwmLeft = 0;
int pwmRight = 0;
double error = 0;
double previousError = 0; // Error from the previous loop iteration

// Single Wall PID Variables
double singleWallError = 0;
double singleWallPreviousError = 0;
double singleWallPidOutput = 0;
double pTermSingle = 0;  // Added missing variable
double dTermSingle = 0;  // Added for consistency
double derivativeSingle = 0;  // Moved to top level

// --- Wall Detection Variables ---
bool wallsPID = false;
bool singleWallMode = false;  // Flag for single wall following mode

// --- Timing Variables ---
unsigned long lastPIDTime = 0;
const unsigned long pidInterval = 5; // 5ms PID update rate (200Hz)

void applyPID() {
  if (millis() - lastPIDTime < pidInterval) {
    return; // Skip if not enough time has passed
  }
  lastPIDTime = millis();

  // Calculate the error based on wall following mode
  error = filteredRight - filteredLeft - setpoint;

  // Apply deadband
  if (abs(error) < deadband) {
    error = 0;
  }

  // --- Proportional Term ---
  pTerm = Kp * error;

  // --- Derivative Term ---
  derivative = error - previousError;
  dTerm = Kd * derivative;
  previousError = error;

  // --- Calculate PID Output ---
  pidOutput = pTerm + dTerm;
  
  // Clamp final output
  pidOutput = constrain(pidOutput, -maxOutput, maxOutput);

  // --- Adjust Motor Speeds ---
  pwmLeft = baseSpeed + (int)pidOutput;
  pwmRight = baseSpeed - (int)pidOutput;  // Add 8 to right motor

  // --- Clamp Motor Speeds to valid range ---
  pwmLeft = constrain(pwmLeft, minSpeed, maxSpeed);
  pwmRight = constrain(pwmRight, minSpeed, maxSpeed);

  // --- Apply speeds to motors ---
  analogWrite(pwmPinA, pwmLeft);
  analogWrite(pwmPinB, pwmRight);
}

void applySingleWallPID(bool isLeftWall){
  if (millis() - lastPIDTime < pidInterval) {
    return; // Skip if not enough time has passed
  }
  lastPIDTime = millis();

  // Calculate error based on which wall we're following
  if (isLeftWall) {
    // Following left wall
    singleWallError = singleWallSetpoint - filteredLeft;
    
    // If too far from wall, increase response
    if (filteredLeft > maxWallDistance) {
      // Increase proportional response when too far
      pTermSingle = KpSingle * 2.0 * singleWallError;
    }
    // If too close to wall, increase response
    else if (filteredLeft < minWallDistance) {
      // Increase proportional response when too close
      pTermSingle = KpSingle * 2.0 * singleWallError;
    }
    else {
      pTermSingle = KpSingle * singleWallError;
    }
  } else {
    // Following right wall
    singleWallError = filteredRight - singleWallSetpoint;
    
    // If too far from wall, increase response
    if (filteredRight > maxWallDistance) {
      // Increase proportional response when too far
      pTermSingle = KpSingle * 2.0 * singleWallError;
    }
    // If too close to wall, increase response
    else if (filteredRight < minWallDistance) {
      // Increase proportional response when too close
      pTermSingle = KpSingle * 2.0 * singleWallError;
    }
    else {
      pTermSingle = KpSingle * singleWallError;
    }
  }

  // Apply deadband
  if (abs(singleWallError) < deadband) {
    singleWallError = 0;
  }

  // Derivative term
  derivativeSingle = singleWallError - singleWallPreviousError;
  dTermSingle = KdSingle * derivativeSingle;
  singleWallPreviousError = singleWallError;

  // Calculate final PID output
  singleWallPidOutput = pTermSingle + dTermSingle;
  singleWallPidOutput = constrain(singleWallPidOutput, -maxOutput, maxOutput);

  // Apply to motors with single wall specific speed constraints
  pwmLeft = baseSpeed + (int)singleWallPidOutput;
  pwmRight = baseSpeed - (int)singleWallPidOutput + 8;  // Right wheel slightly faster

  // Clamp motor speeds to single wall specific range
  pwmLeft = constrain(pwmLeft, singleWallMinSpeed, singleWallMaxSpeed);
  pwmRight = constrain(pwmRight, singleWallMinSpeed, singleWallMaxSpeed);

  // Apply speeds to motors
  analogWrite(pwmPinA, pwmLeft);
  analogWrite(pwmPinB, pwmRight);
}

void wallFollowingControl(){
  updateFilteredSensors();
  
  // Update wall detection states
  wallsPID = (filteredLeft < wallThresholdLeft && filteredRight < wallThresholdRight);
  
  // Apply PID only if we have walls on both sides
  if (wallsPID){ 
    applyPID();  // Dual wall mode
  } else {
    // No walls or single wall detected, go straight at base speed
    analogWrite(pwmPinA, baseSpeed);
    analogWrite(pwmPinB, baseSpeed + 8);  // Right wheel slightly faster
    previousError = 0;
  } 
}
