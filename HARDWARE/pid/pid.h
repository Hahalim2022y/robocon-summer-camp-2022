#ifndef PID_H
#define PID_H

typedef struct PidStruct_
{
	float kp, ki, kd;
	float error;
	float last_error;
	float error_sum;
} PidStruct;

void pidInit(PidStruct *pid, float kp, float ki, float kd);
float pidOutput(PidStruct *pid, float target, float current);

#endif
