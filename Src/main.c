/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define ARM_MATH_CM4  // indicate we're using Cortex-M4
#include "arm_math.h"

#include "lab1math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ITM_Port32(n) (*((volatile unsigned long *) (0xE0000000+4*n)))  // ITM base addr (see Cortex-M4 manual p.93)
#define ARRAY_LEN(a) sizeof(a) / sizeof(a[0])
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
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
  /* USER CODE BEGIN 2 */
  // define our variables and array
  float max = 0;
  uint32_t maxIndex = 0;
  float maxAsm = 0;
  uint32_t maxIndexAsm = 0;
  float maxCMSIS = 0;
  uint32_t maxIndexCMSIS = 0;
  // the max is 88.49 at index 5
  float array[10] = {48.21, 79.48, 24.27, 28.82, 78.24, 88.49, 31.19, 5.52, 82.70, 77.73};

  // element-wise multiplication
  float vec_a[3] = {5.78, 2.31, 3.00};
  float vec_b[3] = {12.0, -4.22, 76.93};
  float vec_res[3] = {0};  // fill result vector with 0
  float vec_res_asm[3] = {0};
  float vec_res_CMSIS[3] = {0};

  // standard deviation
//  const float stddev_data[10] = {48.21, 79.48, 24.27, 28.82, 78.24, 88.49, 31.19, 5.52, 82.70, 77.73};  // CMSIS is different
  const float stddev_data[10] = {21.2, 99.85, -48.2, 2.32, 123.32, 9.76, -12.22, -50.0, 33.33, 10.98};  // old cStd is different due to division rounding
  float stddev_res = 0;
  float stddev_res_asm = 0;
  float stddev_res_CMSIS = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  // array max
	  ITM_Port32(31) = 1;
	  for (uint32_t i = 0; i < 1000; i++)
	  {
		  cMax(array, 10, &max, &maxIndex);
	  }
	  ITM_Port32(31) = 2;
	  for (uint32_t i = 0; i < 1000; i++)
	  {
		  asmMax(array, 10, &maxAsm, &maxIndexAsm);
	  }
	  ITM_Port32(31) = 3;
	  for (uint32_t i = 0; i < 1000; i++)
	  {
		  arm_max_f32(array, 10, &maxCMSIS, &maxIndexCMSIS);
	  }
	  ITM_Port32(31) = 4;
	  // element-wise multiplication
	  for (uint32_t i = 0; i < 1000; i++)
	  {
		  cMult(vec_a, vec_b, vec_res, ARRAY_LEN(vec_a));
	  }
	  ITM_Port32(31) = 5;
	  for (uint32_t i = 0; i < 1000; i++)
	  {
		  asmMult(vec_a, vec_b, vec_res_asm, ARRAY_LEN(vec_a));
	  }
	  ITM_Port32(31) = 6;
	  for (uint32_t i = 0; i < 1000; i++)
	  {
		  arm_mult_f32(vec_a, vec_b, vec_res_CMSIS, ARRAY_LEN(vec_a));
	  }
	  ITM_Port32(31) = 7;
	  // standard deviation
	  for (uint32_t i = 0; i < 1000; i++)
	  {
		  cStd(stddev_data, ARRAY_LEN(stddev_data), &stddev_res);
	  }
	  ITM_Port32(31) = 8;
	  for (uint32_t i = 0; i < 1000; i++)
	  {
		  asmStd(stddev_data, ARRAY_LEN(stddev_data), &stddev_res_asm);
	  }
	  ITM_Port32(31) = 9;
	  for (uint32_t i = 0; i < 1000; i++)
	  {
		  arm_std_f32(stddev_data, ARRAY_LEN(stddev_data), &stddev_res_CMSIS);
	  }
	  ITM_Port32(31) = 10;
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
