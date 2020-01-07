/*
	BCHTS4085.cp
	Created By: dh.kim, 2019-01-24

  This library facilitates communication with, and configuration of,
  the BCHTS4085 Sensor(HDC2010_Temperature and Humidity Sensor) for stm32f407vgt6 . .
*/


#ifndef BCHTS4085_H
#define BCHTS4085_h

#include "main.h"

//Define Register Map
#define BCHTS4085_TEMP_LOW 0x00
#define BCHTS4085_TEMP_HIGH 0x01
#define BCHTS4085_HUMID_LOW 0x02
#define BCHTS4085_HUMID_HIGH 0x03
#define BCHTS4085_INTERRUPT_DRDY 0x04
#define BCHTS4085_TEMP_MAX 0x05
#define BCHTS4085_HUMID_MAX 0x06
#define BCHTS4085_INTERRUPT_CONFIG 0x07
#define BCHTS4085_TEMP_OFFSET_ADJUST 0x08
#define BCHTS4085_HUM_OFFSET_ADJUST 0x09
#define BCHTS4085_TEMP_THR_L 0x0A
#define BCHTS4085_TEMP_THR_H 0x0B
#define BCHTS4085_HUMID_THR_L 0x0C
#define BCHTS4085_HUMID_THR_H 0x0D
#define BCHTS4085_CONFIG 0x0E
#define BCHTS4085_MEASUREMENT_CONFIG 0x0F
#define BCHTS4085_MID_L 0xFC
#define BCHTS4085_MID_H 0xFD
#define BCHTS4085_DEVICE_ID_L 0xFE
#define BCHTS4085_DEVICE_ID_H 0xFF


//  Constants for setting measurement resolution
#define BCHTS4085_FOURTEEN_BIT 0
#define BCHTS4085_ELEVEN_BIT 1
#define BCHTS4085_NINE_BIT  2

//  Constants for setting sensor mode
#define BCHTS4085_TEMP_AND_HUMID 0
#define BCHTS4085_TEMP_ONLY	   1
#define BCHTS4085_HUMID_ONLY	   2
#define BCHTS4085_ACTIVE_LOW	   0
#define BCHTS4085_ACTIVE_HIGH	   1
#define BCHTS4085_LEVEL_MODE		0
#define BCHTS4085_COMPARATOR_MODE 1

//  Constants for setting sample rate
#define BCHTS4085_MANUAL			0
#define BCHTS4085_TWO_MINS		1
#define BCHTS4085_ONE_MINS		2
#define BCHTS4085_TEN_SECONDS		3 
#define	BCHTS4085_FIVE_SECONDS	4
#define BCHTS4085_ONE_HZ			5
#define BCHTS4085_TWO_HZ			6
#define BCHTS4085_FIVE_HZ			7	

static uint8_t BCHTS4085_addr;
static I2C_HandleTypeDef *BCHTS4085_hi2c;


void BCHTS4085_init(uint8_t _addr, I2C_HandleTypeDef *hi2c);//input i2c device address, used handler address for initializing

float BCHTS4085_readTemp(void);					// Returns the temperature in degrees C
float BCHTS4085_readHumidity(void);				// Returns the relative humidity
void BCHTS4085_enableHeater(void);				// Enables the heating element
void BCHTS4085_disableHeater(void);				// Disables the heating element
void BCHTS4085_setLowTemp(float temp);			// Sets low threshold temperature (in c)
void BCHTS4085_setHighTemp(float temp);			// Sets high threshold temperature (in c)
void BCHTS4085_setHighHumidity(float humid);		// Sets high Humiditiy threshold
void BCHTS4085_setLowHumidity(float humid);		// Sets low Humidity threshold 
float BCHTS4085_readLowHumidityThreshold(void);	// Returns contents of low humidity threshold register
float BCHTS4085_readHighHumidityThreshold(void);	// Returns contents of high humidity threshold register
float BCHTS4085_readLowTempThreshold(void);		// Returns contents of low temperature threshold register (in C)
float BCHTS4085_readHighTempThreshold(void);		// Returns contents of high temperature threshold register (in C)
void BCHTS4085_triggerMeasurement(void);			// Triggers a manual temperature/humidity reading
void BCHTS4085_reset(void); 						// Triggers a software reset
void BCHTS4085_enableInterrupt(void);				// Enables the interrupt/DRDY pin
void BCHTS4085_disableInterrupt(void); 			// Disables the interrupt/DRDY pin (High Z)
uint8_t BCHTS4085_readInterruptStatus(void); 		// Reads the status of the interrupt register
void BCHTS4085_clearMaxTemp(void);				// Clears the Maximum temperature register
void BCHTS4085_clearMaxHumidity(void);			// Clears the Maximum humidity register
float BCHTS4085_readMaxTemp(void); 				// Reads the maximum temperature register
float BCHTS4085_readMaxHumidity(void);			// Reads the maximum humidity register
void BCHTS4085_enableThresholdInterrupt(void);	// Enables high and low temperature/humidity interrupts
void BCHTS4085_disableThresholdInterrupt(void);	// Disables high and low temperature/humidity interrupts
void BCHTS4085_enableDRDYInterrupt(void);			// Enables data ready interrupt
void BCHTS4085_disableDRDYInterrupt(void);		// Disables data ready interrupt



/* Sets Temperature & Humidity Resolution, 3 options
		   0 - 14 bit
		   1 - 11 bit
		   2 - 9 bit
		   default - 14 bit							*/
void BCHTS4085_setTempRes(int resolution);
void BCHTS4085_setHumidRes(int resolution);

/* Sets measurement mode, 3 options
   0 - Temperature and Humidity
   1 - Temperature only
   2 - Humidity only
   default - Temperature & Humidity			*/
void BCHTS4085_setMeasurementMode(int mode);

/* Read measurement config
   7:6 - Temperature Resolution
		 00: 14bit
		 01: 11bit
		 10: 9bit
   5:4 - Humidity resolution
		 00: 14bit
		 01: 11bit
		 10: 9bit
   3 - Reserved
   2:1 - Measurement configuration
		 00: Humidity + Temperature
		 01: Temperature only
		 10: Humidity Only
		 11: NA
   0 - Measurement trigger
	   0: no action
	   1: Start measurement Self-clearing bit when measurement completed     */
uint8_t BCHTS4085_readMeasurementConfig(void);

/* Sets reading rate, 8 options
   0 - Manual
   1 - reading every 2 minutes
   2 - reading every minute
   3 - reading every ten seconds
   4 - reading every 5 seconds
   5 - reading every second
   6 - reading at 2Hz
   7 - reading at 5Hz
   default - Manual		*/
void BCHTS4085_setRate(int rate);

/* Read measurement config
   7 - Soft Reset
	   0: Normal Operation mode, this bit is self-clear
	   1: Soft Reset
   6:4 - Output Data Rate
		 000 = No repeated measurements. Trigger on demand
		 001 = 1/120Hz (1 samples every 2 minutes)
		 010 = 1/60Hz (1 samples every minute)
		 011 = 0.1Hz (1 samples every 10 seconds)
		 100 = 0.2 Hz (1 samples every 5 second)
		 101 = 1Hz (1 samples every second)
		 110 = 2Hz (2 samples every second)
		 111 = 5Hz (5 samples every second)
   3 - Heater Enable
	   0 = Heater off
	   1 = Heater on
   2 - DRDY/INT_EN pin configuration
	   0 = High Z
	   1 = Enable
   1 - Interrupt polarity
	   0 = Active Low
	   1 = Active High
   0 - Interrupt mode
	   0 = Level sensitive
	   1 = Comparator mode    */
uint8_t BCHTS4085_readConfig(void);

/* Sets Interrupt polarity, 2 options
   0 - Active Low
   1 - Active High
   default - Active Low			*/
void BCHTS4085_setInterruptPolarity(int polarity);


/* Sets Interrupt mode, 2 options
   0 - Level sensitive
   1 - Comparator mode
   default - Level sensitive	*/
void BCHTS4085_setInterruptMode(int polarity);

uint8_t BCHTS4085_readReg(uint8_t reg);				// Reads a given register, returns 1 byte
void BCHTS4085_writeReg(uint8_t reg, uint8_t data); 	// Writes a byte of data to one register

#endif
