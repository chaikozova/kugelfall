#include "kugelfall_labo.h"

const int fallingTime = 495;
const int fast[] = {1, 1, 1, 0};
const int medium[] = {1, 0, 1, 1};
const int slow[] = {1, 0, 1, 0};

const int patternSelcetion[] = {fast, medium, slow};


void setup() {
  //Setup the hardware when booting the Arduino
  setupAll();
}

void loop() {


  if(isClicked()) {
    Serial.println("Button was clicked");
    bool patternDone = false;

    

  }
}
