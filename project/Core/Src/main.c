/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
const unsigned char  test_24x24_ches[] =
{
0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x38,0x30,0x00,0x30,0x60,0x00,0x30,0x60,0x00,0x7F,0xFF,0x00,0x60,0x01,0x00,0xC0,0x0C,0x00,0xE7,0x1C,0x00,0xC6,0x38,0x07,0xFF,0xFF,0x01,0x8E,0x61,0x01,0x8C,0xFB,0x01,0x8D,0xB8,0x03,0xFF,0x30,0x03,0x18,0x78,0x03,0x18,0xF8,0x06,0x38,0xD8,0x06,0x31,0x98,0x07,0xF3,0x1C,0x0C,0x7E,0x1E,0x0C,0x78,0x0F,0x00,0x70,0x00,0x00,0x00,0x00,/*"«∂",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x03,0xC0,0x00,0x00,0xC0,0x00,0x00,0xE0,0x00,0x00,0xE0,0x00,0x01,0xE0,0x00,0x01,0xE0,0x00,0x03,0xE0,0x00,0x07,0xE0,0x00,0x06,0xE0,0x00,0x0C,0xE0,0x00,0x1C,0xE0,0x00,0x38,0x60,0x00,0x70,0x70,0x00,0xE0,0x70,0x01,0xC0,0x78,0x03,0x00,0x38,0x06,0x00,0x3C,0x1C,0x00,0x1F,0x30,0x00,0x0C,0xE0,0x00,0x00,0x00,0x00,0x00,/*"»Î",1*/
0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x1D,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x38,0x01,0xFF,0xFF,0x00,0x00,0x30,0x00,0x00,0x30,0x00,0x00,0x70,0x00,0x03,0x70,0x03,0xFF,0xE0,0x00,0x30,0x60,0x00,0x70,0x60,0x00,0x60,0x60,0x00,0x60,0x60,0x00,0xE0,0x61,0x00,0xC0,0x63,0x00,0xFE,0x73,0x03,0xE0,0x7B,0x3E,0x00,0x3E,0x30,0x00,0x1E,0x00,0x00,0x0E,0x00,0x00,0x00,/*" Ω",2*/
};
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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_SDIO_SD_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

	LCD_Init();
		LCD_printf(20,20 ,16, "LCD Init ok");
	

	
	if(hsd.ErrorCode == SDMMC_ERROR_NONE)
	{
		LCD_printf(20,40 ,16, "SD Card Init OK");
		LCD_printf(20,60 ,16, "SD Card is %.1f M", (float)hsd.SdCard.BlockNbr * hsd.SdCard.BlockSize / 1000 / 1000 );
	}
	else
	{
		LCD_printf(20,60 ,16,"SD Card file %d\r\n",hsd.ErrorCode);
	}
	
	W25QXX_Init();
	LCD_printf(20,80 ,16, "W25Q128 Init OK, ID is:0x%X",W25QXX_ReadID()); 
	
	
	LCD_printf(20,100,16, "236545");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  { 
		HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port,LED_DEBUG_Pin,GPIO_PIN_RESET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port,LED_DEBUG_Pin,GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(LED_RUN_GPIO_Port,LED_RUN_Pin,GPIO_PIN_RESET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(LED_RUN_GPIO_Port,LED_RUN_Pin,GPIO_PIN_SET);
		HAL_Delay(500);
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
