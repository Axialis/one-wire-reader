/*
 * memory.h
 *
 *  Created on: 16 сент. 2021 г.
 *      Author: Alexandr
 */

#ifndef INC_MEMORY_H_
#define INC_MEMORY_H_

typedef enum
{
	Line_1 = 0x00,
	Line_2 = 0x01,
	Line_3 = 0x02,
	Line_4 = 0x03,
	Line_5 = 0x04,
	Line_6 = 0x05,
	Line_7 = 0x06,
	Line_8 = 0x07,
	Line_9 = 0x08,
	Line_10 = 0x09,
	Line_11 = 0x0A,
	Line_12 = 0x0B,
	Line_13 = 0x0C,
	Line_14 = 0x0D,
	Line_15 = 0x0E,
	Line_16 = 0x0F
} Line;

typedef enum
{
	Column_1 = 0x00,
	Column_2 = 0x01,
	Column_3 = 0x02,
	Column_4 = 0x03,
	Column_5 = 0x04,
	Column_6 = 0x05,
	Column_7 = 0x06,
	Column_8 = 0x07,
	Column_9 = 0x08,
	Column_10 = 0x09,
	Column_11 = 0x0A,
	Column_12 = 0x0B,
	Column_13 = 0x0C,
	Column_14 = 0x0D,
	Column_15 = 0x0E,
	Column_16 = 0x0F
} Column;

void writeToMemory(uint8_t Data, uint8_t MBS, uint8_t LBS);
uint8_t readFromMemory(uint8_t MBS, uint8_t LBS);

#endif /* INC_MEMORY_H_ */
