#include <Servo.h>

class MainServo
{
  private:
    Servo mainServo;
  public:
    void setup(int pin);
    void write(int deg);
};