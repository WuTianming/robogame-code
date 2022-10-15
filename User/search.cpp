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
#define DEBUG

#ifdef DEBUG
    static int qwq = 0;
    ++qwq;
    if (qwq == 2) return 1;
    else return 0;
#endif

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

void Run_Left()
{
    if(A_1 && A_2 && A_3){
        GoLeft();
    }
    else if(A_2 == 0){
        AdjustL();
        HAL_Delay(10);
    }
    else if(A_3 == 0){
        AdjustR();
        HAL_Delay(10);
    }
}


void Run_Forward(){
    while(!((WL_1 == 0) && (WL_2 == 0) && (WR_2 == 1) && (WR_3 == 1))){
        Run1();
    }
    Stop();
}

void Stage1() {
    Run_Forward();
}

void Stage2() {
    GoForward();
    // HAL_Delay(500);
    HAL_Delay(800);
    GoRight(1.0);
    HAL_Delay(5200 * 2.2);
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
    RRotate(-0.4);
    while(!A_1);
    Stop();
}

void NextLane() {
    while(!WL_2){ Run_Left(); }
    while( WL_2){ Run_Left(); }
    Stop();
}

void GoPickup() {
    uint32_t t0 = HAL_GetTick();
    uint32_t LEN = 1800;
    while (HAL_GetTick() - t0 < LEN){
        Run1();
    }
    Stop();

    claw.open();
    actuator_down();
    HAL_Delay(ACTUATOR_HAL_DELAY);
    actuator_stop();
    claw.close();
    actuator_up();
    HAL_Delay(ACTUATOR_HAL_DELAY * 1.1);
    actuator_stop();

    GoBackward();
    HAL_Delay(1000);
    GoBackward(0.2);
    while ( A_1);
    while (!A_1);
    Stop();
}

void Stage4() {
    int color;
    bool pickedup = false;

// 1st lane
    NextLane();
    color = Recognize();
    if (color) {
        GoPickup();
        pickedup = true;
    }

// 2nd lane
    NextLane();
    if (pickedup) {
        // pass
        NextLane();
    } else {
        // pickup
        pickedup = true;
    }
}

void Stage5() {
    RRotate(-1);
    while(A_1);
    RRotate(-0.4);
    while(!A_1);
    RRotate(-1);
    while(A_1);
    RRotate(-0.4);
    while(!A_1);
    Stop();
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
    while (!A_1 || !A_2 || !A_3) {
        if (!A_3) {
            GoForward(0.2);
        } else {
            GoBackward(0.2);
        }
        has_adj = true;
    }
    Stop();
    return has_adj;
}

bool FixRight(void) {
    bool has_adj = false;
    while (!D_1 || !D_2 || !D_3) {
        if (!D_1) {
            // RRotate(-rmicro);
            GoForward(rmicro * 0.50 * CHASSIS_A);
            AdjustL(rmicro);
        } else {
            // RRotate(rmicro);
            GoBackward(rmicro * 0.50 * CHASSIS_A);
            AdjustR(rmicro);
        }
        has_adj = true;
    }
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
    while (WR_1 || WR_2 || WR_3 || WL_1 || WL_2 || WL_3) {
        if (WL_1 || WL_2 || WL_3) {
            GoLeft(0.2);   // 
        } else {
            GoRight(0.2);
        }
        has_adj = true;
    }
    Stop();
    return has_adj;
}

void Fix(void) {
    while (1) {
        bool has_adj1 = false;
        has_adj1 |= FixY();
        has_adj1 |= FixX();
        if (!has_adj1) break;
    }
    Stop();
}