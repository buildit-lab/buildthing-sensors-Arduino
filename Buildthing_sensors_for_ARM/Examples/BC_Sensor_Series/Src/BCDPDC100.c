/*
  BCDPDC100.c
  Created By: dh.kim, 2019-05-16

  This library facilitates communication with, and configuration of,
  the BCDPDC100 Sensor(TMD2671_Digital Proximity Detector) for stm32f407vgtx.
*/


#include "BCDPDC100.h"



void BCDPDC100_init(uint8_t addr, I2C_HandleTypeDef *hi2c)
{
	BCDPDC100_addr = addr<<1;
	BCDPDC100_hi2c = hi2c;
}

void BCDPDC100_openReg(uint8_t reg)
{
	reg = reg | BCDPDC100_REPEATED_BYTE_PROTOCOL;
	HAL_I2C_Master_Transmit(BCDPDC100_hi2c, BCDPDC100_addr, &reg, 1, 10);

}

uint8_t BCDPDC100_readReg(uint8_t reg)
{
	uint8_t reading;          // holds byte of read data
	
	HAL_I2C_Mem_Read(BCDPDC100_hi2c, BCDPDC100_addr, reg | BCDPDC100_REPEATED_BYTE_PROTOCOL, I2C_MEMADD_SIZE_8BIT, &reading, 1, 10);

	return reading;
}

void BCDPDC100_writeReg(uint8_t reg, uint8_t data)
{
	
	HAL_I2C_Mem_Write(BCDPDC100_hi2c, BCDPDC100_addr, reg | BCDPDC100_REPEATED_BYTE_PROTOCOL, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);

}

uint16_t BCDPDC100_readProximityAdc(void)
{
	uint8_t data[2];
	uint16_t promximityAdc;
	data[0] = BCDPDC100_readReg(BCDPDC100_PDATAL);
	data[1] = BCDPDC100_readReg(BCDPDC100_PDATAH);

	promximityAdc = (uint16_t)data[1] << 8 | data[0];

	return promximityAdc;
}

void BCDPDC100_enableInterruptMask(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = BCDPDC100_readReg(BCDPDC100_ENABLE);

	//set bit 5 to 1 to enable
	configContents = (configContents | 0x20);

	BCDPDC100_writeReg(BCDPDC100_ENABLE, configContents);
}

void BCDPDC100_disableInterruptMask(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = BCDPDC100_readReg(BCDPDC100_ENABLE);

	//set bit 5 to 0 to disable (all other bits 1)
	configContents = (configContents & 0xDF);
	BCDPDC100_writeReg(BCDPDC100_ENABLE, configContents);
}

void BCDPDC100_enableWait(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = BCDPDC100_readReg(BCDPDC100_ENABLE);

	//set bit 3 to 1 to enable
	configContents = (configContents | 0x08);

	BCDPDC100_writeReg(BCDPDC100_ENABLE, configContents);
}

void BCDPDC100_disableWait(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = BCDPDC100_readReg(BCDPDC100_ENABLE);

	//set bit 3 to 0 to disable (all other bits 1)
	configContents = (configContents & 0xF7);
	BCDPDC100_writeReg(BCDPDC100_ENABLE, configContents);
}

void BCDPDC100_enableProximity(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = BCDPDC100_readReg(BCDPDC100_ENABLE);

	//set bit 2:1 to 1 to enable
	configContents = (configContents | 0x06);

	BCDPDC100_writeReg(BCDPDC100_ENABLE, configContents);
}

void BCDPDC100_disableProximity(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = BCDPDC100_readReg(BCDPDC100_ENABLE);

	//set bit 2:1 to 0 to disable (all other bits 1)
	configContents = (configContents & 0xF9);
	BCDPDC100_writeReg(BCDPDC100_ENABLE, configContents);
}

void BCDPDC100_powerOn(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = BCDPDC100_readReg(BCDPDC100_ENABLE);
	//set bit 0 to 1 to enable
	configContents = (configContents | 0x01);

	BCDPDC100_writeReg(BCDPDC100_ENABLE, configContents);
	configContents = BCDPDC100_readReg(BCDPDC100_ENABLE);

}

void BCDPDC100_powerOff(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = BCDPDC100_readReg(BCDPDC100_ENABLE);

	//set bit 0 to 0 to disable (all other bits 1)
	configContents = (configContents & 0xFE);
	BCDPDC100_writeReg(BCDPDC100_ENABLE, configContents);
}

uint8_t BCDPDC100_readEnable(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = BCDPDC100_readReg(BCDPDC100_ENABLE);

	return configContents;
}

void BCDPDC100_enableWlong(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = BCDPDC100_readReg(BCDPDC100_CONFIG);

	//set bit 1 to 1 to enable
	configContents = (configContents | 0x02);

	BCDPDC100_writeReg(BCDPDC100_CONFIG, configContents);
}

void BCDPDC100_disableWlong(void)
{
	uint8_t configContents; //Stores current contents of config register

	configContents = BCDPDC100_readReg(BCDPDC100_CONFIG);

	//set bit 1 to 0 to heater (all other bits 1)
	configContents = (configContents & 0xFD);
	BCDPDC100_writeReg(BCDPDC100_CONFIG, configContents);
}

void BCDPDC100_writeAtime(uint8_t value)
{
	BCDPDC100_writeReg(BCDPDC100_ATIME, value);
}

uint8_t BCDPDC100_readAtime(void)
{
	uint8_t Atime;
	Atime = BCDPDC100_readReg(BCDPDC100_ATIME);

	return Atime;
}

void BCDPDC100_writePtime(uint8_t value)
{
	BCDPDC100_writeReg(BCDPDC100_PTIME, value);
}

uint8_t BCDPDC100_readPtime(void)
{
	uint8_t Ptime;
	Ptime = BCDPDC100_readReg(BCDPDC100_PTIME);

	return Ptime;
}

void BCDPDC100_writeWtime(uint8_t value)
{
	BCDPDC100_writeReg(BCDPDC100_WTIME, value);
}

uint16_t BCDPDC100_readWtime(void)
{
	uint8_t data[2];
	uint16_t Wtime;
	data[0] = BCDPDC100_readReg(BCDPDC100_WTIME);
	data[1] = BCDPDC100_readReg(BCDPDC100_CONFIG);

	Wtime = (uint16_t)data[1] << 7 | data[0];

	return Wtime;
}


void BCDPDC100_setPpulse(uint8_t p_pluse)
{
	BCDPDC100_writeReg(BCDPDC100_PPULSE, p_pluse);
}

uint8_t BCDPDC100_readPpulse(void)
{
	uint8_t contents;
	contents = BCDPDC100_readReg(BCDPDC100_PPULSE);

	return contents;
}


void BCDPDC100_setLowThresold(uint16_t low_threshold)
{
	uint8_t data[2];
	data[0] = low_threshold % 256;
	data[1] = low_threshold / 256;
	BCDPDC100_writeReg(BCDPDC100_PILTL, data[0]);
	BCDPDC100_writeReg(BCDPDC100_PILTH, data[1]);
}

uint16_t BCDPDC100_readLowThresold(void)
{
	uint8_t data[2];
	uint16_t LowThresold;
	data[0] = BCDPDC100_readReg(BCDPDC100_PILTL);
	data[1] = BCDPDC100_readReg(BCDPDC100_PILTH);

	LowThresold = (uint16_t)data[1] << 8 | data[0];

	return LowThresold;
}

void BCDPDC100_setHighThresold(uint16_t high_threshold)
{
	uint8_t data[2];
	data[0] = high_threshold % 256;
	data[1] = high_threshold / 256;
	BCDPDC100_writeReg(BCDPDC100_PIHTL, data[0]);
	BCDPDC100_writeReg(BCDPDC100_PIHTH, data[1]);
}

uint16_t BCDPDC100_readHighThresold(void)
{
	uint8_t data[2];
	uint16_t HighThresold;
	data[0] = BCDPDC100_readReg(BCDPDC100_PIHTL);
	data[1] = BCDPDC100_readReg(BCDPDC100_PIHTH);

	HighThresold = (uint16_t)data[1] << 8 | data[0];

	return HighThresold;
}


void BCDPDC100_setPersistence(int persistence)
{
	uint8_t configContents;

	switch (persistence)
	{
	case 0:
		configContents = 0x00;
		break;

	case 1:
		configContents = 0x10;
		break;

	case 2:
		configContents = 0x20;
		break;

	case 3:
		configContents = 0x30;
		break;

	case 4:
		configContents = 0x40;
		break;

	case 5:
		configContents = 0x50;
		break;

	case 6:
		configContents = 0x60;
		break;

	case 7:
		configContents = 0x70;
		break;

	case 8:
		configContents = 0x80;
		break;

	case 9:
		configContents = 0x90;
		break;

	case 10:
		configContents = 0xA0;
		break;

	case 11:
		configContents = 0xB0;
		break;

	case 12:
		configContents = 0xC0;
		break;

	case 13:
		configContents = 0xD0;
		break;

	case 14:
		configContents = 0xE0;
		break;

	case 15:
		configContents = 0xF0;
		break;

	default:
		configContents = 0x00;
	}

	BCDPDC100_writeReg(BCDPDC100_PERS, configContents);
}

uint8_t BCDPDC100_readPersistence(void)
{
	uint8_t persistence;
	persistence = BCDPDC100_readReg(BCDPDC100_PERS);

	return persistence;
}


void BCDPDC100_setLedPower(int led_power)
{
	uint8_t configContents;
	configContents = BCDPDC100_readReg(BCDPDC100_CONTROL);

	switch (led_power)
	{
	case BCDPDC100_LED100P:
		configContents = (configContents & 0x3F);
		break;

	case BCDPDC100_LED50P:
		configContents = (configContents & 0x3F);
		configContents = (configContents | 0x40);
		break;

	case BCDPDC100_LED25P:
		configContents = (configContents & 0x3F);
		configContents = (configContents | 0x80);
		break;

	case BCDPDC100_LED12D5P:
		configContents = (configContents | 0xC0);
		break;

	default:
		configContents = (configContents | 0xC0);
	}

	BCDPDC100_writeReg(BCDPDC100_CONTROL, configContents);
}

void BCDPDC100_setDiode(int diode)
{
	uint8_t configContents;
	configContents = BCDPDC100_readReg(BCDPDC100_CONTROL);

	switch (diode)
	{
	case BCDPDC100_CHANNEL0:
		configContents = (configContents & 0xCF);
		configContents = (configContents | 0x10);
		break;

	case BCDPDC100_CHANNEL1:
		configContents = (configContents & 0xCF);
		configContents = (configContents | 0x20);
		break;

	case BCDPDC100_BOTH:
		configContents = (configContents | 0x30);
		break;

	default:
		configContents = (configContents & 0xCF);
		configContents = (configContents | 0x10);
	}

	BCDPDC100_writeReg(BCDPDC100_CONTROL, configContents);
}

uint8_t BCDPDC100_readControl(void)
{
	uint8_t Control;
	Control = BCDPDC100_readReg(BCDPDC100_CONTROL);

	return Control;
}

uint8_t BCDPDC100_readStatus(void)
{
	uint8_t Status;
	Status = BCDPDC100_readReg(BCDPDC100_STATUS);

	return Status;
}

uint8_t BCDPDC100_readInterrupt(void)
{
	uint8_t Status;
	Status = BCDPDC100_readReg(BCDPDC100_STATUS);
	BCDPDC100_interruptClear();

	return ((Status >> 5) & 0x01) == 1 ? 1 : 0;
}

uint8_t BCDPDC100_readDeviceID(void)
{
	uint8_t DeviceID;
	DeviceID = BCDPDC100_readReg(BCDPDC100_DEVICE_ID);

	return DeviceID;
}

void BCDPDC100_interruptClear(void)
{
	uint8_t clr_data = BCDPDC100_PROXIMITY_INTERRUPT_CLEAR;
	HAL_I2C_Master_Transmit(BCDPDC100_hi2c, BCDPDC100_addr, &clr_data, 1, 10);
}

