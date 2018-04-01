#include <Wire.h>
#include "Bmp180.h"
#include "Htu21.h"

Bmp180 bmp = Bmp180(Bmp180::C_FORMAT, Bmp180::MMHG_FORMAT);
Htu21  htu = Htu21();

void setup() {
  Serial.begin(9600);
  Wire.begin();
  bmp.init();
}

void loop() {
  bmp.measure();
  Serial.print(bmp.getTemperature()); Serial.print("C, ");
  Serial.print(bmp.getPressure());    Serial.print("mmHg, ");
  Serial.print(htu.getTemperature()); Serial.print("C, ");
  Serial.print(htu.getHumidity());    Serial.print("% ");

  Serial.println("==================");
  delay(5000);
}
