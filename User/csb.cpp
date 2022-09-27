#include "csb.hpp"

//使用TIM1来做us级延时函数,此函数为1us
void TIM1_Delay_us(uint16_t n_us)
{
	__HAL_TIM_SetCounter(&htim1, 0);

	/* 开启定时器1计数 */
	__HAL_TIM_ENABLE(&htim1);

	//获取定时器的计数值！！！再判断计数值，但是计数值不得大于定时器配置的重装载值。\
	假如：定时器设定的重装载值为65536-1， 比较值需小于重装载值，即(计数1次 * n_us)-1) 小于（65536-1）\
																	  即传进来的参数n_us需要小于65536-1；
	while(__HAL_TIM_GetCounter(&htim1) < ((1 * n_us)-1) );
	/* Disable the Peripheral */
	__HAL_TIM_DISABLE(&htim1);
}

//获取超声波模块的检测距离
uint32_t csb_get_distance(void)
{
  uint32_t CSB_value = 0 ;
	//给发射引脚一个高电平
	HAL_GPIO_WritePin(CSB_Trig_GPIO_Port, CSB_Trig_Pin, GPIO_PIN_SET);
	//延时10us以上
	TIM1_Delay_us(20);
	//给发射引脚一个低电平
	HAL_GPIO_WritePin(CSB_Trig_GPIO_Port, CSB_Trig_Pin, GPIO_PIN_RESET);
	//等待接收引脚变成高电平
	while( HAL_GPIO_ReadPin(CSB_Echo_GPIO_Port,CSB_Echo_Pin) == 0);
	//设置定时器初始值为0
	__HAL_TIM_SetCounter(&htim1, 0);
	//开始计时
	__HAL_TIM_ENABLE(&htim1);
	//接收完全后不再为高电平，即当接收引脚变成低电平后，停止计时，获取计数时间
    while( HAL_GPIO_ReadPin(CSB_Echo_GPIO_Port,CSB_Echo_Pin) == 1);
	//获取定时器的计数值,赋值操作  a = b;
	CSB_value = __HAL_TIM_GetCounter(&htim1);
	//停止计时
	__HAL_TIM_DISABLE(&htim1);
	//已知高电平总时间，即可利用公式（ 测试距离= (高电平时间*声速(340M/S))/2 ），计算超声波模块距离障碍物的单程距离；
	//如果需要返回 毫米级别距离,公式为（ 测试距离= (高电平时间*声速(340M/1000mS))/2 ）

    // 返回毫米
	return ( CSB_value*340/1000/2);
}

uint32_t csb2_get_distance(void)
{
	// /*
    uint32_t CSB_value2 = 0 ;
	//给发射引脚一个高电平
	HAL_GPIO_WritePin(CSB2_Trig_GPIO_Port, CSB2_Trig_Pin, GPIO_PIN_SET);
	//延时10us以上
	TIM1_Delay_us(20);
	//给发射引脚一个低电平
	HAL_GPIO_WritePin(CSB2_Trig_GPIO_Port, CSB2_Trig_Pin, GPIO_PIN_RESET);
	//等待接收引脚变成高电平
	while( HAL_GPIO_ReadPin(CSB2_Echo_GPIO_Port,CSB2_Echo_Pin) == 0);
	//设置定时器初始值为0
	__HAL_TIM_SetCounter(&htim1, 0);
	//开始计时
	__HAL_TIM_ENABLE(&htim1);
	//接收完全后不再为高电平，即当接收引脚变成低电平后，停止计时，获取计数时间
    while( HAL_GPIO_ReadPin(CSB2_Echo_GPIO_Port,CSB2_Echo_Pin) == 1);
	//获取定时器的计数值,赋值操作  a = b;
	CSB_value2 = __HAL_TIM_GetCounter(&htim1);
	//停止计时
	__HAL_TIM_DISABLE(&htim1);
	//已知高电平总时间，即可利用公式（ 测试距离= (高电平时间*声速(340M/S))/2 ），计算超声波模块距离障碍物的单程距离；
	//如果需要返回 毫米级别距离,公式为（ 测试距离= (高电平时间*声速(340M/1000mS))/2 ）
	return ( CSB_value2*340/1000/2);	//毫米级别
	// */
return 0;
}