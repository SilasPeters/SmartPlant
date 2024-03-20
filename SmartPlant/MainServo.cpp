#ifndef mainservo_h
#define mainservo_h

#include "MainServo.h"
#include <Servo.h> // https://github.com/arduino-libraries/Servo


void MainServo::setup(int pin)
{
  mainServo.attach(pin, 500, 2400);
}

void MainServo::write(int deg)
{
  mainServo.write(deg);
  delay(15);
}

#endif
