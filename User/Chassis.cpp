/**
 * @file Chassis.cpp
 * @author wtm
 * @brief 底盘控制
 * @version 0.1
 * @date 2022-07-25
 *
 * @copyright Copyright (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "Chassis.hpp"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/**
 * @brief 限幅函数
 *
 * @tparam Type
 * @param x 传入数据
 * @param Min 最小值
 * @param Max 最大值
 */
template <typename Type>
void Math_Constrain(Type *x, Type Min, Type Max) {
    if (*x < Min) {
        *x = Min;
    } else if (*x > Max) {
        *x = Max;
    }
}

/**
 * @brief 求绝对值
 *
 * @tparam Type
 * @param x 传入数据
 * @return Type x的绝对值
 */
template <typename Type>
Type Math_Abs(Type x) {
    return((x > 0) ? x : -x);
}

/**
 * @brief 初始化底盘
 *
 * @param __Driver_PWM_TIM 电机驱动定时器编号
 * @param __Calculate_EXTI_TIM 速度计算定时器编号
 */
void Class_Chassis::Init()
{
    //电机初始化

    Motor[0].Init(
        &htim5, TIM_CHANNEL_1, TIM_CHANNEL_2,
        &htim3, m1_yellow_Pin, m1_yellow_GPIO_Port, m1_green_Pin, m1_green_GPIO_Port);

    Motor[1].Init(
        &htim5, TIM_CHANNEL_3, TIM_CHANNEL_4,
        &htim3, m2_yellow_Pin, m2_yellow_GPIO_Port, m2_green_Pin, m2_green_GPIO_Port);

    Motor[2].Init(
        &htim4, TIM_CHANNEL_1, TIM_CHANNEL_2,
        &htim3, m3_yellow_Pin, m3_yellow_GPIO_Port, m3_green_Pin, m3_green_GPIO_Port);

    Motor[3].Init(
        &htim4, TIM_CHANNEL_3, TIM_CHANNEL_4,
        &htim3, m4_yellow_Pin, m4_yellow_GPIO_Port, m4_green_Pin, m4_green_GPIO_Port);

    Motor[0].Set_Rotate_Direction_Flag(CCW);
    Motor[1].Set_Rotate_Direction_Flag(CCW);
    Motor[2].Set_Rotate_Direction_Flag( CW);
    Motor[3].Set_Rotate_Direction_Flag( CW);

    //电机PID初始化
    for(int i = 0; i < 4; i++) {
        // Motor[i].Omega_PID.Init(omega_kp, omega_ki, omega_kd, 10000, 10000);
        Motor[i].Omega_PID.Init(omega_kp, omega_ki, omega_kd, omega_I_outmax, omega_outmax);
        Motor[i].Set_Control_Method(Control_Method_OMEGA);
    }
}

/**
 * @brief 设定底盘速度
 *
 * @param __Velocity 底盘速度
 */
void Class_Chassis::Set_Velocity(SpeedTypeDef __Velocity)
{
    Velocity = __Velocity;
}

/**
 * @brief 设定底盘控制方式
 *
 * @param __Control_Method 底盘控制方式
 */
void Class_Chassis::Set_Control_Method(Enum_Control_Method __Control_Method)
{
    Control_Method = __Control_Method;

    for(int i = 0; i < 4; i++) {
        Motor[i].Set_Control_Method(__Control_Method);
    }
}

/**
 * @brief 底盘电机霍尔编码器触发中断处理函数
 *
 */
void Class_Chassis::Hall_Encoder_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    //判断中断源, 并指向电机的中断处理函数
    switch (GPIO_Pin) {
        case m1_yellow_Pin: {
            Motor[0].Hall_Encoder_GPIO_EXTI_Callback();
            break;
        }
        case m2_yellow_Pin: {
            Motor[1].Hall_Encoder_GPIO_EXTI_Callback();
            break;
        }
        case m3_yellow_Pin: {
            Motor[2].Hall_Encoder_GPIO_EXTI_Callback();
            break;
        }
        case m4_yellow_Pin: {
            Motor[3].Hall_Encoder_GPIO_EXTI_Callback();
            break;
        }
        default: break;
    }
}

/**
 * @brief 定时器中断处理函数
 *
 */
#include "usart.h"
#include "printf.h"
void Class_Chassis::Calculate_TIM_PeriodElapsedCallback()
{
    Math_Constrain(&Velocity.X, -X_MAX, X_MAX);
    Math_Constrain(&Velocity.Y, -Y_MAX, Y_MAX);
    Math_Constrain(&Velocity.Omega, -OMEGA_MAX, OMEGA_MAX);

    //速度解算
    /*
    Motor[0].Set_Omega_Target((-OMEGA_TO_MS * Velocity.Omega + Velocity.X - Velocity.Y) / WHEEL_RADIUS * ((Motor[0].Get_Rotate_Direction_Flag() == CW)?1:(-1)));
    Motor[1].Set_Omega_Target((-OMEGA_TO_MS * Velocity.Omega - Velocity.X - Velocity.Y) / WHEEL_RADIUS * ((Motor[1].Get_Rotate_Direction_Flag() == CW)?1:(-1)));
    Motor[2].Set_Omega_Target((OMEGA_TO_MS * Velocity.Omega + Velocity.X - Velocity.Y) / WHEEL_RADIUS * ((Motor[2].Get_Rotate_Direction_Flag() == CW)?1:(-1)));
    Motor[3].Set_Omega_Target((OMEGA_TO_MS * Velocity.Omega - Velocity.X - Velocity.Y) / WHEEL_RADIUS * ((Motor[3].Get_Rotate_Direction_Flag() == CW)?1:(-1)));
    */
    float velRot = Velocity.Omega * OMEGA_TO_LINEAR / WHEEL_RADIUS;
    float velr   = (Velocity.Y + Velocity.X) * K_LINEAR_COSINE / WHEEL_RADIUS;
    float vell   = (Velocity.Y - Velocity.X) * K_LINEAR_COSINE / WHEEL_RADIUS;
    Motor[0].Set_Omega_Target(velr - velRot);
    Motor[1].Set_Omega_Target(vell - velRot);
    Motor[2].Set_Omega_Target(velr + velRot);
    Motor[3].Set_Omega_Target(vell + velRot);

    //电机输出值设定并输出
    char buf[40];
    for(int i = 0; i < 4; i++) {
        Motor[i].Calculate_TIM_PeriodElapsedCallback();
        Motor[i].Output();
    }
    int len = sprintf(buf, "%d,%d,%d,%d\n", Motor[0].Hall_Encoder_Count, Motor[1].Hall_Encoder_Count, Motor[2].Hall_Encoder_Count, Motor[3].Hall_Encoder_Count);
    HAL_USART_Transmit(&husart1, (const uint8_t *)buf, len, 1000);
}

/* Function prototypes -------------------------------------------------------*/

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
