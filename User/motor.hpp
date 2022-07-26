#ifndef _MOTOR_HPP_INCLUDED_

#include "default_includes.h"
#include "PID.hpp"

//默认PID参数，为特定的轮子调节得到
const float omega_kp = 400,
            omega_ki = 2000,
            omega_kd = 0,
            omega_I_outmax = 20000,
            omega_outmax = 20000;

//圆周率
const float PI = 3.1415927f;

//电机减速后满转转速, rad/s
// const float MOTOR_FULL_OMEGA = (190.0f / 60.0f * 2.0f * PI);
const float MOTOR_FULL_OMEGA = (440.0f / 60.0f * 2.0f * PI);

//电机减速后霍尔编码器编码数, /rad
//电机参数：减速比27，霍尔一圈17，似乎满速是440rpm
//ms用的电机的减速比更大，速度更小
// const float MOTOR_ENCODER_NUM_PER_RAD = (867.0f / 2.0f / PI);
const float MOTOR_ENCODER_NUM_PER_RAD = (27 * 17 / 2.0f / PI);

//电机PWM满占空比对应的数值
const int32_t MOTOR_CALCULATE_PRESCALER = 32767;

//计算定时器频率, s
//this is achieved by setting APB1 Timer = 84 MHz, then using TIM3 (prescaler=419, period=9999) for interrupts
const float MOTOR_CALCULATE_PERIOD = 0.05f;

/**
 * @brief 电机控制方式
 *
 */
enum Enum_Control_Method
{
    Control_Method_OPENLOOP = 0,        // 开环控制直接输出速度
    Control_Method_OMEGA,               // 速度控制, 单环PID
    Control_Method_ANGLE                // 角度控制, 双环PID
};

/**
 * @brief 电机旋转方向
 *
 */
enum Enum_Rotate_Direction {
    CW = 0,
    CCW
};

/*
 * the motor receives a voltage of (V - Vr)
 * where V  =  pwm_modulate(24 volts, pwm)
 *       Vr = -pwm_modulate(24 volts, rpwm)
 */
class Class_Motor {
    public:
        void Init(TIM_HandleTypeDef *htim, uint32_t pwmchn, uint32_t rpwmchn);

        void Set_Rotate_Direction_Flag(Enum_Rotate_Direction __Rotate_Direction_Flag);
        void Set_Motor_Full_Omega(float __Motor_Full_Omega);
        void Set_Motor_PWM_Period(int32_t __Motor_PWM_Period);
        void Set_Out(int32_t __Out);

        Enum_Rotate_Direction Get_Rotate_Direction_Flag();
        float Get_Motor_Full_Omega();
        int32_t Get_Motor_PWM_Period();
        int32_t Get_Out();

        void Output();

    // protected:
    public:
        TIM_HandleTypeDef *htim;
        uint32_t pwmchn, rpwmchn;

        //电机正向旋转方向
        //指的是，如果我让Omega或者Out为正值，那么电机会怎么转
        //接线的时候统一红正黑负，由代码负责决定PWM是否反向（r）
        Enum_Rotate_Direction Rotate_Direction_Flag = CW;
        //电机减速后满转转速, rad/s
        float Motor_Full_Omega = MOTOR_FULL_OMEGA;
        //电机PWM满占空比对应的数值
        int32_t Motor_PWM_Period = MOTOR_CALCULATE_PRESCALER;
        //电机目标输出强度, 即电机PWM占空比的分子
        volatile int32_t Out = 0;
};

class Class_Motor_With_Hall_Encoder : public Class_Motor {
    public:

        //电机速度环PID参数
        Class_PID Omega_PID;
        //电机角度环PID参数
        Class_PID Angle_PID;

        void Init(TIM_HandleTypeDef *htim, uint32_t pwmchn, uint32_t rpwmchn, TIM_HandleTypeDef *__Calculate_TIM, uint16_t __Input_A_GPIO_Pin, GPIO_TypeDef *__Input_A_GPIOx, uint16_t __Input_B_GPIO_Pin, GPIO_TypeDef *__Input_B_GPIOx);

        void Set_Control_Method(Enum_Control_Method Control_Method);
        void Set_Omega_Target(float Omega_Target);
        void Set_Angle_Target(float Angle_Target);

        Enum_Control_Method Get_Control_Method();
        float Get_Omega_Now();
        float Get_Angle_Now();

        void Hall_Encoder_GPIO_EXTI_Callback();
        void Calculate_TIM_PeriodElapsedCallback();

    // protected:
    public:

        //计算定时器编号
        TIM_HandleTypeDef *Calculate_TIM;
        //霍尔编码器方向A
        uint16_t Input_A_GPIO_Pin;
        GPIO_TypeDef *Input_A_GPIOx;
        //霍尔编码器方向B
        uint16_t Input_B_GPIO_Pin;
        GPIO_TypeDef *Input_B_GPIOx;

        //电机控制方式
        Enum_Control_Method Control_Method = Control_Method_OMEGA;
        //霍尔编码器计数
        int32_t Hall_Encoder_Count = 0;
        //霍尔编码器前一个计数
        int32_t Prev_Hall_Encoder_Count = 0;
        //当前电机转速, rad/s
        float Omega_Now = 0;
        //目标电机转速, rad/s
        float Omega_Target = 0;
        //当前电机角度, rad
        float Angle_Now = 0;
        //目标电机角度, rad
        float Angle_Target = 0;

};

#define _MOTOR_HPP_INCLUDED_

#endif