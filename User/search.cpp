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

void GoForward() {
	SpeedTypeDef vel;
	vel.Omega = 0;
	vel.Y = 0.1;
	vel.X = 0.0;
	car.Set_Velocity(vel);
}

void AdjustR() {
	SpeedTypeDef vel;
	vel.Omega = -0.3;
	vel.Y = 0.0;
	vel.X = 0.0;
	car.Set_Velocity(vel);
}

void AdjustL() {
	SpeedTypeDef vel;
	vel.Omega = 0.3;
	vel.Y = 0.0;
	vel.X = 0.0;
	car.Set_Velocity(vel);
}

void Run1()
{
	GoForward();
	if (false) {
		if((CS10011) || (CS11001) || (CS10001) || (CS11011))
		{
			GoForward();
		}
		else if((CS11000) || (CS11100))
		{
			AdjustR();
		}
		else if((CS00011) || (CS00111))
		{
			AdjustL();
		}
	}
	HAL_Delay(100);
}

int Ranging()
{
	return 0;
}

void Run2()
{
	if(Ranging() == 0) GoForward();
	//Ranging()是第二阶段超声测距函数，用于比较车前后与挡板距离
	else if(Ranging() == 1) AdjustR();//前距离大于后距离
	else if(Ranging() == -1) AdjustL();//反之
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
	while(!(CS00000))
	{
		Run2();
	}
	TurnR90();
	Run3();
}
