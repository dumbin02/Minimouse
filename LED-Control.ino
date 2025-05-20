#include "MouseMain.h"
#include <Adafruit_NeoPixel.h>

// --- LED Configuration ---
const uint8_t NUMPIXELS = 1;
Adafruit_NeoPixel pixels(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);

// --- LED Color Control ---
uint8_t currentColorIndex = 0;
const uint8_t NUM_COLORS = 4;
const uint8_t COLORS[NUM_COLORS][3] = {
  {255, 0, 0},    /* Red*/
  {0, 255, 0},    /* Green*/
  {0, 0, 255},    /* Blue*/
  {255, 255, 255} /* White*/};

// --- LED Control Functions ---
void clearLED(){
  pixels.clear();
  pixels.show();
  digitalWrite(ledPower, LOW); /* Power On*/}

void colorLED(uint8_t R, uint8_t G, uint8_t B){
  digitalWrite(ledPower, HIGH); // Power On
  pixels.clear(); // Clear previous color
  pixels.setPixelColor(0, pixels.Color(R, G, B));
  pixels.show();}

void cycleLEDColor() {
  // Set the current color
  colorLED(COLORS[currentColorIndex][0], 
           COLORS[currentColorIndex][1], 
           COLORS[currentColorIndex][2]);
  
  // Move to next color
  currentColorIndex = (currentColorIndex + 1) % NUM_COLORS;
}
  