#include "pid.h"

void pidInit(PidStruct *pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->error = 0;
	pid->last_error = 0;
	pid->error_sum = 0;
}

float pidOutput(PidStruct *pid, float target, float current)
{
	float res = 0;
	pid->error = target - current;
	if(pid->error > 0.1 || pid->error < -0.1) pid->error_sum += pid->error;
	res = pid->kp * pid->error + pid->ki * pid->error_sum + pid->kd * (pid->error - pid->last_error);
	pid->last_error = pid->error;
	return res;
}
