#include "MouseMain.h"

// --- Motor Driver Pins ---
const int pwmPinA = D6;      // Motor A Speed (PWM)
const int motorA1 = D5;      // Motor A Direction 1
const int motorA2 = D7;      // Motor A Direction 2
const int motorB1 = D8;      // Motor B Direction 1
const int motorB2 = D9;      // Motor B Direction 2
const int pwmPinB = D10;     // Motor B Speed (PWM)

// --- Sensor Pins ---
const int sensorFrontPin = A2;
const int sensorLeftPin  = A3;
const int sensorRightPin = A1;
const int Pin4           = A0;    // Changed from A3 to A4 to avoid conflict
const int Pin5           = D4;    // Digital

// --- LED Pins ---
const int ledPower = 11;     // Control pin for NeoPixel Power
const int ledPin = 12;       // NeoPixel Data Pin

void pinSETUP(){
  // Set Sensor Pin Modes
  pinMode(sensorFrontPin, INPUT);
  pinMode(sensorLeftPin, INPUT);
  pinMode(sensorRightPin, INPUT);
  pinMode(Pin4, INPUT);      // Initialize Pin4
  pinMode(Pin5, INPUT);      // Initialize Pin5

  // Init Motor Pins
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(pwmPinA, OUTPUT);
  pinMode(pwmPinB, OUTPUT);
  analogWrite(pwmPinA, 0); // Start with speed 0
  analogWrite(pwmPinB, 0); // Start with speed 0

  // Init RGB LED
  pinMode(ledPower, OUTPUT);
  digitalWrite(ledPower, LOW); // Ensure power is off initially if controlled
  pixels.begin(); // IMPORTANT: Initialize NeoPixel
  pixels.clear(); // Clear pixel data
  pixels.show();  /* Send cleared data to pixel*/}      

  