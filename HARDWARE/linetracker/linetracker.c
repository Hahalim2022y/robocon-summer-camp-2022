#include "linetracker.h"
#include "uart.h"

//float angularVelocity_match[21];
float angle_match[21];
float translationSpeed_match[21];
float lineTrackerSpeed;
float lineTrackerAngle;
// float linetrackerSpeed = 0;

// FCycleArray errorTester;

// char error_id[200];

void linetracker(void)
{
	u16 res = grayScaleSensor2_Read();
	int i;
	int sum = 0;
	int lightNum = 0;
	static int id = 11;
	
	for(i = 0; i < 11; i++)
	{
		if((res & (1 << i)) != 0)
		{
			sum += i * 2;
			lightNum ++;
		}
	}
	if(lightNum != 0)
	{
		id = sum / lightNum;
		sum = 0;
		lightNum = 0;
	}
	
	if(id % 2 == 0)
	{
		for(i = ( (id / 2 - 1) > 0 ? (id / 2 - 1) : 0 ); i <= ( (id / 2 + 1) < 10 ? (id / 2 + 1) : 10 ); i++)
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
		for(i = ( (id / 2 - 1) > 0 ? (id / 2 - 1) : 0 ); i <= ( (id / 2 + 2) < 10 ? (id / 2 + 2) : 10 ); i++)
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
		id = sum / lightNum;
	}
	
	float angle = chassis.angle;
	if(angle > 360 || angle < -360) return;
	
	chassisSetState(-lineTrackerSpeed * sinf(angle / 180 * 3.14159)
		, lineTrackerSpeed * cosf(angle / 180 * 3.14159), angle + angle_match[id]);
}

float fxxk;

void linetracker_translation(void)
{
	u16 res = grayScaleSensor2_Read();
	int i;
	int sum = 0;
	int lightNum = 0;
	static int id = 10;
	
	for(i = 0; i < 11; i++)
	{
		if((res & (1 << i)) != 0)
		{
			sum += i * 2;
			lightNum ++;
		}
	}
	if(lightNum != 0)
	{
		id = sum / lightNum;
		sum = 0;
		lightNum = 0;
	}
	
	if(id % 2 == 0)
	{
		for(i = ( (id / 2 - 1) > 0 ? (id / 2 - 1) : 0 ); i <= ( (id / 2 + 1) < 10 ? (id / 2 + 1) : 10 ); i++)
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
		for(i = ( (id / 2 - 1) > 0 ? (id / 2 - 1) : 0 ); i <= ( (id / 2 + 2) < 10 ? (id / 2 + 2) : 10 ); i++)
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
		id = sum / lightNum;
	}
	
	float angle = chassis.angle;
	if(angle > 360 || angle < -360) return;
	
	fxxk = -lineTrackerSpeed * sinf(angle / 180 * 3.14159) 
		+ translationSpeed_match[id] * cosf(angle / 180 * 3.14159);
	
	chassisSetState(-lineTrackerSpeed * sinf(angle / 180 * 3.14159) 
		+ translationSpeed_match[id] * cosf(angle / 180 * 3.14159)
	
		, lineTrackerSpeed * cosf(angle / 180 * 3.14159)
		+ translationSpeed_match[id] * sinf(angle / 180 * 3.14159)
		
		, lineTrackerAngle);
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

void angle_match_init(void)
{
	angle_match[0] = 19;
	angle_match[1] = 17;
	angle_match[2] = 15;
	angle_match[3] = 13;
	angle_match[4] = 11;
	angle_match[5] = 9;
	angle_match[6] = 7;
	angle_match[7] = 5;
	angle_match[8] = 3;
	angle_match[9] = 1;
	angle_match[10] = 0;
	angle_match[11] = -1;
	angle_match[12] = -3;
	angle_match[13] = -5;
	angle_match[14] = -7;
	angle_match[15] = -9;
	angle_match[16] = -11;
	angle_match[17] = -13;
	angle_match[18] = -15;
	angle_match[19] = -17;
	angle_match[20] = -19;
}

void translationSpeed_match_init(void)
{
	translationSpeed_match[0] = -350;
	translationSpeed_match[1] = -315;
	translationSpeed_match[2] = -280;
	translationSpeed_match[3] = -245;
	translationSpeed_match[4] = -210;
	translationSpeed_match[5] = -175;
	translationSpeed_match[6] = -140;
	translationSpeed_match[7] = -105;
	translationSpeed_match[8] = -70;
	translationSpeed_match[9] = -35;
	translationSpeed_match[10] = 0;
	translationSpeed_match[11] = 35;
	translationSpeed_match[12] = 70;
	translationSpeed_match[13] = 105;
	translationSpeed_match[14] = 140;
	translationSpeed_match[15] = 175;
	translationSpeed_match[16] = 210;
	translationSpeed_match[17] = 245;
	translationSpeed_match[18] = 280;
	translationSpeed_match[19] = 315;
	translationSpeed_match[20] = 350;
}

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
