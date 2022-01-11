/*
 * DS2482.c
 *
 *  Created on: 4 нояб. 2021 г.
 *      Author: Alexandr
 */


#include "DS2482.h"
#include "stm32f1xx_hal.h"
#include "lcd.h"

extern I2C_HandleTypeDef hi2c1;

Lcd_HandleTypeDef lcd;

void DS2842Reset(void)
{
	uint8_t stat[1] = {0x00};

	DS2482SendCommand(DS2482_CMD_RESET);

	while(1){

		HAL_I2C_Master_Receive(&hi2c1, READ_ADDRESS, stat, 1, HAL_MAX_DELAY);
		if((stat[0] & 0xF7) == 0x10)
		{
			break;
		}
	}
}

void DS2842Configuration(void)
{
	uint8_t stat[1] = {0x00};

	DS2482SendCommandData(DS2482_CMD_WRITE_CONFIG, 0x00);

	while(1){

		HAL_I2C_Master_Receive(&hi2c1, READ_ADDRESS, stat, 1, HAL_MAX_DELAY);
		if(stat[0] == 0x00)
		{
			break;
		}
	}
}

void DS2482SendCommand(uint8_t cmd)
{
	uint8_t temp1[1] = {cmd};
	HAL_I2C_Master_Transmit(&hi2c1, WRITE_ADDRESS, temp1, sizeof(temp1), HAL_MAX_DELAY);
}

void DS2482SendCommandData(uint8_t cmd, uint8_t data)
{
	uint8_t temp1[2] = {cmd, data};
	HAL_I2C_Master_Transmit(&hi2c1, WRITE_ADDRESS, temp1, sizeof(temp1), HAL_MAX_DELAY);
}

void OWReset(void)
{
	uint8_t stat[1] = {0x00};

	DS2482SendCommand(DS2482_CMD_1WIRE_RESET);

	while(1){

		HAL_I2C_Master_Receive(&hi2c1, READ_ADDRESS, stat, 1, HAL_MAX_DELAY);
		if((stat[0] & DS2482_REG_STS_1WB) == 0x00)
		{
			break;
		}
	}
}

void OWWriteByte(uint8_t byte)
{
	uint8_t stat[1] = {0x00};

	DS2482SendCommandData(DS2482_CMD_1WIRE_WRITE_BYTE, byte);

	while(1){

		HAL_I2C_Master_Receive(&hi2c1, READ_ADDRESS, stat, 1, HAL_MAX_DELAY);
		if((stat[0] & DS2482_REG_STS_1WB) == 0x00)
		{
			break;
		}
	}
}

uint8_t OWReadByte(void)
{
	uint8_t stat[1] = {0x00};
	uint8_t data[1] = {0x00};

	DS2482SendCommand(DS2482_CMD_1WIRE_READ_BYTE);

	while(1){

		HAL_I2C_Master_Receive(&hi2c1, READ_ADDRESS, stat, 1, HAL_MAX_DELAY);
		if((stat[0] & DS2482_REG_STS_1WB) == 0x00)
		{
			break;
		}
	}

	DS2482SendCommandData(DS2482_CMD_SET_READ_PTR, 0xE1);
	HAL_I2C_Master_Receive(&hi2c1, READ_ADDRESS, data, 1, HAL_MAX_DELAY);

	return data[0];
}


