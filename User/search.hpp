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

/* Exported macros ------------------------------------------------------------*/
# define BLACK 0    //识别为黑线时低电平
# define WHITE 1    //识别为白线时高电平

// 红外巡线的五个输入
// 前面左边巡线模块
# define WL_1  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
# define WL_2  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)
# define WL_3   HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_9)
// 前面右边巡线模块
# define WR_1   HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5)
# define WR_2  HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13)
# define WR_3  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)

#define ACTUATOR_HAL_DELAY 2300

extern Class_Chassis car;
extern Class_Steer claw;
extern Class_Motor shooter;
void actuator_up();
void actuator_down();
void actuator_stop();

/* Exported types -------------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/

/* Exported function declarations----------------------------------------------*/

int Ranging(void);
void RunAll(void);
void RunAll_Moveleft(void);
void Run1(void);    //阶段一，直线到第一个转弯
void Run1Q(void);
void Run1L(void);
void Run2(void);    //阶段二，关闭正常巡线模式，通过激光测距确保直行
void Run3(void);    //阶段三，开启巡线模块，识别、投壶
int Recognize(void);   //识别
void Capture(void);  //抓取
void Cast(void);	//投掷
void GoForward(float);	//直行
void GoBackward(void);
void GoRight(float);
void GoLeft(void);
void RRotate(float);
void AdjustR(float);		//向右小角度调整
void AdjustL(float);		//向左小角度调整
void Stop(void);		//停止
void TurnR90(void);		//向右转90度
void TurnL90(void);		//向左转90度
void Turn180(void);
void TurnOffTrailing(void);	//关闭红外巡线模块
void TurnOnRanging(void);	//打开激光测距模块

#endif
