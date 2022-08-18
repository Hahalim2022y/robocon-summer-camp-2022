#include "chassis.h"

Chassis chassis;

//速度单位：mm/s
//转速单位：度/s
//转速单位：rpm
void chassisSetState(float vx, float vy, float targetAngle)
{
	float body_vx, body_vy;
	body_vx = vx * cos(-chassis.angle / 360 * (2 * 3.14159)) 
						- vy * sin(-chassis.angle / 360 * (2 * 3.14159));
	body_vy = vx * sin(-chassis.angle / 360 * (2 * 3.14159)) 
						+ vy * cos(-chassis.angle / 360 * (2 * 3.14159));
	
	//把mm/s转换成rpm	
	chassis.translationSpeed_1 = (body_vx / 2 - body_vy * (1.732 / 2)) * 60 / (WHEEL_DIAMETER * 3.1415);
	chassis.translationSpeed_2 = (body_vx / 2 + body_vy * (1.732 / 2)) * 60 / (WHEEL_DIAMETER * 3.1415);
	chassis.translationSpeed_3 = -body_vx * 60 / (WHEEL_DIAMETER * 3.1415);
	
	//                       |  一秒底盘转几圈   |    乘旋转一圈轮子转动距离    |乘60秒|除轮子周长
	// = 一秒轮子滚多远 * 60 / 轮子周长
	// = 一分钟转几圈(rpm)
	//chassis->rotatingSpeed = rotatingSpeed / 360 * (CHASSIS_RADIUS * 2 * 3.1415) * 60 / (WHEEL_DIAMETER * 3.1415);
	chassis.targetAngle = targetAngle;
	
	chassis.enableAngleRing = 1;
	
//	chassis.v1 = chassis.translationSpeed_1 + chassis.rotatingSpeed;
//	chassis.v2 = chassis.translationSpeed_2 + chassis.rotatingSpeed;
//	chassis.v3 = chassis.translationSpeed_3 + chassis.rotatingSpeed;
//	
//	motorSetTargetRpm(&motor[0], chassis.v1);
//	motorSetTargetRpm(&motor[1], chassis.v2);
//	motorSetTargetRpm(&motor[2], chassis.v3);
}

// 角速度单位：度每秒
void chassisSetSpeed(float vx, float vy, float angularVelocity)
{
//	float body_vx, body_vy;
//	body_vx = vx * cos(-chassis.angle / 360 * (2 * 3.14159)) 
//						- vy * sin(-chassis.angle / 360 * (2 * 3.14159));
//	body_vy = vx * sin(-chassis.angle / 360 * (2 * 3.14159)) 
//						+ vy * cos(-chassis.angle / 360 * (2 * 3.14159));
	
	//把mm/s转换成rpm	
	chassis.translationSpeed_1 = (vx / 2 - vy * (1.732 / 2)) * 60 / (WHEEL_DIAMETER * 3.1415);
	chassis.translationSpeed_2 = (vx / 2 + vy * (1.732 / 2)) * 60 / (WHEEL_DIAMETER * 3.1415);
	chassis.translationSpeed_3 = -vx * 60 / (WHEEL_DIAMETER * 3.1415);
	
	//                       |  一秒底盘转几圈   |    乘旋转一圈轮子转动距离    |乘60秒|除轮子周长
	// = 一秒轮子滚多远 * 60 / 轮子周长
	// = 一分钟转几圈(rpm)
	//chassis->rotatingSpeed = rotatingSpeed / 360 * (CHASSIS_RADIUS * 2 * 3.1415) * 60 / (WHEEL_DIAMETER * 3.1415);
	chassis.rotatingSpeed = angularVelocity / 6;
	
	chassis.enableAngleRing = 0;
	
	chassis.v1 = chassis.translationSpeed_1 + chassis.rotatingSpeed;
	chassis.v2 = chassis.translationSpeed_2 + chassis.rotatingSpeed;
	chassis.v3 = chassis.translationSpeed_3 + chassis.rotatingSpeed;
	
	motorSetTargetRpm(&motor[0], chassis.v1);
	motorSetTargetRpm(&motor[1], chassis.v2);
	motorSetTargetRpm(&motor[2], chassis.v3);
}

void chassisAngleRing(void)
{
	if(chassis.enableAngleRing == 0) return;
	
	while(chassis.targetAngle >= chassis.angle + chassis.numOfTurns * 360.0 + 180) chassis.targetAngle -= 360;
	while(chassis.targetAngle < chassis.angle + chassis.numOfTurns * 360.0 - 180) chassis.targetAngle += 360;
	
	if(chassis.smoothTargetAngle < chassis.targetAngle - 0.8)
	{
		chassis.smoothTargetAngle += 0.8;
	}
	else if(chassis.smoothTargetAngle > chassis.targetAngle + 0.8)
	{
		chassis.smoothTargetAngle -= 0.8;
	}
	else
	{
		chassis.smoothTargetAngle = chassis.targetAngle;
	}
	
	chassis.rotatingSpeed = -pidOutput(&(chassis.AngleRing_pid), chassis.smoothTargetAngle, chassis.angle + chassis.numOfTurns * 360.0);
	
	chassis.v1 = chassis.translationSpeed_1 + chassis.rotatingSpeed;
	chassis.v2 = chassis.translationSpeed_2 + chassis.rotatingSpeed;
	chassis.v3 = chassis.translationSpeed_3 + chassis.rotatingSpeed;
	
	motorSetTargetRpm(&motor[0], chassis.v1);
	motorSetTargetRpm(&motor[1], chassis.v2);
	motorSetTargetRpm(&motor[2], chassis.v3);
}

void chassisInit(void)
{
	pidInit(&(chassis.AngleRing_pid), 2.2, 0.0003, 100.0);// 0.45, 0.00012, 10.0
	chassis.world_x = 0;
	chassis.world_y = 0;
	chassis.angle = 0;
	chassis.angleBias = 0;
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
	chassis.enableAngleRing = 1;
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
				//chassis.angle = -attitude.yaw;
				chassis.angle = attitude.yaw - chassis.angleBias;
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

void chassisGetPosition(void)
{
	//每次接收到一个电机的数据都会调用这个函数，但是要三个电机都收到数据才能更新底盘
	if(motor[0].dataReceived == 0 || motor[1].dataReceived == 0 || motor[2].dataReceived == 0)
	{
		return;
	}
	
	//各个值的增量
	float world_x_increment = 0, world_y_increment = 0, body_x_increment = 0, body_y_increment = 0;
	
	//计算得到的速度（与设定的速度不同）
	//float angular_velocity = 0, vx = 0, vy = 0;
	
	//各个轮子的位移
	float x1 = ((motor[0].absolutAngle - motor[0].LastabsolutAngle) / 360) * 2 * 3.14159 * WHEEL_RADIUS;
	float x2 = ((motor[1].absolutAngle - motor[1].LastabsolutAngle) / 360) * 2 * 3.14159 * WHEEL_RADIUS;
	float x3 = ((motor[2].absolutAngle - motor[2].LastabsolutAngle) / 360) * 2 * 3.14159 * WHEEL_RADIUS;

//  w为角速度，逆时针为正
//	v1 = vx * (1/2) - vy * (1.732/2) - w * CHASSIS_RADIUS;
//	v2 = vx * (1/2) + vy * (1.732/2) - w * CHASSIS_RADIUS;
//	v3 = -vx - w * CHASSIS_RADIUS;
//	w = -(v1 + v2 + v3) / (3 * CHASSIS_RADIUS);
//	vx = (v1 + v2 - 2*v3) / 3;
//	vy = (-v1 + v2) /  1.732;
	
	body_x_increment = (x1 + x2 - 2 * x3) / 3;
	body_y_increment = (-x1 + x2) / 1.732;
	
	world_x_increment= body_x_increment * cos(chassis.angle  * (2 * 3.14159)/ 360) 
						- body_y_increment * sin(chassis.angle  * (2 * 3.14159)/ 360);
	world_y_increment = body_x_increment * sin(chassis.angle  * (2 * 3.14159)/ 360) 
						+ body_y_increment * cos(chassis.angle  * (2 * 3.14159)/ 360);
	
	chassis.world_x += world_x_increment;
	chassis.world_y += world_y_increment;

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
	chassisGetPosition();
	//速度环会清零motor[i].dataReceived
	motorSpeedRing(i);
}
