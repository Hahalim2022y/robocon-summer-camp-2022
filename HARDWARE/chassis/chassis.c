#include "chassis.h"

Chassis chassis;

//速度单位：mm/s
//转速单位：度/s
//转速单位：rpm
void chassisSetState(float vx, float vy, float targetAngle)
{
	//把mm/s转换成rpm
	chassis.translationSpeed_1 = (vx / 2 - vy * (1.732 / 2)) * 60 / (WHEEL_DIAMETER * 3.1415);
	chassis.translationSpeed_2 = (vx / 2 + vy * (1.732 / 2)) * 60 / (WHEEL_DIAMETER * 3.1415);
	chassis.translationSpeed_3 = -vx * 60 / (WHEEL_DIAMETER * 3.1415);
	
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
	chassis.world_x = 0;
	chassis.world_y = 0;
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

void position(void)
{
	float world_x=0,world_y=0,body_x=0,body_y=0;
	float w=0,vx=0,vy=0;
	float lastangle1 = motor[0].oriLastAngle / 8191.0 * 360 / 3591 * 187;
	float lastangle2 = motor[1].oriLastAngle / 8191.0 * 360 / 3591 * 187;
	float lastangle3 = motor[2].oriLastAngle / 8191.0 * 360 / 3591 * 187;
//	float x1 = motor[0].rpm * (motor[0].numOfTurns + motor[0].angle/360) * (WHEEL_DIAMETER * 3.1415) *(1.0/60000);
//	float x2 = motor[1].rpm * (motor[1].numOfTurns + motor[1].angle/360) * (WHEEL_DIAMETER * 3.1415)*(1.0/60000);
//	float x3 = motor[2].rpm * (motor[2].numOfTurns + motor[2].angle/360) * (WHEEL_DIAMETER * 3.1415)*(1.0/60000);
	float x1 = ((motor[0].absolutAngle - motor[0].LastabsolutAngle) / 360)* 2 * 3.14159 * WHEEL_RADIUS;
	float x2 = ((motor[1].absolutAngle - motor[1].LastabsolutAngle) / 360)* 2 * 3.14159 * WHEEL_RADIUS;
	float x3 = ((motor[2].absolutAngle - motor[2].LastabsolutAngle) / 360)* 2 * 3.14159 * WHEEL_RADIUS;
	float v1 = x1/(1.0/60000);
	float v2 = x2/(1.0/60000);
	float v3 = x3/(1.0/60000);
	//double 


//  w为角速度，逆时针为正
//	v1 = vx * (1/2) - vy * (1.732/2) - w * CHASSIS_RADIUS;
//	v2 = vx * (1/2) + vy * (1.732/2) - w * CHASSIS_RADIUS;
//	v3 = -vx - w * CHASSIS_RADIUS;
	w = -(v1 + v2 + v3) / (3 * CHASSIS_RADIUS);
	vx = (v1 + v2 - 2*v3) / 3;
	vy = (-v1 + v2) / 2;
	
	body_x = (x1 + x2 - 2*x3) / 3;
	body_y = (-x1 + x2) / 2;
	
	world_x = body_x * cos(chassis.angle/360 * (2*3.14159)) - body_y * sin(chassis.angle/360 * (2*3.14159));
	world_y = body_x * sin(chassis.angle/360 * (2*3.14159)) + body_y * cos(chassis.angle/360 * (2*3.14159));
	
	chassis.world_x += world_x;
	chassis.world_y += world_y;

}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
	u8 i = 0, j = 0;
	
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	
	i = RxMessage.StdId - 0x201;
	
	for(j = 0; j < 8; j++)
	{
		can_motor_receive_databuff[i][j] = RxMessage.Data[j];
	}

	motorGetData(i);
	motorSpeedRing(&motor[i], i);
	position();
}
