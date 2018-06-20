/*
  BCDPDC100.h
  Created By: sy.baik, 2018-06-17
  
  This library facilitates communication with, and configuration of, 
  the BCDPDC100 Sensor(TMD2671_Digital Proximity Detector) for Arduino.  
*/

#ifndef BCDPDC100_H
#define BCDPDC100_h

#include <Arduino.h>
#include <Wire.h>

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

  
class BCDPDC100
{
  public:
    BCDPDC100(uint8_t addr);          // Initialize the BCDPDC100
    uint16_t readProximityAdc(void);  // Returns the ADC data
    void enableInterruptMask(void);   //Enables the Interrupt Mask
    void enableWait(void);            //Activates the wait feature.
    void enableProximity(void);       //Activates proximity function
    void powerOn(void);               //Activates the internal oscillator(timer and ADC)
    void enableWlong(void);           //Set Wait long config

    void disableInterruptMask(void);   
    void disableWait(void);            
    void disableProximity(void);       
    void powerOff(void);               
    void disableWlong(void); 

    uint8_t readEnable(void);         //read Interrupt Mask, wait_En, Proximity_En, Power on

    void writeAtime(uint8_t);         //ALS timing period setting (Atime = (256-inputValue)*2.72 ms)
    void writePtime(uint8_t);         //Proximity timing period setting (Atime = (256-inputValue)*2.72 ms) recommentded 0xFF_ 2.72ms
 
    /* if WLONG =0 :
       Wait timing period setting (Atime = (256-inputValue)*2.72 ms)
       if WLONG =1 :
       Wait timing period setting (Atime = (256-inputValue)*2.72*12 ms)  */
    void writeWtime(uint8_t);         
   
    uint8_t readAtime(void);
    uint8_t readPtime(void);
    uint16_t readWtime(void);

    void setPpulse(uint8_t);         
    uint8_t readPpulse(void);
    
    void setLowThresold(uint16_t);      //Set Proximity low threshold
    void setHighThresold(uint16_t);     //Set Proximity high threshold

    uint16_t readLowThresold(void);
    uint16_t readHighThresold(void);

    /* Proximity Interrupt persistence. 
      Controls rate of perximity interrupt 
      0 : Every proximity cycle generates an interrupt
      1 : 1 proximity value out of range
      2 : 2 consecutive proximity values out of range      
          ........
      15 : 15 consecutive proximity values out of range      */
    void setPersistence(int);           //Control the filtering interrupt capabilities of the device.
    uint8_t readPersistence(void);

    void setLedPower(int);              //Set LED Drive Strength
    void setDiode(int);                 //Select Diode 
    uint8_t readControl(void);          //Read LED Drive Strength and Diode selection

    uint8_t readStatus(void);           //Read proximity Interrupt
    bool readInterrupt(void);
    
    uint8_t readDeviceID(void);         //Read Device ID

    void interruptClear(void);          
    
  private:
    int _addr;                  // Address of sensor 
    void openReg(uint8_t reg);            // Points to a given register
    uint8_t readReg(uint8_t reg);       // Reads a given register, returns 1 byte
    void writeReg(uint8_t reg, uint8_t data);   // Writes a byte of data to one register
};


#endif
