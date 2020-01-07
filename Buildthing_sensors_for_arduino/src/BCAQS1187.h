/*
  BCAQS1187.h
  Created By: sy.baik, 2018-06-17
  
  This library facilitates communication with, and configuration of, 
  the BCAQS1187 Sensor(CCS811_Indoor air quality sensor) for Arduino.  
*/

#ifndef BCAQS1187_H
#define BCAQS1187_h

#include <Arduino.h>
#include <Wire.h>

#define BCAQS1187_STATUS 0x00
#define BCAQS1187_MEAS_MODE 0x01
#define BCAQS1187_ALG_RESULT_DATA 0x02
#define BCAQS1187_RAW_DATA 0x03
#define BCAQS1187_ENV_DATA 0x05
#define BCAQS1187_NTC 0x06
#define BCAQS1187_THRESHOLDS 0x10
#define BCAQS1187_BASELINE 0x11
#define BCAQS1187_HW_ID 0x20
#define BCAQS1187_HW_VERSION 0x21
#define BCAQS1187_FW_BOOT_VERSION 0x23
#define BCAQS1187_FW_APP_VERSION 0x24
#define BCAQS1187_ERROR_ID 0xE0
#define BCAQS1187_SW_RESET 0xFF

  
  //bootloader registers

#define BCAQS1187_BOOTLOADER_APP_ERASE 0xF1
#define BCAQS1187_BOOTLOADER_APP_DATA 0xF2
#define BCAQS1187_BOOTLOADER_APP_VERIFY 0xF3
#define BCAQS1187_BOOTLOADER_APP_START 0xF4

#define BCAQS1187_DRIVE_MODE_IDLE 0x00
#define BCAQS1187_DRIVE_MODE_1SEC 0x01
#define BCAQS1187_DRIVE_MODE_10SEC 0x02
#define BCAQS1187_DRIVE_MODE_60SEC 0x03
#define BCAQS1187_DRIVE_MODE_250MS 0x04


class BCAQS1187 {
  public:
    BCAQS1187(uint8_t addr);

    void appStart(void);
    uint8_t readHW_ID(void);    
    uint8_t readError(void);    
    uint8_t readStatus(void);    
    uint8_t readMeasMode(void);
    void setEnvironmentalData(uint16_t, uint16_t);    
    
    void setThresholds(uint16_t, uint16_t, uint8_t);
    void SWReset(void);
    
    void setDriveMode(uint8_t);
    void enableInterrupt(void);
    void disableInterrupt(void);
    
    bool dataReady(void);                     // 0 mean data is ready
    uint8_t readResultData(void);             // return status
    uint16_t getTVOC(void); //{ return _TVOC; }
    uint16_t geteCO2(void); //{ return _eCO2; }

    double readNTC(void);


  private:
    int _addr;                  // Address of sensor 
    uint16_t _TVOC;
    uint16_t _eCO2;

    void openReg(uint8_t reg);    
    uint8_t readReg(uint8_t reg);    
    void writeReg(uint8_t reg, uint8_t value);    
};

#endif
