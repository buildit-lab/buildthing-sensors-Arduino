/*
  BCALS083K.h
  Created By: sy.baik, 2018-05-27

  This library facilitates communication with, and configuration of,
  the BCALS083K Sensor(OPT3001_Q1_Ambient Light Sensor) for Arduino.
*/

#ifndef BCALS083K_H
#define BCALS083K_h

#include "main.h"

#define BCALS083K_RESULT 0x00
#define BCALS083K_CONFIG 0x01
#define BCALS083K_LOW_LIMIT 0x02
#define BCALS083K_HIGH_LIMIT 0x03
#define BCALS083K_MANUFACTURER_ID 0x7E
#define BCALS083K_DEVICE_ID 0x7F

#define BCALS083K_SHUTDOWN 0
#define BCALS083K_SINGLESHOT 1
#define BCALS083K_CONTINUOUS 2

#define BCALS083K_ACTIVE_LOW 0
#define BCALS083K_ACTIVE_HIGH 1

#define BCALS083K_HYSTERESIS_STYLE 0
#define BCALS083K_WINDOW_STYLE 1

#define BCALS083K_FAULT_CNT_ONE 0
#define BCALS083K_FAULT_CNT_TWO 1
#define BCALS083K_FAULT_CNT_FOUR 2
#define BCALS083K_FAULT_CNT_EIGHT 3

static uint8_t BCALS083K_addr;
static I2C_HandleTypeDef *BCALS083K_hi2c;

void BCALS083K_init(uint8_t addr, I2C_HandleTypeDef *hi2c);

uint16_t BCALS083K_readManufacturerID(void);
uint16_t BCALS083K_readDeviceID(void);
float BCALS083K_readResult(void);

void BCALS083K_setHighLimit(uint16_t);
float BCALS083K_readHighLimit(void);

void BCALS083K_setLowLimit(uint16_t);
float BCALS083K_readLowLimit(void);

uint16_t BCALS083K_readConfig(void);

void BCALS083K_setRange(uint8_t);
void BCALS083K_setTime(int);
void BCALS083K_setMode(int);
void BCALS083K_setPolarity(int);
void BCALS083K_setLatch(int);
void BCALS083K_setMaskExponent(int);
void BCALS083K_setFaultCount(int);

uint16_t BCALS083K_readReg(uint8_t reg);       // Reads a given register, returns 2 byte
void BCALS083K_writeReg(uint8_t reg, uint16_t data);   // Writes a byte of data to one register



#endif 
