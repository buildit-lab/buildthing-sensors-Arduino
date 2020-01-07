/*
	BCHTS4085.cp
	Created By: dh.kim, 2019-01-24
  
  This library facilitates communication with, and configuration of, 
  the BCHTS4085 Sensor(HDC2010_Temperature and Humidity Sensor) for stm32f407vgt6 . . 
*/


#include "BCHTS4085.h"


//_addr : i2c address your BCHTS4085 device
//hi2c : reference address your i2c handler
void BCHTS4085_init(uint8_t _addr, I2C_HandleTypeDef *hi2c)
{
	BCHTS4085_addr = _addr << 1;
	BCHTS4085_hi2c = hi2c;
}


float BCHTS4085_readTemp()
{
	uint8_t byte[2];
	uint16_t temp;
	byte[0] = BCHTS4085_readReg(BCHTS4085_TEMP_LOW);
	byte[1] = BCHTS4085_readReg(BCHTS4085_TEMP_HIGH);
	
	temp = (unsigned int)byte[1] << 8 | byte[0];
	
	return (float)(temp) * 165 / 65536 - 40;
	
}

float BCHTS4085_readHumidity()
{
	uint8_t byte[2];
	uint16_t humidity;
	byte[0] = BCHTS4085_readReg(BCHTS4085_HUMID_LOW);
	byte[1] = BCHTS4085_readReg(BCHTS4085_HUMID_HIGH);
	
	humidity = (unsigned int)byte[1] << 8 | byte[0];
	
	return (float)(humidity)/( 65536 )* 100;
	
}

void BCHTS4085_enableHeater(void)
{
	uint8_t configContents;	//Stores current contents of config register
	
	configContents = BCHTS4085_readReg(BCHTS4085_CONFIG);
	
	//set bit 3 to 1 to enable heater
	configContents = (configContents | 0x08);
	
	BCHTS4085_writeReg(BCHTS4085_CONFIG, configContents);
	
}

void BCHTS4085_disableHeater(void)
{
	uint8_t configContents;	//Stores current contents of config register
	
	configContents = BCHTS4085_readReg(BCHTS4085_CONFIG);
	
	//set bit 3 to 0 to disable heater (all other bits 1)
	configContents = (configContents & 0xF7);
	BCHTS4085_writeReg(BCHTS4085_CONFIG, configContents);
	
}

uint8_t BCHTS4085_readReg(uint8_t reg)
{
	uint8_t reading; 					// holds byte of read data
	HAL_I2C_Mem_Read(BCHTS4085_hi2c, BCHTS4085_addr, reg, I2C_MEMADD_SIZE_8BIT, &reading, 1, 10);
	
	return reading;
}

void BCHTS4085_writeReg(uint8_t reg, uint8_t data)
{
	HAL_I2C_Mem_Write(BCHTS4085_hi2c, BCHTS4085_addr, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

void BCHTS4085_setLowTemp(float temp)
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
	
	BCHTS4085_writeReg(BCHTS4085_TEMP_THR_L, temp_thresh_low);
	
}

void BCHTS4085_setHighTemp(float temp)
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
	
	BCHTS4085_writeReg(BCHTS4085_TEMP_THR_H, temp_thresh_high);
	
}

void BCHTS4085_setHighHumidity(float humid)
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
	
	BCHTS4085_writeReg(BCHTS4085_HUMID_THR_H, humid_thresh);
	
}

void BCHTS4085_setLowHumidity(float humid)
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
	
	BCHTS4085_writeReg(BCHTS4085_HUMID_THR_L, humid_thresh);
	
}

//  Return humidity from the low threshold register
float BCHTS4085_readLowHumidityThreshold(void)
{
	uint8_t regContents;
	
	regContents = BCHTS4085_readReg(BCHTS4085_HUMID_THR_L);
	
	return (float)regContents * 100/256;
	
}

//  Return humidity from the high threshold register
float BCHTS4085_readHighHumidityThreshold(void)
{
	uint8_t regContents;
	
	regContents = BCHTS4085_readReg(BCHTS4085_HUMID_THR_H);
	
	return (float)regContents * 100/256;
	
}

//  Return temperature from the low threshold register
float BCHTS4085_readLowTempThreshold(void)
{
	uint8_t regContents;
	
	regContents = BCHTS4085_readReg(BCHTS4085_TEMP_THR_L);
	
	return (float)regContents * 165/256 - 40;
	
}

//  Return temperature from the high threshold register
float BCHTS4085_readHighTempThreshold(void)
{
	uint8_t regContents;
	
	regContents = BCHTS4085_readReg(BCHTS4085_TEMP_THR_H);
	
	return (float)regContents * 165/256 - 40;
	
}


/* Upper two bits of the MEASUREMENT_CONFIG register controls
   the temperature resolution*/
void BCHTS4085_setTempRes(int resolution)
{ 
	uint8_t configContents;
	configContents = BCHTS4085_readReg(BCHTS4085_MEASUREMENT_CONFIG);
	
	switch(resolution)
	{
		case BCHTS4085_FOURTEEN_BIT:
			configContents = (configContents & 0x3F);
			break;
			
		case BCHTS4085_ELEVEN_BIT:
			configContents = (configContents & 0x7F);
			configContents = (configContents | 0x40);  
			break;
			
		case BCHTS4085_NINE_BIT:
			configContents = (configContents & 0xBF);
			configContents = (configContents | 0x80); 
			break;
			
		default:
			configContents = (configContents & 0x3F);
	}
	
	BCHTS4085_writeReg(BCHTS4085_MEASUREMENT_CONFIG, configContents);
	
}

/*  Bits 5 and 6 of the MEASUREMENT_CONFIG register controls
    the humidity resolution*/
void BCHTS4085_setHumidRes(int resolution)
{ 
	uint8_t configContents;
	configContents = BCHTS4085_readReg(BCHTS4085_MEASUREMENT_CONFIG);
	
	switch(resolution)
	{
		case BCHTS4085_FOURTEEN_BIT:
			configContents = (configContents & 0xCF);
			break;
			
		case BCHTS4085_ELEVEN_BIT:
			configContents = (configContents & 0xDF);
			configContents = (configContents | 0x10);  
			break;
			
		case BCHTS4085_NINE_BIT:
			configContents = (configContents & 0xEF);
			configContents = (configContents | 0x20); 
			break;
			
		default:
			configContents = (configContents & 0xCF);
	}
	
	BCHTS4085_writeReg(BCHTS4085_MEASUREMENT_CONFIG, configContents);
}

/*  Bits 2 and 1 of the MEASUREMENT_CONFIG register controls
    the measurement mode  */
void BCHTS4085_setMeasurementMode(int mode)
{ 
	uint8_t configContents;
	configContents = BCHTS4085_readReg(BCHTS4085_MEASUREMENT_CONFIG);
	
	switch(mode)
	{
		case BCHTS4085_TEMP_AND_HUMID:
			configContents = (configContents & 0xF9);
			break;
			
		case BCHTS4085_TEMP_ONLY:
			configContents = (configContents & 0xFC);
			configContents = (configContents | 0x02);  
			break;
			
		case BCHTS4085_HUMID_ONLY:
			configContents = (configContents & 0xFD);
			configContents = (configContents | 0x04); 
			break;
			
		default:
			configContents = (configContents & 0xF9);
	}
	
	BCHTS4085_writeReg(BCHTS4085_MEASUREMENT_CONFIG, configContents);
}

uint8_t BCHTS4085_readMeasurementConfig(void)
{ 
  uint8_t configContents;
  configContents = BCHTS4085_readReg(BCHTS4085_MEASUREMENT_CONFIG);

  return (configContents);
}

/*  Bit 0 of the MEASUREMENT_CONFIG register can be used
    to trigger measurements  */
void BCHTS4085_triggerMeasurement(void)
{ 
	uint8_t configContents;
	configContents = BCHTS4085_readReg(BCHTS4085_MEASUREMENT_CONFIG);

	configContents = (configContents | 0x01);
	BCHTS4085_writeReg(BCHTS4085_MEASUREMENT_CONFIG, configContents);
}

/*  Bit 7 of the CONFIG register can be used to trigger a 
    soft reset  */
void BCHTS4085_reset(void)
{
	uint8_t configContents;
	configContents = BCHTS4085_readReg(BCHTS4085_CONFIG);

	configContents = (configContents | 0x80);
	BCHTS4085_writeReg(BCHTS4085_CONFIG, configContents);
	HAL_Delay(50);
}

/*  Bit 2 of the CONFIG register can be used to enable/disable 
    the interrupt pin  */
void BCHTS4085_enableInterrupt()
{
	uint8_t configContents;
	configContents = BCHTS4085_readReg(BCHTS4085_CONFIG);

	configContents = (configContents | 0x04);
	BCHTS4085_writeReg(BCHTS4085_CONFIG, configContents);
}

/*  Bit 2 of the CONFIG register can be used to enable/disable 
    the interrupt pin  */
void BCHTS4085_disableInterrupt(void)
{
	uint8_t configContents;
	configContents = BCHTS4085_readReg(BCHTS4085_CONFIG);

	configContents = (configContents & 0xFB);
	BCHTS4085_writeReg(BCHTS4085_CONFIG, configContents);
}


/*  Bits 6-4  of the CONFIG register controls the measurement 
    rate  */
void BCHTS4085_setRate(int rate)
{ 
	uint8_t configContents;
	configContents = BCHTS4085_readReg(BCHTS4085_CONFIG);
	
	switch(rate)
	{
		case BCHTS4085_MANUAL:
			configContents = (configContents & 0x8F);
			break;
			
		case BCHTS4085_TWO_MINS:
			configContents = (configContents & 0x9F);
			configContents = (configContents | 0x10);  
			break;
			
		case BCHTS4085_ONE_MINS:
			configContents = (configContents & 0xAF);
			configContents = (configContents | 0x20); 
			break;
		
		case BCHTS4085_TEN_SECONDS:
			configContents = (configContents & 0xBF);
			configContents = (configContents | 0x30); 
			break;
		
		case BCHTS4085_FIVE_SECONDS:
			configContents = (configContents & 0xCF);
			configContents = (configContents | 0x40); 
			break;
		
		case BCHTS4085_ONE_HZ:
			configContents = (configContents & 0xDF);
			configContents = (configContents | 0x50); 
			break;
		
		case BCHTS4085_TWO_HZ:
			configContents = (configContents & 0xEF);
			configContents = (configContents | 0x60); 
			break;
		
		case BCHTS4085_FIVE_HZ:
			configContents = (configContents | 0x70); 
			break;
			
		default:
			configContents = (configContents & 0x8F);
	}
	
	BCHTS4085_writeReg(BCHTS4085_CONFIG, configContents);
}

uint8_t BCHTS4085_readConfig(void)
{ 
  uint8_t configContents;
  configContents = BCHTS4085_readReg(BCHTS4085_CONFIG);

  return (configContents);
}
/*  Bit 1 of the CONFIG register can be used to control the  
    the interrupt pins polarity */
void BCHTS4085_setInterruptPolarity(int polarity)
{
	uint8_t configContents;
	configContents = BCHTS4085_readReg(BCHTS4085_CONFIG);
	
	switch(polarity)
	{
		case BCHTS4085_ACTIVE_LOW:
			configContents = (configContents & 0xFD);
			break;
			
		case BCHTS4085_ACTIVE_HIGH:
			configContents = (configContents | 0x02);  
			break;
			
		default:
			configContents = (configContents & 0xFD);
	}
	
	BCHTS4085_writeReg(BCHTS4085_CONFIG, configContents);	
}

/*  Bit 0 of the CONFIG register can be used to control the  
    the interrupt pin's mode */
void BCHTS4085_setInterruptMode(int mode)
{
	uint8_t configContents;
	configContents = BCHTS4085_readReg(BCHTS4085_CONFIG);
	
	switch(mode)
	{
		case BCHTS4085_LEVEL_MODE:
			configContents = (configContents & 0xFE);
			break;
			
		case BCHTS4085_COMPARATOR_MODE:
			configContents = (configContents | 0x01);  
			break;
			
		default:
			configContents = (configContents & 0xFE);
	}
	
	BCHTS4085_writeReg(BCHTS4085_CONFIG, configContents);
}

uint8_t BCHTS4085_readInterruptStatus(void)
{
	uint8_t regContents;
	regContents = BCHTS4085_readReg(BCHTS4085_INTERRUPT_DRDY);
	return regContents;
	
}

//  Clears the maximum temperature register
void BCHTS4085_clearMaxTemp(void)
{ 
	BCHTS4085_writeReg(BCHTS4085_TEMP_MAX, 0x00);
}

//  Clears the maximum humidity register
void BCHTS4085_clearMaxHumidity(void)
{ 
	BCHTS4085_writeReg(BCHTS4085_HUMID_MAX, 0x00);
}

//  Reads the maximum temperature register
float BCHTS4085_readMaxTemp(void)
{
	uint8_t regContents;
	
	regContents = BCHTS4085_readReg(BCHTS4085_TEMP_MAX);
	
	return (float)regContents * 165/256 - 40;
	
}

//  Reads the maximum humidity register
float BCHTS4085_readMaxHumidity(void)
{
	uint8_t regContents;
	
	regContents = BCHTS4085_readReg(BCHTS4085_HUMID_MAX);
	
	return (float)regContents /256 * 100;
	
}


// Enables the interrupt pin for comfort zone operation
void BCHTS4085_enableThresholdInterrupt(void)
{
	
	uint8_t regContents;
	regContents = BCHTS4085_readReg(BCHTS4085_INTERRUPT_CONFIG);

	regContents = (regContents | 0x78);

	BCHTS4085_writeReg(BCHTS4085_INTERRUPT_CONFIG, regContents);
}

// Disables the interrupt pin for comfort zone operation
void BCHTS4085_disableThresholdInterrupt(void)
{
	uint8_t regContents;
	regContents = BCHTS4085_readReg(BCHTS4085_INTERRUPT_CONFIG);

	regContents = (regContents & 0x87);

	BCHTS4085_writeReg(BCHTS4085_INTERRUPT_CONFIG, regContents);
}

// enables the interrupt pin for DRDY operation
void BCHTS4085_enableDRDYInterrupt(void)
{
	uint8_t regContents;
	regContents = BCHTS4085_readReg(BCHTS4085_INTERRUPT_CONFIG);

	regContents = (regContents | 0x80);

	BCHTS4085_writeReg(BCHTS4085_INTERRUPT_CONFIG, regContents);
}

// disables the interrupt pin for DRDY operation
void BCHTS4085_disableDRDYInterrupt(void)
{
	uint8_t regContents;
	regContents = BCHTS4085_readReg(BCHTS4085_INTERRUPT_CONFIG);

	regContents = (regContents & 0x7F);

	BCHTS4085_writeReg(BCHTS4085_INTERRUPT_CONFIG, regContents);
}
