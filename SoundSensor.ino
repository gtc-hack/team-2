#include <ArduinoRobot.h>
#include <Wire.h>
#include <SPI.h>

const int ledPin =  12;      // the number of the LED pin
const int thresholdvalue = 100; //The threshold to turn the led on
int  maximumValue = 0;
uint8_t LED_ARRAY[]  = { D0, D2, D3, D4 };
int MAX_LED = 4;
int desiredStatus = HIGH;
int ledId = 0;

void setup() {
  Robot.beginTFT();
  Robot.stroke(0, 0, 0);
  Robot.begin();
  Robot.beginSpeaker();
  Robot.text("Running the Sound Program", 5, 5);
  Robot.clearScreen();
  beginLEDs();
}

void loop() {
  Robot.stroke(0, 0, 0);
  Robot.clearScreen();

  int north = Robot.compassRead();
  int heading = 0;
  int audioHeading = 0;
  int inc = 60;
  for (int tries = 0; tries < 20; tries++) {
    maximumValue = 0;
    audioHeading = 0;
    for (int i = 0; i <360; i = i + inc) {
      updateLEDs();
      delay (100);
      int sensorValue = analogRead(TKD1);//use M2 to read the electrical signal
      Robot.drawCompass(maximumValue);
      if (sensorValue > maximumValue) {
        maximumValue = sensorValue;
        audioHeading = i;
      }
      Robot.turn(inc);
      updateLEDs();
    }

    inc = inc - 1;
    Robot.clearScreen();
    Robot.text("Stopped Locating", 5, 5);
    Robot.text( audioHeading, 20, 20);
    delay(100);
    Robot.drawCompass(audioHeading);

    if (audioHeading < 180) {
      Robot.turn(audioHeading);
    } else {
      Robot.turn((audioHeading - 360));
    }
    delay(1000);
    Robot.clearScreen();
    Robot.text("Pointing to sound", 5, 5);

    Robot.motorsWrite(255, 255);
    delay(1000);
    Robot.motorsWrite(0, 0);
    delay(1000);
  }

  delay(1000000);

}

void beginLEDs() {
  for (int i = 0; i != MAX_LED; i++) {
    pinMode (LED_ARRAY[i], OUTPUT);
    Robot.digitalWrite(LED_ARRAY[i], LOW);
  }
}

void updateLEDs() {
  Robot.digitalWrite(LED_ARRAY[ledId], desiredStatus);

  ledId = ledId + 1;
  if (ledId == MAX_LED) {
    ledId = 0;
    if (desiredStatus == HIGH) {
      desiredStatus = LOW;
    } else {
      desiredStatus = HIGH;
    }
  }
}
