#include <Servo.h> // https://github.com/arduino-libraries/Servo

class MainServo
{
  private:
    Servo mainServo;
  public:
    void setup(int pin);
    void write(int deg);
};
