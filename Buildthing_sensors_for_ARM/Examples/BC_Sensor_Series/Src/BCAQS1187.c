/*
  BCAQS1187.cpp
  Created By: dh.kim, 2019-05-16

  This library facilitates communication with, and configuration of,
  the BCAQS1187 Sensor(CSS811_air quality sensor) for stm32f407vgtx.
*/


#include "BCAQS1187.h"
#include <math.h>

void BCAQS1187_init(uint8_t addr, I2C_HandleTypeDef *hi2c)
{
	BCAQS1187_addr = addr << 1;
	BCAQS1187_hi2c = hi2c;
}

void BCAQS1187_openReg(uint8_t reg)
{
	HAL_I2C_Master_Transmit(BCAQS1187_hi2c, BCAQS1187_addr, &reg, 1, 10);
}

uint8_t BCAQS1187_readReg(uint8_t reg)
{
	uint8_t reading;          // holds byte of read data
	HAL_I2C_Mem_Read(BCAQS1187_hi2c, BCAQS1187_addr, reg, I2C_MEMADD_SIZE_8BIT, &reading, 1, 10);

	return reading;
}

void BCAQS1187_writeReg(uint8_t reg, uint8_t data)
{
	HAL_I2C_Mem_Write(BCAQS1187_hi2c, BCAQS1187_addr, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

void BCAQS1187_appStart(void)
{
	BCAQS1187_openReg(BCAQS1187_BOOTLOADER_APP_START);
}

uint8_t BCAQS1187_readHW_ID(void)
{
	uint8_t contents;
	contents = BCAQS1187_readReg(BCAQS1187_HW_ID);

	return contents;
}

uint8_t BCAQS1187_readError(void)
{
	uint8_t contents;
	contents = BCAQS1187_readReg(BCAQS1187_ERROR_ID);

	return contents;
}

uint8_t BCAQS1187_readStatus(void)
{
	uint8_t contents;
	contents = BCAQS1187_readReg(BCAQS1187_STATUS);

	return contents;
}

uint8_t BCAQS1187_readMeasMode(void)
{
	uint8_t contents;
	contents = BCAQS1187_readReg(BCAQS1187_MEAS_MODE);

	return contents;
}

void BCAQS1187_setThresholds(uint16_t low_med, uint16_t med_high, uint8_t hysteresis)
{
	uint8_t data[5] = { (uint8_t)((low_med >> 8) & 0xF), (uint8_t)(low_med & 0xF),
	(uint8_t)((med_high >> 8) & 0xF), (uint8_t)(med_high & 0xF), hysteresis };

	HAL_I2C_Mem_Write(BCAQS1187_hi2c, BCAQS1187_addr, BCAQS1187_THRESHOLDS, I2C_MEMADD_SIZE_8BIT, data, 5, 10);
}

void BCAQS1187_SWReset(void)
{
	//reset sequence from the datasheet
	uint8_t data[] = { 0x11, 0xE5, 0x72, 0x8A };

	HAL_I2C_Mem_Write(BCAQS1187_hi2c, BCAQS1187_addr, BCAQS1187_SW_RESET, I2C_MEMADD_SIZE_8BIT, data, 4, 10);
}

void BCAQS1187_setEnvironmentalData(uint16_t humidity, uint16_t temperature)
{
	/* Humidity is stored as an unsigned 16 bits in 1/512%RH. The
	   default value is 50% = 0x64, 0x00. As an example 48.5%
	   humidity would be 0x61, 0x00.*/

	   /* Temperature is stored as an unsigned 16 bits integer in 1/512
		  degrees; there is an offset: 0 maps to -25°C. The default value is
		  25°C = 0x64, 0x00. As an example 23.5% temperature would be  0x61, 0x00.
		  The internal algorithm uses these values (or default values if
		  not set by the application) to compensate for changes in
		  relative humidity and ambient temperature.*/

	uint8_t data[4] = { (uint8_t)((humidity >> 8) & 0xFF),
					 (uint8_t)(humidity & 0xFF),
					 (uint8_t)((temperature >> 8) & 0xFF),
					 (uint8_t)(temperature & 0xFF) };

	HAL_I2C_Mem_Write(BCAQS1187_hi2c, BCAQS1187_addr, BCAQS1187_SW_RESET, I2C_MEMADD_SIZE_8BIT, data, 4, 10);
}

void BCAQS1187_setDriveMode(uint8_t mode)
{
	uint8_t contents;
	contents = BCAQS1187_readReg(BCAQS1187_MEAS_MODE);

	mode = mode & 0x07;
	contents = contents & 0x8F;

	BCAQS1187_writeReg(BCAQS1187_MEAS_MODE, contents | (mode << 4));
}

void BCAQS1187_enableInterrupt(void)
{
	uint8_t contents;
	contents = BCAQS1187_readReg(BCAQS1187_MEAS_MODE);

	contents = contents | 0x08;

	BCAQS1187_writeReg(BCAQS1187_MEAS_MODE, contents);
}

void BCAQS1187_disableInterrupt(void)
{
	uint8_t contents;
	contents = BCAQS1187_readReg(BCAQS1187_MEAS_MODE);

	contents = contents & 0xF7;

	BCAQS1187_writeReg(BCAQS1187_MEAS_MODE, contents);
}

uint8_t BCAQS1187_dataReady(void)
{
	uint8_t contents;
	contents = BCAQS1187_readReg(BCAQS1187_STATUS);

	contents = contents & 0x08;

	if (contents == 0x08)
		return 0;
	else return 1;
}

uint8_t BCAQS1187_readResultData(void)
{
	uint8_t adr = BCAQS1187_ALG_RESULT_DATA;
	uint8_t reading[8];          // holds byte of read data
	
	HAL_I2C_Master_Transmit(BCAQS1187_hi2c, BCAQS1187_addr, &adr, 1, 0xFF);
	HAL_I2C_Master_Receive(BCAQS1187_hi2c, BCAQS1187_addr, reading, 8, 0xFF);
	
	BCAQS1187__eCO2 = ((uint16_t)reading[0] << 8) | ((uint16_t)reading[1]);
	BCAQS1187__TVOC = ((uint16_t)reading[2] << 8) | ((uint16_t)reading[3]);
	
	return reading[5];
}

uint16_t BCAQS1187_getTVOC(void)
{
	return BCAQS1187__TVOC;
}

uint16_t BCAQS1187_geteCO2(void)
{
	return BCAQS1187__eCO2;
}

double BCAQS1187_readNTC()
{
	uint8_t reading[4];          // holds byte of read data

	HAL_I2C_Mem_Read(BCAQS1187_hi2c, BCAQS1187_addr, BCAQS1187_NTC, I2C_MEMADD_SIZE_8BIT, reading, 4, 10);

	uint32_t vref = ((uint32_t)reading[0] << 8) | reading[1];
	uint32_t vntc = ((uint32_t)reading[2] << 8) | reading[3];

	//from ams ccs811 app note
	uint32_t rntc = vntc * 100000 / vref;

	double ntc_temp;
	ntc_temp = log((double)rntc / 100000); // 1
	ntc_temp /= 3380; // 2
	ntc_temp += 1.0 / (25 + 273.15); // 3
	ntc_temp = 1.0 / ntc_temp; // 4
	ntc_temp -= 273.15; // 5
	return ntc_temp;
}

