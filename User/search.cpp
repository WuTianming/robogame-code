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
#include "printf.h"
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
// const float fb_vel = 0.7;
// const float lr_vel = 0.7;
// const float rvel   = 1.3;
// const float rmicro = 0.7;

void actuator_up() {
  car.Set_Control_Method(Control_Method_OPENLOOP);
  HAL_GPIO_WritePin(actuator1_GPIO_Port, actuator1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(actuator2_GPIO_Port, actuator2_Pin, GPIO_PIN_SET);
}

void actuator_down() {
  car.Set_Control_Method(Control_Method_OPENLOOP);
  HAL_GPIO_WritePin(actuator1_GPIO_Port, actuator1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(actuator2_GPIO_Port, actuator2_Pin, GPIO_PIN_RESET);
}

void actuator_stop() {
  HAL_GPIO_WritePin(actuator1_GPIO_Port, actuator1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(actuator2_GPIO_Port, actuator2_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  car.Set_Control_Method(Control_Method_OMEGA);
}

void GoForward(float v=fb_vel) {
    SpeedTypeDef vel;
    vel.Omega = 0;
    vel.Y = v;
    vel.X = 0.0;
    car.Set_Velocity(vel);
}

void GoBackward(float v=fb_vel) {
    GoForward(-v);
}

void GoLeft(float vv = lr_vel) {
    GoRight(-vv);
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
    // return 1;

    claw.open();
    HAL_Delay(1000);
    const int LEN = 61;
    char str[LEN]; str[LEN-1] = '\0';
    HAL_UART_Receive(&huart4, (uint8_t *)str, LEN, 1000);
    HAL_UART_Receive(&huart4, (uint8_t *)str, LEN, 1000);
    {
        char buf[LEN+2];
        int len = sprintf(buf, "%s\n", str);
        HAL_USART_Transmit(&husart1, (const uint8_t *)buf, len, 1000);
    }
    int i = 0;
    for (i = 0; i < LEN; ++i) {
        if (str[i] > '0' && str[i] <= '9') {
            char buf[40];
            int len = sprintf(buf, "\nis red\n", str);
            HAL_USART_Transmit(&husart1, (const uint8_t *)buf, len, 1000);
            return 1;
        }
    }
    char buf[LEN];
    int len = sprintf(buf, "\nis NOT red\n", str);
    HAL_USART_Transmit(&husart1, (const uint8_t *)buf, len, 1000);
    return 0;
    // for (i = 0; i < 15; i++) { if (str[i] == 'x') break; }
    // int cx = 0, cy = 0;
    // for (i += 2; str[i] <= '9' && str[i] >= '0'; ++i) cx = cx * 10 + str[i] - '0';
    // for (i += 4; str[i] <= '9' && str[i] >= '0'; ++i) cy = cy * 10 + str[i] - '0';
    // if (cx != 0 || cy != 0) {
    //     return 1;
    // } else {
    //     return 0;
    // }
    return 0;
}


void Run1()
{
    if(!(WL_1 + WL_2 + WL_3 + WR_1 + WR_2 + WR_3)){
        GoForward();
    }
    else if(WL_3 == 1){
        AdjustL();
        HAL_Delay(10);
    }
    else if(WR_1 == 1){
        AdjustR();
        HAL_Delay(10);
    }
}

void Run1Straight()
{
    if(!(WL_1 + WL_2 + WL_3 + WR_1 + WR_2 + WR_3)){
        if (S_2) {
            // AdjustL();
        }
    }
    else if(WL_3 == 1){
        AdjustL();
        HAL_Delay(10);
    }
    else if(WR_1 == 1){
        AdjustR();
        HAL_Delay(10);
    }
}

void Run_Left()
{
    if(A_1 && A_2 && A_3){
        GoLeft();
    } else if(A_2 == 0){
        AdjustL();
        HAL_Delay(10);
    } else if(A_3 == 0){
        AdjustR();
        HAL_Delay(10);
    }
}

void Run_Right()
{
    if(D_1 && D_2 && D_3){
        GoRight();
    } else if(D_1 == 0){
        AdjustL();
        HAL_Delay(10);
    } else if(D_2 == 0){
        AdjustR();
        HAL_Delay(10);
    }
}

void Run_Forward(){
    GoForward();
    while(!((WL_1 == 0) && (WL_2 == 0) && (WR_2 == 1) && (WR_3 == 1))){
        Run1();
    }
    Stop();
}

void Stage1() {
    uint32_t t0 = HAL_GetTick();
    uint32_t LEN = 3000;
    GoForward();
    while (HAL_GetTick() - t0 < LEN){
        Run1();
    }
    Run_Forward();
}

void Stage2() {
    GoForward();
    HAL_Delay(1000);
    // HAL_Delay(500);
    // HAL_Delay(800);

    { // 向右狂奔
        uint32_t nowtick = HAL_GetTick();
        GoRight(1.0);
        // HAL_Delay(5200 * 2.2);   // low battery
        uint32_t limit = 5200 * 1.3;
        while (HAL_GetTick() - nowtick < limit) {
            /*
            if (getAngle() > 3) { // too left
                AdjustR();
            } else if (getAngle() < -3) {
                AdjustL();
            } else {
                AdjustL(0);
            }
            */
        }
    }
    GoRight(0.4);

#define RIGHT_HAS_BLACK (!WR_1 || !WR_2 || !WR_3)
    while (!RIGHT_HAS_BLACK);
    while ( RIGHT_HAS_BLACK);
#undef  RIGHT_HAS_BLACK
    Stop();
}

void Stage3() {
    RRotate(-1);
    while(A_1);
    RRotate(-rmicro);
    while(!A_1);
    Stop();
}

void NextLane() {
    GoLeft();
    HAL_Delay(300);
    while(!WL_2){ Run_Left(); }
    while( WL_2){ Run_Left(); }
    Stop();
}

void PrevLane() {
    GoRight();
    HAL_Delay(300);
    while(!WR_2){ Run_Right(); }
    while( WR_2){ Run_Right(); }
    Stop();
}

void backoff() {
    GoBackward();
    HAL_Delay(1000);
    GoBackward(0.2);
    while ( A_1);
    while (!A_1);
    Stop();
    Fix();
}

// 在右侧发射道上的挪动
// 绕着右前方轮做逆时针旋转
void Nudge1(float t) {
    car.Set_Control_Method(Control_Method_OPENLOOP);

    GoBackward(1);
    HAL_Delay(50);
    Stop();
    // static const double targetAngle = 2.0;

    float o  = 0.6 * t;
    // float o  = 1.0 * t;
    // TODO
    float vy = -o * CHASSIS_A;
    float vx = o * CHASSIS_A;
    // float vy = 0, vx = 0;

    SpeedTypeDef V;
    double angle;

    V.Omega = o; V.X = vx; V.Y = vy;
    car.Set_Velocity(V);
    HAL_Delay(50);
    // int i = 0;
    // while (i < 40) {
    //     angle = getAngle();
    //     if (t > 0 && (angle >= targetAngle)) { break; }
    //     if (t < 0 && (angle <= 0)) { break; }
    //     // break;      // minimal
    //     ++i;
    // }
    Stop();

    car.Set_Control_Method(Control_Method_OMEGA);
    Stop();
    HAL_Delay(10);
}

// 在左侧发射道上的挪动
// 绕着左前方轮做顺时针旋转
void Nudge2(float t) {
    car.Set_Control_Method(Control_Method_OPENLOOP);

    GoBackward(1);
    HAL_Delay(50);
    Stop();
    // static const double targetAngle = 2.0;

    float o  = -1.0 * t;
    float vy = o * CHASSIS_A;
    float vx = o * CHASSIS_A;

    SpeedTypeDef V;
    // double angle;

    V.Omega = o; V.X = vx; V.Y = vy;
    car.Set_Velocity(V);
    HAL_Delay(150);
    // int i = 0;
    // while (i < 4000) {
    //     angle = -getAngle();
    //     // if (t > 0 && (angle >= targetAngle)) { break; }
    //     // if (t < 0 && (angle <= 0)) { break; }
    //     // break;      // minimal
    //     ++i;
    // }
    Stop();

    car.Set_Control_Method(Control_Method_OMEGA);
    Stop();
    HAL_Delay(10);
}

void GoPickup() {
    // TODO
    car.Set_Control_Method(Control_Method_OMEGA);   // reset integral variable

    HAL_Delay(500);

    uint32_t t0 = HAL_GetTick();
    // uint32_t LEN = 1800;
    // uint32_t LEN = 1200;    // 恰恰好碰到那个位置，有时会差一点 TODO
    uint32_t LEN = 1400;    // 恰恰好碰到那个位置，有时会差一点 TODO
    GoForward();
    while (HAL_GetTick() - t0 < LEN){
        Run1();
    }
    Stop();

    // claw.open();
    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY);
    actuator_stop();
    claw.close();
    actuator_up();
    HAL_Delay(ACTUATOR_HAL_DELAY);
    actuator_stop();
}

void backoffBack() {
    GoBackward();
    HAL_Delay(1500);
    GoBackward(0.2);
    while ( A_1);
    while (!A_1);
    Stop();
    Fix();
}

void GoPickupBack() {
    claw.open();
    HAL_Delay(500);

    uint32_t t0 = HAL_GetTick();
    uint32_t LEN = 2500;
    GoForward();
    while (HAL_GetTick() - t0 < LEN){
        Run1();
    }
    Stop();
    HAL_Delay(250);
    // GoBackward(0.4);
    // HAL_Delay(400);
    // Stop();

    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY);
    actuator_stop();
    claw.close();
    actuator_up();
    HAL_Delay(ACTUATOR_HAL_DELAY);
    actuator_stop();
}

#define NUDGES1
#define NUDGES2

void GoPutdown(int nudge) {
    car.Set_Control_Method(Control_Method_OMEGA);   // reset integral variable

    uint32_t t0 = HAL_GetTick();
    uint32_t LEN = 2000;        // TODO
    GoForward();
    while (HAL_GetTick() - t0 < LEN){
        Run1();
    }
    Stop();
    HAL_Delay(500);
    if (nudge == 1) {
        #ifdef NUDGES1
            Nudge1();
            HAL_Delay(300);
        #endif
    } else if (nudge == 2) {
        #ifdef NUDGES2
            Nudge2();
            HAL_Delay(300);
        #endif
    }

    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY);
    actuator_stop();
    claw.open();
    HAL_Delay(300);

    // GoBackward(0.4);
    // HAL_Delay(250);

    Stop();
    actuator_up();
    HAL_Delay(ACTUATOR_HAL_DELAY);
    actuator_stop();
    claw.close();
}

void TurnAtCrossing(int interval) {
    RRotate(-1);
    HAL_Delay(interval);
    // while (!WL_3);
    // while ( WL_3);
    RRotate(-0.4);
    while (!WL_3);
    while ( WL_3);
    Stop();
}

void Stage4() {
    bool pickedup = false;

    NextLane();
    if (Recognize()) {
        GoPickup(); backoff();
        pickedup = true;
    }

    NextLane();
    HAL_Delay(200);
    if (!pickedup && Recognize()) {
        GoPickup(); backoff();
        pickedup = true;
    }

    NextLane();
    HAL_Delay(200);

    Fix();
    GoPutdown(1);   // with nudge = 1
    HAL_Delay(500);
    HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_SET);
    HAL_Delay(700);
    HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_RESET);
// #ifdef NUDGES1
//     HAL_Delay(500);
//     Nudge1(-1);
// #endif
    backoff();
    Fix();

    NextLane();
    HAL_Delay(1000);
    GoPickup(); backoff();

    NextLane();
    HAL_Delay(200);

    Fix();
    GoPutdown(2);   // with nudge = 2
    HAL_Delay(500);
    HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_SET);
    HAL_Delay(700);
    HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_RESET);
// #ifdef NUDGES2
//     HAL_Delay(500);
//     Nudge2(-1);
// #endif
    backoff();
    Fix();

// 后面的三个壶
    TurnAtCrossing();
    gyro_calibrate();       // TODO
    Fix();

    if (Recognize()) {
        GoPickupBack();
        backoffBack();
        TurnAtCrossing(4300);
        Fix();

        GoPutdown(2);
        HAL_Delay(500);
        HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_SET);
        HAL_Delay(700);
        HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_RESET);
// #ifdef NUDGES2
//         HAL_Delay(500);
//         Nudge2(-1);
// #endif
        backoff();
        Fix(); 
        TurnAtCrossing();
        Fix(); 
        HAL_Delay(200);
    }

    NextLane();
    HAL_Delay(200);

    if (Recognize()) {
        GoPickupBack();
        backoffBack();
        Fix();
        NextLane();
        TurnAtCrossing(4300);
        Fix();

        GoPutdown(1);
        HAL_Delay(500);
        HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_SET);
        HAL_Delay(700);
        HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_RESET);
// #ifdef NUDGES1
//         HAL_Delay(500);
//         Nudge1(-1);
// #endif
        backoff();
        Fix(); 
        TurnAtCrossing();
    } else {
        NextLane();
    }
    Fix(); 
    HAL_Delay(200);

    if (Recognize()) {
        GoPickupBack();
        backoffBack();
        TurnAtCrossing(4300);
        Fix();

        GoPutdown(1);
        HAL_Delay(500);
        HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_SET);
        HAL_Delay(700);
        HAL_GPIO_WritePin(solenoid_GPIO_Port, solenoid_Pin, GPIO_PIN_RESET);
// #ifdef NUDGES1
//         HAL_Delay(500);
//         Nudge1(-1);
// #endif
        backoff();
        Fix(); 
    } else {
        ;
    }

    while (1);
}

/*
1. 先确保左边卡到位置（通过纯粹的前后平移）
2. 再旋转，让右边卡到位置（通过旋转）
3. 再返回 1 重复，直到不产生任何多余调整，此时可以认为车的朝向已经调整完成
4. 再用前面的巡线模块卡好左右位置
5. 返回 1 重复，直到不产生任何多余调整。
*/

bool FixLeft(void) {
    bool has_adj = false;
    while (!A_3) { GoForward(0.3); has_adj = true; }    // move A_3 out of the black
    Stop();
    while (!A_1) { GoBackward(0.3); has_adj = true; }
    Stop();
    while (!A_3) { GoForward(0.3); has_adj = true; }
    Stop();
    /*
    while (!A_1 || !A_2 || !A_3) {
        if (!A_3) {
            GoForward(0.2);
        } else {
            GoBackward(0.2);
        }
        has_adj = true;
    }
    */
    Stop();
    return has_adj;
}

bool FixRight(void) {
    bool has_adj = false;
    while (!D_1) {
        // RRotate(-rmicro);
        GoForward(rmicro * CHASSIS_A);
        AdjustL(rmicro);
        has_adj = true;
    } Stop();
    while (!D_3) {
        // RRotate(rmicro);
        GoBackward(rmicro * CHASSIS_A);
        AdjustR(rmicro);
        has_adj = true;
    } Stop();
    while (!D_1) {
        // RRotate(-rmicro);
        GoForward(rmicro * CHASSIS_A);
        AdjustL(rmicro);
        has_adj = true;
    } Stop();
    /*
    while (!D_1 || !D_2 || !D_3) {
        if (!D_1) {
            // RRotate(-rmicro);
            GoForward(rmicro * CHASSIS_A);
            AdjustL(rmicro);
        } else {
            // RRotate(rmicro);
            GoBackward(rmicro * CHASSIS_A);
            AdjustR(rmicro);
        }
        has_adj = true;
    }
    */
    /*
    while (WR_1 || WR_2 || WR_3 || WL_1 || WL_2 || WL_3) {
        if (WL_1 || WL_2 || WL_3) {
            // RRotate(-rmicro);
            GoForward(rmicro * CHASSIS_A);
            AdjustL(rmicro);
        } else {
            // RRotate(rmicro);
            GoBackward(rmicro * CHASSIS_A);
            AdjustR(rmicro);
        }
        has_adj = true;
    }
    */
    Stop();
    return has_adj;
}

bool FixY(void) {
    bool has_adj = false;
    while (1) {
        bool has_adj1 = false;
        has_adj1 |= FixLeft();
        has_adj1 |= FixRight();
        has_adj  |= has_adj1;
        if (!has_adj1) break;
    }
    Stop();
    return has_adj;
}

bool FixX(void) {
    bool has_adj = false;
    while ((WR_1 + WR_2 + WR_3) - (WL_1 + WL_2 + WL_3) > 0) {
        GoRight(0.3); has_adj = true;
    } Stop();
    while ((WR_1 + WR_2 + WR_3) - (WL_1 + WL_2 + WL_3) < 0) {
        GoLeft(0.3); has_adj = true;
    } Stop();
    while ((WR_1 + WR_2 + WR_3) - (WL_1 + WL_2 + WL_3) > 0) {
        GoRight(0.3); has_adj = true;
    } Stop();
    /*
    while (WR_1 || WR_2 || WR_3 || WL_1 || WL_2 || WL_3) {
        int qwq = (WR_1 + WR_2 + WR_3) - (WL_1 + WL_2 + WL_3);
        if (qwq > 0) {
            GoRight(0.2);   // 
        } else if (qwq < 0) {
            GoLeft(0.2);
        } else {
            break;
        }
        has_adj = true;
    }
    */
    Stop();
    return has_adj;
}

void FixGyro() {
    while ((S_1 + S_2) - (S_3 + S_4) > 0) {
        GoRight(0.3);
    } Stop();
    while ((S_1 + S_2) - (S_3 + S_4) < 0) {
        GoLeft(0.3);
    } Stop();
    while ((S_1 + S_2) - (S_3 + S_4) > 0) {
        GoRight(0.3);
    } Stop();
}

void Fix(void) {
    /*
    Stop();
    return;
    */

    Stop();
    HAL_Delay(200);
    // while (1) {
        bool has_adj1 = false;
        has_adj1 |= FixY();
        has_adj1 |= FixX();
    //     if (!has_adj1) break;
    // }
    Stop();
    FixGyro();
    gyro_calibrate();
    mpu.AngleZ = 0.00;
    HAL_Delay(200);
}