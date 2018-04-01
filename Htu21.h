#ifndef Htu21_h
#define Htu21_h

#include "Arduino.h"

class Htu21 {
  public:
    static const uint8_t C_FORMAT = 0x00;
    static const uint8_t F_FORMAT = 0x01;

    Htu21();
    Htu21(uint8_t defaultTemperatureFormat);
    float getTemperature();
    float getTemperature(uint8_t format);
    float getHumidity();

  private:
    static const uint8_t I2C_ID = 0x40;

    static const uint8_t TEMPERATURE        = 0xE3;
    static const uint8_t    HUMIDITY        = 0xE5;
    static const uint8_t TEMPERATURE_NOHOLD = 0xF3;
    static const uint8_t    HUMIDITY_NOHOLD = 0xF5;

    byte temperatureFormat;

    uint16_t measure(uint8_t command);

    float calculateTemperature();
    float formatTemperature(long T, uint8_t format);
};

#endif
