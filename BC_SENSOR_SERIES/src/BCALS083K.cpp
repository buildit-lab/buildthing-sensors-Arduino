/*
  BCALS083K.c
  Created By: sy.baik, 2018-05-27
  
  This library facilitates communication with, and configuration of, 
  the BCALS083K Sensor(OPT3001_Q1_Ambient Light Sensor) for Arduino. 
*/
#include <Wire.h>

#include "BCALS083K.h"

BCALS083K::BCALS083K(uint8_t addr)
{
  _addr=addr;
}

void BCALS083K::openReg(uint8_t reg)
{
  Wire.beginTransmission(_addr);     // Connect to BCALS083K
  Wire.write(reg);            // point to specified register
  Wire.endTransmission();         // Relinquish bus control
}

uint16_t BCALS083K::readReg(uint8_t reg)
{
  openReg(reg);
  uint8_t data[2];
  uint16_t datas;
    
  Wire.requestFrom(_addr, 2);
  Wire.endTransmission();

  int counter = 0;
  while (Wire.available() < 2)
  {
    counter++;
    delay(10);
    if (counter > 250)
      return 0;
  }

  Wire.readBytes(data, 2);
  datas = (uint16_t)data[0] << 8 | data[1];
  
  return datas;
}

void BCALS083K::writeReg(uint8_t reg, uint16_t datas)
{
  uint8_t data[2];
  data[0]=datas/256;
  data[1]=datas%256;
  
  Wire.beginTransmission(_addr);    // Open Device
  Wire.write(reg);            // Point to register
  Wire.write(data[0]);           // Write data to register 
  Wire.write(data[1]);           // Write data to register 
  Wire.endTransmission();       // Relinquish bus control  
}

uint16_t BCALS083K::readManufacturerID(void) { 
  uint16_t regContents;
  regContents = readReg (MANUFACTURER_ID);
 
  return regContents;
}

uint16_t BCALS083K::readDeviceID(void) {
  uint16_t regContents;
  regContents = readReg (DEVICE_ID);
  
  return regContents;
}

float BCALS083K::readResult(void) {
  uint16_t regContents, exponent, result;
  float ALS=0.01;
  regContents = readReg (RESULT);

  exponent=regContents/4096;
  result=regContents%4096;

  for(int i=0;i<exponent;i++)
  {
    ALS*=2;
  }
  
  return (float)ALS*result;
}

void BCALS083K::setHighLimit(uint16_t HighLimit) {  
  writeReg (HIGH_LIMIT, HighLimit);  
}

float BCALS083K::readHighLimit(void) {
  uint16_t regContents, exponent, result;
  float ALS=0.01;
  regContents = readReg (HIGH_LIMIT);

  exponent=regContents/4096;
  result=regContents%4096;

  for(int i=0;i<exponent;i++)
  {
    ALS*=2;
  }
  
  return (float)ALS*result;
}

void BCALS083K::setLowLimit(uint16_t LowLimit) {  
  writeReg (LOW_LIMIT, LowLimit);  
}

float BCALS083K::readLowLimit(void) {
  uint16_t regContents, exponent, result;
  float ALS=0.01;
  regContents = readReg (LOW_LIMIT);

  exponent=regContents/4096;
  result=regContents%4096;

  for(int i=0;i<exponent;i++)
  {
    ALS*=2;
  }
  
  return (float)ALS*result;
}

uint16_t BCALS083K::readConfig(void) {
  uint16_t regContents;
  regContents = readReg (CONFIG);
  
  return regContents;
}

void BCALS083K::setRange(uint8_t range) {
  uint16_t regContents, rangeBit;
  regContents = readReg (CONFIG);
  rangeBit=range << 12;
  
  writeReg(CONFIG, regContents & 0x0FFF);
  writeReg(CONFIG, regContents | rangeBit);
}

void BCALS083K::setTime(int CTime) {
  uint16_t regContents;
  regContents = readReg (CONFIG);
  
  switch(CTime)
  {
    case 0: //100ms
      writeReg(CONFIG, regContents & 0xF7FF);
    break;

    case 1: //800ms
      writeReg(CONFIG, regContents | 0x0800);
    break;

    default:  //800ms
      writeReg(CONFIG, regContents | 0x0800);
    break;
  }  
}

void BCALS083K::setMode(int Mode) {
  uint16_t regContents;
  regContents = readReg (CONFIG);

  writeReg(CONFIG, regContents & 0xF9FF);
  
  switch(Mode)
  {
    case SHUTDOWN:
          
    break;

    case SINGLESHOT:
      writeReg(CONFIG, regContents | 0x0200);
    break;

    case COUTINUOUS:
      writeReg(CONFIG, regContents | 0x0400);
    break;
    
    default:  //COUTINUOUS
      writeReg(CONFIG, regContents | 0x0400);
    break;
  }  
}

void BCALS083K::setPolarity(int Polarity) {
  uint16_t regContents;
  regContents = readReg (CONFIG);
  
  switch(Polarity)
  {
    case ACTIVE_LOW: //Interrupt pin active low
      writeReg(CONFIG, regContents & 0xFFF7);
    break;

    case ACTIVE_HIGH: //Interrupt pin active high
      writeReg(CONFIG, regContents | 0x0008);
    break;

    default:  //Interrupt pin active high
      writeReg(CONFIG, regContents | 0x0008);
    break;
  }  
}

void BCALS083K::setLatch(int Latch) {
  uint16_t regContents;
  regContents = readReg (CONFIG);
  
  switch(Latch)
  {
    case HYSTERESIS_STYLE: 
      writeReg(CONFIG, regContents & 0xFFEF);
    break;

    case WINDOW_STYLE: 
      writeReg(CONFIG, regContents | 0x0010);
    break;

    default:  //Window style
      writeReg(CONFIG, regContents | 0x0010);
    break;
  }  
}

void BCALS083K::setMaskExponent(int MaskExponent) {
  uint16_t regContents;
  regContents = readReg (CONFIG);
  
  switch(MaskExponent)
  {
    case 0:
      writeReg(CONFIG, regContents & 0xFFFB);
    break;

    case 1: //Range is set to less than 14
      writeReg(CONFIG, regContents | 0x0004);
    break;

    default:  //Range is set to less than 14
      writeReg(CONFIG, regContents | 0x0004);
    break;
  }  
}

void BCALS083K::setFaultCount(int FaultCount) {
  uint16_t regContents;
  regContents = readReg (CONFIG);

  writeReg(CONFIG, regContents & 0xFFFC);
  
  switch(FaultCount)
  {
    case FAULT_CNT_ONE:
          
    break;

    case FAULT_CNT_TWO:
      writeReg(CONFIG, regContents | 0x0001);
    break;

    case FAULT_CNT_FOUR:
      writeReg(CONFIG, regContents | 0x0002);
    break;

    case FAULT_CNT_EIGHT:
      writeReg(CONFIG, regContents | 0x0003);
    break;
        
    default:  //One fault count
      
    break;
  }  
}

