#include <Servo.h>

const int photoSensor = 2;
const int hallSensor = 3;
const int trigger = 4;
const int switcher = 5;
const int gelbLED = 7;
const int servoPin = 9;
const int button1 = 10;
const int button2 = 11;
const int led1 = 12;
const int led2 = 13;
const int fallingTime = 495;


volatile long lastPhotoTimestamp = 0;
volatile int lastSpeed = 0;
volatile int currentSpeed = 0;
volatile long lastHoleTime = 0;
volatile long nextHoleTime = 0;
volatile int velocityMode = 0;
volatile bool isDone = false;



Servo servo;


void gateClose() {
  /*
    Sets the servo to 35° to close the system after throwning the ball
    and at the begining of algorithm.
  */
  servo.write(35);
}


void gateOpen() {
  /*
    Sets the servo to 60° to open the system for throwning the ball.
  */
  servo.write(60);
}


bool isClicked() {
  /*
    Checks if the trigger-button was pressed.
    Returns True if the button is (was) pressed, false otherwise.
  */
  if (digitalRead(trigger) == 1) return true;
  else return false;
}


void HallSensorISR() {
  /*
    Interrupt service routine (ISR) for the Hall sensor.

    This function updates two variables:
                      lastHoleTime - is the timestamp when the ISR is called
                      nextHoleTima - is the estimated time when the next falling
                                      edge is detected

    Note: within ISR-function, it is forbidden to use delay and/or
          serial prints, writes or reads
  */

  lastHoleTime = millis();

  nextHoleTime = lastHoleTime + currentSpeed;
}


void PhotoSensorISR() {
  /*
    Interrupt service routine (ISR) for the Photo sensor.

    This function calculates the speed of rotation
    by keeping a timestamp of when this function was last called and
    determining the difference between the actual time and when
    this function was last called.

    According to the rotation speed the corresponding LEDs will light up
    (yellow, green, red)
    and the matching throwing pattern will be selected (fast, medium or slow)

    Note: within ISR-function, it is forbidden to use delay and/or
          serial prints, writes or reads
  */

  lastSpeed = currentSpeed;

  currentSpeed = 6 * (millis() - lastPhotoTimestamp);

  lastPhotoTimestamp = millis();

  //fast
  if (currentSpeed < 1000) {
    velocityMode = 1;
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  }

  //medium
  if (currentSpeed <= 3000 && currentSpeed >= 1000) {
    velocityMode = 2;
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  }

  //slow
  if (currentSpeed > 3000) {
    velocityMode = 3;
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  }
}

void fireBall(int mode[], int i) {
  if (i >= 4) {
    isDone = true;
    Serial.println("The pattern done!");
  }
  switch (mode[i]) {
    case 1: {
      // throw a ball
        int t = nextHoleTime - millis() - fallingTime;

        if (t < 0) {
          t = t + currentSpeed;
        }
        Serial.print("Waiting for ");
        Serial.println(t);

        delay(t);

        gateOpen();
        // wait either half a turn or 500 ms, whichever comes first,
        // to close the gate
        delay(min(currentSpeed / 2, 500));
        gateClose();
        break;
      }
    case 0: {
        delay(currentSpeed);
        break;
      }
  }
}


void setupAll() {
  /*
    Initializes the hardware by initializing the serial port,
    declaring all pins, initializing the servo
    and attaching HallSensorISR and PhotoSensorISR interrupt routines
    to the appropriate pins.
  */

  Serial.begin(9600);
  pinMode(photoSensor, INPUT);
  pinMode(hallSensor, INPUT);
  pinMode(trigger, INPUT);
  pinMode(switcher, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(gelbLED, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  servo.attach(servoPin);
  gateClose();

  attachInterrupt(digitalPinToInterrupt(photoSensor), PhotoSensorISR, RISING);
  attachInterrupt(digitalPinToInterrupt(hallSensor), HallSensorISR, FALLING);

}
