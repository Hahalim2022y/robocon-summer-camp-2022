#ifndef LINETRACKER_LINEARREGRESSION_H
#define LINETRACKER_LINEARREGRESSION_H

#include "cycleArray.h"
#include <stm32f10x.h>
#include "grayScaleSensor.h"
#include "chassis.h"
#include "uart.h"
#include <math.h>

extern FCycleArray pointsInLine_x;
extern FCycleArray pointsInLine_y;

extern float xSum, ySum, xySum, xSquareSum;

void lineTracker_linearRegression_init(void);
void recordLightPos(float x, float y);
void lineTracker_linearRegression(float speed);

#endif
