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

/* Exported macros ------------------------------------------------------------*/
# define BLACK 0    //识别为黑线时低电平
# define WHITE 1    //识别为白线时高电平

# define CS00000    LL1_IN == 0 && L1_IN == 0 && M1_IN == 0 && R1_IN == 0 && RR1_IN == 0
# define CS00001    LL1_IN == 0 && L1_IN == 0 && M1_IN == 0 && R1_IN == 0 && RR1_IN == 1
# define CS00010    LL1_IN == 0 && L1_IN == 0 && M1_IN == 0 && R1_IN == 1 && RR1_IN == 0
# define CS00011    LL1_IN == 0 && L1_IN == 0 && M1_IN == 0 && R1_IN == 1 && RR1_IN == 1
# define CS00100    LL1_IN == 0 && L1_IN == 0 && M1_IN == 1 && R1_IN == 0 && RR1_IN == 0
# define CS00101    LL1_IN == 0 && L1_IN == 0 && M1_IN == 1 && R1_IN == 0 && RR1_IN == 1
# define CS00110    LL1_IN == 0 && L1_IN == 0 && M1_IN == 1 && R1_IN == 1 && RR1_IN == 0
# define CS00111    LL1_IN == 0 && L1_IN == 0 && M1_IN == 1 && R1_IN == 1 && RR1_IN == 1
# define CS01000    LL1_IN == 0 && L1_IN == 1 && M1_IN == 0 && R1_IN == 0 && RR1_IN == 0
# define CS01001    LL1_IN == 0 && L1_IN == 1 && M1_IN == 0 && R1_IN == 0 && RR1_IN == 1
# define CS01010    LL1_IN == 0 && L1_IN == 1 && M1_IN == 0 && R1_IN == 1 && RR1_IN == 0
# define CS01011    LL1_IN == 0 && L1_IN == 1 && M1_IN == 0 && R1_IN == 1 && RR1_IN == 1
# define CS01100    LL1_IN == 0 && L1_IN == 1 && M1_IN == 1 && R1_IN == 0 && RR1_IN == 0
# define CS01101    LL1_IN == 0 && L1_IN == 1 && M1_IN == 1 && R1_IN == 0 && RR1_IN == 1
# define CS01110    LL1_IN == 0 && L1_IN == 1 && M1_IN == 1 && R1_IN == 1 && RR1_IN == 0
# define CS01111    LL1_IN == 0 && L1_IN == 1 && M1_IN == 1 && R1_IN == 1 && RR1_IN == 1
# define CS10000    LL1_IN == 1 && L1_IN == 0 && M1_IN == 0 && R1_IN == 0 && RR1_IN == 0
# define CS10001    LL1_IN == 1 && L1_IN == 0 && M1_IN == 0 && R1_IN == 0 && RR1_IN == 1
# define CS10010    LL1_IN == 1 && L1_IN == 0 && M1_IN == 0 && R1_IN == 1 && RR1_IN == 0
# define CS10011    LL1_IN == 1 && L1_IN == 0 && M1_IN == 0 && R1_IN == 1 && RR1_IN == 1
# define CS10100    LL1_IN == 1 && L1_IN == 0 && M1_IN == 1 && R1_IN == 0 && RR1_IN == 0
# define CS10101    LL1_IN == 1 && L1_IN == 0 && M1_IN == 1 && R1_IN == 0 && RR1_IN == 1
# define CS10110    LL1_IN == 1 && L1_IN == 0 && M1_IN == 1 && R1_IN == 1 && RR1_IN == 0
# define CS10111    LL1_IN == 1 && L1_IN == 0 && M1_IN == 1 && R1_IN == 1 && RR1_IN == 1
# define CS11000    LL1_IN == 1 && L1_IN == 1 && M1_IN == 0 && R1_IN == 0 && RR1_IN == 0
# define CS11001    LL1_IN == 1 && L1_IN == 1 && M1_IN == 0 && R1_IN == 0 && RR1_IN == 1
# define CS11010    LL1_IN == 1 && L1_IN == 1 && M1_IN == 0 && R1_IN == 1 && RR1_IN == 0
# define CS11011    LL1_IN == 1 && L1_IN == 1 && M1_IN == 0 && R1_IN == 1 && RR1_IN == 1
# define CS11100    LL1_IN == 1 && L1_IN == 1 && M1_IN == 1 && R1_IN == 0 && RR1_IN == 0
# define CS11101    LL1_IN == 1 && L1_IN == 1 && M1_IN == 1 && R1_IN == 0 && RR1_IN == 1
# define CS11110    LL1_IN == 1 && L1_IN == 1 && M1_IN == 1 && R1_IN == 1 && RR1_IN == 0
# define CS11111    LL1_IN == 1 && L1_IN == 1 && M1_IN == 1 && R1_IN == 1 && RR1_IN == 1

# define CS_R00000  LL2_IN == 0 && L2_IN == 0 && M2_IN == 0 && R2_IN == 0 && RR2_IN == 0
# define CS_R00001  LL2_IN == 0 && L2_IN == 0 && M2_IN == 0 && R2_IN == 0 && RR2_IN == 1
# define CS_R00010  LL2_IN == 0 && L2_IN == 0 && M2_IN == 0 && R2_IN == 1 && RR2_IN == 0
# define CS_R00011  LL2_IN == 0 && L2_IN == 0 && M2_IN == 0 && R2_IN == 1 && RR2_IN == 1
# define CS_R00100  LL2_IN == 0 && L2_IN == 0 && M2_IN == 1 && R2_IN == 0 && RR2_IN == 0
# define CS_R00101  LL2_IN == 0 && L2_IN == 0 && M2_IN == 1 && R2_IN == 0 && RR2_IN == 1
# define CS_R00110  LL2_IN == 0 && L2_IN == 0 && M2_IN == 1 && R2_IN == 1 && RR2_IN == 0
# define CS_R00111  LL2_IN == 0 && L2_IN == 0 && M2_IN == 1 && R2_IN == 1 && RR2_IN == 1
# define CS_R01000  LL2_IN == 0 && L2_IN == 1 && M2_IN == 0 && R2_IN == 0 && RR2_IN == 0
# define CS_R01001  LL2_IN == 0 && L2_IN == 1 && M2_IN == 0 && R2_IN == 0 && RR2_IN == 1
# define CS_R01010  LL2_IN == 0 && L2_IN == 1 && M2_IN == 0 && R2_IN == 1 && RR2_IN == 0
# define CS_R01011  LL2_IN == 0 && L2_IN == 1 && M2_IN == 0 && R2_IN == 1 && RR2_IN == 1
# define CS_R01100  LL2_IN == 0 && L2_IN == 1 && M2_IN == 1 && R2_IN == 0 && RR2_IN == 0
# define CS_R01101  LL2_IN == 0 && L2_IN == 1 && M2_IN == 1 && R2_IN == 0 && RR2_IN == 1
# define CS_R01110  LL2_IN == 0 && L2_IN == 1 && M2_IN == 1 && R2_IN == 1 && RR2_IN == 0
# define CS_R01111  LL2_IN == 0 && L2_IN == 1 && M2_IN == 1 && R2_IN == 1 && RR2_IN == 1
# define CS_R10000  LL2_IN == 1 && L2_IN == 0 && M2_IN == 0 && R2_IN == 0 && RR2_IN == 0
# define CS_R10001  LL2_IN == 1 && L2_IN == 0 && M2_IN == 0 && R2_IN == 0 && RR2_IN == 1
# define CS_R10010  LL2_IN == 1 && L2_IN == 0 && M2_IN == 0 && R2_IN == 1 && RR2_IN == 0
# define CS_R10011  LL2_IN == 1 && L2_IN == 0 && M2_IN == 0 && R2_IN == 1 && RR2_IN == 1
# define CS_R10100  LL2_IN == 1 && L2_IN == 0 && M2_IN == 1 && R2_IN == 0 && RR2_IN == 0
# define CS_R10101  LL2_IN == 1 && L2_IN == 0 && M2_IN == 1 && R2_IN == 0 && RR2_IN == 1
# define CS_R10110  LL2_IN == 1 && L2_IN == 0 && M2_IN == 1 && R2_IN == 1 && RR2_IN == 0
# define CS_R10111  LL2_IN == 1 && L2_IN == 0 && M2_IN == 1 && R2_IN == 1 && RR2_IN == 1
# define CS_R11000  LL2_IN == 1 && L2_IN == 1 && M2_IN == 0 && R2_IN == 0 && RR2_IN == 0
# define CS_R11001  LL2_IN == 1 && L2_IN == 1 && M2_IN == 0 && R2_IN == 0 && RR2_IN == 1
# define CS_R11010  LL2_IN == 1 && L2_IN == 1 && M2_IN == 0 && R2_IN == 1 && RR2_IN == 0
# define CS_R11011  LL2_IN == 1 && L2_IN == 1 && M2_IN == 0 && R2_IN == 1 && RR2_IN == 1
# define CS_R11100  LL2_IN == 1 && L2_IN == 1 && M2_IN == 1 && R2_IN == 0 && RR2_IN == 0
# define CS_R11101  LL2_IN == 1 && L2_IN == 1 && M2_IN == 1 && R2_IN == 0 && RR2_IN == 1
# define CS_R11110  LL2_IN == 1 && L2_IN == 1 && M2_IN == 1 && R2_IN == 1 && RR2_IN == 0
# define CS_R11111  LL2_IN == 1 && L2_IN == 1 && M2_IN == 1 && R2_IN == 1 && RR2_IN == 1


//红外巡线的五个输入
# define LL1_IN  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)
# define L1_IN   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)
# define M1_IN   HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_9)
# define R1_IN   HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5)
# define RR1_IN  HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13)

# define LL2_IN  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)
# define L2_IN   HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)
# define M2_IN   HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)
# define R2_IN   HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6)
# define RR2_IN  HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7)

/* Exported types -------------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/

/* Exported function declarations----------------------------------------------*/

int Ranging(void);
void Run(void);     //小车总的运行模块
void Run12(void);
void Run1(void);    //阶段一，直线到第一个转弯
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
void AdjustR(void);		//向右小角度调整
void AdjustL(void);		//向左小角度调整
void Stop(void);		//停止
void TurnR90(void);		//向右转90度
void TurnL90(void);		//向左转90度
void Turn180(void);
void TurnOffTrailing(void);	//关闭红外巡线模块
void TurnOnRanging(void);	//打开激光测距模块

#endif
