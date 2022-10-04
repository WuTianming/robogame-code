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
#include "csb.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ACTUATOR_HAL_DELAY 2300

void actuator_up() {
  HAL_GPIO_WritePin(actuator1_GPIO_Port, actuator1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(actuator2_GPIO_Port, actuator2_Pin, GPIO_PIN_SET);
}

void actuator_down() {
  HAL_GPIO_WritePin(actuator1_GPIO_Port, actuator1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(actuator2_GPIO_Port, actuator2_Pin, GPIO_PIN_RESET);
}

void actuator_stop() {
  HAL_GPIO_WritePin(actuator1_GPIO_Port, actuator1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(actuator2_GPIO_Port, actuator2_Pin, GPIO_PIN_RESET);
}
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

Class_Chassis car;
Class_Steer   claw;
Class_Motor   shooter;

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
  /* USER CODE BEGIN 2 */
  car.Init();
  // car.Set_Control_Method(Control_Method_OPENLOOP);
  car.Set_Control_Method(Control_Method_OMEGA);
  car.Set_DR16(true);
  claw.Init(&htim8, TIM_CHANNEL_1);
  shooter.Init(&htim8, TIM_CHANNEL_3, TIM_CHANNEL_4);
  shooter.Set_Motor_PWM_Period(10000);
  shooter.Set_Rotate_Direction_Flag(CCW);
  HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  SpeedTypeDef vel;
  vel.Omega = 0; vel.X = 0; vel.Y = 0;
  // while (1);

if(0)
  while (1) {
    if (RR1_IN == 0) {
      vel.X = 0.3;
    } else {
      vel.X = 0;
    }
    car.Set_Velocity(vel);
  }

  // while (1) {}

  // while (true) {
  //   Run2();
  // }

  /*
  // 发射冰壶
  actuator_up();
  HAL_Delay(ACTUATOR_HAL_DELAY * 1.5);
  actuator_stop();
  while (1) {
    // shooter.Set_Out(6000);
    // shooter.Output();
    // HAL_Delay(1000);
    // HAL_Delay(1000);
    HAL_Delay(50);
    shooter.Set_Out(9999);
    shooter.Output();
    HAL_Delay(300);
    shooter.Set_Out(0);
    shooter.Output();
    while (1);

    shooter.Set_Out(-6000);
    shooter.Output();
    // HAL_Delay(1000);
    // HAL_Delay(1000);
    HAL_Delay(300);
  }
  */

  // 走黑线
  Run12();
  while (1);

/*
  // 测试升降
  while (1) {
    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY);
    actuator_stop();
    while (1);

    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY / 2);
    actuator_stop();

    HAL_Delay(300);
    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY / 2);
  }
  */

/*
  Run12();

  vel.Y = 0.3;
  car.Set_Velocity(vel);
  while (1) {
    HAL_Delay(1000);
    vel.Y = 0.0;
    vel.Omega = 0.3;
    car.Set_Velocity(vel);
    HAL_Delay(1000);
    vel.Y = 0.3;
    car.Set_Velocity(vel);
  }
*/

/*
  while (1) {
    const char *s = "heartbeartttt\n";
    HAL_USART_Transmit(&husart1, (const uint8_t *)s, sizeof("heartbeartttt\n"), 100);
    char ss[40];
    int len = sprintf(ss, "%d\n", csb_get_distance());
    HAL_USART_Transmit(&husart1, (const uint8_t *)ss, len, 100);
  }
  */

  /*
  // 平地抓壶
  while (1) {
    claw.open();
    HAL_Delay(1000);
    claw.close();
    HAL_Delay(1000);
    actuator_up();
    HAL_Delay(ACTUATOR_HAL_DELAY);
    actuator_stop();
    HAL_Delay(ACTUATOR_HAL_DELAY / 2);
    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY);
  }
  */

  /*
  // 高度抓壶
  actuator_up();
  HAL_Delay(ACTUATOR_HAL_DELAY);
  claw.open();
  HAL_Delay(ACTUATOR_HAL_DELAY / 4);
  while (1) {
    claw.open();
    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY / 2 + ACTUATOR_HAL_DELAY / 5);
    actuator_stop();
    claw.close();
    HAL_Delay(500);
    actuator_up();
    HAL_Delay(ACTUATOR_HAL_DELAY / 2 + ACTUATOR_HAL_DELAY / 3);
    actuator_stop();
    HAL_Delay(500);
    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY / 2 + ACTUATOR_HAL_DELAY / 5);
    actuator_stop();
    claw.open();
    actuator_up();
    HAL_Delay(ACTUATOR_HAL_DELAY / 2 + ACTUATOR_HAL_DELAY / 3);
    actuator_stop();
  }
  */

  while (1)
  {
    vel.Omega = 0.3f;
    vel.X = 0;
    vel.Y = 0;
    car.Set_Velocity(vel);
    HAL_Delay(1200);
    vel.Omega = 0;
    vel.X = 0;
    vel.Y = 0;
    car.Set_Velocity(vel);
    HAL_Delay(1000);
    vel.Omega = 0.0f;
    vel.X = 0.2;
    vel.Y = 0;
    car.Set_Velocity(vel);
    HAL_Delay(1200);
    vel.Omega = 0;
    vel.X = 0;
    vel.Y = 0;
    car.Set_Velocity(vel);
    HAL_Delay(1000);
    vel.Omega = 0.0f;
    vel.X = 0;
    vel.Y = 0.2;
    car.Set_Velocity(vel);
    HAL_Delay(1200);
    vel.Omega = 0;
    vel.X = 0;
    vel.Y = 0;
    car.Set_Velocity(vel);
    HAL_Delay(1000);
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
