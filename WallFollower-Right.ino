/* --- Timing Variables ---
static unsigned long lastUpdate = 0;
static unsigned long currentTime = 0;
static unsigned long lastSquareTime = 0;
static bool inSquare = false;

void wallFollowerRight(){
   // Update filtered sensor values for wall detection
   updateRawSensors();
   
   //si no hay una pared a la derecha tomar la derecha
   if (rawRight > wallThresholdRight){      
      turnRightW();
      moveF();
   }
   //si tenemos una pared enfrente y no a la izquierda doblar a la izquierda
   else if (rawFront < wallThresholdFront && rawLeft > wallThresholdLeft){
      turnLeftW();
      moveF();
   }
   //si estan las tres paredes dar media vuelta
   else if (rawLeft < wallThresholdLeft && rawRight < wallThresholdRight && rawFront < wallThresholdFrontClose){
      stop();
      turnRight();
      turnRight();
      moveF();
   }
   else {
      updateFilteredSensors();
      
      // Check for both walls first (dual wall mode)
      if (filteredLeft < wallThresholdLeft && filteredRight < wallThresholdRight) {
         // Both walls detected - use dual wall PID
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
         clearLED();
         analogWrite(pwmPinA, baseSpeed);
         analogWrite(pwmPinB, baseSpeed + 8);
      }
   }
   
   // Non-blocking delay using millis()
   currentTime = millis();
   if (currentTime - lastUpdate >= 5) {
     lastUpdate = currentTime;
   }
}

void straightMode(){
   updateFilteredSensors();
   
   // Check for both walls first (dual wall mode)
   if (filteredLeft < wallThresholdLeft && filteredRight < wallThresholdRight) {
      // Both walls detected - use dual wall PID
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
      clearLED();
      analogWrite(pwmPinA, baseSpeed);
      analogWrite(pwmPinB, baseSpeed + 8);
   }
}
*/
