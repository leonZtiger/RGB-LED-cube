#pragma once

#include <FastLED.h>

#ifndef DIMENSION
#define DIMENSION 7
#endif

#ifndef LED_PIN
#define LED_PIN 2
#endif

#ifndef LAYER_PIN_START
#define LAYER_PIN_START 3
#endif

// cubes pixels
CRGB cube[DIMENSION][DIMENSION * DIMENSION];

// led controller
CLEDController *cled;

// renders all pixels to cube
void render_cube() {
  
  // render each layer one by one
  for (int i = 0; i < DIMENSION; i++) {

    pinMode(i + LAYER_PIN_START, OUTPUT);

    // display values
    cled->show(cube[i], DIMENSION * DIMENSION, 255);

    // small delay to make the leds vissible
    // to long and the cube will flicker and to short the lights wont have time to light up
    delayMicroseconds(500);
    
    pinMode(i + LAYER_PIN_START, INPUT);
  }
}

// configures the cube
void setup_cube() {
  //initialize led controller
  cled = &FastLED.addLeds<WS2812, LED_PIN, GRB>(nullptr,0);
}

// clears all pixels
void clear(CRGB color) {
  for (int i = 0; i < DIMENSION; i++) {
    for (int j = 0; j < DIMENSION * DIMENSION; j++) {
      cube[i][j] = color;
    }
  }
}
// sets pixel at coordinates, RGB color
void set_pixel(char x, char y, char z, CRGB color) {

  cube[z][y + DIMENSION * x] = color;
}
// sets pixel at coordinates, HSV color
void set_pixel(char x, char y, char z, CHSV color) {

  cube[z][y + DIMENSION * x] = color;
}
// adds color to pixel at coordinates, RGB color
void add_color_pixel(char x, char y, char z, CRGB color) {

  cube[z][y + DIMENSION * x].r += color.r;
  cube[z][y + DIMENSION * x].g += color.g;
  cube[z][y + DIMENSION * x].b += color.b;
}
// adds color to pixel at coordinates, HSV color
void add_color_pixel(char x, char y, char z, CHSV color) {

  cube[z][y + DIMENSION * x].r += color.raw[0];
  cube[z][y + DIMENSION * x].g += color.raw[1];
  cube[z][y + DIMENSION * x].b += color.raw[2];
}
