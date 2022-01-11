/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "macro.h"
#include "memory.h"
#include "DS2482.h"



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ErrorLed(uint8_t status);
void BusyLed(uint8_t status);
void ReadLed(uint8_t status);
void SelectChannels();
void DrawSelectChannel();
void DrawAnimation();
void RelayControl();
void Rel(int number);
void WriteMemory();
void ReadOWDevice();
void AddressMenu(uint8_t channel);
void StatusPercent(uint8_t per);
uint8_t LinePosition(uint8_t ch);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef enum
{
	ON = 1,
	OFF = 0
} LedState;


Lcd_HandleTypeDef lcd;



uint8_t channel[7];
uint8_t sensor[8][8] = {0x00};
uint8_t key = 0;
uint8_t menuposstart = 0;

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */



	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */
	HAL_Delay(100);
	Lcd_PortType ports[] = {
			D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port
	};

	Lcd_PinType pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};



	lcd = Lcd_create(ports, pins, RS_GPIO_Port, RS_Pin, E_GPIO_Port, E_Pin, LCD_4_BIT_MODE);

	Lcd_cursor(&lcd, 0,3);
	Lcd_string(&lcd, "TENDO 4/8");
	Lcd_cursor(&lcd, 1,0);
	Lcd_string(&lcd, "ONE WIRE READER");


	Lcd_clear(&lcd);

	DS2842Reset();
	DS2842Configuration();

	ErrorLed(OFF);
	BusyLed(OFF);
	ReadLed(OFF);


	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		switch (key) {
		case 0:

			Lcd_cursor(&lcd, 0,0);
			Lcd_string(&lcd, "SELECT CHANNELS");
			SelectChannels();
			DrawSelectChannel();
			if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0) == 0) //read
			{
				key = 1;
			}

			if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == 0) //write
			{
				key = 9;
			}

			break;

		case 1:
			Lcd_clear(&lcd);
			key = 2;
			break;

		case 2:
			DrawAnimation();
			key = 5;
			break;

		case 3:
			Lcd_clear(&lcd);
			key = 4;
			break;

		case 4:
			DrawAnimation();
			key = 7;
			break;

		case 5:
			Lcd_clear(&lcd);
			key = 6;
			break;

		case 6:
			Lcd_cursor(&lcd, 0,0);
			Lcd_string(&lcd, "Press READ to");
			Lcd_cursor(&lcd, 1,0);
			Lcd_string(&lcd, "continue");
			for(;;)
			{
				Lcd_cursor(&lcd, 1,15);
				Lcd_string(&lcd, ">");
				if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0) == 0) //read
				{
					ReadOWDevice();
					key = 7;
					break;
				}
				HAL_Delay(150);
				Lcd_cursor(&lcd, 1,15);
				Lcd_string(&lcd, " ");
				if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0) == 0) //read
				{
					ReadOWDevice();
					key = 7;
					break;
				}
				HAL_Delay(150);
			}

			break;

		case 7:
			AddressMenu(menuposstart);
			key = 8;
			break;

		case 8:

			if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0) == 0) //<R
			{
				if(menuposstart != 0)
				{
					HAL_Delay(150);
					menuposstart = menuposstart - 1;
					key = 7;
				}
			}

			if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == 0) //W>
			{
				if(menuposstart != 9)
				{
					HAL_Delay(150);
					menuposstart = menuposstart + 1;
					key = 7;
				}
				if(menuposstart == 9)
				{
					menuposstart = 0;
					Lcd_clear(&lcd);
					for(int k = 0; k<8; k++)
					{
						for(int m = 0; m<8; m++)
						{
							sensor[k][m] = 0;
						}
					}
					key = 0;
				}
			}
			break;
			//-----------------------------------------------------WRITE TO MEMORY--------------------------------------------//
		case 9:
			Lcd_clear(&lcd);
			key = 10;
			break;

		case 10:
			Lcd_cursor(&lcd, 0,0);
			Lcd_string(&lcd, "Insert IC memory ");
			Lcd_cursor(&lcd, 1,0);
			Lcd_string(&lcd, "and press WRITE");
			for(;;)
			{
				Lcd_cursor(&lcd, 1,15);
				Lcd_string(&lcd, ">");
				if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == 0) //write
				{
					key = 11;
					break;
				}
				HAL_Delay(150);
				Lcd_cursor(&lcd, 1,15);
				Lcd_string(&lcd, " ");
				if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == 0) //write
				{
					key = 11;
					break;
				}
				HAL_Delay(150);
			}
			break;

		case 11:
			Lcd_clear(&lcd);
			key = 12;
			break;

		case 12:

			if(HAL_I2C_IsDeviceReady(&hi2c1, 0xA0, 1, HAL_MAX_DELAY) != HAL_OK)
			{
				ErrorLed(ON);
				Lcd_cursor(&lcd, 0,0);
				Lcd_string(&lcd, "NO MEMORY IC");
				Lcd_cursor(&lcd, 1,0);
				Lcd_string(&lcd, "Check slot");
				key = 13;
			}
			else
			{
				ErrorLed(OFF);
				key = 15;
			}
			break;

		case 13:

			if(HAL_I2C_IsDeviceReady(&hi2c1, 0xA0, 1, HAL_MAX_DELAY) != HAL_OK)
			{
				key = 12;
			}
			else
			{
				key = 14;
			}
			break;

		case 14:
			Lcd_clear(&lcd);
			key = 15;
			break;

		case 15:
			ErrorLed(OFF);
			Lcd_cursor(&lcd, 0,0);
			Lcd_string(&lcd, "OK");
			Lcd_cursor(&lcd, 1,0);
			Lcd_string(&lcd, "Press WRITE");
			for(;;)
			{
				Lcd_cursor(&lcd, 1,15);
				Lcd_string(&lcd, ">");
				if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == 0) //read
				{
					key = 16;
					break;
				}
				HAL_Delay(150);
				Lcd_cursor(&lcd, 1,15);
				Lcd_string(&lcd, " ");
				if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == 0) //read
				{
					key = 16;
					break;
				}
				HAL_Delay(150);
			}
			break;

		case 16:
			Lcd_clear(&lcd);
			key = 17;
			break;

		case 17:
			ReadOWDevice();
			WriteMemory();
			key = 18;
			break;

		case 18:
			Lcd_clear(&lcd);
			Lcd_cursor(&lcd, 0,0);
			Lcd_string(&lcd, "OK! Press 'W' to");
			Lcd_cursor(&lcd, 1,0);
			Lcd_string(&lcd, "return Main Menu");
			key = 19;
			break;

		case 19:
				if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == 0) //read
				{
					Lcd_clear(&lcd);
					key = 0;
					break;
				}
			break;


		}

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure the Systick interrupt time
	 */
	__HAL_RCC_PLLI2S_ENABLE();
}

/* USER CODE BEGIN 4 */

void ErrorLed(uint8_t status)
{
	if(status == 0)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	}
}

void BusyLed(uint8_t status)
{
	if(status == 0)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
	}
}

void ReadLed(uint8_t status)
{
	if(status == 0)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
	}
}

void SelectChannels()
{
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == 0)
	{
		channel[0] = 1;
	}
	else
	{
		channel[0] = 0;
	}
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) == 0)
	{
		channel[1] = 1;
	}
	else
	{
		channel[1] = 0;
	}
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2) == 0)
	{
		channel[2] = 1;
	}
	else
	{
		channel[2] = 0;
	}
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3) == 0)
	{
		channel[3] = 1;
	}
	else
	{
		channel[3] = 0;
	}
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == 0)
	{
		channel[4] = 1;
	}
	else
	{
		channel[4] = 0;
	}
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5) == 0)
	{
		channel[5] = 1;
	}
	else
	{
		channel[5] = 0;
	}
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6) == 0)
	{
		channel[6] = 1;
	}
	else
	{
		channel[6] = 0;
	}
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7) == 0)
	{
		channel[7] = 1;
	}
	else
	{
		channel[7] = 0;
	}
}

void DrawSelectChannel()
{
	if(channel[0] == 0)
	{
		Lcd_cursor(&lcd, 1, 0);
		Lcd_string(&lcd, "1");
	}
	else
	{
		Lcd_cursor(&lcd, 1, 0);
		Lcd_string(&lcd, " ");
	}

	if(channel[1] == 0)
	{
		Lcd_cursor(&lcd, 1, 2);
		Lcd_string(&lcd, "2");
	}
	else
	{
		Lcd_cursor(&lcd, 1, 2);
		Lcd_string(&lcd, " ");
	}

	if(channel[2] == 0)
	{
		Lcd_cursor(&lcd, 1, 4);
		Lcd_string(&lcd, "3");
	}
	else
	{
		Lcd_cursor(&lcd, 1, 4);
		Lcd_string(&lcd, " ");
	}

	if(channel[3] == 0)
	{
		Lcd_cursor(&lcd, 1, 6);
		Lcd_string(&lcd, "4");
	}
	else
	{
		Lcd_cursor(&lcd, 1, 6);
		Lcd_string(&lcd, " ");
	}

	if(channel[4] == 0)
	{
		Lcd_cursor(&lcd, 1, 8);
		Lcd_string(&lcd, "5");
	}
	else
	{
		Lcd_cursor(&lcd, 1, 8);
		Lcd_string(&lcd, " ");
	}

	if(channel[5] == 0)
	{
		Lcd_cursor(&lcd, 1, 10);
		Lcd_string(&lcd, "6");
	}
	else
	{
		Lcd_cursor(&lcd, 1, 10);
		Lcd_string(&lcd, " ");
	}

	if(channel[6] == 0)
	{
		Lcd_cursor(&lcd, 1, 12);
		Lcd_string(&lcd, "7");
	}
	else
	{
		Lcd_cursor(&lcd, 1, 12);
		Lcd_string(&lcd, " ");
	}

	if(channel[7] == 0)
	{
		Lcd_cursor(&lcd, 1, 14);
		Lcd_string(&lcd, "8");
	}
	else
	{
		Lcd_cursor(&lcd, 1, 14);
		Lcd_string(&lcd, " ");
	}
}

void DrawAnimation()
{
	for (int i = 0; i<15; i++)
	{
		Lcd_cursor(&lcd, 0,i);
		Lcd_string(&lcd, ">");
		HAL_Delay(10);
	}
	for (int i = 15; i>0; i--)
	{
		Lcd_cursor(&lcd, 1,i);
		Lcd_string(&lcd, "<");
		HAL_Delay(10);
	}
}

void RelayControl()
{
	for(int i = 0; i<7; i++)
	{
		if(channel[i] == 0)
		{
			Rel(i);
		}
	}
}

void Rel(int number)
{
	switch (number) {
	case 0:
		REL_1(ON); REL_2(OFF); REL_3(OFF); REL_4(OFF); REL_5(OFF); REL_6(OFF); REL_7(OFF); REL_8(OFF);
		break;
	case 1:
		REL_1(OFF); REL_2(ON); REL_3(OFF); REL_4(OFF); REL_5(OFF); REL_6(OFF); REL_7(OFF); REL_8(OFF);
		break;
	case 2:
		REL_1(OFF); REL_2(OFF); REL_3(ON); REL_4(OFF); REL_5(OFF); REL_6(OFF); REL_7(OFF); REL_8(OFF);
		break;
	case 3:
		REL_1(OFF); REL_2(OFF); REL_3(OFF); REL_4(ON); REL_5(OFF); REL_6(OFF); REL_7(OFF); REL_8(OFF);
		break;
	case 4:
		REL_1(OFF); REL_2(OFF); REL_3(OFF); REL_4(OFF); REL_5(ON); REL_6(OFF); REL_7(OFF); REL_8(OFF);
		break;
	case 5:
		REL_1(OFF); REL_2(OFF); REL_3(OFF); REL_4(OFF); REL_5(OFF); REL_6(ON); REL_7(OFF); REL_8(OFF);
		break;
	case 6:
		REL_1(OFF); REL_2(OFF); REL_3(OFF); REL_4(OFF); REL_5(OFF); REL_6(OFF); REL_7(ON); REL_8(OFF);
		break;
	case 7:
		REL_1(OFF); REL_2(OFF); REL_3(OFF); REL_4(OFF); REL_5(OFF); REL_6(OFF); REL_7(OFF); REL_8(ON);
		break;
	default:
		REL_1(OFF); REL_2(OFF); REL_3(OFF); REL_4(OFF); REL_5(OFF); REL_6(OFF); REL_7(OFF); REL_8(OFF);
		break;
	}
}

void WriteMemory()
{
	uint8_t xpos = 0x00;
	BusyLed(ON);
	for(int k = 0; k<8; k++)
	{
		if(channel[k] == 0)
		{
			StatusPercent(k);
		}
		xpos = LinePosition(k);
		for(int i = 0; i<8; i++)
		{
			{
				if(channel[k] == 0)
				{
					writeToMemory(sensor[k][i], 0x00 , xpos);
					xpos = xpos+1;
				}
			}
		}
	}

	Lcd_clear(&lcd);
	Lcd_cursor(&lcd, 1, 6);
	Lcd_string(&lcd, "100%");
	BusyLed(OFF);

}

void ReadOWDevice()
{
	ReadLed(ON);
	Lcd_clear(&lcd);
	for(int k = 0; k<8; k++)
	{
		if(channel[k] == 0)
		{
			Rel(k);
			HAL_Delay(500);
			OWReset();
			OWWriteByte(0x33);
		}

		if(channel[k] == 0)
		{

			for(int i = 0; i<8; i++)
			{
				sensor[k][i] = OWReadByte();
			}

			DS2842Reset();
			OWReset();
		}
	}
	ReadLed(OFF);

}

void AddressMenu(uint8_t channel)
{
	char str[8];
	char ch[2];

	if(channel != 8)
	{
		Lcd_clear(&lcd);
		Lcd_cursor(&lcd, 0, 0);

		for(int i = 0; i<8; i++)
		{
			sprintf(str, "%02X" , sensor[channel][i]);
			Lcd_string(&lcd, str);
		}

		sprintf(ch, "%i" , channel+1);

		Lcd_cursor(&lcd, 1, 3);
		Lcd_string(&lcd, "Channel #");
		Lcd_string(&lcd, ch);
		Lcd_cursor(&lcd, 1, 14);
		Lcd_string(&lcd, "W>");
		if(channel != 0)
		{
			Lcd_cursor(&lcd, 1,0);
			Lcd_string(&lcd, "<R");
		}
	}


	if(channel == 8)
	{
		Lcd_clear(&lcd);
		Lcd_cursor(&lcd, 0, 0);
		Lcd_string(&lcd, "Exit to menu?");
		Lcd_cursor(&lcd, 1, 0);
		Lcd_string(&lcd, "<R(No)");
		Lcd_cursor(&lcd, 1, 9);
		Lcd_string(&lcd, "(Yes)W>");
	}
}

uint8_t LinePosition(uint8_t ch)
{
	return (8*ch);
}

void StatusPercent(uint8_t per)
{
	if(per == 0)
	{
		Lcd_clear(&lcd);
		Lcd_cursor(&lcd, 1, 6);
		Lcd_string(&lcd, "12.5%");
	}
	if(per == 1)
	{
		Lcd_clear(&lcd);
		Lcd_cursor(&lcd, 1, 6);
		Lcd_string(&lcd, "25%");
	}
	if(per == 3)
	{
		Lcd_clear(&lcd);
		Lcd_cursor(&lcd, 1, 6);
		Lcd_string(&lcd, "37.5%");
	}
	if(per == 3)
	{
		Lcd_clear(&lcd);
		Lcd_cursor(&lcd, 1, 6);
		Lcd_string(&lcd, "50%");
	}
	if(per == 4)
	{
		Lcd_clear(&lcd);
		Lcd_cursor(&lcd, 1, 6);
		Lcd_string(&lcd, "62.5%");
	}
	if(per == 5)
	{
		Lcd_clear(&lcd);
		Lcd_cursor(&lcd, 1, 6);
		Lcd_string(&lcd, "75%");
	}
	if(per == 6)
	{
		Lcd_clear(&lcd);
		Lcd_cursor(&lcd, 1, 6);
		Lcd_string(&lcd, "87.5%");
	}
	if(per == 7)
	{
		Lcd_clear(&lcd);
		Lcd_cursor(&lcd, 1, 6);
		Lcd_string(&lcd, "100%");
	}
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
