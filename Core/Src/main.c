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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "printf.h"
#include "motor.hpp"
#include "Chassis.hpp"
#include "Steer.hpp"
#include "search.hpp"
#include "gyro.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int myAtoi(char* str)
{
    // Initialize result
    int res = 0;
 
    for (int i = 0; str[i] <= '9' && str[i] >= '0'; ++i)
        res = res * 10 + str[i] - '0';
 
    // return result.
    return res;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

Class_Chassis car;
Class_Steer   claw;

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
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_Init();
  MX_TIM5_Init();
  MX_UART8_Init();
  MX_TIM8_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_TIM1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  car.Init();
  car.Set_Control_Method(Control_Method_OMEGA);
  car.Set_DR16(false);

  claw.Init(&htim8, TIM_CHANNEL_1);

  HAL_TIM_Base_Start_IT(&htim3);
  // gyro_init();

  actuator_up();
  // HAL_Delay(ACTUATOR_HAL_DELAY);
  // actuator_stop();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  SpeedTypeDef vel;
  vel.Omega = 0; vel.X = 0; vel.Y = 0;

/*
  // 检查巡线模块
  while (1) {
    if (RR1_IN == 0) {
      vel.X = 0.3;
    } else {
      vel.X = 0;
    }
    car.Set_Velocity(vel);
  }
*/

// /* // 调整 nudges
#ifdef ADJUST
  claw.open();
  actuator_down();
  HAL_Delay(ACTUATOR_HAL_DELAY);
  actuator_stop();
  claw.close();
  actuator_up();
  HAL_Delay(ACTUATOR_HAL_DELAY);
  actuator_stop();
  HAL_Delay(1000);
  if (0) Nudge1();
  else   Nudge2();
  actuator_down();
  HAL_Delay(ACTUATOR_HAL_DELAY);
  actuator_stop();
  claw.open();
  actuator_up();
  HAL_Delay(ACTUATOR_HAL_DELAY);
  actuator_stop();
  HAL_Delay(500);
  HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_SET);
  HAL_Delay(700);
  HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_RESET);
  while (1);
#endif

  // 正式比赛流程
  Stage1();
  Stage2();
  Stage3();
  Stage4();
  while (1);
  // */

  while (1)
  {
    if (0) {
      vel.Omega = 3.0f; vel.X = 0; vel.Y = 0;
      car.Set_Velocity(vel); HAL_Delay(1200);
      vel.Omega = 0; vel.X = 0; vel.Y = 0;
      car.Set_Velocity(vel); HAL_Delay(1000);
      vel.Omega = 0.0f; vel.X = 2; vel.Y = 0;
      car.Set_Velocity(vel); HAL_Delay(1200);
      vel.Omega = 0; vel.X = 0; vel.Y = 0;
      car.Set_Velocity(vel); HAL_Delay(1000);
      vel.Omega = 0.0f; vel.X = 0; vel.Y = 0.5;
      car.Set_Velocity(vel); HAL_Delay(1200);
      vel.Omega = 0; vel.X = 0; vel.Y = 0;
      car.Set_Velocity(vel); HAL_Delay(1000);
    } else if (0) {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (i == j)
            car.Motor[j].Set_Omega_Target(5);
          else
            car.Motor[j].Set_Omega_Target(0);
          car.Motor[j].Output();
        }
        HAL_Delay(4000);
      }
    }
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
    default:
      car.Hall_Encoder_GPIO_EXTI_Callback(GPIO_Pin);  // forward
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  // htim3 is used to trigger PID recalculation
  if (htim == &htim3) {
    car.Calculate_TIM_PeriodElapsedCallback();
    // gyro_callback();
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart8)
  {
    car.DR16.Alive_UART_RxCpltCallback();
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
