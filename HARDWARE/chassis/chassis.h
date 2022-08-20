#ifndef CHASSIS_H
#define CHASSIS_H

#include "motor3508.h"
#include "gyro.h"
#include "pid.h"
#include <math.h>

#define WHEEL_DIAMETER 150
#define WHEEL_RADIUS 75
#define CHASSIS_RADIUS 190

//三角形的底边为前
//从右上轮子开始逆时针算，编号为1，2，3
//轮子转动方向为从外向内看，逆时针为正方向
//底盘角度，以向前为0度，逆时针为正方向

typedef struct _Chassis 
{
	//底盘的世界坐标系位置
	//float world_x, world_y;
	
	//底盘当前角度
	float angle;
	float angleBias; //角度偏置
	
	//底盘上一次的角度，只能在USART2_IRQHandler()函数里使用，在其他地方无意义
	float lastAngle;
	
	int numOfTurns;
	
	//设定的角度
	float targetAngle;
	float smoothTargetAngle;
	
	//设定的平移速度
	float vx, vy;
	
	//计算的平移速度
	float translationSpeed_1, translationSpeed_2, translationSpeed_3;
	
	//计算的旋转速度
	float rotatingSpeed;
	
	//输出给速度环的速度
	float v1, v2, v3;
	
	//使能角度环
	u8 enableAngleRing;
	
	//角度环pid
	PidStruct AngleRing_pid;	
} Chassis;

extern Chassis chassis;

void chassisInit(void);
void chassisAngleRing(void);
void chassisSetState(float vx, float vy, float targetAngle);
void chassisGetPosition(void);
void chassisSetSpeed(float vx, float vy, float angularVelocity);

#endif
