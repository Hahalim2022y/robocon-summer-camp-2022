#ifndef _LINETRACKER_h_
#define _LINETRACKER_h_

#include <math.h>
#include "grayScaleSensor.h"
#include "chassis.h"

void linetracker(void);
void linetracker_translation(void);
void angle_match_init(void);
void linetracker_translation(void);
void translationSpeed_match_init(void);

extern float lineTrackerSpeed;
extern float lineTrackerAngle;

//extern float angularVelocity_match[21];
extern float angle_match[21];
extern float translationSpeed_match[21];
extern int grayScaleSensor_id;

#endif
