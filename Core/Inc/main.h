/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "default_includes.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define m1_yellow_Pin GPIO_PIN_1
#define m1_yellow_GPIO_Port GPIOC
#define m1_yellow_EXTI_IRQn EXTI1_IRQn
#define m2_yellow_Pin GPIO_PIN_6
#define m2_yellow_GPIO_Port GPIOA
#define m2_yellow_EXTI_IRQn EXTI9_5_IRQn
#define m1_green_Pin GPIO_PIN_4
#define m1_green_GPIO_Port GPIOC
#define m1_pwm_Pin GPIO_PIN_10
#define m1_pwm_GPIO_Port GPIOH
#define m1r_pwm_Pin GPIO_PIN_11
#define m1r_pwm_GPIO_Port GPIOH
#define m2_pwm_Pin GPIO_PIN_12
#define m2_pwm_GPIO_Port GPIOH
#define m3_yellow_Pin GPIO_PIN_13
#define m3_yellow_GPIO_Port GPIOB
#define m3_yellow_EXTI_IRQn EXTI15_10_IRQn
#define m3_green_Pin GPIO_PIN_14
#define m3_green_GPIO_Port GPIOB
#define m4_green_Pin GPIO_PIN_6
#define m4_green_GPIO_Port GPIOC
#define m4_yellow_Pin GPIO_PIN_7
#define m4_yellow_GPIO_Port GPIOC
#define m4_yellow_EXTI_IRQn EXTI9_5_IRQn
#define m2r_pwm_Pin GPIO_PIN_0
#define m2r_pwm_GPIO_Port GPIOI
#define m2_green_Pin GPIO_PIN_5
#define m2_green_GPIO_Port GPIOB
#define m3_pwm_Pin GPIO_PIN_6
#define m3_pwm_GPIO_Port GPIOB
#define m3r_pwm_Pin GPIO_PIN_7
#define m3r_pwm_GPIO_Port GPIOB
#define m4_pwm_Pin GPIO_PIN_8
#define m4_pwm_GPIO_Port GPIOB
#define m4r_pwm_Pin GPIO_PIN_9
#define m4r_pwm_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
