#include <Adafruit_SSD1306.h> // https://github.com/adafruit/Adafruit_SSD1306

class OLED
{
  private:
    Adafruit_SSD1306 display;

  public:
    void setup();
    void clear();
    void cursor(int x = 0, int y = 0);
    void finish();
    void drawFroge();
    void drawText(int line, String text);
    void sensorScreen(float temp, int light, int moist, float pressure);
    void lastWaterScreen(int minutesAgo);
};
