#include <Wire.h>

const byte TEMPERATURE_AND_HUMIDITY_SENSOR_ID     = 0x40;
const byte PRESURE_SENSOR_ID                      = 0x77;

const byte TRIGGER_TEMPERATURE_MEASUREMENT_HOLD   = 0xE3;
const byte TRIGGER_HUMIDITY_MEASUREMENT_HOLD      = 0xE5;
const byte TRIGGER_PRESURE_MEASUREMENT_HOLD       = 0xF6;
const byte TRIGGER_TEMPERATURE_MEASUREMENT_NOHOLD = 0xF3;
const byte TRIGGER_HUMIDITI_MEASUREMENT_NOHOLD    = 0xF5;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(PRESURE_SENSOR_ID);
  Wire.write(0xAA);
  byte status = Wire.endTransmission();
  Serial.print("Status: ");
  Serial.println(status);

  byte amount = Wire.requestFrom(PRESURE_SENSOR_ID, 2);
  Serial.print("Amount of read bytes: ");
  Serial.println(amount);

  byte msb, lsb;

  msb = Wire.read();
  lsb = Wire.read();
  uint16_t rawTemperature = ((uint16_t) msb << 8) | (uint16_t) lsb;
  Serial.println(rawTemperature);
  
}

void loop() {
  // TEMPERATURE
  Wire.beginTransmission(TEMPERATURE_AND_HUMIDITY_SENSOR_ID);
  Wire.write(TRIGGER_TEMPERATURE_MEASUREMENT_HOLD);
  byte status = Wire.endTransmission();
  Serial.print("Status: ");
  Serial.println(status);

  byte amount = Wire.requestFrom(TEMPERATURE_AND_HUMIDITY_SENSOR_ID, 3);

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
  }

  // HUMIDITY
  Wire.beginTransmission(TEMPERATURE_AND_HUMIDITY_SENSOR_ID);
  Wire.write(TRIGGER_HUMIDITY_MEASUREMENT_HOLD);
  status = Wire.endTransmission();

  amount = Wire.requestFrom(TEMPERATURE_AND_HUMIDITY_SENSOR_ID, 3);

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


  // PRESURE
  Wire.beginTransmission(PRESURE_SENSOR_ID);
  Wire.write(TRIGGER_PRESURE_MEASUREMENT_HOLD);
  status = Wire.endTransmission();
//  Serial.print("Status: ");
//  Serial.println(status);

  amount = Wire.requestFrom(PRESURE_SENSOR_ID, 3);

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
    Serial.print(rawTemperature);
    Serial.print("P ");
  }

  Serial.println("==================");
  delay(2000);
}
