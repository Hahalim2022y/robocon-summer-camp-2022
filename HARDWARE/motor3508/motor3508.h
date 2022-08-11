#ifndef MOTOR3508_H
#define MOTOR3508_H

#include <stm32f10x.h>
#include "can.h"
#include "pid.h"

typedef struct Motor3508_
{
	short numOfTurns;
	float absolutAngle;
	float LastabsolutAngle;
	u16 oriAngle;
	u16 oriLastAngle;
	float angle;
	short oriRpm;
	float rpm;
	float targetRpm;
	PidStruct pid;
	u8 dataReceived;
} Motor3508;

extern Motor3508 motor[3];

void motorInit(void);
void motorGetData(u8 i);
void motorSendCurrent(void);
void motorSpeedRing(Motor3508 *motor, u8 id);
void motorSetTargetRpm(Motor3508 *motor, float targetRpm);

#endif
