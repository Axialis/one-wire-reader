/*
 * memory.c
 *
 *  Created on: 16 сент. 2021 г.
 *      Author: Alexandr
 */


#include "stm32f1xx_hal.h"
#include "memory.h"
#define MEMADDRESS 0xA0
extern I2C_HandleTypeDef hi2c1;

/*----------------------------------------------------------------------------------------*/

struct memory
{
	uint8_t sensor_1[7];
	uint8_t sensor_2[7];
	uint8_t sensor_3[7];
	uint8_t sensor_4[7];
	uint8_t sensor_5[7];
	uint8_t sensor_6[7];
	uint8_t sensor_7[7];
}sensor;


/*----------------------------------------------------------------------------------------*/

void writeToMemory(uint8_t Data, uint8_t MBS, uint8_t LBS)
{
	uint8_t buf[3];
	buf[0] = MBS;
	buf[1] = LBS;
	buf[2] = Data;
	HAL_I2C_Master_Transmit(&hi2c1, MEMADDRESS, buf, sizeof(buf), 10);
	HAL_Delay(50);
}

/*----------------------------------------------------------------------------------------*/

uint8_t readFromMemory(uint8_t MBS, uint8_t LBS)
{
	uint8_t data[1];
	uint8_t buf[2];
	buf[0] = MBS;
	buf[1] = LBS;
	HAL_I2C_Master_Transmit(&hi2c1, MEMADDRESS, buf, sizeof(buf), 10);
	HAL_Delay(100);
	HAL_I2C_Master_Receive(&hi2c1, MEMADDRESS, data, sizeof(data), 10);
	HAL_Delay(100);
	return data[0];
}

/*----------------------------------------------------------------------------------------*/

