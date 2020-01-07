/*
	BCHTS4085.h
	Created By: dh.kim, 2019-05-16

  This library facilitates communication with, and configuration of,
  the BCACC316G Sensor(ADXL343_3axis accelerometer) for stm32f407vgt6.
*/

#ifndef BCACC316G_h
#define BCACC316G_h

#include "main.h"

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


static uint8_t BCACC316G_addr;
static I2C_HandleTypeDef *BCACC316G_hi2c;

static uint8_t BCACC316G_buff[6]; //6 bytes buffer for saving data read from the device
static uint8_t BCACC316G_status; // set when error occurs 

// see error code for details
static uint8_t BCACC316G_error_code; // Initial state
static double BCACC316G_gains[3]; // counts to Gs

void BCACC316G_init(uint8_t addr, I2C_HandleTypeDef *hi2c);

void BCACC316G_powerOn(void);
void BCACC316G_readAccel(int* xyx);
void BCACC316G_readAccel_seperate(int* x, int* y, int* z);
void BCACC316G_get_Gxyz(double *xyz);

void BCACC316G_setTapThreshold(int tapThreshold);
int BCACC316G_getTapThreshold(void);
void BCACC316G_setAxisGains(double *_gains);
void BCACC316G_getAxisGains(double *_gains);
void BCACC316G_setAxisOffset(int x, int y, int z);
void BCACC316G_getAxisOffset(int* x, int* y, int*z);
void BCACC316G_setTapDuration(int tapDuration);
int BCACC316G_getTapDuration(void);
void BCACC316G_setDoubleTapLatency(int doubleTapLatency);
int BCACC316G_getDoubleTapLatency(void);
void BCACC316G_setDoubleTapWindow(int doubleTapWindow);
int BCACC316G_getDoubleTapWindow(void);
void BCACC316G_setActivityThreshold(int activityThreshold);
int BCACC316G_getActivityThreshold(void);
void BCACC316G_setInactivityThreshold(int inactivityThreshold);
int BCACC316G_getInactivityThreshold(void);
void BCACC316G_setTimeInactivity(int timeInactivity);
int BCACC316G_getTimeInactivity(void);
void BCACC316G_setFreeFallThreshold(int freeFallthreshold);
int BCACC316G_getFreeFallThreshold(void);
void BCACC316G_setFreeFallDuration(int freeFallDuration);
int BCACC316G_getFreeFallDuration(void);

uint8_t BCACC316G_isActivityXEnabled(void);
uint8_t BCACC316G_isActivityYEnabled(void);
uint8_t BCACC316G_isActivityZEnabled(void);
uint8_t BCACC316G_isInactivityXEnabled(void);
uint8_t BCACC316G_isInactivityYEnabled(void);
uint8_t BCACC316G_isInactivityZEnabled(void);
uint8_t BCACC316G_isActivityAc(void);
uint8_t BCACC316G_isInactivityAc(void);
void BCACC316G_setActivityAc(uint8_t state);
void BCACC316G_setInactivityAc(uint8_t state);

uint8_t BCACC316G_getSuppressBit(void);
void BCACC316G_setSuppressBit(uint8_t state);
uint8_t BCACC316G_isTapDetectionOnX(void);
void BCACC316G_setTapDetectionOnX(uint8_t state);
uint8_t BCACC316G_isTapDetectionOnY(void);
void BCACC316G_setTapDetectionOnY(uint8_t state);
uint8_t BCACC316G_isTapDetectionOnZ(void);
void BCACC316G_setTapDetectionOnZ(uint8_t state);

void BCACC316G_setActivityX(uint8_t state);
void BCACC316G_setActivityY(uint8_t state);
void BCACC316G_setActivityZ(uint8_t state);
void BCACC316G_setInactivityX(uint8_t state);
void BCACC316G_setInactivityY(uint8_t state);
void BCACC316G_setInactivityZ(uint8_t state);

uint8_t BCACC316G_isActivitySourceOnX(void);
uint8_t BCACC316G_isActivitySourceOnY(void);
uint8_t BCACC316G_isActivitySourceOnZ(void);
uint8_t BCACC316G_isTapSourceOnX(void);
uint8_t BCACC316G_isTapSourceOnY(void);
uint8_t BCACC316G_isTapSourceOnZ(void);
uint8_t BCACC316G_isAsleep(void);

uint8_t BCACC316G_isLowPower(void);
void BCACC316G_setLowPower(uint8_t state);
double BCACC316G_getRate(void);
void BCACC316G_setRate(double rate);
void BCACC316G_set_bw(uint8_t bw_code);
uint8_t BCACC316G_get_bw_code(void);


uint8_t BCACC316G_triggered(uint8_t interrupts, int mask);


uint8_t BCACC316G_getInterruptSource(uint8_t interruptBit, uint8_t sel);
uint8_t BCACC316G_getInterruptMapping(uint8_t interruptBit);
void BCACC316G_setInterruptMapping(uint8_t interruptBit, uint8_t interruptPin);
uint8_t BCACC316G_isInterruptEnabled(uint8_t interruptBit);
void BCACC316G_setInterrupt(uint8_t interruptBit, uint8_t state);

void BCACC316G_getRangeSetting(uint8_t* rangeSetting);
void BCACC316G_setRangeSetting(int val);
uint8_t BCACC316G_getSelfTestBit(void);
void BCACC316G_setSelfTestBit(uint8_t selfTestBit);
uint8_t BCACC316G_getSpiBit(void);
void BCACC316G_setSpiBit(uint8_t spiBit);
uint8_t BCACC316G_getInterruptLevelBit(void);
void BCACC316G_setInterruptLevelBit(uint8_t interruptLevelBit);
uint8_t BCACC316G_getFullResBit(void);
void BCACC316G_setFullResBit(uint8_t fullResBit);
uint8_t BCACC316G_getJustifyBit(void);
void BCACC316G_setJustifyBit(uint8_t justifyBit);
void BCACC316G_printAllRegister(void);

void BCACC316G_writeTo(uint8_t address, uint8_t val);
void BCACC316G_readFrom(uint8_t address, int num, uint8_t buff[]);
void BCACC316G_setRegisterBit(uint8_t regAdress, int bitPos, uint8_t state);
uint8_t BCACC316G_getRegisterBit(uint8_t regAdress, int bitPos);

void BCACC316G_print_byte(uint8_t val);
int BCACC316G_constrain(int value, int min, int max);
#endif
