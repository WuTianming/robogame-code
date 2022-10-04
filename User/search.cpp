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

void GoForward() {
    SpeedTypeDef vel;
    vel.Omega = 0;
    vel.Y = 0.4;
    vel.X = 0.0;
    car.Set_Velocity(vel);
}

void GoBackward() {
    SpeedTypeDef vel;
    vel.Omega = 0;
    vel.Y = -0.4;
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

void AdjustR() {
    SpeedTypeDef vel;
#ifdef INCREMENTAL
    vel = car.Get_Velocity();
    vel.Omega = -0.4;
#else
    vel.Omega = -0.4;
    // vel.Y = 0.3;
    vel.Y = 0.0;
    vel.X = 0.0;
#endif
    car.Set_Velocity(vel);
}

void AdjustL() {
    SpeedTypeDef vel;
#ifdef INCREMENTAL
    vel = car.Get_Velocity();
    vel.Omega = 0.4;
#else
    vel.Omega = 0.4;
    // vel.Y = 0.3;
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

void GoLeft() {
    SpeedTypeDef vel;
    vel.Omega = 0;
    vel.Y = 0.0;
    vel.X = -0.3;
    car.Set_Velocity(vel);
}

void GoRight() {
    SpeedTypeDef vel;
    vel.Omega = 0;
    vel.Y = 0.0;
    vel.X = 0.3;
    car.Set_Velocity(vel);
}

#define CONT 0
#define NEXT 1

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
            GoLeft();   // need to complete
        }
        status[idx] = Recognize();idx++;
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

void Run12() {
    // 先越过开始区的一坨黑线
    for (int i = 0; i < 100; i++) {
        Run1();
    }
    // 再走直线直到第一个转弯
    while (true) {
        Run1();
        if ((CS10000) || (CS00000)) {
            break;
        }
    }
    // 再开始往右平移
    for (int i = 0; i < 100; i++) {
        i = 2;      // while 1
        GoRight();
    }
    while (true) {
        Run2();
    }
}

void Run1()
{
    if((CS10011) || (CS11001) || (CS10001) || (CS11011))
    {
        GoForward();
        // HAL_Delay(20);
    }
    else if((CS11000) || (CS11100))
    {
        AdjustR();
        HAL_Delay(40);
    }
    else if((CS00011) || (CS00111))
    {
        AdjustL();
        HAL_Delay(40);
    }
    // else {
    //     Stop();
    // }
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
