/*
 * DS2482.h
 *
 *  Created on: 4 нояб. 2021 г.
 *      Author: TMedica
 */

#ifndef INC_DS2482_H_
#define INC_DS2482_H_

#include "stdint.h"

#define READ_ADDRESS 0x31
#define WRITE_ADDRESS 0x30
#define POLL_LIMIT			0x30	// 0x30 is the minimum poll limit

#define DS2482_CMD_RESET		0xF0	/* No param */
#define DS2482_CMD_SET_READ_PTR		0xE1	/* Param: DS2482_PTR_CODE_xxx */
#define DS2482_CMD_WRITE_CONFIG		0xD2	/* Param: Config byte */
#define DS2482_CMD_1WIRE_RESET		0xB4	/* Param: None */
#define DS2482_CMD_1WIRE_SINGLE_BIT	0x87	/* Param: Bit byte (bit7) */
#define DS2482_CMD_1WIRE_WRITE_BYTE	0xA5	/* Param: Data byte */
#define DS2482_CMD_1WIRE_READ_BYTE	0x96	/* Param: None */
/* Note to read the byte, Set the ReadPtr to Data then read (any addr) */
#define DS2482_CMD_1WIRE_TRIPLET	0x78	/* Param: Dir byte (bit7) */

/* Values for DS2482_CMD_SET_READ_PTR */
#define DS2482_PTR_CODE_STATUS		0xF0
#define DS2482_PTR_CODE_DATA		0xE1
#define DS2482_PTR_CODE_CONFIG		0xC3

/*
 * Configure Register bit definitions
 * The top 4 bits always read 0.
 * To write, the top nibble must be the 1's compl. of the low nibble.
 */
#define DS2482_REG_CFG_1WS		0x08	/* 1-wire speed */
#define DS2482_REG_CFG_SPU		0x04	/* strong pull-up */
#define DS2482_REG_CFG_PPM		0x02	/* presence pulse masking */
#define DS2482_REG_CFG_APU		0x01	/* active pull-up */

/*
 * Status Register bit definitions (read only)
 */
#define DS2482_REG_STS_DIR		0x80
#define DS2482_REG_STS_TSB		0x40
#define DS2482_REG_STS_SBR		0x20
#define DS2482_REG_STS_RST		0x10
#define DS2482_REG_STS_LL		0x08
#define DS2482_REG_STS_SD		0x04
#define DS2482_REG_STS_PPD		0x02
#define DS2482_REG_STS_1WB		0x01

void DS2842Reset(void);
void DS2842Configuration(void);

void DS2482SendCommand(uint8_t cmd);
void DS2482SendCommandData(uint8_t cmd, uint8_t data);

void OWReset(void);

void OWWriteByte(uint8_t bit);
uint8_t OWReadByte(void);


#endif /* INC_DS2482_H_ */
