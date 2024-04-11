#include <Adafruit_BMP280.h> // https://github.com/adafruit/Adafruit_BMP280_Library

class BMP
{
  private:
    Adafruit_BMP280 bmp;
  public:
    void setup();
    float temperature();
    float pressure();
    float altitude();
};
