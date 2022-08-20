#ifndef _LINETRACKER_h_
#define _LINETRACKER_h_

#include <stm32f10x.h>
#include <math.h>
#include "grayScaleSensor.h"
#include "chassis.h"

void linetracker(void);
void linetracker_translation(void);
void angle_match_init(void);
void linetracker_translation(void);

extern float lineTrackerSpeed;
extern float lineTrackerAngle;

#endif
