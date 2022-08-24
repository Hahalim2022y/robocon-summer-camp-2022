#ifndef MOTOR3508_H
#define MOTOR3508_H

#include <stm32f10x.h>
#include "can.h"
#include "pid.h"

typedef struct Motor3508_
{
	short numOfTurns;
	
	//电机旋转的绝对角度，可以超过360度
	//float absolutAngle;
	
	//电机上次的绝对角度，在USB_LP_CAN1_RX0_IRQHandler()无效，在其外有效
	//float LastabsolutAngle;
	
	//接收到的原始角度数据
	u16 oriAngle;
	
	//上次接收到的原始角度数据，在USB_LP_CAN1_RX0_IRQHandler()有效，在其外无效
	u16 oriLastAngle;
	
	//电机当前角度
	float angle;
	
	//原始rpm数据
	short oriRpm;
	
	//实际rpm数据
	float rpm;
	
	//目标rpm
	float targetRpm;
	
	float smoothTargetRpm;
	
	PidStruct pid;
	
	//标志是否已经接收到了数据
	u8 dataReceived;
} Motor3508;

extern Motor3508 motor[3];

void motorInit(void);
void motorGetData(u8 i);
void motorSendCurrent(void);
void motorSpeedRing(u8 id);
void motorSetTargetRpm(Motor3508 *motor, float targetRpm);

#endif
