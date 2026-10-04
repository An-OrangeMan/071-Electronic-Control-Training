#ifndef __SERVO_H__
#define __SERVO_H__

#include "stm32f1xx_hal.h"
#include "tim.h"         


#define SERVO_TIM        (&htim2)          
#define SERVO_CHANNEL    TIM_CHANNEL_2    


#define SERVO_MIN_PULSE  500
#define SERVO_MAX_PULSE  2500
#define SERVO_MAX_ANGLE  180


void Servo_Init(void);
void Servo_SetAngle(uint8_t angle);

#endif
