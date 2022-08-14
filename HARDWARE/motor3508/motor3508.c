#include "motor3508.h"
#include "uart.h"

Motor3508 motor[3];

void motorSpeedRing(u8 id)
{
	long targetCurrent;
	targetCurrent = (long)pidOutput(&(motor[id].pid), motor[id].targetRpm, motor[id].rpm);
	if(targetCurrent < -16384) targetCurrent = -16384;
	if(targetCurrent > 16384) targetCurrent = 16384;
	can_motor_send_databuff[0 + id * 2] = targetCurrent >> 8;
	can_motor_send_databuff[1 + id * 2] = targetCurrent;
	
	if(motor[0].dataReceived == 1 && motor[1].dataReceived == 1 && motor[2].dataReceived == 1)
	{
		motorSendCurrent();
		motor[0].dataReceived = 0;
		motor[1].dataReceived = 0;
		motor[2].dataReceived = 0;
	}
}

void motorSendCurrent(void)
{
	u8 mbox;
	u8 i = 0;
	CanTxMsg TxMessage;

	TxMessage.StdId = 0x200; // 标准标识符 
	TxMessage.IDE = CAN_Id_Standard; // 标准帧
	TxMessage.RTR = CAN_RTR_Data; // 数据帧
	TxMessage.DLC = 8; // 要发送的数据长度
	for(i = 0; i < 8; i++)
	{
		TxMessage.Data[i] = can_motor_send_databuff[i];
	}
	
	mbox = CAN_Transmit(CAN1, &TxMessage);   
	
	i = 0;
	while((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0Xff)) i++;	//等待发送结束
}	

void motorSetTargetRpm(Motor3508 *motor, float targetRpm)
{
	motor->targetRpm = targetRpm;
}

void motorInit(void)
{
	u8 i;
	for(i = 0; i < 3; i++)
	{
		motor[i].oriAngle = 0;
		motor[i].oriLastAngle = 0xffff;
		motor[i].absolutAngle = 0;
		motor[i].LastabsolutAngle = 0;
		motor[i].oriRpm = 0;
		motor[i].numOfTurns = 0;
		motor[i].angle = 0;
		motor[i].rpm = 0;
		motor[i].dataReceived = 0;
		pidInit(&motor[i].pid, 100, 0.3, 10);
	}
}

void motorGetData(u8 i)
{
	motor[i].oriAngle = 0;
	motor[i].oriAngle = motor[i].oriAngle | (can_motor_receive_databuff[i][0] << 8);
	motor[i].oriAngle = motor[i].oriAngle | can_motor_receive_databuff[i][1];
	
	if(motor[i].oriLastAngle == 0xffff) motor[i].oriLastAngle = motor[i].oriAngle;
	
	if(motor[i].oriLastAngle <= 2047 && motor[i].oriAngle >= 6144)
	{
		motor[i].numOfTurns -= 1;
	}
	if(motor[i].oriAngle <= 2047 && motor[i].oriLastAngle >= 6144)
	{
		motor[i].numOfTurns += 1;
	}
	
	motor[i].angle = motor[i].oriAngle / 8191.0 * 360 / 3591 * 187;
	motor[i].oriLastAngle = motor[i].oriAngle;

	motor[i].LastabsolutAngle = motor[i].absolutAngle;	
	motor[i].absolutAngle = (motor[i].numOfTurns * 360) / 3591 * 187 + motor[i].oriAngle;

	
	motor[i].oriRpm = 0;
	motor[i].oriRpm = motor[i].oriRpm | (can_motor_receive_databuff[i][2] << 8);
	motor[i].oriRpm = motor[i].oriRpm | can_motor_receive_databuff[i][3];
	motor[i].rpm = motor[i].oriRpm / 3591.0 * 187;
	
	motor[i].dataReceived = 1;
}


