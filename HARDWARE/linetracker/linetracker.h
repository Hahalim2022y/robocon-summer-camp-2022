#ifndef _LINETRACKER_h_
#define _LINETRACKER_h_

#include <stm32f10x.h>
#include <math.h>
#include "grayScaleSensor.h"
#include "chassis.h"

float linetracker(void);
void correspond(float speed);
void angle_match_init(void);

extern float angle_match[21];

#endif
