#include <Arduino.h>

class AMUX
{
  private:
    int PIN_AMUX_SEL;
    int PIN_AMUX_ANALOG_READ;
    bool readingMoist;
    unsigned long lastMoistReadingTime;
    unsigned long moistReadingInterval;
    unsigned long moistReadingDuration;
    word lastMoistReading;
    word lastLdrReading;

    void startReadingMoist();
    void stopReadingMoist();
    void readLdr();

  public:
    void setup(int pin_sel, int pin_read, unsigned long moistReadingIntervalParam, unsigned long moistReadingDurationParam);
    void loop();
    int getLastMoistReading();
    int getLastLdrReading();
};