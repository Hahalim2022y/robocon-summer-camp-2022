#include "chassis.h"

Chassis chassis;

//速度单位：mm/s
//转速单位：度/s
//转速单位：rpm
void chassisSetState(float vx, float vy, float targetAngle)
{
	chassis.translationSpeed_1 = (vx * 2 - vy * 1.1547) * 60 / (WHEEL_DIAMETER * 3.1415);
	chassis.translationSpeed_2 = (vx * 2 + vy * 1.1547) * 60 / (WHEEL_DIAMETER * 3.1415);
	chassis.translationSpeed_3 = vx * 60 / (WHEEL_DIAMETER * 3.1415);
	
	//                       |  一秒底盘转几圈   |    乘旋转一圈轮子转动距离    |乘60秒|除轮子周长
	// = 一秒轮子滚多远 * 60 / 轮子周长
	// = 一分钟转几圈(rpm)
	//chassis->rotatingSpeed = rotatingSpeed / 360 * (CHASSIS_RADIUS * 2 * 3.1415) * 60 / (WHEEL_DIAMETER * 3.1415);
	chassis.targetAngle = targetAngle;
	
	chassis.v1 = chassis.translationSpeed_1 + chassis.rotatingSpeed;
	chassis.v2 = chassis.translationSpeed_2 + chassis.rotatingSpeed;
	chassis.v3 = chassis.translationSpeed_3 + chassis.rotatingSpeed;
	
	motorSetTargetRpm(&motor[0], chassis.v1);
	motorSetTargetRpm(&motor[1], chassis.v2);
	motorSetTargetRpm(&motor[2], chassis.v3);
}

void chassisAngleRing(void)
{
	//待检查可能有bug
	while(chassis.targetAngle >= chassis.angle + chassis.numOfTurns * 360.0 + 180) chassis.targetAngle -= 360;
	while(chassis.targetAngle < chassis.angle + chassis.numOfTurns * 360.0 - 180) chassis.targetAngle += 360;
	chassis.rotatingSpeed = pidOutput(&(chassis.AngleRing_pid), chassis.targetAngle, chassis.angle + chassis.numOfTurns * 360.0);
}

void chassisInit(void)
{
	pidInit(&(chassis.AngleRing_pid), 0, 0, 0);
	chassis.x = 0;
	chassis.y = 0;
	chassis.angle = 0;
	chassis.lastAngle = 361;
	chassis.numOfTurns = 0;
	chassis.targetAngle = 0;
	chassis.vx = 0;
	chassis.vy = 0;
	chassis.translationSpeed_1 = 0;
	chassis.translationSpeed_2 = 0;
	chassis.translationSpeed_3 = 0;
	chassis.v1 = 0;
	chassis.v2 = 0;
	chassis.v3 = 0;
	chassis.rotatingSpeed = 0;
}

void USART2_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE)) //必须是RXNE才能继续
	{
		res = USART_ReceiveData(USART2);
		if(gyro_unpack(res))
		{
			atkpParsing(&gyroDataFrame);
			if(gyroDataFrame.msgID == UP_ATTITUDE)
			{
				chassis.angle = -attitude.yaw;
				if(chassis.lastAngle == 361) chassis.lastAngle = chassis.angle;
				if(chassis.angle >= 90 && chassis.lastAngle <= -90)
				{
					chassis.numOfTurns--;
				}
				if(chassis.angle <= -90 && chassis.lastAngle >= 90)
				{
					chassis.numOfTurns++;
				}
				chassis.lastAngle = chassis.angle;
				chassisAngleRing();
			}
		}
	}
}
