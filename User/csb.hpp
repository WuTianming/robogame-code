#ifndef CSB_HPP

#include "gpio.h"
#include "tim.h"

//使用TIM1来做us级延时函数,此函数为1us
void TIM1_Delay_us(uint16_t n_us);

//获取超声波模块的检测距离
uint32_t csb_get_distance(void);

#define CSB_HPP
#endif