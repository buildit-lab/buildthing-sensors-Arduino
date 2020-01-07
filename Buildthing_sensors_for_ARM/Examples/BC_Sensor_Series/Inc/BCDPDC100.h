/*
  BCDPDC100.h
  Created By: sy.baik, 2018-06-17

  This library facilitates communication with, and configuration of,
  the BCDPDC100 Sensor(TMD2671_Digital Proximity Detector) for Arduino.
*/

#ifndef BCDPDC100_H
#define BCDPDC100_h

#include <main.h>

//Define Register Map
#define BCDPDC100_ENABLE 0x00
#define BCDPDC100_ATIME 0x01
#define BCDPDC100_PTIME 0x02
#define BCDPDC100_WTIME 0x03
#define BCDPDC100_PILTL 0x08
#define BCDPDC100_PILTH 0x09
#define BCDPDC100_PIHTL 0x0A
#define BCDPDC100_PIHTH 0x0B
#define BCDPDC100_PERS 0x0C
#define BCDPDC100_CONFIG 0x0D
#define BCDPDC100_PPULSE 0x0E
#define BCDPDC100_CONTROL 0x0F
#define BCDPDC100_DEVICE_ID 0x12
#define BCDPDC100_STATUS 0x13
#define BCDPDC100_PDATAL 0x18
#define BCDPDC100_PDATAH 0x19



//  Command Register
#define BCDPDC100_REPEATED_BYTE_PROTOCOL 0x80
#define BCDPDC100_AUTO_INCREMENT_PROTOCOL 0xA0
#define BCDPDC100_PROXIMITY_INTERRUPT_CLEAR 0xE5




//  Constants for setting timing
#define BCDPDC100_WLONG_ON 0x02
#define BCDPDC100_WLONG_OFF 0x00

//  Constants for LED Strength
#define BCDPDC100_LED100P 0
#define BCDPDC100_LED50P 1
#define BCDPDC100_LED25P 2
#define BCDPDC100_LED12D5P 3

//  Constants for diode selection
#define BCDPDC100_CHANNEL0 0
#define BCDPDC100_CHANNEL1 1
#define BCDPDC100_BOTH 2

static uint8_t BCDPDC100_addr;
static I2C_HandleTypeDef *BCDPDC100_hi2c;

void BCDPDC100_init(uint8_t addr, I2C_HandleTypeDef *hi2c);  //Set I2C_device address & I2C_Handler


uint16_t BCDPDC100_readProximityAdc(void);  // Returns the ADC data
void BCDPDC100_enableInterruptMask(void);   //Enables the Interrupt Mask
void BCDPDC100_enableWait(void);            //Activates the wait feature.
void BCDPDC100_enableProximity(void);       //Activates proximity function
void BCDPDC100_powerOn(void);               //Activates the internal oscillator(timer and ADC)
void BCDPDC100_enableWlong(void);           //Set Wait long config

void BCDPDC100_disableInterruptMask(void);
void BCDPDC100_disableWait(void);
void BCDPDC100_disableProximity(void);
void BCDPDC100_powerOff(void);
void BCDPDC100_disableWlong(void);

uint8_t BCDPDC100_readEnable(void);         //read Interrupt Mask, wait_En, Proximity_En, Power on

void BCDPDC100_writeAtime(uint8_t);         //ALS timing period setting (Atime = (256-inputValue)*2.72 ms)
void BCDPDC100_writePtime(uint8_t);         //Proximity timing period setting (Atime = (256-inputValue)*2.72 ms) recommentded 0xFF_ 2.72ms

/* if WLONG =0 :
   Wait timing period setting (Atime = (256-inputValue)*2.72 ms)
   if WLONG =1 :
   Wait timing period setting (Atime = (256-inputValue)*2.72*12 ms)  */
void BCDPDC100_writeWtime(uint8_t);

uint8_t BCDPDC100_readAtime(void);
uint8_t BCDPDC100_readPtime(void);
uint16_t BCDPDC100_readWtime(void);

void BCDPDC100_setPpulse(uint8_t);
uint8_t BCDPDC100_readPpulse(void);

void BCDPDC100_setLowThresold(uint16_t);      //Set Proximity low threshold
void BCDPDC100_setHighThresold(uint16_t);     //Set Proximity high threshold

uint16_t BCDPDC100_readLowThresold(void);
uint16_t BCDPDC100_readHighThresold(void);

/* Proximity Interrupt persistence.
  Controls rate of perximity interrupt
  0 : Every proximity cycle generates an interrupt
  1 : 1 proximity value out of range
  2 : 2 consecutive proximity values out of range
	  ........
  15 : 15 consecutive proximity values out of range      */
void BCDPDC100_setPersistence(int);           //Control the filtering interrupt capabilities of the device.
uint8_t BCDPDC100_readPersistence(void);

void BCDPDC100_setLedPower(int);              //Set LED Drive Strength
void BCDPDC100_setDiode(int);                 //Select Diode 
uint8_t BCDPDC100_readControl(void);          //Read LED Drive Strength and Diode selection

uint8_t BCDPDC100_readStatus(void);           //Read proximity Interrupt
uint8_t BCDPDC100_readInterrupt(void);

uint8_t BCDPDC100_readDeviceID(void);         //Read Device ID

void BCDPDC100_interruptClear(void);

void BCDPDC100_openReg(uint8_t reg);            // Points to a given register
uint8_t BCDPDC100_readReg(uint8_t reg);       // Reads a given register, returns 1 byte
void BCDPDC100_writeReg(uint8_t reg, uint8_t data);   // Writes a byte of data to one register

#endif
