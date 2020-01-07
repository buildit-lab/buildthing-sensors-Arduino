/*
  BCALS083K.c
  Created By: sy.baik, 2018-05-27
  
  This library facilitates communication with, and configuration of, 
  the BCALS083K Sensor(OPT3001_Q1_Ambient Light Sensor) for Arduino. 
*/


#include "BCALS083K.h"

void BCALS083K_init(uint8_t addr, I2C_HandleTypeDef *hi2c)
{
	BCALS083K_addr = addr << 1;
	BCALS083K_hi2c = hi2c;
}

uint16_t BCALS083K_readReg(uint8_t reg)
{
  uint8_t data[2];
  uint16_t datas;
  
  HAL_I2C_Mem_Read(BCALS083K_hi2c, BCALS083K_addr, reg, I2C_MEMADD_SIZE_8BIT, data, 2, 10);

  datas = (uint16_t)data[0] << 8 | data[1];
  
  return datas;
}

void BCALS083K_writeReg(uint8_t reg, uint16_t datas)
{
  uint8_t data[2];
  data[0]=datas/256;
  data[1]=datas%256;
  
  HAL_I2C_Mem_Write(BCALS083K_hi2c, BCALS083K_addr, reg, I2C_MEMADD_SIZE_8BIT, data, 2, 10);
}

uint16_t BCALS083K_readManufacturerID(void) {
  uint16_t regContents;
  regContents = BCALS083K_readReg (BCALS083K_MANUFACTURER_ID);
 
  return regContents;
}

uint16_t BCALS083K_readDeviceID(void) {
  uint16_t regContents;
  regContents = BCALS083K_readReg (BCALS083K_DEVICE_ID);
  
  return regContents;
}

float BCALS083K_readResult(void) {
  uint16_t regContents, exponent, result;
  float ALS=0.01;
  regContents = BCALS083K_readReg (BCALS083K_RESULT);
  
  exponent=regContents/4096;
  result=regContents%4096;
  
  for(int i=0;i<exponent;i++)
  {
    ALS*=2;
  }
  
  return (float)ALS*result;
}

void BCALS083K_setHighLimit(uint16_t HighLimit) {
  BCALS083K_writeReg (BCALS083K_HIGH_LIMIT, HighLimit);
}

float readHighLimit(void) {
  uint16_t regContents, exponent, result;
  float ALS=0.01;
  regContents = BCALS083K_readReg (BCALS083K_HIGH_LIMIT);

  exponent=regContents/4096;
  result=regContents%4096;

  for(int i=0;i<exponent;i++)
  {
    ALS*=2;
  }
  
  return (float)ALS*result;
}

void BCALS083K_setLowLimit(uint16_t LowLimit) {
  BCALS083K_writeReg (BCALS083K_LOW_LIMIT, LowLimit);
}

float BCALS083K_readLowLimit(void) {
  uint16_t regContents, exponent, result;
  float ALS=0.01;
  regContents = BCALS083K_readReg (BCALS083K_LOW_LIMIT);

  exponent=regContents/4096;
  result=regContents%4096;

  for(int i=0;i<exponent;i++)
  {
    ALS*=2;
  }
  
  return (float)ALS*result;
}

uint16_t BCALS083K_readConfig(void) {
  uint16_t regContents;
  regContents = BCALS083K_readReg (BCALS083K_CONFIG);
  
  return regContents;
}

void BCALS083K_setRange(uint8_t range) {
  uint16_t regContents, rangeBit;
  regContents = BCALS083K_readReg (BCALS083K_CONFIG);
  rangeBit=range << 12;
  
  BCALS083K_writeReg(BCALS083K_CONFIG, regContents & 0x0FFF);
  BCALS083K_writeReg(BCALS083K_CONFIG, regContents | rangeBit);
}

void BCALS083K_setTime(int CTime) {
  uint16_t regContents;
  regContents = BCALS083K_readReg (BCALS083K_CONFIG);
  switch(CTime)
  {
    case 0: //100ms
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents & 0xF7FF);
    break;

    case 1: //800ms
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0800);
    break;

    default:  //800ms
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0800);
    break;
  }  
}

void BCALS083K_setMode(int Mode) {
  uint16_t regContents;
  regContents = BCALS083K_readReg (BCALS083K_CONFIG);

  BCALS083K_writeReg(BCALS083K_CONFIG, regContents & 0xF9FF);
  switch(Mode)
  {
    case BCALS083K_SHUTDOWN:
          
    break;

    case BCALS083K_SINGLESHOT:
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0200);
    break;

    case BCALS083K_CONTINUOUS:
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0400);
    break;
    
    default:  //COUTINUOUS
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0400);
    break;
  }  
}

void BCALS083K_setPolarity(int Polarity) {
  uint16_t regContents;
  regContents = BCALS083K_readReg (BCALS083K_CONFIG);
  
  switch(Polarity)
  {
    case BCALS083K_ACTIVE_LOW: //Interrupt pin active low
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents & 0xFFF7);
    break;

    case BCALS083K_ACTIVE_HIGH: //Interrupt pin active high
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0008);
    break;

    default:  //Interrupt pin active high
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0008);
    break;
  }  
}

void BCALS083K_setLatch(int Latch) {
  uint16_t regContents;
  regContents = BCALS083K_readReg (BCALS083K_CONFIG);
  
  switch(Latch)
  {
    case BCALS083K_HYSTERESIS_STYLE:
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents & 0xFFEF);
    break;

    case BCALS083K_WINDOW_STYLE:
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0010);
    break;

    default:  //Window style
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0010);
    break;
  }  
}

void BCALS083K_setMaskExponent(int MaskExponent) {
  uint16_t regContents;
  regContents = BCALS083K_readReg (BCALS083K_CONFIG);
  
  switch(MaskExponent)
  {
    case 0:
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents & 0xFFFB);
    break;

    case 1: //Range is set to less than 14
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0004);
    break;

    default:  //Range is set to less than 14
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0004);
    break;
  }  
}

void BCALS083K_setFaultCount(int FaultCount) {
  uint16_t regContents;
  regContents = BCALS083K_readReg (BCALS083K_CONFIG);

  BCALS083K_writeReg(BCALS083K_CONFIG, regContents & 0xFFFC);
  
  switch(FaultCount)
  {
    case BCALS083K_FAULT_CNT_ONE:
          
    break;

    case BCALS083K_FAULT_CNT_TWO:
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0001);
    break;

    case BCALS083K_FAULT_CNT_FOUR:
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0002);
    break;

    case BCALS083K_FAULT_CNT_EIGHT:
      BCALS083K_writeReg(BCALS083K_CONFIG, regContents | 0x0003);
    break;
        
    default:  //One fault count
      
    break;
  }  
}

