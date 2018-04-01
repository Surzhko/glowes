#include "Bmp180.h"
#include <Wire.h>

Bmp180::Bmp180() {
  temperatureFormat = C01_FORMAT;
  pressureFormat    =  PA_FORMAT;
}

Bmp180::Bmp180(byte defaultTemperatureFormat, byte defaultPressureFormat) {
  temperatureFormat = defaultTemperatureFormat;
  pressureFormat    = defaultPressureFormat;
}

void Bmp180::init() {
  init(STANDARD_MODE);
}

void Bmp180::init(byte new_mode) {
  mode = new_mode;
  readCalibrationData();
}

void Bmp180::reset() {
  Wire.beginTransmission(I2C_ID);
  Wire.write(SOFT_RESET_REGISTER);
  Wire.write(SOFT_RESET_COMMAND);
  Wire.endTransmission();
}

void Bmp180::measure() {
  Wire.beginTransmission(I2C_ID);
  Wire.write(CONTROL_REGISTER);
  Wire.write(MEASURE_TEMPERATURE_COMMAND);
  Wire.endTransmission();
  delay(5);
//  uncompensatedTemperature = 27898;
  uncompensatedTemperature = readIntFromRegister(MSB_REGISTER);

  Wire.beginTransmission(I2C_ID);
  Wire.write(CONTROL_REGISTER);
  Wire.write(MEASURE_PRESSURE_COMMAND + (mode << 6));
  Wire.endTransmission();
  switch (mode) {
    case       ULTRA_LOW_POWER_MODE: delay(5); break;
    case              STANDARD_MODE: delay(8); break;
    case       HIGH_RESOLUTION_MODE: delay(14); break;
    case ULTRA_HIGH_RESOLUTION_MODE: delay(26); break;
  }
//  uncompensatedPressure = 23843;
  uncompensatedPressure = readLongFromRegister(MSB_REGISTER) >> (8 - mode);
}

float Bmp180::getTemperature() {
  return getTemperature(temperatureFormat);
}

float Bmp180::getTemperature(byte format) {
  return formatTemperature(calculateTemperature(), format);
}

float Bmp180::getPressure() {
  return getPressure(pressureFormat);
}

float Bmp180::getPressure(byte format) {
  return formatPressure(calculatePressure(), format);
}

void Bmp180::readCalibrationData() {
//  calibrationData.ac1 = 408;
//  calibrationData.ac2 = -72;
//  calibrationData.ac3 = -14383;
//  calibrationData.ac4 = 32741;
//  calibrationData.ac5 = 32757;
//  calibrationData.ac6 = 23153;
//  calibrationData.b1  = 6190;
//  calibrationData.b2  = 4;
//  calibrationData.mb  = -32768;
//  calibrationData.mc  = -8711;
//  calibrationData.md  = 2868;

  calibrationData.ac1 = readIntFromRegister(CALIBRATION_REGISTERS.ac1);
  calibrationData.ac2 = readIntFromRegister(CALIBRATION_REGISTERS.ac2);
  calibrationData.ac3 = readIntFromRegister(CALIBRATION_REGISTERS.ac3);
  calibrationData.ac4 = readIntFromRegister(CALIBRATION_REGISTERS.ac4);
  calibrationData.ac5 = readIntFromRegister(CALIBRATION_REGISTERS.ac5);
  calibrationData.ac6 = readIntFromRegister(CALIBRATION_REGISTERS.ac6);
  calibrationData.b1  = readIntFromRegister(CALIBRATION_REGISTERS.b1);
  calibrationData.b2  = readIntFromRegister(CALIBRATION_REGISTERS.b2);
  calibrationData.mb  = readIntFromRegister(CALIBRATION_REGISTERS.mb);
  calibrationData.mc  = readIntFromRegister(CALIBRATION_REGISTERS.mc);
  calibrationData.md  = readIntFromRegister(CALIBRATION_REGISTERS.md);

//  Serial.print(calibrationData.ac1); Serial.print(", ");
//  Serial.print(calibrationData.ac2); Serial.print(", ");
//  Serial.print(calibrationData.ac3); Serial.print(", ");
//  Serial.print(calibrationData.ac4); Serial.print(", ");
//  Serial.print(calibrationData.ac5); Serial.print(", ");
//  Serial.print(calibrationData.ac6); Serial.print(", ");
//  Serial.print(calibrationData.b1);  Serial.print(", ");
//  Serial.print(calibrationData.b2);  Serial.print(", ");
//  Serial.print(calibrationData.mb);  Serial.print(", ");
//  Serial.print(calibrationData.mc);  Serial.print(", ");
//  Serial.print(calibrationData.md);  Serial.print(" ==== COEFICIENTS ====\n");
}

void Bmp180::selectRegister(byte reg) {
  Wire.beginTransmission(I2C_ID);
  Wire.write(reg);
  Wire.endTransmission();
}

byte Bmp180::readByteFromRegister(byte reg) {
  selectRegister(reg);
  Wire.beginTransmission(I2C_ID);
  Wire.requestFrom(I2C_ID, (byte)1);
  byte b = Wire.read();
  Wire.endTransmission();
  return b;
}

unsigned int Bmp180::readIntFromRegister(byte reg) {
  selectRegister(reg);
  Wire.beginTransmission(I2C_ID);
  Wire.requestFrom(I2C_ID, (byte)2);
  unsigned int i = Wire.read();
  i = i << 8 | Wire.read();
  Wire.endTransmission();
  return i;
}

unsigned long Bmp180::readLongFromRegister(byte reg) {
  selectRegister(reg);
  Wire.beginTransmission(I2C_ID);
  Wire.requestFrom(I2C_ID, (byte)3);
  unsigned long l = Wire.read();
  l = l << 8 | Wire.read();
  l = l << 8 | Wire.read();
  Wire.endTransmission();

  return l;
}

unsigned long Bmp180::calculateB5() {
  long x1 = (uncompensatedTemperature - (long)calibrationData.ac6) * ((long)calibrationData.ac5) >> 15;
  long x2 = ((long)calibrationData.mc << 11) / (x1+(long)calibrationData.md);
  return x1 + x2;
}

long Bmp180::calculateTemperature() {
  return (calculateB5() + 8) >> 4;
}

long Bmp180::calculatePressure() {
  long b6 = calculateB5() - 4000;
  long x1 = ((long)calibrationData.b2 * ((b6 * b6) >> 12)) >> 11;
  long x2 = ((long)calibrationData.ac2 * b6) >> 11;
  long x3 = x1 + x2;
  long b3 = ((((long)calibrationData.ac1 * 4 + x3) << mode) + 2) / 4;
  x1 = ((long)calibrationData.ac3 * b6) >> 13;
  x2 = ((long)calibrationData.b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  unsigned long b4 = ((long)calibrationData.ac4 * (long)(x3 + 32768)) >> 15;
  unsigned long b7 = (uncompensatedPressure - b3) * (long)( 50000UL >> mode );
  long p;
  if (b7 < 0x80000000) {
    p = (b7 * 2) / b4;
  } else {
    p = (b7 / b4) * 2;
  }
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  p = p + ((x1 + x2 + (long)3791)>>4);
  return p;
}

float Bmp180::formatTemperature(long temperature, byte format) {
  if (format == C01_FORMAT) return temperature;
  if (format ==   C_FORMAT) return temperature / 10.0;
}

float Bmp180::formatPressure(long pressure, byte format) {
  if (format ==   PA_FORMAT) return pressure;
  if (format == MMHG_FORMAT) return pressure / 133.3224;
  if (format == MBAR_FORMAT) return pressure / 100.0;
}
