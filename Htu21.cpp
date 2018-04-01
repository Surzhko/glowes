#include "Htu21.h"
#include <Wire.h>

Htu21::Htu21() {
  temperatureFormat = C_FORMAT;
}

Htu21::Htu21(uint8_t defaultTemperatureFormat) {
  temperatureFormat = defaultTemperatureFormat;
}

float Htu21::getTemperature() {
  return getTemperature(temperatureFormat);
}

float Htu21::getTemperature(uint8_t format) {
  return formatTemperature(calculateTemperature(), format);
}

float Htu21::getHumidity() {
  return measure(HUMIDITY) * (125.0 / 65536.0) - 6.0;
}

uint16_t Htu21::measure(uint8_t command) {
  Wire.beginTransmission(I2C_ID);
  Wire.write(command);
  uint8_t status = Wire.endTransmission();
//  Serial.print("Status: "); Serial.println(status);

  uint8_t amount = Wire.requestFrom(I2C_ID, (uint8_t)3);
//  Serial.print("Amount of read bytes: "); Serial.println(amount);

  if (amount == 3) {
    uint8_t msb, lsb, checksum;

    msb = Wire.read();
    lsb = Wire.read();
    checksum = Wire.read();

    return ((uint16_t) msb << 8) | (uint16_t) lsb;
  }
}

float Htu21::calculateTemperature() {
  return measure(TEMPERATURE) * (175.72 / 65536.0) - 46.85;
}

float Htu21::formatTemperature(long temperature, uint8_t format) {
  if (format == C_FORMAT) return temperature;
}
