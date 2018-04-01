#ifndef Bmp180_h
#define Bmp180_h

#include "Arduino.h"

////typedef uint8_t byte;

class Bmp180 {
  public:
    struct CalibrationData {
               short ac1;
               short ac2;
               short ac3;
      unsigned short ac4;
      unsigned short ac5;
      unsigned short ac6;
               short b1;
               short b2;
               short mb;
               short mc;
               short md;
    };

    static const byte       ULTRA_LOW_POWER_MODE = 0x00;
    static const byte              STANDARD_MODE = 0x01;
    static const byte       HIGH_RESOLUTION_MODE = 0x02;
    static const byte ULTRA_HIGH_RESOLUTION_MODE = 0x03;

    static const byte C01_FORMAT = 0x00;
    static const byte   C_FORMAT = 0x01;
    static const byte   F_FORMAT = 0x02;

    static const byte   PA_FORMAT = 0x00;
    static const byte MMHG_FORMAT = 0x01;
    static const byte MBAR_FORMAT = 0x03;

    Bmp180();
    Bmp180(byte defaultTemperatureFormat, byte defaultPressureFormat);
    void init();
    void init(byte mode);
    void reset();
    void measure();
    float getTemperature();
    float getTemperature(byte format);
    float getPressure();
    float getPressure(byte format);

  private:
    static const byte I2C_ID = 0x77;

    static const byte SOFT_RESET_REGISTER = 0xE0;
    static const byte    CONTROL_REGISTER = 0xF4;
    static const byte        MSB_REGISTER = 0xF6;
    static const byte        LSB_REGISTER = 0xF7;
    static const byte       XLSB_REGISTER = 0xF8;
//    static const byte    CHIP_ID_REGISTER = 0xD0;
    static const CalibrationData CALIBRATION_REGISTERS = {0xAA, 0xAC, 0xAE, 0xB0, 0xB2, 0xB4, 0xB6, 0xB8, 0xBA, 0xBC, 0xBE};

    static const byte          SOFT_RESET_COMMAND = 0xB6;
    static const byte MEASURE_TEMPERATURE_COMMAND = 0x2E;
    static const byte    MEASURE_PRESSURE_COMMAND = 0x34;
    static const byte             CHIP_ID_COMMAND = 0x55;

    CalibrationData calibrationData;
    byte temperatureFormat;
    byte    pressureFormat;
    byte mode;
    unsigned int  uncompensatedTemperature;
    unsigned long uncompensatedPressure;

    void readCalibrationData();
    void selectRegister(byte reg);
    byte          readByteFromRegister(byte reg);
    unsigned int   readIntFromRegister(byte reg);
    unsigned long readLongFromRegister(byte reg);

    unsigned long calculateB5();
    long          calculateTemperature();
    long          calculatePressure();
    float formatTemperature(long T, byte format);
    float formatPressure(long P, byte format);
};

#endif
