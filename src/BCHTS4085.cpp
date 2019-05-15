/*
  BCHTS4085.cpp
  Created By: sy.baik, 2018-05-25
  
  This library facilitates communication with, and configuration of, 
  the BCHTS4085 Sensor(HDC2010_Temperature and Humidity Sensor) for Arduino. . 
*/


#include "BCHTS4085.h"
#include <Wire.h>
	
BCHTS4085::BCHTS4085(uint8_t addr)
{
  _addr = addr;
  
}

float BCHTS4085::readTemp(void)
{
	uint8_t byte[2];
	uint16_t temp;
	byte[0] = readReg(TEMP_LOW);
	byte[1] = readReg(TEMP_HIGH);
	
	temp = (unsigned int)byte[1] << 8 | byte[0];
	
	return (float)(temp) * 165 / 65536 - 40;
	
}

float BCHTS4085::readHumidity(void)
{
	uint8_t byte[2];
	uint16_t humidity;
	byte[0] = readReg(HUMID_LOW);
	byte[1] = readReg(HUMID_HIGH);
	
	humidity = (unsigned int)byte[1] << 8 | byte[0];
	
	return (float)(humidity)/( 65536 )* 100;
	
}

void BCHTS4085::enableHeater(void)
{
	uint8_t configContents;	//Stores current contents of config register
	
	configContents = readReg(BCHTS4085_CONFIG);
	
	//set bit 3 to 1 to enable heater
	configContents = (configContents | 0x08);
	
	writeReg(BCHTS4085_CONFIG, configContents);
	
}

void BCHTS4085::disableHeater(void)
{
	uint8_t configContents;	//Stores current contents of config register
	
	configContents = readReg(BCHTS4085_CONFIG);
	
	//set bit 3 to 0 to disable heater (all other bits 1)
	configContents = (configContents & 0xF7);
	writeReg(BCHTS4085_CONFIG, configContents);
	
}

void BCHTS4085::openReg(uint8_t reg)
{
  Wire.beginTransmission(_addr); 		// Connect to BCHTS4085
  Wire.write(reg); 						// point to specified register
  Wire.endTransmission(); 				// Relinquish bus control
}

uint8_t BCHTS4085::readReg(uint8_t reg)
{
	openReg(reg);
	uint8_t reading; 					// holds byte of read data
	Wire.requestFrom(_addr, 1); 		// Request 1 byte from open register
	Wire.endTransmission();				// Relinquish bus control
	
	if (1 <= Wire.available())
	{
		reading = (Wire.read());			// Read byte
	}
	
	return reading;
}

void BCHTS4085::writeReg(uint8_t reg, uint8_t data)
{
	
  Wire.beginTransmission(_addr);		// Open Device
  Wire.write(reg);						// Point to register
  Wire.write(data);						// Write data to register 
  Wire.endTransmission();				// Relinquish bus control
  
}

void BCHTS4085::setLowTemp(float temp)
{
	uint8_t temp_thresh_low;
	
	// Verify user is not trying to set value outside bounds
	if (temp < -40)
	{
		temp = -40;
	}
	else if (temp > 125)
	{
		temp = 125;
	}
	
	// Calculate value to load into register
	temp_thresh_low = (uint8_t)(256 * (temp + 40)/165);
	
	writeReg(TEMP_THR_L, temp_thresh_low);
	
}

void BCHTS4085::setHighTemp(float temp)
{ 
	uint8_t temp_thresh_high;
	
	// Verify user is not trying to set value outside bounds
	if (temp < -40)
	{
		temp = -40;
	}
	else if (temp > 125)
	{
		temp = 125;
	}
	
	// Calculate value to load into register
	temp_thresh_high = (uint8_t)(256 * (temp + 40)/165);
	
	writeReg(TEMP_THR_H, temp_thresh_high);
	
}

void BCHTS4085::setHighHumidity(float humid)
{
	uint8_t humid_thresh;
	
	// Verify user is not trying to set value outside bounds
	if (humid < 0)
	{
		humid = 0;
	}
	else if (humid > 100)
	{
		humid = 100;
	}
	
	// Calculate value to load into register
	humid_thresh = (uint8_t)(256 * (humid)/100);
	
	writeReg(HUMID_THR_H, humid_thresh);
	
}

void BCHTS4085::setLowHumidity(float humid)
{
	uint8_t humid_thresh;
	
	// Verify user is not trying to set value outside bounds
	if (humid < 0)
	{
		humid = 0;
	}
	else if (humid > 100)
	{
		humid = 100;
	}
	
	// Calculate value to load into register
	humid_thresh = (uint8_t)(256 * (humid)/100);
	
	writeReg(HUMID_THR_L, humid_thresh);
	
}

//  Return humidity from the low threshold register
float BCHTS4085::readLowHumidityThreshold(void)
{
	uint8_t regContents;
	
	regContents = readReg(HUMID_THR_L);
	
	return (float)regContents * 100/256;
	
}

//  Return humidity from the high threshold register
float BCHTS4085::readHighHumidityThreshold(void)
{
	uint8_t regContents;
	
	regContents = readReg(HUMID_THR_H);
	
	return (float)regContents * 100/256;
	
}

//  Return temperature from the low threshold register
float BCHTS4085::readLowTempThreshold(void)
{
	uint8_t regContents;
	
	regContents = readReg(TEMP_THR_L);
	
	return (float)regContents * 165/256 - 40;
	
}

//  Return temperature from the high threshold register
float BCHTS4085::readHighTempThreshold(void)
{
	uint8_t regContents;
	
	regContents = readReg(TEMP_THR_H);
	
	return (float)regContents * 165/256 - 40;
	
}


/* Upper two bits of the MEASUREMENT_CONFIG register controls
   the temperature resolution*/
void BCHTS4085::setTempRes(int resolution)
{ 
	uint8_t configContents;
	configContents = readReg(MEASUREMENT_CONFIG);
	
	switch(resolution)
	{
		case FOURTEEN_BIT:
			configContents = (configContents & 0x3F);
			break;
			
		case ELEVEN_BIT:
			configContents = (configContents & 0x7F);
			configContents = (configContents | 0x40);  
			break;
			
		case NINE_BIT:
			configContents = (configContents & 0xBF);
			configContents = (configContents | 0x80); 
			break;
			
		default:
			configContents = (configContents & 0x3F);
	}
	
	writeReg(MEASUREMENT_CONFIG, configContents);
	
}

/*  Bits 5 and 6 of the MEASUREMENT_CONFIG register controls
    the humidity resolution*/
void BCHTS4085::setHumidRes(int resolution)
{ 
	uint8_t configContents;
	configContents = readReg(MEASUREMENT_CONFIG);
	
	switch(resolution)
	{
		case FOURTEEN_BIT:
			configContents = (configContents & 0xCF);
			break;
			
		case ELEVEN_BIT:
			configContents = (configContents & 0xDF);
			configContents = (configContents | 0x10);  
			break;
			
		case NINE_BIT:
			configContents = (configContents & 0xEF);
			configContents = (configContents | 0x20); 
			break;
			
		default:
			configContents = (configContents & 0xCF);
	}
	
	writeReg(MEASUREMENT_CONFIG, configContents);	
}

/*  Bits 2 and 1 of the MEASUREMENT_CONFIG register controls
    the measurement mode  */
void BCHTS4085::setMeasurementMode(int mode)
{ 
	uint8_t configContents;
	configContents = readReg(MEASUREMENT_CONFIG);
	
	switch(mode)
	{
		case TEMP_AND_HUMID:
			configContents = (configContents & 0xF9);
			break;
			
		case TEMP_ONLY:
			configContents = (configContents & 0xFC);
			configContents = (configContents | 0x02);  
			break;
			
		case HUMID_ONLY:
			configContents = (configContents & 0xFD);
			configContents = (configContents | 0x04); 
			break;
			
		default:
			configContents = (configContents & 0xF9);
	}
	
	writeReg(MEASUREMENT_CONFIG, configContents);
}

uint8_t BCHTS4085::readMeasurementConfig(void)
{ 
  uint8_t configContents;
  configContents = readReg(MEASUREMENT_CONFIG);

  return (configContents);
}

/*  Bit 0 of the MEASUREMENT_CONFIG register can be used
    to trigger measurements  */
void BCHTS4085::triggerMeasurement(void)
{ 
	uint8_t configContents;
	configContents = readReg(MEASUREMENT_CONFIG);

	configContents = (configContents | 0x01);
	writeReg(MEASUREMENT_CONFIG, configContents);
}

/*  Bit 7 of the CONFIG register can be used to trigger a 
    soft reset  */
void BCHTS4085::reset(void)
{
	uint8_t configContents;
	configContents = readReg(BCHTS4085_CONFIG);

	configContents = (configContents | 0x80);
	writeReg(BCHTS4085_CONFIG, configContents);
	delay(50);
}

/*  Bit 2 of the CONFIG register can be used to enable/disable 
    the interrupt pin  */
void BCHTS4085::enableInterrupt(void)
{
	uint8_t configContents;
	configContents = readReg(BCHTS4085_CONFIG);

	configContents = (configContents | 0x04);
	writeReg(BCHTS4085_CONFIG, configContents);
}

/*  Bit 2 of the CONFIG register can be used to enable/disable 
    the interrupt pin  */
void BCHTS4085::disableInterrupt(void)
{
	uint8_t configContents;
	configContents = readReg(BCHTS4085_CONFIG);

	configContents = (configContents & 0xFB);
	writeReg(BCHTS4085_CONFIG, configContents);
}


/*  Bits 6-4  of the CONFIG register controls the measurement 
    rate  */
void BCHTS4085::setRate(int rate)
{ 
	uint8_t configContents;
	configContents = readReg(BCHTS4085_CONFIG);
	
	switch(rate)
	{
		case MANUAL:
			configContents = (configContents & 0x8F);
			break;
			
		case TWO_MINS:
			configContents = (configContents & 0x9F);
			configContents = (configContents | 0x10);  
			break;
			
		case ONE_MINS:
			configContents = (configContents & 0xAF);
			configContents = (configContents | 0x20); 
			break;
		
		case TEN_SECONDS:
			configContents = (configContents & 0xBF);
			configContents = (configContents | 0x30); 
			break;
		
		case FIVE_SECONDS:
			configContents = (configContents & 0xCF);
			configContents = (configContents | 0x40); 
			break;
		
		case ONE_HZ:
			configContents = (configContents & 0xDF);
			configContents = (configContents | 0x50); 
			break;
		
		case TWO_HZ:
			configContents = (configContents & 0xEF);
			configContents = (configContents | 0x60); 
			break;
		
		case FIVE_HZ:
			configContents = (configContents | 0x70); 
			break;
			
		default:
			configContents = (configContents & 0x8F);
	}
	
	writeReg(BCHTS4085_CONFIG, configContents);
}

uint8_t BCHTS4085::readConfig(void)
{ 
  uint8_t configContents;
  configContents = readReg(BCHTS4085_CONFIG);

  return (configContents);
}
/*  Bit 1 of the CONFIG register can be used to control the  
    the interrupt pins polarity */
void BCHTS4085::setInterruptPolarity(int polarity)
{
	uint8_t configContents;
	configContents = readReg(BCHTS4085_CONFIG);
	
	switch(polarity)
	{
		case ACTIVE_LOW:
			configContents = (configContents & 0xFD);
			break;
			
		case ACTIVE_HIGH:
			configContents = (configContents | 0x02);  
			break;
			
		default:
			configContents = (configContents & 0xFD);
	}
	
	writeReg(BCHTS4085_CONFIG, configContents);	
}

/*  Bit 0 of the CONFIG register can be used to control the  
    the interrupt pin's mode */
void BCHTS4085::setInterruptMode(int mode)
{
	uint8_t configContents;
	configContents = readReg(BCHTS4085_CONFIG);
	
	switch(mode)
	{
		case LEVEL_MODE:
			configContents = (configContents & 0xFE);
			break;
			
		case COMPARATOR_MODE:
			configContents = (configContents | 0x01);  
			break;
			
		default:
			configContents = (configContents & 0xFE);
	}
	
	writeReg(BCHTS4085_CONFIG, configContents);	
}

uint8_t BCHTS4085::readInterruptStatus(void)
{
	uint8_t regContents;
	regContents = readReg(INTERRUPT_DRDY);
	return regContents;
	
}

//  Clears the maximum temperature register
void BCHTS4085::clearMaxTemp(void)
{ 
	writeReg(TEMP_MAX, 0x00);
}

//  Clears the maximum humidity register
void BCHTS4085::clearMaxHumidity(void)
{ 
	writeReg(HUMID_MAX, 0x00);
}

//  Reads the maximum temperature register
float BCHTS4085::readMaxTemp(void)
{
	uint8_t regContents;
	
	regContents = readReg(TEMP_MAX);
	
	return (float)regContents * 165/256 - 40;
	
}

//  Reads the maximum humidity register
float BCHTS4085::readMaxHumidity(void)
{
	uint8_t regContents;
	
	regContents = readReg(HUMID_MAX);
	
	return (float)regContents /256 * 100;
	
}


// Enables the interrupt pin for comfort zone operation
void BCHTS4085::enableThresholdInterrupt(void)
{
	
	uint8_t regContents;
	regContents = readReg(INTERRUPT_CONFIG);

	regContents = (regContents | 0x78);

	writeReg(INTERRUPT_CONFIG, regContents);	
}

// Disables the interrupt pin for comfort zone operation
void BCHTS4085::disableThresholdInterrupt(void)
{
	uint8_t regContents;
	regContents = readReg(INTERRUPT_CONFIG);

	regContents = (regContents & 0x87);

	writeReg(INTERRUPT_CONFIG, regContents);	
}

// enables the interrupt pin for DRDY operation
void BCHTS4085::enableDRDYInterrupt(void)
{
	uint8_t regContents;
	regContents = readReg(INTERRUPT_CONFIG);

	regContents = (regContents | 0x80);

	writeReg(INTERRUPT_CONFIG, regContents);	
}

// disables the interrupt pin for DRDY operation
void BCHTS4085::disableDRDYInterrupt(void)
{
	uint8_t regContents;
	regContents = readReg(INTERRUPT_CONFIG);

	regContents = (regContents & 0x7F);

	writeReg(INTERRUPT_CONFIG, regContents);	
}
