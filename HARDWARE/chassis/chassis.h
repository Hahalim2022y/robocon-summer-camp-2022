#ifndef CHASSIS_H
#define CHASSIS_H

#include "motor3508.h"
#include "gyro.h"
#include "pid.h"
#include "math.h"

#define WHEEL_DIAMETER 150
#define WHEEL_RADIUS 75
#define CHASSIS_RADIUS 190

//三角形的底边为前
//从右上轮子开始逆时针算，编号为1，2，3
//轮子转动方向为从外向内看，逆时针为正方向
//底盘角度，以向前为0度，逆时针为正方向

typedef struct _Chassis 
{
	float world_x, world_y;
	float angle;
	float lastAngle;
	short numOfTurns;
	float targetAngle;
	float vx, vy;
	//平移速度
	float translationSpeed_1, translationSpeed_2, translationSpeed_3;
	float v1, v2, v3;
	float rotatingSpeed;
	PidStruct AngleRing_pid;	
} Chassis;

extern Chassis chassis;

void chassisInit(void);
void chassisAngleRing(void);
void chassisSetState(float vx, float vy, float targetAngle);
void position(void);


#endif
