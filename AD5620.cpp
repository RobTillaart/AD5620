//
//    FILE: AD5620.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
//    DATE: 2024-10-25
// PURPOSE: Arduino library for AD5620 Digital Analog Convertor (12 bit).


#include "AD5620.h"


//  HARDWARE SPI
AD5620::AD5620(uint8_t slaveSelect, __SPI_CLASS__ * mySPI)
{
  _select   = slaveSelect;
  _hwSPI    = true;
  _mySPI    = mySPI;
  _value    = 0;
  _type     = 12;
  _maxValue = 4095;
}

//  SOFTWARE SPI
AD5620::AD5620(uint8_t slaveSelect, uint8_t spiData, uint8_t spiClock)
{
  _select   = slaveSelect;
  _hwSPI    = false;
  _mySPI    = NULL;
  _dataOut  = spiData;
  _clock    = spiClock;
  _value    = 0;
  _type     = 12;
  _maxValue = 4095;
}


//  initializes the SPI
//  and sets internal state
void AD5620::begin()
{
  pinMode(_select, OUTPUT);
  digitalWrite(_select, HIGH);

  _spi_settings = SPISettings(_SPIspeed, MSBFIRST, SPI_MODE0);

  if (_hwSPI)
  {
    //  _mySPI->end();
    //  _mySPI->begin();
    //  delay(1);
  }
  else  //  SOFTWARE SPI
  {
    pinMode(_dataOut, OUTPUT);
    pinMode(_clock, OUTPUT);
    digitalWrite(_dataOut, LOW);
    digitalWrite(_clock, LOW);
  }
}


uint8_t AD5620::getType()
{
  return _type;
}


bool AD5620::setValue(uint16_t value)
{
  if (value > _maxValue) return false;
  _value = value;
  updateDevice(_value);
  return true;
}


uint16_t AD5620::getValue()
{
  return _value;
}


uint16_t AD5620::getMaxValue()
{
  return _maxValue;
}


bool AD5620::setPercentage(float percentage)
{
  if ((percentage < 0) || (percentage > 100)) return false;

  uint32_t value = round( 40.95 * percentage);
  return setValue(value);
}


float AD5620::getPercentage()
{
  float value = getValue();
  if (value > 0)
  {
    return value * ( 1.0 / 40.95);
  }
  return 0;
}


//
//  SPI
//
void AD5620::setSPIspeed(uint32_t speed)
{
  _SPIspeed = speed;
  _spi_settings = SPISettings(_SPIspeed, MSBFIRST, SPI_MODE0);
};


uint32_t AD5620::getSPIspeed()
{
  return _SPIspeed;
}


bool AD5620::usesHWSPI()
{
  return _hwSPI;
}


//////////////////////////////////////////////////////////////////
//
//  PRIVATE
//

void AD5620::updateDevice(uint32_t value)
{
  uint16_t val = value << 2;

  digitalWrite(_select, LOW);
  if (_hwSPI)
  {
    _mySPI->beginTransaction(_spi_settings);
    _mySPI->transfer16(val);
    _mySPI->endTransaction();
  }
  else  //  Software SPI
  {
    swSPI_transfer(val);
  }
  digitalWrite(_select, HIGH);
}


//  simple one mode version
void AD5620::swSPI_transfer(uint16_t value)
{
  uint8_t clk = _clock;
  uint8_t dao = _dataOut;
  for (uint16_t mask = 0x8000; mask; mask >>= 1)
  {
    digitalWrite(dao,(value & mask));
    digitalWrite(clk, HIGH);
    digitalWrite(clk, LOW);
  }
}


//  -- END OF FILE --

