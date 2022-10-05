/**
 * @file Chassis.hpp
 * @author wtm
 * @brief 底盘控制
 * @version 0.1
 * @date 2022-07-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CHASSIS_HPP
#define CHASSIS_HPP

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "Motor.hpp"
#include "DR16.hpp"
#include "Steer.hpp"

/* Exported macros -----------------------------------------------------------*/

//轮半径, m
const float WHEEL_RADIUS = 0.0635f;

//轮组满转线速度, m/s
const float WHEEL_FULL_V = (MOTOR_FULL_OMEGA * WHEEL_RADIUS);

//车的(1/2)(长边长、短边长、对角线长)
const float CHASSIS_A = 0.25f;
const float CHASSIS_B = 0.20f;
const float CHASSIS_R = 0.3201562f;

//45度角 1/sqrt(2)
const float K_LINEAR_COSINE = 0.707107f;

//旋转的速度分解因子
// k = cos(pi/4 - arctan(b/a)), a>b
// consider k ~ 1
const float K_OMEGA_COSINE = 0.993884f;

//omega到轮子线速度映射系数, m/rad
//v_spin = k * sqrt(a^2 + b^2) * omega
const float OMEGA_TO_LINEAR = (K_OMEGA_COSINE * CHASSIS_R);

//绝对速度限制, 任何情况不能超过
//横移速度上限, m/s
const float X_MAX = 1.0f;
//前进速度上限, m/s
const float Y_MAX = 1.0f;
//旋转速度上限, rad/s
const float OMEGA_MAX = 2.0f;

/* Exported types ------------------------------------------------------------*/

class Class_Chassis
{
    public:

        /**
         * 底盘对应的电机
         * 按照 Y+ 方向设置正方向
         *
         * M[0]  O-------O  M[3]
         *       |   ^   |
         * M[1]  O-------O  M[2]
         */
        Class_Motor_With_Hall_Encoder Motor[4];
        //底盘对应的遥控器
        Class_DR16 DR16;

        // 遥控器可以控制shooter
        Class_Motor *Shooter;
        Class_Steer *claw;

        void Init();        // arguments implicitly contained in the function

        void Set_Velocity(SpeedTypeDef __Velocity);
        SpeedTypeDef Get_Velocity();
        void Set_Control_Method(Enum_Control_Method __Control_Method);
        void Set_DR16(bool active);

        void Hall_Encoder_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
        void Calculate_TIM_PeriodElapsedCallback();

    protected:

        bool use_dr16 = false;

        //电机PID控制方式
        Enum_Control_Method Control_Method = Control_Method_OMEGA;

        //底盘速度
        SpeedTypeDef Velocity =
        {
            0.0f,
            0.0f,
            0.0f
        };

};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
