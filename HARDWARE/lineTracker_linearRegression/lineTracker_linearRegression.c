#include "lineTracker_linearRegression.h"
/*
FCycleArray pointsInLine_x;
FCycleArray pointsInLine_y;

float xSum, ySum, xySum, xSquareSum;

float lightPos[11][2];

void lineTracker_linearRegression_init(void)
{
	fCycleArray_init(&pointsInLine_x);
	fCycleArray_init(&pointsInLine_y);
	
	xSum = 0;
	ySum = 0;
	xSum = 0;
	xSquareSum = 0;
	
	lightPos[0][0] = -108; lightPos[0][1] = 190; 
	lightPos[1][0] = -83; lightPos[1][1] = 190; 
	lightPos[2][0] = -58; lightPos[2][1] = 190; 
	lightPos[3][0] = -33; lightPos[3][1] = 190; 
	lightPos[4][0] = -8; lightPos[4][1] = 190; 
	lightPos[5][0] = 0; lightPos[5][1] = 190; 
	lightPos[6][0] = 8; lightPos[6][1] = 190; 
	lightPos[7][0] = 33; lightPos[7][1] = 190; 
	lightPos[8][0] = 58; lightPos[8][1] = 190; 
	lightPos[9][0] = 83; lightPos[9][1] = 190; 
	lightPos[10][0] = 108; lightPos[10][1] = 190;
}

void recordLightPos(float x, float y)
{
	float pop_x, pop_y;
	
	xSum += x;
	ySum += y;
	xySum += x * y;
	xSquareSum += x * x;
	
	fCycleArray_push(&pointsInLine_x, x);
	fCycleArray_push(&pointsInLine_y, y);
	
	while(fCycleArray_len(&pointsInLine_x) > 17 && fCycleArray_len(&pointsInLine_y) > 17)
	{
		pop_x = fCycleArray_pop(&pointsInLine_x);
		pop_y = fCycleArray_pop(&pointsInLine_y);
		
		xSum -= pop_x;
		ySum -= pop_y;
		xySum -= pop_x * pop_y;
		xSquareSum -= pop_x * pop_x;
	}
	
	if(fCycleArray_len(&pointsInLine_x) != fCycleArray_len(&pointsInLine_y)) uart4_send("ERROR!!!!!\n");
}

#include <stdio.h>

void lineTracker_linearRegression(float speed)
{
	static float lastChassis_x = 0, lastChassis_y = 0;
	static int last_lightNum = 0;
	static float targetAngle = 0, k = 0;
	
	
	u16 res = grayScaleSensor2_Read();
	int i;
	int lightNum = 0;
	
	for(i = 0; i < 11; i++)
	{
		if((res & (1 << i)) != 0)
		{
			lightNum++;
		}
	}
	
	if( (chassis.world_x - lastChassis_x) * (chassis.world_x - lastChassis_x) 
		+ (chassis.world_y - lastChassis_y) * (chassis.world_y - lastChassis_y) < 1600
		&& (lightNum - last_lightNum < 2 && lightNum - last_lightNum > -2) )
	{
		//如果灯数量变化小，并且位移也小，就返回
		return;
	}
	
	lastChassis_x = chassis.world_x;
	lastChassis_y = chassis.world_y;
	last_lightNum = lightNum;
	
	for(i = 5; i < 11; i++)
	{
		if((res & (1 << i)) != 0)
		{
			recordLightPos( chassis.world_x * 1.0175 + 114.55 + lightPos[i][0] * cosf(chassis.angle * 3.14159 / 180)
				- lightPos[i][1] * sinf(chassis.angle  * 3.14159 / 180),
				chassis.world_y * 1.0175 + 114.55 + lightPos[i][0] * sinf(chassis.angle * 3.14159 / 180) 
				+ lightPos[i][1] * cosf(chassis.angle * 3.14159 / 180) );
		}
	}
	
	for(i = 4; i >= 0; i--)
	{
		if((res & (1 << i)) != 0)
		{
			recordLightPos( chassis.world_x * 1.0175 + 114.55 + lightPos[i][0] * cosf(chassis.angle * 3.14159 / 180)
				- lightPos[i][1] * sinf(chassis.angle  * 3.14159 / 180),
				chassis.world_y * 1.0175 + 114.55 + lightPos[i][0] * sinf(chassis.angle * 3.14159 / 180) 
				+ lightPos[i][1] * cosf(chassis.angle * 3.14159 / 180) );
		}
	}
	
	if(fCycleArray_len(&pointsInLine_x) > 10 && fCycleArray_len(&pointsInLine_y) > 10)
	{
		float len = fCycleArray_len(&pointsInLine_x);
		k = ( xySum - xSum * ySum / len ) / ( xSquareSum - xSum * xSum / len );
		targetAngle = atanf(k) * 180 / 3.14159 - 90;
		if(targetAngle - chassis.angle > 100 || targetAngle - chassis.angle < -100)
		{
			targetAngle = targetAngle + 180;
		}
		chassisSetState(-speed * sinf(targetAngle * 3.14159 / 180), speed * cosf(targetAngle * 3.14159 / 180)
			, targetAngle);
		
		char b[30];
		sprintf(b, "%f\n", targetAngle);
		uart4_send(b);
	}
	else
	{
		chassisSetState(-speed * sinf(targetAngle * 3.14159 / 180), speed * cosf(targetAngle * 3.14159 / 180)
			, targetAngle);
	}
	
	return;
}
*/
