#include <stm32f10x.h>
#include "grayScaleSensor.h"
#include "gyro.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "chassis.h"
#include "linetracker.h"
#include "delay.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	u8 res;
	u16 gray_res;
	char command[100];
	char buff[100];
	u8 commandCnt = 0;
	u8 commandFinish = 0;
	
	char f4_command[100];
	u8 f4_commandCnt = 0;
	u8 f4_commandFinish = 0;
	
	can_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_3tq, 6, CAN_Mode_Normal);
	usart1Init(115200);
	usart2Init(115200);
	uart4Init(115200);
	motorInit();
	chassisInit();
	grayScaleSensor2_Init();
	angle_match_init();
	translationSpeed_match_init();
	delay_init();
	
	float world_speed = 0;
	float world_angle = 0;
	float body_angularVelocity = 0, body_angularVelocity_increase = 0;
	float body_speed_x = 0, body_speed_y = 0, body_speed_increase = 0;
	
	u8 linetracker_switch = 0;
	u8 translation_linetracker_switch = 0;
	u8 control_mode = 0; //0 : 世界坐标  1 : 机器人坐标
	u8 camp_color = 0; //0 : 蓝色  1 : 粉色
	u8 auto_grab_ball = 0; //0 : 手动操作和巡线  1 : 顶墙  2 : 后退  3 : 平移  4 : 顶板  5 : 平移  6 : 回撤
	
	int i;
	
	for(i = 0; i < 10000; i++)
	{
		chassis.angleBias = attitude.yaw;
		chassis.angle = 0;
		chassis.lastAngle = 361;
		chassis.numOfTurns = 0;
		grayScaleSensor_id = 10;
		lineTrackerAngle = 0;
		lineTrackerSpeed = 0;
	}
	
	while(1)
	{
		while(1)
		{
			if(uart4_read(&res) == 1)
			{
				if(res != '\n')
				{
					command[commandCnt] = res;
				}
				else
				{
					command[commandCnt] = '\0';
					commandFinish = 1;
					commandCnt = 0;
					break;
				}
			}
			else
			{
				break;
			}
			commandCnt++;
		}
		
		while(1)
		{
			if(uart1_read(&res) == 1)
			{
				if(res != '\n')
				{
					f4_command[f4_commandCnt] = res;
				}
				else
				{
					f4_command[f4_commandCnt] = '\0';
					f4_commandFinish = 1;
					f4_commandCnt = 0;
					break;
				}
			}
			else
			{
				break;
			}
			f4_commandCnt++;
		}
		
		if(commandFinish == 1)
		{
			uart4_send(command);
			uart4_send("\n");
			commandFinish = 0;
			
			if(strcmp(command, "world") == 0)
			{
				control_mode = 0;
			}
			else if(strcmp(command, "body") == 0)
			{
				control_mode = 1;
				chassisSetSpeed(0, 0 ,0);
			}
			else if(strcmp(command, "UP") == 0)
			{
				if(control_mode == 1)
				{
					body_speed_y += body_speed_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
				}
				else
				{
					if(camp_color == 0)
					{
						chassisSetState(-world_speed, 0, world_angle);
					}
					else if(camp_color == 1)
					{
						chassisSetState(world_speed, 0, world_angle);
					}
				}
			}
			else if(strcmp(command, "up") == 0)
			{
				if(control_mode == 1)
				{
					body_speed_y -= body_speed_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
				}
				else
				{
					chassisSetState(0, 0, world_angle);
				}
			}
			else if(strcmp(command, "DOWN") == 0)
			{
				if(control_mode == 1)
				{
					body_speed_y -= body_speed_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
				}
				else
				{
					if(camp_color == 0)
					{
						chassisSetState(world_speed, 0, world_angle);
					}
					else if(camp_color == 1)
					{
						chassisSetState(-world_speed, 0, world_angle);
					}
				}
			}
			else if(strcmp(command, "down") == 0)
			{
				if(control_mode == 1)
				{
					body_speed_y += body_speed_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
				}
				else
				{
					chassisSetState(0, 0, world_angle);
				}
			}
			else if(strcmp(command, "LEFT") == 0)
			{
				if(control_mode == 1)
				{
					body_angularVelocity += body_angularVelocity_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
				}
				else
				{
					if(camp_color == 0)
					{
						chassisSetState(0, -world_speed, world_angle);
					}
					else if(camp_color == 1)
					{
						chassisSetState(0, world_speed, world_angle);
					}
				}
			}
			else if(strcmp(command, "left") == 0)
			{
				if(control_mode == 1)
				{
					body_angularVelocity -= body_angularVelocity_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
				}
				else
				{
					chassisSetState(0, 0, world_angle);
				}
			}
			else if(strcmp(command, "RIGHT") == 0)
			{
				if(control_mode == 1)
				{
					body_angularVelocity -= body_angularVelocity_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
				}
				else
				{
					if(camp_color == 0)
					{
						chassisSetState(0, world_speed, world_angle);
					}
					else if(camp_color == 1)
					{
						chassisSetState(0, -world_speed, world_angle);
					}
				}
			}
			else if(strcmp(command, "right") == 0)
			{
				if(control_mode == 1)
				{
					body_angularVelocity += body_angularVelocity_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
				}
				else
				{
					chassisSetState(0, 0, world_angle);
				}
			}
			else if(strcmp(command, "linetracker") == 0)
			{
				grayScaleSensor_id = 10;
				linetracker_switch = !linetracker_switch;
				translation_linetracker_switch = 0;
				auto_grab_ball = 0;
				control_mode = 0;
				if(linetracker_switch)
				{
					uart4_send("linetracker on\n");
				}
				else
				{
					uart4_send("line tracker off\n");
					if(control_mode == 0)
					{
						chassisSetState(0, 0, chassis.angle);
					}
					else
					{
						chassisSetSpeed(0, 0, 0);
					}
					linetracker_switch = 0;
					translation_linetracker_switch = 0;
				}
			}
			else if(strcmp(command, "translation linetracker") == 0)
			{
				grayScaleSensor_id = 10;
				translation_linetracker_switch = !translation_linetracker_switch;
				linetracker_switch = 0;
				auto_grab_ball = 0;
				control_mode = 0;
				if(translation_linetracker_switch)
				{
					uart4_send("translation tracker on\n");
				}
				else
				{
					uart4_send("translation tracker off\n");
					if(control_mode == 0)
					{
						chassisSetState(0, 0, chassis.angle);
					}
					else
					{
						chassisSetSpeed(0, 0, 0);
					}
					linetracker_switch = 0;
					translation_linetracker_switch = 0;
				}
			}
			else if(strcmp(command, "reset") == 0)
			{
				chassis.angleBias = attitude.yaw;
				chassis.angle = 0;
				chassis.lastAngle = 361;
				chassis.numOfTurns = 0;
				grayScaleSensor_id = 10;
				lineTrackerAngle = 0;
				lineTrackerSpeed = 0;
			}
			else if(strstr(command, "speed") != NULL)
			{
				world_speed = atof(command + 5);
				lineTrackerSpeed = world_speed;
				body_speed_increase = world_speed;
			}
			else if(strstr(command, "angularVelocity") != NULL)
			{
				body_angularVelocity_increase = atof(command + 15);
			}
			else if(strcmp(command, "angle") == 0)
			{
				sprintf(buff, "%f\n", chassis.angle + chassis.numOfTurns * 360);
				uart4_send(buff);
			} 
			else if(strstr(command, "angle") != NULL)
			{
				world_angle = atof(command + 5);
				chassisSetState(0, 0, world_angle);
				lineTrackerAngle = world_angle;
			}
			else if(strstr(command, "gray") != NULL)
			{
				gray_res = grayScaleSensor2_Read();
				for(i = 0; i < 11; i++)
				{
					if((gray_res & (1 << i)) != 0)
					{
						uart4_send("1");
					}
					else
					{
						uart4_send("0");
					}
				}
				uart4_send("\n");
			}
			else if(strstr(command, "putdown") != NULL)
			{
				uart1_send("putdown\r\n");
			}
			else if(strstr(command, "flipped") != NULL)
			{
				uart1_send("flipped\r\n");
			}
			else if(strstr(command, "auto left") != NULL)
			{
				auto_grab_ball = 1;
				linetracker_switch = 0;
				translation_linetracker_switch = 0;
//				pidInit(&(motor[0].pid), 70, 0, 400);
//				pidInit(&(motor[1].pid), 70, 0, 400);
//				pidInit(&(motor[2].pid), 70, 0, 400);
				motor[0].pid.ki = 0;
				motor[1].pid.ki = 0;
				motor[2].pid.ki = 0;
			}
			else if(strstr(command, "auto right") != NULL)
			{
				auto_grab_ball = 3;
				linetracker_switch = 0;
				translation_linetracker_switch = 0;
//				pidInit(&(motor[0].pid), 70, 0, 400);
//				pidInit(&(motor[1].pid), 70, 0, 400);
//				pidInit(&(motor[2].pid), 70, 0, 400);
				motor[0].pid.ki = 0;
				motor[1].pid.ki = 0;
				motor[2].pid.ki = 0;
			}
			else if(strstr(command, "blue") != NULL)
			{
				camp_color = 0;
			}
			else if(strstr(command, "pink") != NULL)
			{
				camp_color = 1;
			}
			else
			{
				if(control_mode == 0)
				{
					chassisSetState(0, 0, chassis.angle);
				}
				else
				{
					chassisSetSpeed(0, 0, 0);
				}
				linetracker_switch = 0;
				translation_linetracker_switch = 0;
				//chassis.AngleRing_pid.error_sum = 0;
				body_speed_x = 0;
				body_speed_y = 0;
				body_angularVelocity = 0;
				auto_grab_ball = 0;
				//pidInit(&(motor[0].pid), 70, 0.04, 400);
				//pidInit(&(motor[1].pid), 70, 0.04, 400);
				//pidInit(&(motor[2].pid), 70, 0.04, 400);
			}
		}
		
		if(f4_commandFinish == 1)
		{
			f4_commandFinish = 0;
			if(auto_grab_ball == 0)
			{
				uart4_send(f4_command);
				uart4_send("\n");
			}
			
			if(strcmp(f4_command, "turnback") == 0)
			{	
				if(auto_grab_ball == 1)
				{
					auto_grab_ball = 2;
				}
				else if(auto_grab_ball == 3)
				{
					auto_grab_ball = 4;
				}					
			}
		}
		
		if(linetracker_switch == 1) 
		{
			linetracker();
		}
		if(translation_linetracker_switch == 1)
		{
			linetracker_translation();
		}
		
		if(auto_grab_ball == 1)
		{	
			if(camp_color == 0)
			{
				chassisSetState(30, 150, -90);
			}
			else if(camp_color == 1)
			{
				chassisSetState(-30, -150, 90);
			}
		}
		if(auto_grab_ball == 2)
		{
			if(camp_color == 0)
			{
				chassisSetState(-150, -150, -90);
				delay_ms(4000);
				uart1_send("flipped\r\n");
				chassisSetState(0, 0, -90);
				auto_grab_ball = 0;
			}
			else if(camp_color == 1)
			{
				chassisSetState(150, 150, 90);
				delay_ms(4000);
				uart1_send("flipped\r\n");
				chassisSetState(0, 0, 90);
				auto_grab_ball = 0;
			}
//			pidInit(&(motor[0].pid), 70, 0.04, 400);
//			pidInit(&(motor[1].pid), 70, 0.04, 400);
//			pidInit(&(motor[2].pid), 70, 0.04, 400);
			motor[0].pid.ki = 0.04;
			motor[1].pid.ki = 0.04;
			motor[2].pid.ki = 0.04;
			motor[0].pid.error_sum = 0;
			motor[1].pid.error_sum = 0;
			motor[2].pid.error_sum = 0;
		}
		
		if(auto_grab_ball == 3)
		{	
			if(camp_color == 0)
			{
				chassisSetState(30, -150, -90);
			}
			else if(camp_color == 1)
			{
				chassisSetState(-30, 150, 90);
			}
		}
		if(auto_grab_ball == 4)
		{
			if(camp_color == 0)
			{
				chassisSetState(-150, 150, -90);
				delay_ms(4000);
				uart1_send("flipped\r\n");
				chassisSetState(0, 0, -90);
				auto_grab_ball = 0;
			}
			else if(camp_color == 1)
			{
				chassisSetState(150, -150, 90);
				delay_ms(4000);
				uart1_send("flipped\r\n");
				chassisSetState(0, 0, 90);
				auto_grab_ball = 0;
			}
//			pidInit(&(motor[0].pid), 70, 0.04, 400);
//			pidInit(&(motor[1].pid), 70, 0.04, 400);
//			pidInit(&(motor[2].pid), 70, 0.04, 400);
			motor[0].pid.ki = 0.04;
			motor[1].pid.ki = 0.04;
			motor[2].pid.ki = 0.04;
			motor[0].pid.error_sum = 0;
			motor[1].pid.error_sum = 0;
			motor[2].pid.error_sum = 0;
		}
	}
}
