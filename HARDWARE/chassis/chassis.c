#include "chassis.h"

Chassis chassis;

//速度单位：mm/s
//转速单位：度/s
//转速单位：rpm
void chassisSetSpeed(float vx, float vy, float targetAngle)
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

void chassisInit()
{
	pidInit(&(chassis.AngleRing_pid), 0, 0, 0);
}