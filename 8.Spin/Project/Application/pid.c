#include "pid.h"

void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, float out_min, float out_max)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
    pid->out_min = out_min;
    pid->out_max = out_max;
}

float PID_Update(PID_TypeDef *pid, float target, float measured, float dt)
{
    float error = target - measured;
    float p_term, i_term, d_term, output;

    if (dt <= 0.0f)
        dt = 0.001f;

    /* accumulate the integral and clamp it to prevent windup */
    pid->integral += error * dt;
    if (pid->integral > pid->out_max) pid->integral = pid->out_max;
    if (pid->integral < pid->out_min) pid->integral = pid->out_min;

    p_term = pid->kp * error;
    i_term = pid->ki * pid->integral;
    d_term = pid->kd * (error - pid->prev_error) / dt;

    output = p_term + i_term + d_term;

    /* clamp the output */
    if (output > pid->out_max) output = pid->out_max;
    else if (output < pid->out_min) output = pid->out_min;

    pid->prev_error = error;
    return output;
}
