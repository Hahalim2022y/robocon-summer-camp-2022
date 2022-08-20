#include "linetracker.h"
#include "uart.h"

float angularVelocity_match[21];
float angle_match[21];
float linetrackerSpeed = 0;

FCycleArray errorTester;

char error_id[200];

float linetracker(void)
{
	u16 res = grayScaleSensor2_Read();
	u8 i;
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
	}
	//return angularVelocity_match[id];
//	sprintf(error_id, "%f, %f, %f\n",-linetrackerSpeed * sin(chassis.angle/360*2*3.14159),linetrackerSpeed * cos(chassis.angle/360*2*3.14159), chassis.angle + angle_match[id]);
//	uart4_send(error_id);
//	fCycleArray_push(&errorTester, angle_match[id]);
	return angle_match[id];
}

void correspond(float speed)
{
	float vx=-speed * sin(chassis.angle/360*2*3.14159);
	float vy=speed * cos(chassis.angle/360*2*3.14159);
	//chassisSetSpeed(0, speed, linetracker());
	if(vx>1000 || vy>1000 || vx<-1000 || vy<-1000) return;
	chassisSetState(vx, vy, chassis.angle + linetracker());
}
void angularVelocity_match_init(void)
{
	angularVelocity_match[0] = 300;
	angularVelocity_match[1] = 260;
	angularVelocity_match[2] = 230;
	angularVelocity_match[3] = 200;
	angularVelocity_match[4] = 170;
	angularVelocity_match[5] = 140;
	angularVelocity_match[6] = 110;
	angularVelocity_match[7] = 80;
	angularVelocity_match[8] = 40;
	angularVelocity_match[9] = 20;
	angularVelocity_match[10] = 0;
	angularVelocity_match[11] = -20;
	angularVelocity_match[12] = -40;
	angularVelocity_match[13] = -80;
	angularVelocity_match[14] = -110;
	angularVelocity_match[15] = -140;
	angularVelocity_match[16] = -170;
	angularVelocity_match[17] = -200;
	angularVelocity_match[18] = -230;
	angularVelocity_match[19] = -260;
	angularVelocity_match[20] = -300;
}

void angle_match_init(void)
{
	angle_match[0] = 20;
	angle_match[1] = 18;
	angle_match[2] = 16;
	angle_match[3] = 14;
	angle_match[4] = 12;
	angle_match[5] = 10;
	angle_match[6] = 8;
	angle_match[7] = 6;
	angle_match[8] = 4;
	angle_match[9] = 2;
	angle_match[10] = 0;
	angle_match[11] = -2;
	angle_match[12] = -4;
	angle_match[13] = -6;
	angle_match[14] = -8;
	angle_match[15] = -10;
	angle_match[16] = -12;
	angle_match[17] = -14;
	angle_match[18] = -16;
	angle_match[19] = -18;
	angle_match[20] = -20;
}

void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟 TIM3 使能
	
	//定时器 TIM3 初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载寄存器周期的值 24 - 1
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置时钟频率除数的预分频值 50000 - 1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化 TIM3
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE ); //允许更新中断
	
	//中断优先级 NVIC 设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //先占优先级 0 级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级 3 级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
	NVIC_Init(&NVIC_InitStructure); //④初始化 NVIC 寄存器
	TIM_Cmd(TIM3, DISABLE); //⑤使能 TIM3
}

void TIM3_IRQHandler(void) //TIM3 中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查 TIM3 更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //清除 TIM3 更新中断标志
		correspond(linetrackerSpeed);
		//sprintf(error_id, "%f, %f, %f\n",-linetrackerSpeed * sin(chassis.angle/360*2*3.14159),linetrackerSpeed * cos(chassis.angle/360*2*3.14159), chassis.angle + angle_match[10]);
		//uart4_send(error_id);
	}
}
