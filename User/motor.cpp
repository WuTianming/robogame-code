#include "motor.hpp"

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
 * @brief 初始化电机
 *
 * @param en 正向(r)使能GPIO引脚组
 * @param enpin 正向(r)使能GPIO引脚号
 * @param ren 反向(l)使能GPIO引脚组
 * @param renpin 反向(l)使能GPIO引脚号
 * @param htim 电机驱动定时器编号（指针）
 * @param pwmchn 正向(r)驱动定时器通道
 * @param rpwmchn 反向(l)驱动定时器通道
 * @param __Calculate_TIM 速度计算定时器编号
 * @param __Input_A_GPIO_Pin 霍尔编码器方向A引脚号
 * @param __Input_A_GPIOx 霍尔编码器方向A引脚组
 * @param __Input_B_GPIO_Pin 霍尔编码器方向B引脚号
 * @param __Input_B_GPIOx 霍尔编码器方向B引脚组
 */
void Class_Motor::Init(TIM_HandleTypeDef *htim, uint32_t pwmchn, uint32_t rpwmchn) {
    this->htim = htim;
    this->pwmchn = pwmchn;
    this->rpwmchn = rpwmchn;

    this->Out = 0;
    HAL_TIM_PWM_Start(htim, pwmchn);
    HAL_TIM_PWM_Start(htim, rpwmchn);
}

/**
 * @brief 设定电机正向旋转方向
 *
 * @param __Rotate_Direction_Flag 电机正向旋转方向
 */
void Class_Motor::Set_Rotate_Direction_Flag(Enum_Rotate_Direction __Rotate_Direction_Flag) {
    Rotate_Direction_Flag = __Rotate_Direction_Flag;
}

/**
 * @brief 设定电机PWM满占空比对应的数值
 *
 * @param __Motor_Full_Omega 电机PWM满占空比对应的数值
 */
void Class_Motor::Set_Motor_Full_Omega(float __Motor_Full_Omega) {
    Motor_Full_Omega = __Motor_Full_Omega;
}

/**
 * @brief 设定电机PWM满占空比对应的数值
 *
 * @param __Motor_PWM_Period 电机PWM满占空比对应的数值
 */
void Class_Motor::Set_Motor_PWM_Period(int32_t __Motor_PWM_Period) {
    Motor_PWM_Period = __Motor_PWM_Period;
}

/**
 * @brief 设定电机目标输出强度, 即电机PWM占空比的分子
 *
 * @param __Out 电机目标输出强度, 即电机PWM占空比的分子
 */
void Class_Motor::Set_Out(int32_t __Out) {
    Out = __Out;
}

/**
 * @brief 获取电机正向旋转方向
 *
 * @return Enum_Rotate_Direction 电机正向旋转方向
 */
Enum_Rotate_Direction Class_Motor::Get_Rotate_Direction_Flag() {
    return(Rotate_Direction_Flag);
}

/**
 * @brief 获取电机减速后满转转速, rad/s
 *
 * @return float 电机减速后满转转速, rad/s
 */
float Class_Motor::Get_Motor_Full_Omega() {
    return(Motor_Full_Omega);
}

/**
 * @brief 获取电机PWM满占空比对应的数值
 *
 * @return int32_t 电机PWM满占空比对应的数值
 */
int32_t Class_Motor::Get_Motor_PWM_Period() {
    return(Motor_PWM_Period);
}

/**
 * @brief 获取电机目标输出强度, 即电机PWM占空比的分子
 *
 * @return int32_t 电机目标输出强度, 即电机PWM占空比的分子
 */
int32_t Class_Motor::Get_Out() {
    return(Out);
}

/**
 * @brief 设定电机占空比, 确定输出
 *
 */
void Class_Motor::Output() {
    int32_t real_out = (Rotate_Direction_Flag == CW) ? -Out : Out;
    if (real_out >= 0) {
        __HAL_TIM_SetCompare(htim, rpwmchn, 0);
        __HAL_TIM_SetCompare(htim, pwmchn, real_out);
    } else if (real_out < 0) {
        __HAL_TIM_SetCompare(htim, pwmchn, 0);
        __HAL_TIM_SetCompare(htim, rpwmchn, -real_out);
    }
}

/**
 * @brief 初始化电机
 *
 * @param en 正向(r)使能GPIO引脚组
 * @param enpin 正向(r)使能GPIO引脚号
 * @param ren 反向(l)使能GPIO引脚组
 * @param renpin 反向(l)使能GPIO引脚号
 * @param htim 电机驱动定时器编号（指针）
 * @param pwmchn 正向(r)驱动定时器通道
 * @param rpwmchn 反向(l)驱动定时器通道
 * @param __Calculate_TIM 速度计算定时器编号
 * @param __Input_A_GPIO_Pin 霍尔编码器方向A引脚号
 * @param __Input_A_GPIOx 霍尔编码器方向A引脚组
 * @param __Input_B_GPIO_Pin 霍尔编码器方向B引脚号
 * @param __Input_B_GPIOx 霍尔编码器方向B引脚组
 */
void Class_Motor_With_Hall_Encoder::Init(TIM_HandleTypeDef *htim, uint32_t pwmchn, uint32_t rpwmchn, TIM_HandleTypeDef *__Calculate_TIM, uint16_t __Input_A_GPIO_Pin, GPIO_TypeDef *__Input_A_GPIOx, uint16_t __Input_B_GPIO_Pin, GPIO_TypeDef *__Input_B_GPIOx)
{
    this->htim = htim;
    this->pwmchn = pwmchn;
    this->rpwmchn = rpwmchn;

    Calculate_TIM = __Calculate_TIM;
    Input_A_GPIO_Pin = __Input_A_GPIO_Pin;
    Input_A_GPIOx = __Input_A_GPIOx;
    Input_B_GPIO_Pin = __Input_B_GPIO_Pin;
    Input_B_GPIOx = __Input_B_GPIOx;

    this->Out = 0;
    HAL_TIM_PWM_Start(htim, pwmchn);
    HAL_TIM_PWM_Start(htim, rpwmchn);
}

/**
 * @brief 设定电机控制方式
 *
 * @param Control_Method 电机控制方式
 */
void Class_Motor_With_Hall_Encoder::Set_Control_Method(Enum_Control_Method __Control_Method)
{
    Control_Method = __Control_Method;
}

/**
 * @brief 设定目标电机转速, rad/s
 *
 * @param Omega_Target 目标电机转速, rad/s
 */
void Class_Motor_With_Hall_Encoder::Set_Omega_Target(float __Omega_Target)
{
    Omega_Target = __Omega_Target;
}

/**
 * @brief 设定目标电机角度, rad
 *
 * @param Angle_Target 目标电机角度, rad
 */
void Class_Motor_With_Hall_Encoder::Set_Angle_Target(float __Angle_Target)
{
    Angle_Target = __Angle_Target;
}

/**
 * @brief 设定电机控制方式
 *
 * @return Enum_Control_Method 电机控制方式
 */
Enum_Control_Method Class_Motor_With_Hall_Encoder::Get_Control_Method()
{
    return(Control_Method);
}

/**
 * @brief 设定当前电机转速, rad/s
 *
 * @return float 当前电机转速, rad/s
 */
float Class_Motor_With_Hall_Encoder::Get_Omega_Now()
{
    return(Omega_Now);
}

/**
 * @brief 设定当前电机角度, rad
 *
 * @return float 当前电机角度, rad
 */
float Class_Motor_With_Hall_Encoder::Get_Angle_Now()
{
    return(Angle_Now);
}

/**
 * @brief 霍尔编码器中断处理函数
 *
 */
void Class_Motor_With_Hall_Encoder::Hall_Encoder_GPIO_EXTI_Callback()
{
    //计算电机转过的编码数, 以便后续算出角度和速度
    if(((HAL_GPIO_ReadPin(Input_B_GPIOx, Input_B_GPIO_Pin) == 0) ^ (Rotate_Direction_Flag == CW)) == 0) {
        Hall_Encoder_Count++;
    } else {
        Hall_Encoder_Count--;
    }
}

/**
 * @brief 定时器中断处理函数
 *
 */
#include "printf.h"
#include "usart.h"
void Class_Motor_With_Hall_Encoder::Calculate_TIM_PeriodElapsedCallback()
{
    float delta; float filtered;
    static int32_t prevd=0, pprevd=0;
    static float prevfiltered=0, pprevfiltered=0;

    delta = Hall_Encoder_Count - Prev_Hall_Encoder_Count;
    //看一下滤波（wtm）
    // prevfiltered = 0.12019831 * prevfiltered + 0.43990085 * prevd + 0.43990085 * delta;
    // prevd = delta;
    // delta = prevfiltered;
    const float b[] = { 0.01978958, 0.03957917, 0.01978958 },
                a[] = { 1.56450399, -0.64366232 };
    if (0) {
        filtered = a[0] * prevfiltered + a[1] * pprevfiltered + b[0] * delta + b[1] * prevd + b[2] * pprevd;
        pprevfiltered = prevfiltered;
        prevfiltered = filtered;
        pprevd = prevd;
        prevd = delta;

        delta = filtered;
    }
    //计算角度
    Angle_Now += (float)delta / MOTOR_ENCODER_NUM_PER_RAD;
    //计算速度
    Omega_Now = (float)delta / MOTOR_ENCODER_NUM_PER_RAD / MOTOR_CALCULATE_PERIOD;
    // float Omega_unfiltered = (float)(Hall_Encoder_Count - Prev_Hall_Encoder_Count) / MOTOR_ENCODER_NUM_PER_RAD / MOTOR_CALCULATE_PERIOD;

    // char buf[40];
    // int len = sprintf(buf, "/*%.6f,%.6f", Omega_Now, Omega_Target);
    // HAL_USART_Transmit(&husart1, (const uint8_t *)buf, len, 1000);
    Prev_Hall_Encoder_Count = Hall_Encoder_Count;

    if(Control_Method == Control_Method_OPENLOOP) {
        //开环控制直接输出速度
        Out = Omega_Target * Motor_PWM_Period / Motor_Full_Omega;
    } else if(Control_Method == Control_Method_OMEGA) {
        //速度控制, 单环PID

        Omega_PID.Set_Now(Omega_Now);
        Omega_PID.Set_Target(Omega_Target);
        Omega_PID.Adjust_TIM_PeriodElapsedCallback();
        Out = Omega_PID.Get_Out();

    } else if(Control_Method == Control_Method_ANGLE) {
        //角度控制, 双环PID

        Angle_PID.Set_Target(Angle_Target);

        Angle_PID.Set_Now(Angle_Now);
        Angle_PID.Adjust_TIM_PeriodElapsedCallback();
        Omega_PID.Set_Target(Angle_PID.Get_Out());

        Omega_PID.Set_Now(Omega_Now);
        Omega_PID.Adjust_TIM_PeriodElapsedCallback();
        Out = Omega_PID.Get_Out();
    }
    // Math_Constrain(&Out, -MOTOR_CALCULATE_PRESCALER, MOTOR_CALCULATE_PRESCALER);
    if (Out < -MOTOR_CALCULATE_PRESCALER) {
        Out = -MOTOR_CALCULATE_PRESCALER;
    } else if (Out > MOTOR_CALCULATE_PRESCALER) {
        Out = MOTOR_CALCULATE_PRESCALER;
    }
    // len = sprintf(buf, ",%f,%f,%f*/\n", Out / 1000.0f, Omega_Now, Omega_unfiltered);
    // HAL_USART_Transmit(&husart1, (const uint8_t *)buf, len, 1000);
}

/*
void motor_setdetach(Motor *M, bool detach) {
    // when detached, the wheel spins freely
    HAL_GPIO_WritePin(M->en, M->enpin, detach ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(M->ren, M->renpin, detach ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
*/