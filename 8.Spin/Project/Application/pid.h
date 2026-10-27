#ifndef __PID_H__
#define __PID_H__

typedef struct
{
    float kp;
    float ki;
    float kd;
    float integral;      /* accumulated integral term */
    float prev_error;    /* previous error, for the derivative term */
    float out_min;       /* output lower limit */
    float out_max;       /* output upper limit */
} PID_TypeDef;

void  PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, float out_min, float out_max);
float PID_Update(PID_TypeDef *pid, float target, float measured, float dt);

#endif
