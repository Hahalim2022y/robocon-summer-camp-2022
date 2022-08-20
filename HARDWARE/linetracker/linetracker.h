#ifndef _LINETRACKER_h_
#define _LINETRACKER_h_

#include <stm32f10x.h>
#include <stdio.h>
#include <math.h>
#include "grayScaleSensor.h"
#include "chassis.h"
//#include <timer.h>

void linetracker(void);
void linetracker_translation(void);
void angle_match_init(void);
void linetracker_translation(void);

extern float lineTrackerSpeed;
extern float lineTrackerAngle;
// float linetracker(void);
// void correspond(float speed);
// void angularVelocity_match_init(void);
// void angle_match_init(void);
// void TIM3_Init(u16 arr,u16 psc);

// extern float angularVelocity_match[21];
// extern float angle_match[21];
// extern float linetrackerSpeed;
// extern FCycleArray errorTester;

#endif
