/*
  BCHTS4085.h
  Created By: sy.baik, 2018-05-25
  
  This library facilitates communication with, and configuration of, 
  the BCHTS4085 Sensor(HDC2010_Temperature and Humidity Sensor) for Arduino.  
*/

#ifndef BCHTS4085_H
#define BCHTS4085_h

#include <Arduino.h>
#include <Wire.h>

//Define Register Map
  #define TEMP_LOW 0x00
  #define TEMP_HIGH 0x01
  #define HUMID_LOW 0x02
  #define HUMID_HIGH 0x03
  #define INTERRUPT_DRDY 0x04
  #define TEMP_MAX 0x05
  #define HUMID_MAX 0x06
  #define INTERRUPT_CONFIG 0x07
  #define TEMP_OFFSET_ADJUST 0x08
  #define HUM_OFFSET_ADJUST 0x09
  #define TEMP_THR_L 0x0A
  #define TEMP_THR_H 0x0B
  #define HUMID_THR_L 0x0C
  #define HUMID_THR_H 0x0D
  #define BCHTS4085_CONFIG 0x0E
  #define MEASUREMENT_CONFIG 0x0F
  #define MID_L 0xFC
  #define MID_H 0xFD
  #define DEVICE_ID_L 0xFE
  #define DEVICE_ID_H 0xFF


//  Constants for setting measurement resolution
#define FOURTEEN_BIT 0
#define ELEVEN_BIT 1
#define NINE_BIT  2

//  Constants for setting sensor mode
#define TEMP_AND_HUMID 0
#define TEMP_ONLY	   1
#define HUMID_ONLY	   2
#define ACTIVE_LOW	   0
#define ACTIVE_HIGH	   1
#define LEVEL_MODE		0
#define COMPARATOR_MODE 1

//  Constants for setting sample rate
#define MANUAL			0
#define TWO_MINS		1
#define ONE_MINS		2
#define TEN_SECONDS		3 
#define	FIVE_SECONDS	4
#define ONE_HZ			5
#define TWO_HZ			6
#define FIVE_HZ			7	



  
class BCHTS4085
{
	public:
		BCHTS4085(uint8_t addr);					// Initialize the BCHTS4085
		float readTemp(void);					// Returns the temperature in degrees C
		float readHumidity(void);				// Returns the relative humidity
		void enableHeater(void);				// Enables the heating element
		void disableHeater(void);				// Disables the heating element
		void setLowTemp(float temp);			// Sets low threshold temperature (in c)
		void setHighTemp(float temp);			// Sets high threshold temperature (in c)
		void setHighHumidity(float humid);		// Sets high Humiditiy threshold
		void setLowHumidity(float humid);		// Sets low Humidity threshold 
		float readLowHumidityThreshold(void);	// Returns contents of low humidity threshold register
		float readHighHumidityThreshold(void);	// Returns contents of high humidity threshold register
		float readLowTempThreshold(void);		// Returns contents of low temperature threshold register (in C)
		float readHighTempThreshold(void);		// Returns contents of high temperature threshold register (in C)
		void triggerMeasurement(void);			// Triggers a manual temperature/humidity reading
		void reset(void); 						// Triggers a software reset
		void enableInterrupt(void);				// Enables the interrupt/DRDY pin
		void disableInterrupt(void); 			// Disables the interrupt/DRDY pin (High Z)
		uint8_t readInterruptStatus(void); 		// Reads the status of the interrupt register
		void clearMaxTemp(void);				// Clears the Maximum temperature register
		void clearMaxHumidity(void);			// Clears the Maximum humidity register
		float readMaxTemp(void); 				// Reads the maximum temperature register
		float readMaxHumidity(void);			// Reads the maximum humidity register
		void enableThresholdInterrupt(void);	// Enables high and low temperature/humidity interrupts
		void disableThresholdInterrupt(void);	// Disables high and low temperature/humidity interrupts
		void enableDRDYInterrupt(void);			// Enables data ready interrupt
		void disableDRDYInterrupt(void);		// Disables data ready interrupt
		
		
		
		/* Sets Temperature & Humidity Resolution, 3 options
		   0 - 14 bit
		   1 - 11 bit
		   2 - 9 bit
		   default - 14 bit							*/
		void setTempRes(int resolution);		    
		void setHumidRes(int resolution);	    
     
		/* Sets measurement mode, 3 options
		   0 - Temperature and Humidity
		   1 - Temperature only
		   2 - Humidity only
		   default - Temperature & Humidity			*/
		void setMeasurementMode(int mode);

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
    uint8_t readMeasurementConfig(void);
		
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
		void setRate(int rate);

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
    uint8_t readConfig(void);
		
		/* Sets Interrupt polarity, 2 options
		   0 - Active Low
		   1 - Active High
		   default - Active Low			*/		
		void setInterruptPolarity(int polarity);
    
		
		/* Sets Interrupt mode, 2 options
		   0 - Level sensitive
		   1 - Comparator mode
		   default - Level sensitive	*/		
		void setInterruptMode(int polarity);
    
		
	private:
		int _addr; 									// Address of sensor 
		void openReg(uint8_t reg); 	    			// Points to a given register
		uint8_t readReg(uint8_t reg);				// Reads a given register, returns 1 byte
		void writeReg(uint8_t reg, uint8_t data); 	// Writes a byte of data to one register
		

};


#endif
