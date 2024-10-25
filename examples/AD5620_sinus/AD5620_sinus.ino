//
//    FILE: AD5620_sinus.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: test basic behaviour and performance
//     URL: https://github.com/RobTillaart/AD5620


#include "AD5620.h"


AD5620 AD16_HW(8, &SPI);
AD5620 AD16_SW(5, 6, 7);

float frequency = 10.0;
float amplitude = 2047;  //  50% of 12 bits

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("AD5620_LIB_VERSION: ");
  Serial.println(AD5620_LIB_VERSION);

  SPI.begin();
  AD16_HW.begin();
  AD16_SW.begin();

  Serial.print("HWSPI: ");
  Serial.println(AD16_HW.usesHWSPI());
  Serial.print("HWSPI: ");
  Serial.println(AD16_SW.usesHWSPI());
}


void loop()
{
  float phase  = fmod(micros() * (frequency * (TWO_PI * 1e-6)), TWO_PI);
  float x = (1 + sin(phase)) * amplitude;
  AD16_HW.setValue(x);
  Serial.println(x);
}


//  -- END OF FILE --
