/*
  BCACC316G.h
  Created By: sy.baik, 2018-06-19
  
  This library facilitates communication with, and configuration of, 
  the BCACC316G Sensor(ADXL343_3axis accelerometer) for Arduino. 
*/

#ifndef BCACC316G_h
#define BCACC316G_h

#include <Arduino.h>
#include <Wire.h>

/* ------- Register names ------- */
#define BCACC316G_DEVID 0x00
#define BCACC316G_RESERVED1 0x01
#define BCACC316G_THRESH_TAP 0x1d
#define BCACC316G_OFSX 0x1e
#define BCACC316G_OFSY 0x1f
#define BCACC316G_OFSZ 0x20
#define BCACC316G_DUR 0x21
#define BCACC316G_LATENT 0x22
#define BCACC316G_WINDOW 0x23
#define BCACC316G_THRESH_ACT 0x24
#define BCACC316G_THRESH_INACT 0x25
#define BCACC316G_TIME_INACT 0x26
#define BCACC316G_ACT_INACT_CTL 0x27
#define BCACC316G_THRESH_FF 0x28
#define BCACC316G_TIME_FF 0x29
#define BCACC316G_TAP_AXES 0x2a
#define BCACC316G_ACT_TAP_STATUS 0x2b
#define BCACC316G_BW_RATE 0x2c
#define BCACC316G_POWER_CTL 0x2d
#define BCACC316G_INT_ENABLE 0x2e
#define BCACC316G_INT_MAP 0x2f
#define BCACC316G_INT_SOURCE 0x30
#define BCACC316G_DATA_FORMAT 0x31
#define BCACC316G_DATAX0 0x32
#define BCACC316G_DATAX1 0x33
#define BCACC316G_DATAY0 0x34
#define BCACC316G_DATAY1 0x35
#define BCACC316G_DATAZ0 0x36
#define BCACC316G_DATAZ1 0x37
#define BCACC316G_FIFO_CTL 0x38
#define BCACC316G_FIFO_STATUS 0x39

#define BCACC316G_BW_1600 0xF // 1111
#define BCACC316G_BW_800 0xE // 1110
#define BCACC316G_BW_400 0xD // 1101 
#define BCACC316G_BW_200 0xC // 1100
#define BCACC316G_BW_100 0xB // 1011 
#define BCACC316G_BW_50 0xA // 1010 
#define BCACC316G_BW_25 0x9 // 1001 
#define BCACC316G_BW_12 0x8 // 1000 
#define BCACC316G_BW_6 0x7 // 0111
#define BCACC316G_BW_3 0x6 // 0110


/* 
Interrupt PINs
INT1: 0
INT2: 1
*/
#define BCACC316G_INT1_PIN 0x00
#define BCACC316G_INT2_PIN 0x01

/* 
Interrupt bit position
*/
#define BCACC316G_INT_DATA_READY_BIT 0x07
#define BCACC316G_INT_SINGLE_TAP_BIT 0x06
#define BCACC316G_INT_DOUBLE_TAP_BIT 0x05
#define BCACC316G_INT_ACTIVITY_BIT 0x04
#define BCACC316G_INT_INACTIVITY_BIT 0x03
#define BCACC316G_INT_FREE_FALL_BIT 0x02
#define BCACC316G_INT_WATERMARK_BIT 0x01
#define BCACC316G_INT_OVERRUNY_BIT 0x00

#define BCACC316G_DATA_READY 0x07
#define BCACC316G_SINGLE_TAP 0x06
#define BCACC316G_DOUBLE_TAP 0x05
#define BCACC316G_ACTIVITY 0x04
#define BCACC316G_INACTIVITY 0x03
#define BCACC316G_FREE_FALL 0x02
#define BCACC316G_WATERMARK 0x01
#define BCACC316G_OVERRUNY 0x00




#define BCACC316G_OK 1 // no error
#define BCACC316G_ERROR 0 // indicates error is predent

#define BCACC316G_NO_ERROR 0 // initial state
#define BCACC316G_READ_ERROR 1 // problem reading accel
#define BCACC316G_BAD_ARG 2 // bad method argument


class BCACC316G
{
public:
bool status; // set when error occurs 
// see error code for details
byte error_code; // Initial state
double gains[3]; // counts to Gs

BCACC316G(uint8_t addr);
void powerOn();
void readAccel(int* xyx);
void readAccel(int* x, int* y, int* z);
void get_Gxyz(double *xyz);

void setTapThreshold(int tapThreshold);
int getTapThreshold();
void setAxisGains(double *_gains);
void getAxisGains(double *_gains);
void setAxisOffset(int x, int y, int z);
void getAxisOffset(int* x, int* y, int*z);
void setTapDuration(int tapDuration);
int getTapDuration();
void setDoubleTapLatency(int doubleTapLatency);
int getDoubleTapLatency();
void setDoubleTapWindow(int doubleTapWindow);
int getDoubleTapWindow();
void setActivityThreshold(int activityThreshold);
int getActivityThreshold();
void setInactivityThreshold(int inactivityThreshold);
int getInactivityThreshold();
void setTimeInactivity(int timeInactivity);
int getTimeInactivity();
void setFreeFallThreshold(int freeFallthreshold);
int getFreeFallThreshold();
void setFreeFallDuration(int freeFallDuration);
int getFreeFallDuration();

bool isActivityXEnabled();
bool isActivityYEnabled();
bool isActivityZEnabled();
bool isInactivityXEnabled();
bool isInactivityYEnabled();
bool isInactivityZEnabled();
bool isActivityAc();
bool isInactivityAc();
void setActivityAc(bool state);
void setInactivityAc(bool state);

bool getSuppressBit();
void setSuppressBit(bool state);
bool isTapDetectionOnX();
void setTapDetectionOnX(bool state);
bool isTapDetectionOnY();
void setTapDetectionOnY(bool state);
bool isTapDetectionOnZ();
void setTapDetectionOnZ(bool state);

void setActivityX(bool state);
void setActivityY(bool state);
void setActivityZ(bool state);
void setInactivityX(bool state);
void setInactivityY(bool state);
void setInactivityZ(bool state);

bool isActivitySourceOnX();
bool isActivitySourceOnY();
bool isActivitySourceOnZ();
bool isTapSourceOnX();
bool isTapSourceOnY();
bool isTapSourceOnZ();
bool isAsleep();

bool isLowPower();
void setLowPower(bool state);
double getRate();
void setRate(double rate);
void set_bw(byte bw_code);
byte get_bw_code(); 


bool triggered(byte interrupts, int mask);


byte getInterruptSource();
bool getInterruptSource(byte interruptBit);
bool getInterruptMapping(byte interruptBit);
void setInterruptMapping(byte interruptBit, bool interruptPin);
bool isInterruptEnabled(byte interruptBit);
void setInterrupt(byte interruptBit, bool state);

void getRangeSetting(byte* rangeSetting);
void setRangeSetting(int val);
bool getSelfTestBit();
void setSelfTestBit(bool selfTestBit);
bool getSpiBit();
void setSpiBit(bool spiBit);
bool getInterruptLevelBit();
void setInterruptLevelBit(bool interruptLevelBit);
bool getFullResBit();
void setFullResBit(bool fullResBit);
bool getJustifyBit();
void setJustifyBit(bool justifyBit);
void printAllRegister();

private:
  int _addr;                  // Address of sensor 
void writeTo(byte address, byte val);
void readFrom(byte address, int num, byte buff[]);
void setRegisterBit(byte regAdress, int bitPos, bool state);
bool getRegisterBit(byte regAdress, int bitPos); 
byte _buff[6] ; //6 bytes buffer for saving data read from the device
};
void print_byte(byte val);
#endif
