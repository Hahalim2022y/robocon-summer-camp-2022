#include "linetracker.h"

float angle_match[21];

float linetracker(void)
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
	}
	return angle_match[id];
	
}

void correspond(float speed)
{
	chassisSetState(-speed * sin(chassis.angle/360*2*3.14159), speed * cos(chassis.angle/360*2*3.14159), chassis.angle + linetracker());
}

void angle_match_init(void)
{
	angle_match[0] = 10;
	angle_match[1] = 9;
	angle_match[2] = 8;
	angle_match[3] = 7;
	angle_match[4] = 6;
	angle_match[5] = 5;
	angle_match[6] = 4;
	angle_match[7] = 3;
	angle_match[8] = 2;
	angle_match[9] = 1;
	angle_match[10] = 0;
	angle_match[11] = -1;
	angle_match[12] = -2;
	angle_match[13] = -3;
	angle_match[14] = -4;
	angle_match[15] = -5;
	angle_match[16] = -6;
	angle_match[17] = -7;
	angle_match[18] = -8;
	angle_match[19] = -9;
	angle_match[20] = -10;
}
