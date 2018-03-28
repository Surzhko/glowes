#include <Wire.h>
#include "Bmp180.h"

const byte TEMPERATURE_AND_HUMIDITY_SENSOR_ID     = 0x40;
const byte PRESURE_SENSOR_ID                      = 0x77;

const byte TRIGGER_TEMPERATURE_MEASUREMENT_HOLD   = 0xE3;
const byte TRIGGER_HUMIDITY_MEASUREMENT_HOLD      = 0xE5;
const byte TRIGGER_TEMPERATURE_MEASUREMENT_NOHOLD = 0xF3;
const byte TRIGGER_HUMIDITI_MEASUREMENT_NOHOLD    = 0xF5;

Bmp180 bmp;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  bmp = Bmp180(Bmp180::C_FORMAT, Bmp180::MMHG_FORMAT);
  bmp.init();
}

float measure_temperature() {
  Wire.beginTransmission(TEMPERATURE_AND_HUMIDITY_SENSOR_ID);
  Wire.write(TRIGGER_TEMPERATURE_MEASUREMENT_HOLD);
  byte status = Wire.endTransmission();
//  Serial.println(Bmp180::CALIBRATION_REGISTERS.ac1);
//  Serial.print("Status: ");
//  Serial.println(status);

  byte amount = Wire.requestFrom(TEMPERATURE_AND_HUMIDITY_SENSOR_ID, (uint8_t)3);

//  Serial.print("Amount of read bytes: ");
//  Serial.println(amount);

  if (amount == 3) {
    byte msb, lsb, checksum;
  
    msb = Wire.read();
    lsb = Wire.read();
    checksum = Wire.read();
  
    uint16_t rawTemperature = ((uint16_t) msb << 8) | (uint16_t) lsb;
    float tempTemperature = rawTemperature * (175.72 / 65536.0); //2^16 = 65536
    float realTemperature = tempTemperature - 46.85; //From page 14
    Serial.print(realTemperature);
    Serial.print("C, ");
    return realTemperature;
  }
}

void measure_humidity() {
  Wire.beginTransmission(TEMPERATURE_AND_HUMIDITY_SENSOR_ID);
  Wire.write(TRIGGER_HUMIDITY_MEASUREMENT_HOLD);
  byte status = Wire.endTransmission();

  byte amount = Wire.requestFrom(TEMPERATURE_AND_HUMIDITY_SENSOR_ID, (uint8_t)3);

//  Serial.print("Amount of read bytes: ");
//  Serial.println(amount);

  if (amount == 3) {
    byte msb, lsb, checksum;
  
    msb = Wire.read();
    lsb = Wire.read();
    checksum = Wire.read();
  
    uint16_t rawTemperature = ((uint16_t) msb << 8) | (uint16_t) lsb;
    float tempTemperature = rawTemperature * (125.0 / 65536.0); //2^16 = 65536
    float realTemperature = tempTemperature - 6.0; //From page 14
    Serial.print(realTemperature);
    Serial.print("% ");
  }
}

//

void loop() {
  bmp.measure();
  Serial.print(bmp.getTemperature());
  Serial.print("C, ");

  Serial.print(bmp.getPressure());
  Serial.print("mmHg, ");
  measure_temperature();
  measure_humidity();

  Serial.println("==================");
  delay(5000);
}
