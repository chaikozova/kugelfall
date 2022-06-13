#include "kugelfall_ISR.h"

const int fast[] = {1, 1, 1, 0};
const int medium[] = {1, 0, 1, 1};
const int slow[] = {1, 0, 1, 0};
const int wait[] = {0, 0, 0, 0};

const int patternSelcetion[] = {wait, fast, medium, slow};


void setup() {
  //Setup the hardware when booting the Arduino
  setupAll();
}

void loop() {


  if (isClicked()) {
    Serial.println("Button was clicked");
    isDone = false;

    int velocityModeStart = velocityMode;

    for (int i = 0; i <= 4 && isDone == false; i++) {

      if (velocityMode != velocityModeStart) {
        Serial.print("The velocity Mode has been changed.");
        Serial.println("Please click the button if you want to star again");
        isDone = true;
        break;
      }

      switch (patternSelcetion[velocityMode]) {
        case 0: {
            Serial.println("Waiting for velocityMode have been known.");
            break;
          }

        case 1: {
            Serial.println("velocityMode is fast");
            fireBall(patternSelcetion[velocityMode], i);
            break;
          }
        case 2: {
            Serial.println("velocityMode is medium");
            fireBall(patternSelcetion[velocityMode], i);
            break;
          }
        case 3: {
            Serial.println("velocityMode is slow");
            fireBall(patternSelcetion[velocityMode], i);
            break;
          }
      }
    }


  }
}
