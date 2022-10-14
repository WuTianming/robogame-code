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
    HAL_Delay(500);
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

void Stage4() {
    while(!WL_2){ Run_Left(); }
    while( WL_2){ Run_Left(); }
    Stop();
}

void Stage5() {
    ;
}