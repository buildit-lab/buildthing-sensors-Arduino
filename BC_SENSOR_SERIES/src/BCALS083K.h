/*
  BCALS083K.h
  Created By: sy.baik, 2018-05-27
  
  This library facilitates communication with, and configuration of, 
  the BCALS083K Sensor(OPT3001_Q1_Ambient Light Sensor) for Arduino. 
*/

#ifndef BCALS083K_H
#define BCALS083K_h

#include <Arduino.h>
#include <Wire.h>

#define RESULT 0x00
#define CONFIG 0x01
#define LOW_LIMIT 0x02
#define HIGH_LIMIT 0x03
#define MANUFACTURER_ID 0x7E
#define DEVICE_ID 0x7F

#define SHUTDOWN 0
#define SINGLESHOT 1
#define COUTINUOUS 2

#define ACTIVE_LOW 0
#define ACTIVE_HIGH 1

#define HYSTERESIS_STYLE 0
#define WINDOW_STYLE 1

#define FAULT_CNT_ONE 0
#define FAULT_CNT_TWO 1
#define FAULT_CNT_FOUR 2
#define FAULT_CNT_EIGHT 3

class BCALS083K {
public:
  BCALS083K(uint8_t addr);
  
  uint16_t readManufacturerID(void);
  uint16_t readDeviceID(void);
  float readResult(void);

  void setHighLimit(uint16_t);
  float readHighLimit(void);
    
  void setLowLimit(uint16_t);
  float readLowLimit(void); 

  uint16_t readConfig(void);

  void setRange(uint8_t);
  void setTime(int);
  void setMode(int);
  void setPolarity(int);
  void setLatch(int);
  void setMaskExponent(int);
  void setFaultCount(int);

private:
  int _addr;                  // Address of sensor 
  void openReg(uint8_t reg);            // Points to a given register  
  uint16_t readReg(uint8_t reg);       // Reads a given register, returns 2 byte
  void writeReg(uint8_t reg, uint16_t data);   // Writes a byte of data to one register
  
};

#endif 
