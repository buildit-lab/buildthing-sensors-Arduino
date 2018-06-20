/*
  BCDPDC100.cpp
  Created By: sy.baik, 2018-06-17
  
  This library facilitates communication with, and configuration of, 
  the BCDPDC100 Sensor(TMD2671_Digital Proximity Detector) for Arduino.  
*/


#include "BCDPDC100.h"
#include <Wire.h>
  
BCDPDC100::BCDPDC100(uint8_t addr)
{
  _addr = addr;  
}

void BCDPDC100::openReg(uint8_t reg)
{
  Wire.beginTransmission(_addr);    // Connect to BCDPDC100
  Wire.write(reg | BCDPDC100_REPEATED_BYTE_PROTOCOL);            // point to specified register
  Wire.endTransmission();         // Relinquish bus control
}

uint8_t BCDPDC100::readReg(uint8_t reg)
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

void BCDPDC100::writeReg(uint8_t reg, uint8_t data)
{  
  Wire.beginTransmission(_addr);    // Open Device
  Wire.write(reg | BCDPDC100_REPEATED_BYTE_PROTOCOL);            // Point to register
  Wire.write(data);           // Write data to register 
  Wire.endTransmission();       // Relinquish bus control
  
}

uint16_t BCDPDC100::readProximityAdc(void)
{
  uint8_t data[2];
  uint16_t promximityAdc;
  data[0] = readReg(BCDPDC100_PDATAL);
  data[1] = readReg(BCDPDC100_PDATAH);
  
  promximityAdc = (uint16_t)data[1] << 8 | data[0];
  
  return promximityAdc;  
}

void BCDPDC100::enableInterruptMask(void)
{
  uint8_t configContents; //Stores current contents of config register
  
  configContents = readReg(BCDPDC100_ENABLE);
  
  //set bit 5 to 1 to enable
  configContents = (configContents | 0x20);
  
  writeReg(BCDPDC100_ENABLE, configContents);  
}

void BCDPDC100::disableInterruptMask(void)
{
  uint8_t configContents; //Stores current contents of config register
  
  configContents = readReg(BCDPDC100_ENABLE);
  
  //set bit 5 to 0 to disable (all other bits 1)
  configContents = (configContents & 0xDF);
  writeReg(BCDPDC100_ENABLE, configContents);  
}

void BCDPDC100::enableWait(void)
{
  uint8_t configContents; //Stores current contents of config register
  
  configContents = readReg(BCDPDC100_ENABLE);
  
  //set bit 3 to 1 to enable
  configContents = (configContents | 0x08);
  
  writeReg(BCDPDC100_ENABLE, configContents);  
}

void BCDPDC100::disableWait(void)
{
  uint8_t configContents; //Stores current contents of config register
  
  configContents = readReg(BCDPDC100_ENABLE);
  
  //set bit 3 to 0 to disable (all other bits 1)
  configContents = (configContents & 0xF7);
  writeReg(BCDPDC100_ENABLE, configContents);  
}

void BCDPDC100::enableProximity(void)
{
  uint8_t configContents; //Stores current contents of config register
  
  configContents = readReg(BCDPDC100_ENABLE);
  
  //set bit 2:1 to 1 to enable
  configContents = (configContents | 0x06);
  
  writeReg(BCDPDC100_ENABLE, configContents);  
}

void BCDPDC100::disableProximity(void)
{
  uint8_t configContents; //Stores current contents of config register
  
  configContents = readReg(BCDPDC100_ENABLE);
  
  //set bit 2:1 to 0 to disable (all other bits 1)
  configContents = (configContents & 0xF9);
  writeReg(BCDPDC100_ENABLE, configContents);  
}

void BCDPDC100::powerOn(void)
{
  uint8_t configContents; //Stores current contents of config register
  
  configContents = readReg(BCDPDC100_ENABLE);
  
  //set bit 0 to 1 to enable
  configContents = (configContents | 0x01);
  
  writeReg(BCDPDC100_ENABLE, configContents);  
}

void BCDPDC100::powerOff(void)
{
  uint8_t configContents; //Stores current contents of config register
  
  configContents = readReg(BCDPDC100_ENABLE);
  
  //set bit 0 to 0 to disable (all other bits 1)
  configContents = (configContents & 0xFE);
  writeReg(BCDPDC100_ENABLE, configContents);  
}

uint8_t BCDPDC100::readEnable(void)
{
  uint8_t configContents; //Stores current contents of config register
  
  configContents = readReg(BCDPDC100_ENABLE);

  return configContents;
}

void BCDPDC100::enableWlong(void)
{
  uint8_t configContents; //Stores current contents of config register
  
  configContents = readReg(BCDPDC100_CONFIG);
  
  //set bit 1 to 1 to enable
  configContents = (configContents | 0x02);
  
  writeReg(BCDPDC100_CONFIG, configContents);  
}

void BCDPDC100::disableWlong(void)
{
  uint8_t configContents; //Stores current contents of config register
  
  configContents = readReg(BCDPDC100_CONFIG);
  
  //set bit 1 to 0 to heater (all other bits 1)
  configContents = (configContents & 0xFD);
  writeReg(BCDPDC100_CONFIG, configContents);  
}

void BCDPDC100::writeAtime(uint8_t value)
{  
  writeReg(BCDPDC100_ATIME, value);  
}

uint8_t BCDPDC100::readAtime(void)
{
  uint8_t Atime;
  Atime = readReg(BCDPDC100_ATIME);
  
  return Atime;  
}

void BCDPDC100::writePtime(uint8_t value)
{  
  writeReg(BCDPDC100_PTIME, value);  
}

uint8_t BCDPDC100::readPtime(void)
{
  uint8_t Ptime;
  Ptime = readReg(BCDPDC100_PTIME);
  
  return Ptime;  
}

void BCDPDC100::writeWtime(uint8_t value)
{  
  writeReg(BCDPDC100_WTIME, value);  
}

uint16_t BCDPDC100::readWtime(void)
{
  uint8_t data[2];
  uint16_t Wtime;
  data[0] = readReg(BCDPDC100_WTIME);
  data[1] = readReg(BCDPDC100_CONFIG);
  
  Wtime = (uint16_t)data[1] << 7 | data[0];
  
  return Wtime;  
}


void BCDPDC100::setPpulse(uint8_t p_pluse)
{   
  writeReg(BCDPDC100_PPULSE, p_pluse);
}

uint8_t BCDPDC100::readPpulse(void)
{   
  uint8_t contents;
  contents = readReg(BCDPDC100_PPULSE);

  return contents;
}


void BCDPDC100::setLowThresold(uint16_t low_threshold)
{ 
  uint8_t data[2];
  data[0]=low_threshold % 256;
  data[1]=low_threshold / 256;
  writeReg(BCDPDC100_PILTL, data[0]);
  writeReg(BCDPDC100_PILTH, data[1]);
}

uint16_t BCDPDC100::readLowThresold(void)
{
  uint8_t data[2];
  uint16_t LowThresold;
  data[0] = readReg(BCDPDC100_PILTL);
  data[1] = readReg(BCDPDC100_PILTH);
  
  LowThresold = (uint16_t)data[1] << 8 | data[0];
  
  return LowThresold;  
}

void BCDPDC100::setHighThresold(uint16_t high_threshold)
{ 
  uint8_t data[2];
  data[0]=high_threshold % 256;
  data[1]=high_threshold / 256;
  writeReg(BCDPDC100_PIHTL, data[0]);
  writeReg(BCDPDC100_PIHTH, data[1]);
}

uint16_t BCDPDC100::readHighThresold(void)
{
  uint8_t data[2];
  uint16_t HighThresold;
  data[0] = readReg(BCDPDC100_PIHTL);
  data[1] = readReg(BCDPDC100_PIHTH);
  
  HighThresold = (uint16_t)data[1] << 8 | data[0];
  
  return HighThresold;  
}


void BCDPDC100::setPersistence(int persistence)
{ 
  uint8_t configContents; 
  
  switch(persistence)
  {
    case 0:
      configContents = 0x00;
      break;
      
    case 1:
      configContents = 0x10;
      break;

    case 2:
      configContents = 0x20;
      break;

    case 3:
      configContents = 0x30;
      break;

    case 4:
      configContents = 0x40;
      break;

    case 5:
      configContents = 0x50;
      break;

    case 6:
      configContents = 0x60;
      break;

    case 7:
      configContents = 0x70;
      break;

    case 8:
      configContents = 0x80;
      break;

    case 9:
      configContents = 0x90;
      break;

    case 10:
      configContents = 0xA0;
      break;

    case 11:
      configContents = 0xB0;
      break;

    case 12:
      configContents = 0xC0;
      break;

    case 13:
      configContents = 0xD0;
      break;

    case 14:
      configContents = 0xE0;
      break;

    case 15:
      configContents = 0xF0;
      break;
      
    default:
      configContents = 0x00;
  }
  
  writeReg(BCDPDC100_PERS, configContents);  
}

uint8_t BCDPDC100::readPersistence(void)
{
  uint8_t persistence;
  persistence = readReg(BCDPDC100_PERS);
  
  return persistence;  
}


void BCDPDC100::setLedPower(int led_power)
{ 
  uint8_t configContents; 
  configContents = readReg(BCDPDC100_CONTROL);
  
  switch(led_power)
  {
    case BCDPDC100_LED100P:
      configContents = (configContents & 0x3F);      
      break;
      
    case BCDPDC100_LED50P:
      configContents = (configContents & 0x3F); 
      configContents = (configContents | 0x40); 
      break;

    case BCDPDC100_LED25P:
      configContents = (configContents & 0x3F); 
      configContents = (configContents | 0x80); 
      break;

    case BCDPDC100_LED12D5P:
      configContents = (configContents | 0xC0);      
      break;
      
    default:
      configContents = (configContents | 0xC0);
  }
  
  writeReg(BCDPDC100_CONTROL, configContents);  
}

void BCDPDC100::setDiode(int diode)
{ 
  uint8_t configContents; 
  configContents = readReg(BCDPDC100_CONTROL);
  
  switch(diode)
  {
    case BCDPDC100_CHANNEL0:
      configContents = (configContents & 0xCF);      
      configContents = (configContents | 0x10); 
      break;
      
    case BCDPDC100_CHANNEL1:
      configContents = (configContents & 0xCF); 
      configContents = (configContents | 0x20); 
      break;

    case BCDPDC100_BOTH:      
      configContents = (configContents | 0x30); 
      break;
      
    default:
      configContents = (configContents & 0xCF);      
      configContents = (configContents | 0x10);
  }
  
  writeReg(BCDPDC100_CONTROL, configContents);  
}

uint8_t BCDPDC100::readControl(void)
{
  uint8_t Control;
  Control = readReg(BCDPDC100_CONTROL);
  
  return Control;  
}

uint8_t BCDPDC100::readStatus(void)
{
  uint8_t Status;
  Status = readReg(BCDPDC100_STATUS);
  
  return Status;  
}

bool BCDPDC100::readInterrupt(void)
{
  uint8_t Status;
  Status = readReg(BCDPDC100_STATUS);  
  interruptClear();
  
  return (bool)((Status>>5)&0x01) ;  
}

uint8_t BCDPDC100::readDeviceID(void)
{
  uint8_t DeviceID;
  DeviceID = readReg(BCDPDC100_DEVICE_ID);
  
  return DeviceID;  
}

void BCDPDC100::interruptClear(void)
{
  Wire.beginTransmission(_addr);    // Open Device
  Wire.write(BCDPDC100_PROXIMITY_INTERRUPT_CLEAR);            // Point to register  
  Wire.endTransmission();       // Relinquish bus control
}

