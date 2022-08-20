#include <stm32f10x.h>
#include "grayScaleSensor.h"
#include "gyro.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "chassis.h"
#include "linetracker.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	u8 res;
	char command[100];
	char buff[100];
	u8 commandCnt = 0;
	u8 commandFinish = 0;
	
	can_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_3tq, 6, CAN_Mode_Normal);
	usart1Init(115200);
	usart2Init(115200);
	uart4Init(115200);
	motorInit();
	chassisInit();
	grayScaleSensor2_Init();
	angle_match_init();
	translationSpeed_match_init();
	
	float world_speed = 0;
	float world_angle = 0;
	float body_angularVelocity = 0, body_angularVelocity_increase = 0;
	float body_speed_x = 0, body_speed_y = 0, body_speed_increase = 0;
	
	u8 linetracker_switch = 0;
	u8 translation_linetracker_switch = 0;
	u8 control_mode = 0; //0 : 世界坐标  1 : 机器人坐标
	
	int i;
	
	for(i = 0; i < 10000; i++)
	{
		chassis.angleBias = attitude.yaw;
		chassis.angle = 0;
		chassis.lastAngle = 361;
		chassis.numOfTurns = 0;
		grayScaleSensor_id = 10;
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
		if(commandFinish == 1)
		{
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
					chassisSetState(0, world_speed, world_angle);
				}
			}
			else if(strcmp(command, "up") == 0)
			{
				if(control_mode == 1)
				{
					body_speed_y -= body_speed_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
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
					chassisSetState(0, -world_speed, world_angle);
				}
			}
			else if(strcmp(command, "down") == 0)
			{
				if(control_mode == 1)
				{
					body_speed_y += body_speed_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
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
					chassisSetState(-world_speed, 0, world_angle);
				}
			}
			else if(strcmp(command, "left") == 0)
			{
				if(control_mode == 1)
				{
					body_angularVelocity -= body_angularVelocity_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
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
					chassisSetState(world_speed, 0, world_angle);
				}
			}
			else if(strcmp(command, "right") == 0)
			{
				if(control_mode == 1)
				{
					body_angularVelocity += body_angularVelocity_increase;
					chassisSetSpeed(body_speed_x, body_speed_y, body_angularVelocity);
				}
			}
			else if(strcmp(command, "linetracker") == 0)
			{
				grayScaleSensor_id = 10;
				linetracker_switch = !linetracker_switch;
				translation_linetracker_switch = 0;
				control_mode = 0;
			}
			else if(strcmp(command, "translation linetracker") == 0)
			{
				grayScaleSensor_id = 10;
				translation_linetracker_switch = !translation_linetracker_switch;
				linetracker_switch = 0;
				control_mode = 0;
			}
			else if(strcmp(command, "reset") == 0)
			{
				chassis.angleBias = attitude.yaw;
				chassis.angle = 0;
				chassis.lastAngle = 361;
				chassis.numOfTurns = 0;
				grayScaleSensor_id = 10;
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
			}
			
			uart4_send(command);
			uart4_send("\n");
			commandFinish = 0;
		}
		if(linetracker_switch == 1) 
		{
			linetracker();
		}
		if(translation_linetracker_switch == 1)
		{
			linetracker_translation();
		}
	}
}
