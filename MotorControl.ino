#include "MouseMain.h"

// --- Motor Speed Control ---

void moveF(){
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH); // Motor A Forward
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
  analogWrite(pwmPinA, baseSpeed+3);  // Left wheel 
  analogWrite(pwmPinB, baseSpeed);}      // Right wheel slightly faster
void moveB(){
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);  // Motor A Backward
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
  analogWrite(pwmPinA, baseSpeed);
  analogWrite(pwmPinB, baseSpeed);}

void turnLeft(){
  analogWrite(pwmPinA, 110);
  analogWrite(pwmPinB, 110);
  digitalWrite(motorA1, HIGH); // Motor A Backward
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);  // Motor B Forward
  digitalWrite(motorB2, HIGH);
  delay(timeTurnLeft);}

void turnRight(){
  analogWrite(pwmPinA, 110);
  analogWrite(pwmPinB, 110);
  digitalWrite(motorA1, LOW);  // Motor A Forward
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, HIGH); // Motor B Backward
  digitalWrite(motorB2, LOW);
  delay(timeTurnRight);}

void turnLeftW(){
  analogWrite(pwmPinA, 75);
  analogWrite(pwmPinB, 240);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH); // Motor A Forward
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
  delay(timeTurnLeftW);}

void turnRightW(){
  analogWrite(pwmPinA, 255);
  analogWrite(pwmPinB, 70);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH); // Motor A Forward
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
  delay(timeTurnRightW);}

void stop(){
  // Option 1: Brake motors (if supported by driver)
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
  analogWrite(pwmPinA, 0); // Ensure PWM is off
  analogWrite(pwmPinB, 0);}