#include "linetracker.h"

//float angularVelocity_match[21];
float angle_match[21];
float translationSpeed_match[21];
float lineTrackerSpeed = 0;
float lineTrackerAngle = 0;
int grayScaleSensor_id = 10;

void linetracker(void)
{
	u16 res = grayScaleSensor2_Read();
	int i;
	int sum = 0;
	int lightNum = 0;
	
	float angle = 0;
	float vx = 0;
	float vy = 0;
	
	for(i = 0; i < 11; i++)
	{
		if((res & (1 << i)) != 0)
		{
			sum += i * 2;
			lightNum ++;
		}
	}
	
	if(lightNum > 0 && lightNum <= 4)
	{
		grayScaleSensor_id = sum / lightNum;
		sum = 0;
		lightNum = 0;
	}
	else
	{
		angle = chassis.angle;
		vx = -lineTrackerSpeed * sinf(angle / 180 * 3.14159);
		vy = lineTrackerSpeed * cosf(angle / 180 * 3.14159);
		if(angle > 500 || angle < -500 || vx > 1000 || vx < -1000 || vy > 1000 || vy < -1000 
			|| (vx == 0 && vy == 0)) return;
		
		if(4 <= grayScaleSensor_id && grayScaleSensor_id <= 16)
		{
			chassisSetState(vx, vy, angle + angle_match[grayScaleSensor_id]);
		}
		else
		{
			chassisSetState(vx / 3, vy / 3, angle + angle_match[grayScaleSensor_id]);
		}
		return;
	}
	
	if(grayScaleSensor_id % 2 == 0)
	{
		for(i = ( (grayScaleSensor_id / 2 - 1) > 0 ? (grayScaleSensor_id / 2 - 1) : 0 );
			i <= ( (grayScaleSensor_id / 2 + 1) < 10 ? (grayScaleSensor_id / 2 + 1) : 10 ); i++)
		{
			if((res & (1 << i)) != 0)
			{
				sum += i * 2;
				lightNum ++;
			}
		}
	}
	else
	{
		for(i = ( (grayScaleSensor_id / 2 - 1) > 0 ? (grayScaleSensor_id / 2 - 1) : 0 );
			i <= ( (grayScaleSensor_id / 2 + 2) < 10 ? (grayScaleSensor_id / 2 + 2) : 10 ); i++)
		{
			if((res & (1 << i)) != 0)
			{
				sum += i * 2;
				lightNum ++;
			}
		}
	}
	
	if(lightNum != 0)
	{
		grayScaleSensor_id = sum / lightNum;
	}
	else
	{
		return;
	}
	
	angle = chassis.angle;
	vx = -lineTrackerSpeed * sinf(angle / 180 * 3.14159);
	vy = lineTrackerSpeed * cosf(angle / 180 * 3.14159);
	if(angle > 500 || angle < -500 || vx > 1000 || vx < -1000 || vy > 1000 || vy < -1000 
		|| (vx == 0 && vy == 0)) return;
	
	chassisSetState(vx, vy, angle + angle_match[grayScaleSensor_id]);
}

void linetracker_translation(void)
{
	u16 res = grayScaleSensor2_Read();
	int i;
	int sum = 0;
	int lightNum = 0;
	
	for(i = 0; i < 11; i++)
	{
		if((res & (1 << i)) != 0)
		{
			sum += i * 2;
			lightNum ++;
		}
	}
	
	if(lightNum > 0 && lightNum <= 4)
	{
		grayScaleSensor_id = sum / lightNum;
		sum = 0;
		lightNum = 0;
	}
	else
	{
		float angle = chassis.angle;
	
		float vx = -lineTrackerSpeed * sinf(angle / 180 * 3.14159) 
				+ translationSpeed_match[grayScaleSensor_id] * cosf(angle / 180 * 3.14159);
	
		float vy = lineTrackerSpeed * cosf(angle / 180 * 3.14159)
				+ translationSpeed_match[grayScaleSensor_id] * sinf(angle / 180 * 3.14159);
		
		if(angle > 500 || angle < -500 || vx > 1000 || vx < -1000 || vy > 1000 || vy < -1000 
			|| (vx == 0 && vy == 0)) return;
		
		chassisSetState(vx, vy, lineTrackerAngle);
		return;
	}
	
	if(grayScaleSensor_id % 2 == 0)
	{
		for(i = ( (grayScaleSensor_id / 2 - 1) > 0 ? (grayScaleSensor_id / 2 - 1) : 0 ); 
			i <= ( (grayScaleSensor_id / 2 + 1) < 10 ? (grayScaleSensor_id / 2 + 1) : 10 ); i++)
		{
			if((res & (1 << i)) != 0)
			{
				sum += i * 2;
				lightNum ++;
			}
		}
	}
	else
	{
		for(i = ( (grayScaleSensor_id / 2 - 1) > 0 ? (grayScaleSensor_id / 2 - 1) : 0 );
			i <= ( (grayScaleSensor_id / 2 + 2) < 10 ? (grayScaleSensor_id / 2 + 2) : 10 ); i++)
		{
			if((res & (1 << i)) != 0)
			{
				sum += i * 2;
				lightNum ++;
			}
		}
	}
	
	if(lightNum != 0)
	{
		grayScaleSensor_id = sum / lightNum;
	}
	else
	{
		return;
	}
	
	float angle = chassis.angle;
	
	float vx = -lineTrackerSpeed * sinf(angle / 180 * 3.14159) 
				+ translationSpeed_match[grayScaleSensor_id] * cosf(angle / 180 * 3.14159);
	
	float vy = lineTrackerSpeed * cosf(angle / 180 * 3.14159)
				+ translationSpeed_match[grayScaleSensor_id] * sinf(angle / 180 * 3.14159);
	
	if(angle > 500 || angle < -500 || vx > 1000 || vx < -1000 || vy > 1000 || vy < -1000 
		|| (vx == 0 && vy == 0)) return;
	
	chassisSetState(vx, vy, lineTrackerAngle);
		
//	chassisSetState(-lineTrackerSpeed * sinf(angle / 180 * 3.14159) 
//		+ translationSpeed_match[grayScaleSensor_id] * cosf(angle / 180 * 3.14159)
//	
//		, lineTrackerSpeed * cosf(angle / 180 * 3.14159)
//		+ translationSpeed_match[grayScaleSensor_id] * sinf(angle / 180 * 3.14159)
//		
//		, lineTrackerAngle);
}

void angle_match_init(void)
{
	angle_match[0] = 21;
	angle_match[1] = 17.5;
	angle_match[2] = 16.1;
	angle_match[3] = 14.1;
	angle_match[4] = 13.1;
	angle_match[5] = 11;
	angle_match[6] = 8.9;
	angle_match[7] = 6.8;
	angle_match[8] = 4.1;
	angle_match[9] = 2.3;
	angle_match[10] = 0;
	angle_match[11] = -2.3;
	angle_match[12] = -4.1;
	angle_match[13] = -6.8;
	angle_match[14] = -8.9;
	angle_match[15] = -11;
	angle_match[16] = -13.1;
	angle_match[17] = -14.1;
	angle_match[18] = -16.1;
	angle_match[19] = -17.5;
	angle_match[20] = -21;
}

void translationSpeed_match_init(void)
{
	translationSpeed_match[0] = -450;
	translationSpeed_match[1] = -405;
	translationSpeed_match[2] = -360;
	translationSpeed_match[3] = -315;
	translationSpeed_match[4] = -270;
	translationSpeed_match[5] = -225;
	translationSpeed_match[6] = -180;
	translationSpeed_match[7] = -135;
	translationSpeed_match[8] = -90;
	translationSpeed_match[9] = -45;
	translationSpeed_match[10] = 0;
	translationSpeed_match[11] = 45;
	translationSpeed_match[12] = 90;
	translationSpeed_match[13] = 135;
	translationSpeed_match[14] = 180;
	translationSpeed_match[15] = 225;
	translationSpeed_match[16] = 270;
	translationSpeed_match[17] = 315;
	translationSpeed_match[18] = 360;
	translationSpeed_match[19] = 405;
	translationSpeed_match[20] = 450;
}

// void angularVelocity_match_init(void)
// {
// 	angularVelocity_match[0] = 300;
// 	angularVelocity_match[1] = 260;
// 	angularVelocity_match[2] = 230;
// 	angularVelocity_match[3] = 200;
// 	angularVelocity_match[4] = 170;
// 	angularVelocity_match[5] = 140;
// 	angularVelocity_match[6] = 110;
// 	angularVelocity_match[7] = 80;
// 	angularVelocity_match[8] = 40;
// 	angularVelocity_match[9] = 20;
// 	angularVelocity_match[10] = 0;
// 	angularVelocity_match[11] = -20;
// 	angularVelocity_match[12] = -40;
// 	angularVelocity_match[13] = -80;
// 	angularVelocity_match[14] = -110;
// 	angularVelocity_match[15] = -140;
// 	angularVelocity_match[16] = -170;
// 	angularVelocity_match[17] = -200;
// 	angularVelocity_match[18] = -230;
// 	angularVelocity_match[19] = -260;
// 	angularVelocity_match[20] = -300;
// }

// void TIM3_Init(u16 arr,u16 psc)
// {
// 	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟 TIM3 使能
	
// 	//定时器 TIM3 初始化
// 	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载寄存器周期的值 24 - 1
// 	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置时钟频率除数的预分频值 50000 - 1
// 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
// 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数
	
// 	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化 TIM3
// 	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE ); //允许更新中断
	
// 	//中断优先级 NVIC 设置
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 中断
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //先占优先级 0 级
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级 3 级
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
// 	NVIC_Init(&NVIC_InitStructure); //④初始化 NVIC 寄存器
// 	TIM_Cmd(TIM3, DISABLE); //⑤使能 TIM3
// }

// void TIM3_IRQHandler(void) //TIM3 中断
// {
// 	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查 TIM3 更新中断发生与否
// 	{
// 		TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //清除 TIM3 更新中断标志
// 		correspond(linetrackerSpeed);
// 		//sprintf(error_id, "%f, %f, %f\n",-linetrackerSpeed * sin(chassis.angle/360*2*3.14159),linetrackerSpeed * cos(chassis.angle/360*2*3.14159), chassis.angle + angle_match[10]);
// 		//uart4_send(error_id);
// 	}
// }
