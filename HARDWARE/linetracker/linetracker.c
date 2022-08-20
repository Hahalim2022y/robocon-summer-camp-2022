#include "linetracker.h"

float angle_match[21];
float translationSpeed_match[21];
float lineTrackerSpeed;
float lineTrackerAngle;

void linetracker(void)
{
	u16 res = grayScaleSensor2_Read();
	int i;
	int sum = 0;
	int lightNum = 0;
	static int id = 11;
	
	for(i = 0; i < 11; i++)
	{
		if((res & (1 << i)) != 0)
		{
			sum += i * 2;
			lightNum ++;
		}
	}
	if(lightNum != 0)
	{
		id = sum / lightNum;
		sum = 0;
		lightNum = 0;
	}
	
	if(id % 2 == 0)
	{
		for(i = ( (id / 2 - 1) > 0 ? (id / 2 - 1) : 0 ); i <= ( (id / 2 + 1) < 10 ? (id / 2 + 1) : 10 ); i++)
		{
			if((res & (1 << i)) != 0)
			{
				sum += i * 2;
				lightNum ++;
			}
		}
	}
	else
	{
		for(i = ( (id / 2 - 1) > 0 ? (id / 2 - 1) : 0 ); i <= ( (id / 2 + 2) < 10 ? (id / 2 + 2) : 10 ); i++)
		{
			if((res & (1 << i)) != 0)
			{
				sum += i * 2;
				lightNum ++;
			}
		}
	}
	
	if(lightNum != 0)
	{
		id = sum / lightNum;
	}
	
	float angle = chassis.angle;
	if(angle > 360 || angle < -360) return;
	
	chassisSetState(-lineTrackerSpeed * sinf(angle / 180 * 3.14159)
		, lineTrackerSpeed * cosf(angle / 180 * 3.14159), angle + angle_match[id]);
}

void linetracker_translation(void)
{
	u16 res = grayScaleSensor2_Read();
	u8 i;
	int sum = 0;
	int lightNum = 0;
	static int id = 11;
	for(i = 0; i < 11; i++)
	{
		if((res & (1 << i)) != 0)
		{
			sum += i * 2;
			lightNum ++;
		}
	}
	if(lightNum != 0)
	{
		id = sum / lightNum;
		sum = 0;
		lightNum = 0;
	}
	
	if(id % 2 == 0)
	{
		for(i = ( (id / 2 - 1) > 0 ? (id / 2 - 1) : 0 ); i <= ( (id / 2 + 1) < 10 ? (id / 2 + 1) : 10 ); i++)
		{
			if((res & (1 << i)) != 0)
			{
				sum += i * 2;
				lightNum ++;
			}
		}
	}
	else
	{
		for(i = ( (id / 2 - 1) > 0 ? (id / 2 - 1) : 0 ); i <= ( (id / 2 + 2) < 10 ? (id / 2 + 2) : 10 ); i++)
		{
			if((res & (1 << i)) != 0)
			{
				sum += i * 2;
				lightNum ++;
			}
		}
	}
	
	if(lightNum != 0)
	{
		id = sum / lightNum;
	}
	
	float angle = chassis.angle;
	if(angle > 360 || angle < -360) return;
	
	chassisSetState(-lineTrackerSpeed * sinf(angle / 180 * 3.14159) 
		+ translationSpeed_match[id] * cosf(angle / 180 * 3.14159)
	
		, lineTrackerSpeed * cosf(angle / 180 * 3.14159)
		+ translationSpeed_match[id] * sinf(angle / 180 * 3.14159)
		
		, lineTrackerAngle);
}

void angle_match_init(void)
{
	angle_match[0] = 20;
	angle_match[1] = 18;
	angle_match[2] = 16;
	angle_match[3] = 14;
	angle_match[4] = 12;
	angle_match[5] = 10;
	angle_match[6] = 8;
	angle_match[7] = 6;
	angle_match[8] = 4;
	angle_match[9] = 2;
	angle_match[10] = 0;
	angle_match[11] = -2;
	angle_match[12] = -4;
	angle_match[13] = -6;
	angle_match[14] = -8;
	angle_match[15] = -10;
	angle_match[16] = -12;
	angle_match[17] = -14;
	angle_match[18] = -16;
	angle_match[19] = -18;
	angle_match[20] = -20;
}

void translationSpeed_match_init(void)
{
	translationSpeed_match[0] = -200;
	translationSpeed_match[1] = -180;
	translationSpeed_match[2] = -160;
	translationSpeed_match[3] = -140;
	translationSpeed_match[4] = -120;
	translationSpeed_match[5] = -100;
	translationSpeed_match[6] = -80;
	translationSpeed_match[7] = -60;
	translationSpeed_match[8] = -40;
	translationSpeed_match[9] = -20;
	translationSpeed_match[10] = 0;
	translationSpeed_match[11] = 20;
	translationSpeed_match[12] = 40;
	translationSpeed_match[13] = 60;
	translationSpeed_match[14] = 80;
	translationSpeed_match[15] = 100;
	translationSpeed_match[16] = 120;
	translationSpeed_match[17] = 140;
	translationSpeed_match[18] = 160;
	translationSpeed_match[19] = 180;
	translationSpeed_match[20] = 200;
}
