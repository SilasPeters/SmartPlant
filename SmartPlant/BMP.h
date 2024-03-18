#include <Adafruit_BMP280.h>

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