/*
  BCAQS1187.cpp
  Created By: sy.baik, 2018-06-17
  
  This library facilitates communication with, and configuration of, 
  the BCAQS1187 Sensor(BCAQS1187_Indoor air quality sensor) for Arduino.  
*/


#include "BCAQS1187.h"
#include <Wire.h>

BCAQS1187::BCAQS1187(uint8_t addr)
{
  _addr = addr;  
}

void BCAQS1187::openReg(uint8_t reg)
{
  Wire.beginTransmission(_addr);    // Connect to BCAQS1187
  Wire.write(reg);            // point to specified register
  Wire.endTransmission();         // Relinquish bus control
}

uint8_t BCAQS1187::readReg(uint8_t reg)
{
  openReg(reg);
  uint8_t reading;          // holds byte of read data
  Wire.requestFrom(_addr, 1);     // Request 1 byte from open register
  Wire.endTransmission();       // Relinquish bus control
  
  if (1 <= Wire.available())
  {
    reading = (Wire.read());      // Read byte
  }
  
  return reading;
}

void BCAQS1187::writeReg(uint8_t reg, uint8_t data)
{  
  Wire.beginTransmission(_addr);    // Open Device
  Wire.write(reg);            // Point to register
  Wire.write(data);           // Write data to register 
  Wire.endTransmission();       // Relinquish bus control
  
}

void BCAQS1187::appStart(void)
{ 
  openReg(BCAQS1187_BOOTLOADER_APP_START);
}

uint8_t BCAQS1187::readHW_ID(void)
{
  uint8_t contents;
  contents = readReg(BCAQS1187_HW_ID);

  return contents;
}

uint8_t BCAQS1187::readError(void)
{
  uint8_t contents;
  contents = readReg(BCAQS1187_ERROR_ID);

  return contents;
}

uint8_t BCAQS1187::readStatus(void)
{
  uint8_t contents;
  contents = readReg(BCAQS1187_STATUS);

  return contents;
}

uint8_t BCAQS1187::readMeasMode(void)
{
  uint8_t contents;
  contents = readReg(BCAQS1187_MEAS_MODE);

  return contents;
}

void BCAQS1187::setThresholds(uint16_t low_med, uint16_t med_high, uint8_t hysteresis)
{
  uint8_t data[5] = {(uint8_t)((low_med >> 8) & 0xF), (uint8_t)(low_med & 0xF),
  (uint8_t)((med_high >> 8) & 0xF), (uint8_t)(med_high & 0xF), hysteresis};
 
  Wire.beginTransmission(_addr);    // Open Device
  Wire.write(BCAQS1187_THRESHOLDS);            // Point to register
  Wire.write(data[0]);           // Write data to register 
  Wire.write(data[1]);           // Write data to register 
  Wire.write(data[2]);           // Write data to register 
  Wire.write(data[3]);           // Write data to register 
  Wire.write(data[4]);           // Write data to register 
  Wire.endTransmission();       // Relinquish bus control;
}

void BCAQS1187::SWReset(void)
{
  //reset sequence from the datasheet
  uint8_t data[] = {0x11, 0xE5, 0x72, 0x8A};
  Wire.beginTransmission(_addr);    // Open Device
  Wire.write(BCAQS1187_SW_RESET);            // Point to register
  Wire.write(data[0]);           // Write data to register 
  Wire.write(data[1]);           // Write data to register 
  Wire.write(data[2]);           // Write data to register 
  Wire.write(data[3]);           // Write data to register 
  Wire.endTransmission();       // Relinquish bus control  
}

void BCAQS1187::setEnvironmentalData(uint16_t humidity, uint16_t temperature)
{
  /* Humidity is stored as an unsigned 16 bits in 1/512%RH. The
     default value is 50% = 0x64, 0x00. As an example 48.5%
     humidity would be 0x61, 0x00.*/
  
  /* Temperature is stored as an unsigned 16 bits integer in 1/512
     degrees; there is an offset: 0 maps to -25°C. The default value is
     25°C = 0x64, 0x00. As an example 23.5% temperature would be  0x61, 0x00.
     The internal algorithm uses these values (or default values if
     not set by the application) to compensate for changes in
     relative humidity and ambient temperature.*/

  uint8_t data[4] = {(uint8_t)((humidity >> 8) & 0xFF),
                   (uint8_t)(humidity & 0xFF),
                   (uint8_t)((temperature >> 8) & 0xFF),
                   (uint8_t)(temperature & 0xFF)};
  
  Wire.beginTransmission(_addr);    // Open Device
  Wire.write(BCAQS1187_ENV_DATA);            // Point to register
  Wire.write(data[0]);           // Write data to register 
  Wire.write(data[1]);           // Write data to register 
  Wire.write(data[2]);           // Write data to register 
  Wire.write(data[3]);           // Write data to register 
  Wire.endTransmission();       // Relinquish bus control  
}

void BCAQS1187::setDriveMode(uint8_t mode)
{
  uint8_t contents;
  contents = readReg(BCAQS1187_MEAS_MODE);

  mode = mode&0x07;
  contents = contents & 0x8F;

  writeReg(BCAQS1187_MEAS_MODE, contents | (mode << 4) );
}

void BCAQS1187::enableInterrupt(void)
{
  uint8_t contents;
  contents = readReg(BCAQS1187_MEAS_MODE);

  contents = contents | 0x08; 
  
  writeReg(BCAQS1187_MEAS_MODE, contents);
}

void BCAQS1187::disableInterrupt(void)
{
  uint8_t contents;
  contents = readReg(BCAQS1187_MEAS_MODE);

  contents = contents & 0xF7; 
  
  writeReg(BCAQS1187_MEAS_MODE, contents);
}

bool BCAQS1187::dataReady(void)
{
  uint8_t contents;  
  contents = readReg(BCAQS1187_STATUS);

  contents = contents & 0x08;
  
  if(contents == 0x08)
    return false;
  else return true;
}

uint8_t BCAQS1187::readResultData(void)
{
  openReg(BCAQS1187_ALG_RESULT_DATA);
  int i=0;
  uint8_t reading[8];          // holds byte of read data
  Wire.requestFrom(_addr, 8);     // Request 8 byte from open register
  Wire.endTransmission();       // Relinquish bus control

  for(int i=0;i<8;i++)
  {
    reading[i] = (Wire.read());      // Read byte    
  }
  
    
  
  _eCO2 = ((uint16_t)reading[0] << 8) | ((uint16_t)reading[1]);
  _TVOC = ((uint16_t)reading[2] << 8) | ((uint16_t)reading[3]);
    
  return reading[5];    
  
}

uint16_t BCAQS1187::getTVOC(void) 
{ 
  return _TVOC;
}

uint16_t BCAQS1187::geteCO2(void) 
{ 
  return _eCO2; 
}

double BCAQS1187::readNTC()
{
  openReg(BCAQS1187_NTC);
  int i=0;
  uint8_t reading[4];          // holds byte of read data
  Wire.requestFrom(_addr, 4);     // Request 1 byte from open register
  Wire.endTransmission();       // Relinquish bus control
  
  if (1 <= Wire.available()||i>=4)
  {
    reading[i] = (Wire.read());      // Read byte
    i++;
  }  

  uint32_t vref = ((uint32_t)reading[0] << 8) | reading[1];
  uint32_t vntc = ((uint32_t)reading[2] << 8) | reading[3];
  
  //from ams ccs811 app note
  uint32_t rntc = vntc * 100000 / vref;
  
  double ntc_temp;
  ntc_temp = log((double)rntc / 100000); // 1
  ntc_temp /= 3380; // 2
  ntc_temp += 1.0 / (25 + 273.15); // 3
  ntc_temp = 1.0 / ntc_temp; // 4
  ntc_temp -= 273.15; // 5
  return ntc_temp;
}

