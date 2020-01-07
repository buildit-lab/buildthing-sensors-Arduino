/*
	BCHTS4085.cp
	Created By: dh.kim, 2019-01-24

  This library facilitates communication with, and configuration of,
  the BCACC316G Sensor(ADXL343_3axis accelerometer) for stm32f407vgt6 . .
*/

#include "BCACC316G.h"

#define BCACC316G_TO_READ (6) // num of bytes we are going to read each time (two bytes for each axis)



void BCACC316G_init(uint8_t addr, I2C_HandleTypeDef *hi2c)
{
	BCACC316G_addr = addr << 1;
	BCACC316G_hi2c = hi2c;
	BCACC316G_status = BCACC316G_OK;
	BCACC316G_error_code = BCACC316G_NO_ERROR;

	BCACC316G_gains[0] = 0.00376390;
	BCACC316G_gains[1] = 0.00376009;
	BCACC316G_gains[2] = 0.00349265;
}

void BCACC316G_powerOn()
{
	BCACC316G_writeTo(BCACC316G_POWER_CTL, 0);
	BCACC316G_writeTo(BCACC316G_POWER_CTL, 16);
	BCACC316G_writeTo(BCACC316G_POWER_CTL, 8);
}

// Reads the acceleration into three variable x, y and z
void BCACC316G_readAccel(int *xyz)
{
	BCACC316G_readAccel_seperate(xyz, xyz + 1, xyz + 2);
}
void BCACC316G_readAccel_seperate(int *x, int *y, int *z)
{
	BCACC316G_readFrom(BCACC316G_DATAX0, BCACC316G_TO_READ, BCACC316G_buff); //read the acceleration data from the BCACC316G

	// each axis reading comes in 10 bit resolution, ie 2 bytes. Least Significat Byte first!!
	// thus we are converting both bytes in to one int
	*x = (((int8_t)BCACC316G_buff[1]) << 8) | BCACC316G_buff[0];
	*y = (((int8_t)BCACC316G_buff[3]) << 8) | BCACC316G_buff[2];
	*z = (((int8_t)BCACC316G_buff[5]) << 8) | BCACC316G_buff[4];
}

void BCACC316G_get_Gxyz(double *xyz)
{
	int i;
	int xyz_int[3];
	BCACC316G_readAccel(xyz_int);
	for (i = 0; i < 3; i++) {
		xyz[i] = xyz_int[i] * BCACC316G_gains[i];
	}
}

// Writes val to address register on device
void BCACC316G_writeTo(uint8_t address, uint8_t val)
{
	HAL_I2C_Mem_Write(BCACC316G_hi2c, BCACC316G_addr, address, I2C_MEMADD_SIZE_8BIT, &val, 1, 10);
}

// Reads num bytes starting from address register on device in to _buff array
void BCACC316G_readFrom(uint8_t address, int num, uint8_t _buff[])
{
	HAL_I2C_Mem_Read(BCACC316G_hi2c, BCACC316G_addr, address, I2C_MEMADD_SIZE_8BIT, _buff, num, 10);
}

// Gets the range setting and return it into rangeSetting
// it can be 2, 4, 8 or 16
void BCACC316G_getRangeSetting(uint8_t* rangeSetting)
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_DATA_FORMAT, 1, &_b);
	*rangeSetting = _b & 3 ;
}

// Sets the range setting, possible values are: 2, 4, 8, 16
void BCACC316G_setRangeSetting(int val)
{
	uint8_t _s;
	uint8_t _b;

	switch (val) {
	case 2:
		_s = 0 ;
		break;

	case 4:
		_s = 1 ;
		break;

	case 8:
		_s = 2 ;
		break;

	case 16:
		_s = 3 ;
		break;

	default:
		_s = 0 ;
		break;
	}
	BCACC316G_readFrom(BCACC316G_DATA_FORMAT, 1, &_b);
	_s |= (_b & 236);
	BCACC316G_writeTo(BCACC316G_DATA_FORMAT, _s);
}

// gets the state of the SELF_TEST bit
uint8_t BCACC316G_getSelfTestBit()
{
	return BCACC316G_getRegisterBit(BCACC316G_DATA_FORMAT, 7);
}

// Sets the SELF-TEST bit
// if set to 1 it applies a self-test force to the sensor causing a shift in the output data
// if set to 0 it disables the self-test force
void BCACC316G_setSelfTestBit(uint8_t selfTestBit)
{
	BCACC316G_setRegisterBit(BCACC316G_DATA_FORMAT, 7, selfTestBit);
}

// Gets the state of the SPI bit
uint8_t BCACC316G_getSpiBit()
{
	return BCACC316G_getRegisterBit(BCACC316G_DATA_FORMAT, 6);
}

// Sets the SPI bit
// if set to 1 it sets the device to 3-wire mode
// if set to 0 it sets the device to 4-wire SPI mode
void BCACC316G_setSpiBit(uint8_t spiBit)
{
	BCACC316G_setRegisterBit(BCACC316G_DATA_FORMAT, 6, spiBit);
}

// Gets the state of the INT_INVERT bit
uint8_t BCACC316G_getInterruptLevelBit()
{
	return BCACC316G_getRegisterBit(BCACC316G_DATA_FORMAT, 5);
}

// Sets the INT_INVERT bit
// if set to 0 sets the interrupts to active high
// if set to 1 sets the interrupts to active low
void BCACC316G_setInterruptLevelBit(uint8_t interruptLevelBit)
{
	BCACC316G_setRegisterBit(BCACC316G_DATA_FORMAT, 5, interruptLevelBit);
}

// Gets the state of the FULL_RES bit
uint8_t BCACC316G_getFullResBit()
{
	return BCACC316G_getRegisterBit(BCACC316G_DATA_FORMAT, 3);
}

// Sets the FULL_RES bit
// if set to 1, the device is in full resolution mode, where the output resolution increases with the
// g range set by the range bits to maintain a 4mg/LSB scal factor
// if set to 0, the device is in 10-bit mode, and the range buts determine the maximum g range
// and scale factor
void BCACC316G_setFullResBit(uint8_t fullResBit)
{
	BCACC316G_setRegisterBit(BCACC316G_DATA_FORMAT, 3, fullResBit);
}

// Gets the state of the justify bit
uint8_t BCACC316G_getJustifyBit()
{
	return BCACC316G_getRegisterBit(BCACC316G_DATA_FORMAT, 2);
}

// Sets the JUSTIFY bit
// if sets to 1 selects the left justified mode
// if sets to 0 selects right justified mode with sign extension
void BCACC316G_setJustifyBit(uint8_t justifyBit)
{
	BCACC316G_setRegisterBit(BCACC316G_DATA_FORMAT, 2, justifyBit);
}

// Sets the THRESH_TAP byte value
// it should be between 0 and 255
// the scale factor is 62.5 mg/LSB
// A value of 0 may result in undesirable behavior
void BCACC316G_setTapThreshold(int tapThreshold)
{
	tapThreshold = BCACC316G_constrain(tapThreshold, 0, 255);
	uint8_t _b = (uint8_t)tapThreshold;
	BCACC316G_writeTo(BCACC316G_THRESH_TAP, _b);
}

// Gets the THRESH_TAP byte value
// return value is comprised between 0 and 255
// the scale factor is 62.5 mg/LSB
int BCACC316G_getTapThreshold()
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_THRESH_TAP, 1, &_b);
	return (int)_b;
}

// set/get the gain for each axis in Gs / count
void BCACC316G_setAxisGains(double *_gains)
{
	int i;
	for (i = 0; i < 3; i++) {
		BCACC316G_gains[i] = _gains[i];
	}
}
void BCACC316G_getAxisGains(double *_gains)
{
	int i;
	for (i = 0; i < 3; i++) {
		BCACC316G_gains[i] = _gains[i];
	}
}


// Sets the OFSX, OFSY and OFSZ bytes
// OFSX, OFSY and OFSZ are user offset adjustments in twos complement format with
// a scale factor of 15,6mg/LSB
// OFSX, OFSY and OFSZ should be comprised between 
void BCACC316G_setAxisOffset(int x, int y, int z)
{
	BCACC316G_writeTo(BCACC316G_OFSX, (uint8_t)x);
	BCACC316G_writeTo(BCACC316G_OFSY, (uint8_t)y);
	BCACC316G_writeTo(BCACC316G_OFSZ, (uint8_t)z);
}

// Gets the OFSX, OFSY and OFSZ bytes
void BCACC316G_getAxisOffset(int* x, int* y, int*z)
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_OFSX, 1, &_b);
	*x = (int)_b;
	BCACC316G_readFrom(BCACC316G_OFSY, 1, &_b);
	*y = (int)_b;
	BCACC316G_readFrom(BCACC316G_OFSZ, 1, &_b);
	*z = (int)_b;
}

// Sets the DUR byte
// The DUR byte contains an unsigned time value representing the maximum time
// that an event must be above THRESH_TAP threshold to qualify as a tap event
// The scale factor is 625µs/LSB
// A value of 0 disables the tap/double tap funcitons. Max value is 255.
void BCACC316G_setTapDuration(int tapDuration)
{
	tapDuration = BCACC316G_constrain(tapDuration, 0, 255);
	uint8_t _b = (uint8_t)tapDuration;
	BCACC316G_writeTo(BCACC316G_DUR, _b);
}

// Gets the DUR byte
int BCACC316G_getTapDuration()
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_DUR, 1, &_b);
	return (int)_b;
}

// Sets the latency (latent register) which contains an unsigned time value
// representing the wait time from the detection of a tap event to the start
// of the time window, during which a possible second tap can be detected.
// The scale factor is 1.25ms/LSB. A value of 0 disables the double tap function.
// It accepts a maximum value of 255.
void BCACC316G_setDoubleTapLatency(int doubleTapLatency)
{
	uint8_t _b = (uint8_t)doubleTapLatency;
	BCACC316G_writeTo(BCACC316G_LATENT, _b);
}

// Gets the Latent value
int BCACC316G_getDoubleTapLatency()
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_LATENT, 1, &_b);
	return (int)_b;
}

// Sets the Window register, which contains an unsigned time value representing
// the amount of time after the expiration of the latency time (Latent register)
// during which a second valud tap can begin. The scale factor is 1.25ms/LSB. A
// value of 0 disables the double tap function. The maximum value is 255.
void BCACC316G_setDoubleTapWindow(int doubleTapWindow)
{
	doubleTapWindow = BCACC316G_constrain(doubleTapWindow, 0, 255);
	uint8_t _b = (uint8_t)doubleTapWindow;
	BCACC316G_writeTo(BCACC316G_WINDOW, _b);
}

// Gets the Window register
int BCACC316G_getDoubleTapWindow()
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_WINDOW, 1, &_b);
	return (int)_b;
}

// Sets the THRESH_ACT byte which holds the threshold value for detecting activity.
// The data format is unsigned, so the magnitude of the activity event is compared 
// with the value is compared with the value in the THRESH_ACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the 
// activity interrupt is enabled. The maximum value is 255.
void BCACC316G_setActivityThreshold(int activityThreshold)
{
	activityThreshold = BCACC316G_constrain(activityThreshold, 0, 255);
	uint8_t _b = (uint8_t)activityThreshold;
	BCACC316G_writeTo(BCACC316G_THRESH_ACT, _b);
}

// Gets the THRESH_ACT byte
int BCACC316G_getActivityThreshold()
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_THRESH_ACT, 1, &_b);
	return (int)_b;
}

// Sets the THRESH_INACT byte which holds the threshold value for detecting inactivity.
// The data format is unsigned, so the magnitude of the inactivity event is compared 
// with the value is compared with the value in the THRESH_INACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the 
// inactivity interrupt is enabled. The maximum value is 255.
void BCACC316G_setInactivityThreshold(int inactivityThreshold)
{
	inactivityThreshold = BCACC316G_constrain(inactivityThreshold, 0, 255);
	uint8_t _b = (uint8_t)inactivityThreshold;
	BCACC316G_writeTo(BCACC316G_THRESH_INACT, _b);
}

// Gets the THRESH_INACT byte
int BCACC316G_getInactivityThreshold()
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_THRESH_INACT, 1, &_b);
	return (int)_b;
}

// Sets the TIME_INACT register, which contains an unsigned time value representing the
// amount of time that acceleration must be less thant the value in the THRESH_INACT
// register for inactivity to be declared. The scale factor is 1sec/LSB. The value must
// be between 0 and 255.
void BCACC316G_setTimeInactivity(int timeInactivity)
{
	timeInactivity = BCACC316G_constrain(timeInactivity, 0, 255);
	uint8_t _b = (uint8_t)timeInactivity;
	BCACC316G_writeTo(BCACC316G_TIME_INACT, _b);
}

// Gets the TIME_INACT register
int BCACC316G_getTimeInactivity()
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_TIME_INACT, 1, &_b);
	return (int)_b;
}

// Sets the THRESH_FF register which holds the threshold value, in an unsigned format, for
// free-fall detection. The root-sum-square (RSS) value of all axes is calculated and
// compared whith the value in THRESH_FF to determine if a free-fall event occured. The 
// scale factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the free-fall
// interrupt is enabled. The maximum value is 255.
void BCACC316G_setFreeFallThreshold(int freeFallThreshold)
{
	freeFallThreshold = BCACC316G_constrain(freeFallThreshold, 0, 255);
	uint8_t _b = (uint8_t)freeFallThreshold;
	BCACC316G_writeTo(BCACC316G_THRESH_FF, _b);
}

// Gets the THRESH_FF register.
int BCACC316G_getFreeFallThreshold()
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_THRESH_FF, 1, &_b);
	return (int)_b;
}

// Sets the TIME_FF register, which holds an unsigned time value representing the minimum
// time that the RSS value of all axes must be less than THRESH_FF to generate a free-fall 
// interrupt. The scale factor is 5ms/LSB. A value of 0 may result in undesirable behavior if
// the free-fall interrupt is enabled. The maximum value is 255.
void BCACC316G_setFreeFallDuration(int freeFallDuration)
{
	freeFallDuration = BCACC316G_constrain(freeFallDuration, 0, 255);
	uint8_t _b = (uint8_t)freeFallDuration;
	BCACC316G_writeTo(BCACC316G_TIME_FF, _b);
}

// Gets the TIME_FF register.
int BCACC316G_getFreeFallDuration()
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_TIME_FF, 1, &_b);
	return (int)_b;
}

uint8_t BCACC316G_isActivityXEnabled()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_INACT_CTL, 6);
}

uint8_t BCACC316G_isActivityYEnabled()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_INACT_CTL, 5);
}

uint8_t BCACC316G_isActivityZEnabled()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_INACT_CTL, 4);
}

uint8_t BCACC316G_isInactivityXEnabled()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_INACT_CTL, 2);
}

uint8_t BCACC316G_isInactivityYEnabled()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_INACT_CTL, 1);
}

uint8_t BCACC316G_isInactivityZEnabled()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_INACT_CTL, 0);
}

void BCACC316G_setActivityX(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_ACT_INACT_CTL, 6, state);
}

void BCACC316G_setActivityY(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_ACT_INACT_CTL, 5, state);
}

void BCACC316G_setActivityZ(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_ACT_INACT_CTL, 4, state);
}

void BCACC316G_setInactivityX(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_ACT_INACT_CTL, 2, state);
}

void BCACC316G_setInactivityY(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_ACT_INACT_CTL, 1, state);
}

void BCACC316G_setInactivityZ(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_ACT_INACT_CTL, 0, state);
}

uint8_t BCACC316G_isActivityAc()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_INACT_CTL, 7);
}

uint8_t BCACC316G_isInactivityAc()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_INACT_CTL, 3);
}

void BCACC316G_setActivityAc(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_ACT_INACT_CTL, 7, state);
}

void BCACC316G_setInactivityAc(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_ACT_INACT_CTL, 3, state);
}

uint8_t BCACC316G_getSuppressBit()
{
	return BCACC316G_getRegisterBit(BCACC316G_TAP_AXES, 3);
}

void BCACC316G_setSuppressBit(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_TAP_AXES, 3, state);
}

uint8_t BCACC316G_isTapDetectionOnX()
{
	return BCACC316G_getRegisterBit(BCACC316G_TAP_AXES, 2);
}

void BCACC316G_setTapDetectionOnX(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_TAP_AXES, 2, state);
}

uint8_t BCACC316G_isTapDetectionOnY()
{
	return BCACC316G_getRegisterBit(BCACC316G_TAP_AXES, 1);
}

void BCACC316G_setTapDetectionOnY(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_TAP_AXES, 1, state);
}

uint8_t BCACC316G_isTapDetectionOnZ()
{
	return BCACC316G_getRegisterBit(BCACC316G_TAP_AXES, 0);
}

void BCACC316G_setTapDetectionOnZ(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_TAP_AXES, 0, state);
}

uint8_t BCACC316G_isActivitySourceOnX()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_TAP_STATUS, 6);
}

uint8_t BCACC316G_isActivitySourceOnY()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_TAP_STATUS, 5);
}

uint8_t BCACC316G_isActivitySourceOnZ()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_TAP_STATUS, 4);
}

uint8_t BCACC316G_isTapSourceOnX()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_TAP_STATUS, 2);
}

uint8_t BCACC316G_isTapSourceOnY()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_TAP_STATUS, 1);
}

uint8_t BCACC316G_isTapSourceOnZ()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_TAP_STATUS, 0);
}

uint8_t BCACC316G_isAsleep()
{
	return BCACC316G_getRegisterBit(BCACC316G_ACT_TAP_STATUS, 3);
}

uint8_t BCACC316G_isLowPower()
{
	return BCACC316G_getRegisterBit(BCACC316G_BW_RATE, 4);
}

void BCACC316G_setLowPower(uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_BW_RATE, 4, state);
}

double BCACC316G_getRate()
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_BW_RATE, 1, &_b);
	_b &= 15;
	return (1 << (((int)_b) - 6)) * 6.25;
}

void BCACC316G_setRate(double rate)
{
	uint8_t _b, _s;
	int v = (int)(rate / 6.25);
	int r = 0;
	while (v >>= 1)
	{
		r++;
	}
	if (r <= 9)
	{
		BCACC316G_readFrom(BCACC316G_BW_RATE, 1, &_b);
		_s = (uint8_t)(r + 6) | (_b & 240);
		BCACC316G_writeTo(BCACC316G_BW_RATE, _s);
	}
}

void BCACC316G_set_bw(uint8_t bw_code)
{
	if ((bw_code < BCACC316G_BW_3) || (bw_code > BCACC316G_BW_1600))
	{
		BCACC316G_status = 0;
		BCACC316G_error_code = BCACC316G_BAD_ARG;
	}
	else
	{
		BCACC316G_writeTo(BCACC316G_BW_RATE, bw_code);
	}
}

uint8_t BCACC316G_get_bw_code()
{
	uint8_t bw_code;
	BCACC316G_readFrom(BCACC316G_BW_RATE, 1, &bw_code);
	return bw_code;
}


//Used to check if action was triggered in interrupts
//Example triggered(interrupts, BCACC316G_SINGLE_TAP);
uint8_t BCACC316G_triggered(uint8_t interrupts, int mask)
{
	return ((interrupts >> mask) & 1);
}


//sel : 0 just read int_source
//sel : 1 read int_source & interruptBit
uint8_t BCACC316G_getInterruptSource(uint8_t interruptBit, uint8_t sel)
{
	uint8_t _b;
	BCACC316G_readFrom(BCACC316G_INT_SOURCE, 1, &_b);
	return sel == 0 ? _b : ((_b >> interruptBit) & 1);
}

uint8_t BCACC316G_getInterruptMapping(uint8_t interruptBit)
{
	return BCACC316G_getRegisterBit(BCACC316G_INT_MAP, interruptBit);
}

// Set the mapping of an interrupt to pin1 or pin2
// eg: setInterruptMapping(BCACC316G_INT_DOUBLE_TAP_BIT,BCACC316G_INT2_PIN);
void BCACC316G_setInterruptMapping(uint8_t interruptBit, uint8_t interruptPin)
{
	BCACC316G_setRegisterBit(BCACC316G_INT_MAP, interruptBit, interruptPin);
}

uint8_t BCACC316G_isInterruptEnabled(uint8_t interruptBit)
{
	return BCACC316G_getRegisterBit(BCACC316G_INT_ENABLE, interruptBit);
}

void BCACC316G_setInterrupt(uint8_t interruptBit, uint8_t state)
{
	BCACC316G_setRegisterBit(BCACC316G_INT_ENABLE, interruptBit, state);
}

void BCACC316G_setRegisterBit(uint8_t regAdress, int bitPos, uint8_t state)
{
	uint8_t _b;
	BCACC316G_readFrom(regAdress, 1, &_b);
	if (state)
	{
		_b |= (1 << bitPos); // forces nth bit of _b to be 1. all other bits left alone.
	}
	else
	{
		_b &= ~(1 << bitPos); // forces nth bit of _b to be 0. all other bits left alone.
	}
	BCACC316G_writeTo(regAdress, _b);
}

uint8_t BCACC316G_getRegisterBit(uint8_t regAdress, int bitPos)
{
	uint8_t _b;
	BCACC316G_readFrom(regAdress, 1, &_b);
	return ((_b >> bitPos) & 1);
}

// print all register value to the serial ouptut, which requires it to be setup
// this can be used to manually to check the current configuration of the device
void BCACC316G_printAllRegister()
{
	uint8_t _b;
	printf("0x00: ");
	BCACC316G_readFrom(0x00, 1, &_b);
	BCACC316G_print_byte(_b);
	printf("\n");
	int i;
	for (i = 29; i <= 57; i++)
	{
		printf("0x");
		printf("%d : ", i);
		BCACC316G_readFrom(i, 1, &_b);
		BCACC316G_print_byte(_b);
		printf("\n");
	}
}

int BCACC316G_constrain(int value, int min, int max)
{
	if (value < min) return min;
	else if (value > max) return max;
	else return value;
}

void BCACC316G_print_byte(uint8_t val)
{
	int i;
	printf("B");
	for (i = 7; i >= 0; i--)
	{
		printf("%d", val >> i & 1);
	}
}

