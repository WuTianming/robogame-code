/**
 * @file search.c
 * @author lixiao (xiao_li@mail.ustc.edu.cn)
 * @brief
 * @version 0.1
 * @date 2022-07
 *
 * @copyright Copyright (c) 2022
 */

/* Includes ------------------------------------------------------------------*/
#include "search.hpp"
#include "csb.hpp"
/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern Class_Chassis car;

/* Private function declarations----------------------------------------------*/

/* Funcion prototypes  -------------------------------------------------------*/

const float fb_vel = 0.4;
const float lr_vel = 0.4;
const float rvel   = 1;
const float rmicro = 0.4;

void actuator_up() {
  car.Set_Control_Method(Control_Method_OPENLOOP);
  HAL_GPIO_WritePin(actuator1_GPIO_Port, actuator1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(actuator2_GPIO_Port, actuator2_Pin, GPIO_PIN_SET);
}

void actuator_down() {
  car.Set_Control_Method(Control_Method_OPENLOOP);
  HAL_GPIO_WritePin(actuator1_GPIO_Port, actuator1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(actuator2_GPIO_Port, actuator2_Pin, GPIO_PIN_RESET);
}

void actuator_stop() {
  HAL_GPIO_WritePin(actuator1_GPIO_Port, actuator1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(actuator2_GPIO_Port, actuator2_Pin, GPIO_PIN_RESET);
  car.Set_Control_Method(Control_Method_OMEGA);
}

void GoForward(float v=fb_vel) {
    SpeedTypeDef vel;
    vel.Omega = 0;
    vel.Y = v;
    vel.X = 0.0;
    car.Set_Velocity(vel);
}

void GoBackward() {
    SpeedTypeDef vel;
    vel.Omega = 0;
    vel.Y = -fb_vel;
    vel.X = 0.0;
    car.Set_Velocity(vel);
}

void GoLeft() {
    SpeedTypeDef vel;
    vel.Omega = 0;
    vel.Y = 0.0;
    vel.X = -lr_vel;
    car.Set_Velocity(vel);
}

void GoRight(float vv = lr_vel) {
    SpeedTypeDef vel;
    vel.Omega = 0;
    vel.Y = 0.0;
    vel.X = vv;
    car.Set_Velocity(vel);
}

void RRotate(float vv = rvel) {
    SpeedTypeDef vel;
    vel.Omega = -vv;
    vel.Y = 0.0;
    vel.X = 0.0;
    car.Set_Velocity(vel);
}

void LRotate(float vv = rvel) {
    SpeedTypeDef vel;
    vel.Omega = vv;
    vel.Y = 0.0;
    vel.X = 0.0;
    car.Set_Velocity(vel);
}

void Stop() {
    SpeedTypeDef vel;
    vel.Omega = 0;
    vel.Y = 0.0;
    vel.X = 0.0;
    car.Set_Velocity(vel);
}

#define INCREMENTAL

void AdjustR(float o = rmicro) {
    SpeedTypeDef vel;
#ifdef INCREMENTAL
    vel = car.Get_Velocity();
    vel.Omega = -o;
#else
    vel.Omega = -o;
    vel.Y = 0.0;
    vel.X = 0.0;
#endif
    car.Set_Velocity(vel);
}

void AdjustL(float o = rmicro) {
    SpeedTypeDef vel;
#ifdef INCREMENTAL
    vel = car.Get_Velocity();
    vel.Omega = o;
#else
    vel.Omega = o;
    vel.Y = 0.0;
    vel.X = 0.0;
#endif
    car.Set_Velocity(vel);
}

void AdjustF() {
    ;
}

void AdjustB() {
    ;
}

int Recognize() {
    return 0;
}

void Run3_new()
{
    //突然想到一个点，可以抓一个壶的同时投一个壶，可以节省时间
    // notice the original status, the car's orientation
    int status[5],idx=0;
    int i=0,j=0;
    for(;idx<2;)
    {
        while(!(CS11001)||!(CS10001)||!(CS10011))
        {
            GoLeft();
        }
        status[idx] = Recognize(); idx++;
        while(!(CS00000))
        {
            GoLeft();
        }
    }
    for(i=0; i<2;)
    {
        GoLeft();
        HAL_Delay(300);
        if((CS11001)||(CS10001)||(CS10011)) i++;
    }
    Cast();

    if(status[1] == 1)
    {
        for(i=0; i<2;)
        {
            GoRight();
            HAL_Delay(300);
            if((CS11001)||(CS10001)||(CS10011)) i++;
        }
        Capture();
        for(i=0; i<2;)
        {
            GoLeft();
            HAL_Delay(300);
            if((CS11001)||(CS10001)||(CS10011)) i++;
        }
        Cast();
    }
    if(status[0] == 1)
    {
        for(i=0; i<3;)
        {
            GoRight();
            HAL_Delay(300);
            if((CS11001)||(CS10001)||(CS10011)) i++;
        }
        Capture();
        for(i=0; i<3;)
        {
            GoLeft();
            HAL_Delay(300);
            if((CS11001)||(CS10001)||(CS10011)) i++;
        }
        Cast();
    }
    
    // 开始检测台阶上的壶

    while(!(CS_R11001)||!(CS_R10001)||!(CS_R10011)) GoBackward();
    for(j=0; j<2;)
    {
        AdjustR();
        HAL_Delay(300);
        if((CS11001)||(CS10001)||(CS10011)) j++;
    }

    status[3] = Recognize();
    
    GoRight();
    HAL_Delay(300);
    while (!(CS11001)||!(CS10001)||!(CS10011)) GoRight();

    status[4] = Recognize();
    for(i=0; i<2;)
    {
        GoLeft();
        HAL_Delay(300);
        if((CS11001)||(CS10001)||(CS10011)) i++;
    }
    status[2] = Recognize();
    
    if(status[2])
    {
        Capture();
        GoRight();
        HAL_Delay(300);
        while (!(CS11001)||!(CS10001)||!(CS10011)) GoRight();
        AdjustR();
        HAL_Delay(300);
        while (!(CS11001)||!(CS10001)||!(CS10011)) AdjustR();
        Cast();

        while(!(CS_R11001)||!(CS_R10001)||!(CS_R10011)) GoBackward();
        for(j=0; j<2;)
        {
            AdjustR();
            HAL_Delay(300);
            if((CS11001)||(CS10001)||(CS10011)) j++;
        }
    }
    else
    {
        GoRight();
        HAL_Delay(300);
        while (!(CS11001)||!(CS10001)||!(CS10011)) GoRight();
    }
    

    if(status[3])
    {
        Capture();
        while(!(CS11001)||!(CS10001)||!(CS10011)) AdjustR();
        while(!(CS_R11001)||!(CS_R10001)||!(CS_R10011)) GoForward();
        Cast();

        while(!(CS_R11001)||!(CS_R10001)||!(CS_R10011)) GoBackward();
        for(j=0; j<2;)
        {
            AdjustR();
            HAL_Delay(300);
            if((CS11001)||(CS10001)||(CS10011)) j++;
        }
    }
    
    if(status[4])
    {
        GoRight();
        HAL_Delay(300);
        while (!(CS11001)||!(CS10001)||!(CS10011)) GoRight();
        Capture();
        GoLeft();
        HAL_Delay(300);
        while (!(CS11001)||!(CS10001)||!(CS10011)) GoLeft();
        AdjustR();
        HAL_Delay(300);
        while(!(CS11001)||!(CS10001)||!(CS10011)) AdjustR();
        Cast();
    }

}

#define ALONG_THE_LINE ((CS10011) || (CS11001) || (CS10001))
#define ALONG_THE_LINE1 (!M1_IN && (!L1_IN || !R1_IN))
// #define ALONG_THE_LINE_R ((CS_R10011) || (CS_R11001) || (CS_R10001))
#define ALONG_THE_LINE_R (!M2_IN && (!L2_IN || !R2_IN))

void RunAll_Moveleft() {
    // goto approaching_bottom_line;
    // goto go_along_bottom_line;
    // goto find_crossing_then_turn_right;
    // goto move_to_adjacent_position;

    // 先越过开始区的一坨黑线
    for (int i = 0; i < 100; i++) {
        Run1();
        HAL_Delay(10);
    }
    // 再走直线直到第一个转弯
    while (true) {
        Run1();
        if ((CS10000) || (CS00000)) { break; }
    }
    // 再开始往右平移
    // 比赛场地：5.85m
    GoRight(1.0);
    AdjustL(0.3);       // 左转对冲
    // HAL_Delay(5200);    // 实际走了3.6m
    HAL_Delay(100);
    GoRight(1.0);       // 取消左转对冲
    HAL_Delay(3800);
    GoRight(1.0);
    HAL_Delay(3900);    // 5.8m
    // GoRight(0.3);
    // while (LL2_IN + L2_IN + M2_IN + R2_IN + RR2_IN >= 1) {}

approaching_bottom_line:
    GoRight(0.5); while (!ALONG_THE_LINE1) {} Stop();

    // RRotate(3); HAL_Delay(2300);  // 180度
    LRotate(3); HAL_Delay(900);  // 90 度
    LRotate(0.5); while (!ALONG_THE_LINE_R) {} Stop();
    for (int i = 0; i < 300; i++) {
        Run1L(); HAL_Delay(5);
    }
    Stop();

go_along_bottom_line:
    while (true) { Run1L(); if (ALONG_THE_LINE1) { break; } }
    Stop(); HAL_Delay(100);     // first crossing
    // look at color
    HAL_Delay(1000);
    char str[15]; str[14] = '\0';
    HAL_UART_Receive(&huart4, (uint8_t *)str, 14, 1000);
    int i = 0;
    for (i = 0; i < 15; i++) { if (str[i] == 'x') break; }
    int cx = 0, cy = 0;
    for (i += 2; str[i] <= '9' && str[i] >= '0'; ++i) cx = cx * 10 + str[i] - '0';
    for (i += 4; str[i] <= '9' && str[i] >= '0'; ++i) cy = cy * 10 + str[i] - '0';
    if (cx != 0 || cy != 0) {
        GoForward();
        for (int i = 0; i < 300; i++) {
            Run1Q();
            HAL_Delay(5);
            // if (CS11111) break;
        }
        Stop();
        claw.open();
        actuator_down(); HAL_Delay(ACTUATOR_HAL_DELAY * 2); actuator_stop();
        claw.close();
        actuator_up(); HAL_Delay(ACTUATOR_HAL_DELAY * 2); actuator_stop();

        GoBackward(); while (!ALONG_THE_LINE_R); Stop();
        GoLeft(); HAL_Delay(500);
        while (true) { Run1L(); if (ALONG_THE_LINE1) { break; } }
        Stop(); HAL_Delay(100);     // 2nd crossing
        GoLeft(); HAL_Delay(500);
        while (true) { Run1L(); if (ALONG_THE_LINE1) { break; } }
        Stop(); HAL_Delay(100);     // 3rd crossing
        GoLeft(); HAL_Delay(500);
        while (true) { Run1L(); if (ALONG_THE_LINE1) { break; } }
        Stop(); HAL_Delay(100);     // 4th crossing

        GoForward();
        for (int i = 0; i < 300; i++) {
            Run1Q();
            HAL_Delay(5);
            // if (CS11111) break;
        }
        Stop();

        // TODO push

        actuator_down();
        HAL_Delay(ACTUATOR_HAL_DELAY * 2);
        actuator_stop();
        claw.open();
        actuator_up();
        HAL_Delay(ACTUATOR_HAL_DELAY * 2);
        actuator_stop();

        // TODO push

        while (1);
    }
    GoLeft(); HAL_Delay(200);

    while (true) { Run1L(); if (ALONG_THE_LINE1) { break; } }
    Stop(); HAL_Delay(100);     // 2nd crossing
    // look at color
    HAL_Delay(1000);
    str[14] = '\0';
    HAL_UART_Receive(&huart4, (uint8_t *)str, 14, 1000);
    i = 0; cx = 0, cy = 0;
    for (i = 0; i < 15; i++) { if (str[i] == 'x') break; }
    for (i += 2; str[i] <= '9' && str[i] >= '0'; ++i) cx = cx * 10 + str[i] - '0';
    for (i += 4; str[i] <= '9' && str[i] >= '0'; ++i) cy = cy * 10 + str[i] - '0';
    if (cx != 0 || cy != 0) {
        GoForward();
        for (int i = 0; i < 300; i++) {
            Run1Q();
            HAL_Delay(5);
        }
        Stop();
        claw.open();
        actuator_down(); HAL_Delay(ACTUATOR_HAL_DELAY * 2); actuator_stop();
        claw.close();
        actuator_up(); HAL_Delay(ACTUATOR_HAL_DELAY * 2); actuator_stop();

        GoBackward(); while (!ALONG_THE_LINE_R); Stop();
        GoLeft(); HAL_Delay(500);
        while (true) { Run1L(); if (ALONG_THE_LINE1) { break; } }
        Stop(); HAL_Delay(100);     // 3rd crossing
        GoLeft(); HAL_Delay(500);
        while (true) { Run1L(); if (ALONG_THE_LINE1) { break; } }
        Stop(); HAL_Delay(100);     // 4th crossing

        GoForward();
        for (int i = 0; i < 300; i++) {
            Run1Q();
            HAL_Delay(5);
            // if (CS11111) break;
        }
        Stop();

        // TODO push

        actuator_down();
        HAL_Delay(ACTUATOR_HAL_DELAY * 2);
        actuator_stop();
        claw.open();
        actuator_up();
        HAL_Delay(ACTUATOR_HAL_DELAY * 2);
        actuator_stop();

        // TODO push

        while (1);
    }
    GoLeft(); HAL_Delay(500);

    while (true) { Run1L(); if (ALONG_THE_LINE1) { break; } }
    Stop(); HAL_Delay(100);     // 3rd crossing
    while (1);
    GoLeft(); HAL_Delay(500);

    while (true) { Run1L(); if (ALONG_THE_LINE1) { break; } }
    Stop(); HAL_Delay(100);     // 4nd crossing, fetch
    while (1);
}

void RunAll() {
    goto go_along_bottom_line;
    // goto find_crossing_then_turn_right;
    // goto move_to_adjacent_position;

    // 先越过开始区的一坨黑线
    for (int i = 0; i < 100; i++) {
        Run1();
        HAL_Delay(10);
    }
    // 再走直线直到第一个转弯
    while (true) {
        Run1();
        if ((CS10000) || (CS00000)) { break; }
    }
    // 再开始往右平移
    // 比赛场地：5.85m
    GoRight(1.0);
    AdjustL(0.3);       // 左转对冲
    // HAL_Delay(5200);    // 实际走了3.6m
    HAL_Delay(100);
    GoRight(1.0);       // 取消左转对冲
    HAL_Delay(3800);
    GoRight(1.0);
    HAL_Delay(3900);    // 5.8m
    // GoRight(0.3);
    // while (LL2_IN + L2_IN + M2_IN + R2_IN + RR2_IN >= 1) {}

approaching_bottom_line:
    GoRight(0.5); while (!ALONG_THE_LINE) {} Stop();

    RRotate(3); HAL_Delay(2300);  // 180度
    RRotate(0.5); while (!ALONG_THE_LINE) {} Stop();
    for (int i = 0; i < 300; i++) {
        Run1(); HAL_Delay(5);
        if (CS11111) break;
    }
    Stop();

go_along_bottom_line:
    for (int i = 0; i < 100; i++) { Run1(); HAL_Delay(5); }

    while (true) { Run1(); if (ALONG_THE_LINE_R) { break; } }
    Stop(); HAL_Delay(100);     // first crossing

    // 右转面对冰壶，对准线
    GoBackward(); HAL_Delay(300); Stop();
    RRotate(3); HAL_Delay(1100);  // 90度
    RRotate(0.5); while (!ALONG_THE_LINE) {} Stop();    // 对准右边的线

    // 怼上去
    for (int i = 0; i < 300; i++) {
        Run1(); HAL_Delay(5);
        if (CS11111) break;
    }
    Stop();

    // 看一眼什么颜色

    // 然后退回去
    GoBackward(); while (!ALONG_THE_LINE_R) {} Stop();
    // GoBackward(); HAL_Delay(300); Stop();
    LRotate(3); HAL_Delay(1100);
    LRotate(0.5); while (!ALONG_THE_LINE) {} Stop();

    // 就当什么都没发生，继续往前走
    GoForward(); HAL_Delay(500);

    while (true) { Run1(); if (ALONG_THE_LINE_R) { break; } }
    Stop(); HAL_Delay(100);     // 2nd crossing
    GoForward(); HAL_Delay(500);

find_crossing_then_turn_right:
    while (true) { Run1(); if (ALONG_THE_LINE_R) { break; } }
    Stop(); HAL_Delay(100);     // 3rd crossing
    GoForward(); HAL_Delay(500);

    while (true) { Run1(); if (ALONG_THE_LINE_R) { break; } }
    Stop();             // 4th crossing, 己方冰壶

    // 右转面对冰壶，对准线
    // 这里后退一些再做旋转，因为旋转中心是玄学。
    GoBackward(); HAL_Delay(300); Stop();
    RRotate(3); HAL_Delay(1100);  // 90度
    RRotate(0.5); while (!ALONG_THE_LINE) {} Stop();

    for (int i = 0; i < 300; i++) {
        Run1(); HAL_Delay(5);
        if (CS11111) break;
    }
    Stop();

    // 对准冰壶之后，

    // LRotate(3);
    // HAL_Delay(50);
    // Stop();

    claw.open();
    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY*2);
    actuator_stop();
    claw.close();
    actuator_up();
    HAL_Delay(ACTUATOR_HAL_DELAY*2);
    actuator_stop();

move_to_adjacent_position:
/*
    GoLeft();
    HAL_Delay(1900);
    Stop();
    while (1);
*/
    GoBackward();
    HAL_Delay(1400);
    Stop();
    GoLeft();
    HAL_Delay(500);
    while (!ALONG_THE_LINE);
    // while (true) { Run1(); if (ALONG_THE_LINE_R) { break; } }
    for (int i = 0; i < 300; i++) {
        Run1Q(); HAL_Delay(5);
        if (CS11111) break;
    }
    Stop();
    // while (1);

    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY*2);
    actuator_stop();
    claw.open();
    while (1);

    // LRotate(3);
    // HAL_Delay(900);
    // Stop();
    // GoForward(0.7);
    // HAL_Delay(250);
    // Stop();
    // LRotate(0.5);
    // while (!ALONG_THE_LINE_R) {}
    // Stop();

    // GoLeft();
    // HAL_Delay(3000);
    // Stop();
}

void Run1()
{
    if((CS10011) || (CS11001) || (CS10001) || (CS11011)) {
        GoForward();
    } else if((CS11000) || (CS11100)) {
        AdjustR();
        HAL_Delay(20);
    } else if((CS00011) || (CS00111)) {
        AdjustL();
        HAL_Delay(20);
    }
}

void Run1Q()
{
    if((CS10011) || (CS11001) || (CS10001) || (CS11011)) {
        GoForward();
    } else if((CS11000) || (CS11100)) {
        AdjustR();
    } else if((CS00011) || (CS00111)) {
        AdjustL();
    }
}

void Run1L()
{
    if((CS_R10011) || (CS_R11001) || (CS_R10001) || (CS_R11011)) {
        GoLeft();
    } else if((CS_R11000) || (CS_R11100)) {
        AdjustR();
        HAL_Delay(40);
    } else if((CS_R00011) || (CS_R00111)) {
        AdjustL();
        HAL_Delay(40);
    }
}

int Ranging()
{
    // double csb module version
    uint32_t dist_mm1 = csb_get_distance();
    HAL_Delay(3);
    uint32_t dist_mm2 = csb2_get_distance();
    int delta = (signed)dist_mm1 - (signed)dist_mm2;
    if (dist_mm1 < 300 || dist_mm2 < 300) {
        return -1;
    } else if (dist_mm1 > 400 || dist_mm2 > 400) {
        return 1;
    }
    if (delta > 20) {
        return 1;
    } else if (delta < -20) {
        return -1;
    }

    /*
    GoRight();
    uint32_t dist_mm1 = csb_get_distance();
    TIM1_Delay_us(100 * 1000);          // 100 ms
    uint32_t dist_mm2 = csb_get_distance();
    if (dist_mm1 < dist_mm2) return 1;
    else return -1;
    */
    return 0;
}

void Run2()
{
    // GoRight();
    // return;
    int t = Ranging();
    if(t == 0) { GoRight(); HAL_Delay(200); }
    //Ranging()是第二阶段超声测距函数，用于比较车前后与挡板距离
    else if(t == 1) {
        AdjustR();//越来越远
        HAL_Delay(50);
    } else if(t == -1) {
        AdjustL();//反之
        HAL_Delay(50);
    }
}

void Run3()
{
    int count_all=0,count_yellow=0;
    int ismine=0;
    int status[5]={0,0,0,0,0};
    int i=0,j=0;
    for(;j < 2;j++)
    {
        /*
        while(!(CS10000))
        {
            Run1();
        }
        */

        while(!(CS_R10001))//增加一个右侧巡线模块
        {
            Run1();
        }
        TurnR90();
        ismine = Recognize();//若识别为我方冰壶，返回1
        count_all++;
        count_yellow += ismine;
        if(ismine)
        {
            status[i]=1;
        }
        i++;
        TurnL90();
    }
    for(;j < 5;j++)
    {
        while(!(CS00000))
        {
            Run1();
        }
        TurnL90();
        ismine = Recognize();
        count_all++;
        count_yellow += ismine;
        if(ismine)
        {
            status[i]=-1;
        }
        i++;
        TurnR90();
        if((count_all == 5) || (count_yellow == 3))
        {
            break;
        }
    }

    if(j == 4)
    {
        Turn180();
        while(!(CS00000))
        {
            Run1();
        }
        TurnL90();
        Cast();
    }
    if(j == 3)
    {
        Turn180();
        Cast();
    }
    if(j == 2)
    {
        while(!(CS00000))
        {
            Run1();
        }
        TurnR90();
        Cast();
    }

    for(i = 0;i < 5;i++)
    {
        if(status[i] != 0)
        {
            if(status[i] == 1)
            {
                TurnR90();
                int count = 3-i;
                while(count != 0)
                {
                    Run1();
                    if((CS00000) || (CS00001)) count--;
                }
                TurnL90();
                Capture();
                TurnL90();
                while(count != 3-i)
                {
                    Run1();
                    if((CS00000) || (CS00001)) count++;
                }
                TurnR90();
                Cast();
            }

            if(status[i] == -1)
            {
                int count = 3-i;
                if(count == 1)
                {
                    TurnR90();
                    while(!(CS00000)) Run1();
                    TurnR90();
                    Capture();
                    TurnR90();
                    while(!(CS00000)) Run1();
                    TurnR90();
                    Cast();
                }
                else if(count == 0)
                {
                    Turn180();
                    Capture();
                    Turn180();
                    Cast();
                }
                else if(count == -1)
                {
                    TurnL90();
                    while(!(CS00000)) Run1();
                    TurnL90();
                    Capture();
                    TurnL90();
                    while(!(CS00000)) Run1();
                    TurnL90();
                    Cast();
                }
            }
        }
    }

}

void Run3_1(void)
{
    int countR = 0,countL = 0;
    int ismine[5];
    while(countR != 2 && countL != 2)
    {
        Run1();
        if((CS10000)) countR++;
        if(CS00000) countL++;
    }
    TurnR90();
    Cast();
    //左一
    TurnL90();
    while(!(CS00000)) Run1();
    TurnL90();
    ismine[4] = Recognize();
    if(ismine[4])
    {
        Capture();
        TurnL90();
        while(!(CS00000)) Run1();
        TurnL90();
        Cast();
    }

    //左二
    if(ismine[4])
    {
        Turn180();
    }
    else
    {
        TurnL90();
        while(!(CS00000)) Run1();
        TurnR90();
    }
    ismine[3] = Recognize();
    if(ismine[3])
    {
        Capture();
        Turn180();
        Cast();
    }

    //左三
    if(ismine[3])
    {
        TurnR90();
    }
    else
    {
        TurnL90();
    }
    while(!(CS00000)) Run1();
    TurnR90();
    ismine[2] = Recognize();
    if(ismine[2])
    {
        Capture();
        TurnR90();
        while(!(CS00000)) Run1();
        TurnR90();
        Cast();
    }

    //右二
    if(ismine[2])
    {
        int count = 2;
        while(count != 0)
        {
            Run1();
            if((CS00000) || (CS00001)) count--;
        }
    }
    else
    {
        TurnL90();
        while(!(CS00001)) Run1();
    }
    TurnL90();
    ismine[1] = Recognize();
    if(ismine[1])
    {
        int count;
        Capture();
        TurnL90();
        count = 2;
        while(count != 0)
        {
            Run1();
            if(CS00000) count--;
        }
        TurnR90();
        Cast();
    }
    //右一
    if(ismine[1])
    {
        int count = 3;
        while(count != 0)
        {
            Run1();
            if((CS00000) || (CS00001)) count--;
        }
    }
    else
    {
        TurnR90();
        while(!(CS00001))
        {
            Run1();
        }
    }
    TurnL90();
    ismine[0] = Recognize();
    if(ismine[0])
    {
        int count;
        Capture();
        TurnL90();
        count = 3;
        while(count != 0)
        {
            Run1();
            if((CS00000) || (CS00001)) count--;
        }
        TurnR90();
        Cast();
    }
}

/*
void Run()
{
    while(!(CS10000))
    {
        Run1();
    }
    Stop();
    TurnR90();
    TurnOffTrailing();
    TurnOnRanging();
    TurnR90();
    Run3();
}
*/