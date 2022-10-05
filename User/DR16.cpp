/**
 * @file DR16.cpp
 * @author yssickjgd 1345578933@qq.com
 * @brief DR16遥控器
 * @version 0.1
 * @date 2022-05-26
 *
 * @copyright Copyright (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "DR16.hpp"

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
void Math_Constrain(Type *x, Type Min, Type Max)
{
    if (*x < Min)
    {
        *x = Min;
    }
    else if (*x > Max)
    {
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
Type Math_Abs(Type x)
{
    return((x > 0) ? x : -x);
}

/**
 * @brief 初始化遥控器
 *
 * @param __Control_UART 遥控器控制UART端口
 */
void Class_DR16::Init(UART_HandleTypeDef *huart)
{
    HAL_UART_Receive_DMA(huart, this->Pack, DR16_PACK_LENGTH);
}

/**
 * @brief
 *
 * @return float
 */
SpeedTypeDef Class_DR16::Get_Velocity()
{
    return(Velocity);
}

/**
 * @brief
 *
 * @return Enum_DR16_Control_Mode
 */
Enum_DR16_Control_Mode Class_DR16::Get_DR16_Control_Mode()
{
    return(DR16_Control_Mode);
}

int Class_DR16::Get_Shooter() {
    return shooter;
}

int Class_DR16::Get_Actuator() {
    return actuator;
}

/**
 * @brief
 *
 * @return uint32_t
 */
uint32_t Class_DR16::Get_Receive_Window()
{
    return(Receive_Window);
}

/**
 * @brief
 *
 * @return uint32_t
 */
uint32_t Class_DR16::Get_Pre_Receive_Window()
{
    return(Pre_Receive_Window);
}

/**
 * @brief 遥控器定时器计算
 *
 */
void Class_DR16::Process_TIM_PeriodElapsedCallback()
{
    //状态检测
    Data.Left_DIP  = static_cast<Enum_DR16_DIP_Status>(((Pack[5] >> 4) & 0x000C) >> 2);
    Data.Right_DIP = static_cast<Enum_DR16_DIP_Status>( (Pack[5] >> 4) & 0x0003 );
    Data.Right_X = ((int16_t)Pack[0] | ((int16_t)Pack[1] << 8)) & 0x07FF;
    // Data.Right_Y = (((int16_t)Pack[1] >> 3) | ((int16_t)Pack[2] << 5)) & 0x07FF;
    Data.Left_X = (((int16_t)Pack[2] >> 6) | ((int16_t)Pack[3] << 2) | ((int16_t)Pack[4] << 10)) & 0x07FF;
    Data.Left_Y = (((int16_t)Pack[4] >> 1) | ((int16_t)Pack[5] << 7)) & 0x07FF;

    if (Receive_Window < 10) {
        DR16_Control_Mode = DR16_CONTROL_MODE_ON;
        shooter = 0;
        actuator = 0;
    } else {
        if(Data.Left_DIP == DR16_DIP_STATUS_DOWN) {
            DR16_Control_Mode = DR16_CONTROL_MODE_OFF;              // 遥控器无效，转自动控制
        } else if(Data.Left_DIP == DR16_DIP_STATUS_MIDDLE) {
            DR16_Control_Mode = DR16_CONTROL_MODE_ON;               // 当遥控器有动作时才控制
        } else if(Data.Left_DIP == DR16_DIP_STATUS_UP) {
            DR16_Control_Mode = DR16_CONTROL_MODE_FORCE;            // 完全遥控器
        }

        if (Data.Right_DIP == DR16_DIP_STATUS_UP) {
            shooter = -1;
        } else if (Data.Right_DIP == DR16_DIP_STATUS_MIDDLE) {
            shooter = 0;
        } else if (Data.Right_DIP == DR16_DIP_STATUS_DOWN) {
            shooter = 1;
        }

    if (0)
        if ((signed)Data.Right_Y - 1024 > 300) {
            actuator = 1;
        } else if ((signed)Data.Right_Y - 1024 < -700) {
            actuator = -2;      // release
        } else if ((signed)Data.Right_Y - 1024 < -300) {
            actuator = -1;
        } else {
            actuator = 0;
        }
    }

    //速度确认
    if(DR16_Control_Mode == DR16_CONTROL_MODE_OFF || Receive_Window < 10) {
        Velocity.X = 0;
        Velocity.Y = 0;
        Velocity.Omega = 0;
    } else {
        Velocity.X = (Data.Left_X - 1024) / 660.0;
        Velocity.Y = (Data.Left_Y - 1024) / 660.0;
        Velocity.Omega = -(Data.Right_X - 1024) / 660.0;        // 右手螺旋向上
        if(Math_Abs(Velocity.X) <= (float)DR16_DEAD_ZONE) { Velocity.X = 0; }
        if(Math_Abs(Velocity.Y) <= (float)DR16_DEAD_ZONE) { Velocity.Y = 0; }
        if(Math_Abs(Velocity.Omega) <= (float)DR16_DEAD_ZONE) { Velocity.Omega = 0; }
    }
    Pre_Receive_Window = Receive_Window;
}

/**
 * @brief 保持活跃的UART自增
 *
 */
void Class_DR16::Alive_UART_RxCpltCallback()
{
    Receive_Window++;
}

/* Function prototypes -------------------------------------------------------*/

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
