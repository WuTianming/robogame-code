/**
 * @file search.h
 * @author lixiao (xiao_li@mail.ustc.edu.cn)
 * @brief
 * @version 0.1
 * @date 2022-07
 *
 * @copyright Copyright (c) 2022
 *
 */

 #ifndef _SEARCH_H_
 #define _SEARCH_H_

/* Includes -------------------------------------------------------------------*/
#include "gpio.h"
#include "Chassis.hpp"
#include "Steer.hpp"
#include "gyro.hpp"

/* Exported macros ------------------------------------------------------------*/
# define BLACK 1    //识别为黑线时高电平
# define WHITE 0    //识别为白线时低电平

// 红外巡线的五个输入
// 前面左边巡线模块
# define WL_1  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)
# define WL_2  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6)
# define WL_3  HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7)
// 前面右边巡线模块
# define WR_1  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
# define WR_2  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)
# define WR_3  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)

// 左边巡线模块
# define A_1   !HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_9)
# define A_2   !HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13)
# define A_3   !HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_3)

// 右边巡线模块
# define D_1   !HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5)
# define D_2   !HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)
# define D_3   !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)

// 后面巡线模块
# define S_1   HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_13)
# define S_2   HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_15)
# define S_3   HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_1)
# define S_4   HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_11)


// #define ACTUATOR_HAL_DELAY 2700
#define ACTUATOR_HAL_DELAY 3400

extern Class_Chassis car;
extern Class_Steer claw;
extern Class_Motor shooter;
void actuator_up();
void actuator_down();
void actuator_stop();

/* Exported types -------------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/

/* Exported function declarations----------------------------------------------*/

void Stage1();
void Stage2();
void Stage3();
void Stage4();

void Nudge1(float t = 1);
void Nudge2(float t = 1);
void GoPickupBack();
void backoffBack();
void TurnAtCrossing(int interval = 4000);
void NextLane(void);
void PrevLane(void);
void GoPickup();
void GoPutdown(int nudge = 0);
void backoff();
void Run1(void);
void GoForward(float);	//直行
void GoBackward(float);
void GoRight(float);
void GoLeft(float);
void RRotate(float);
void AdjustR(float);		//向右小角度调整
void AdjustL(float);		//向左小角度调整
void Stop(void);		//停止
void Run_Forward(void);

void Fix(void);


#endif
