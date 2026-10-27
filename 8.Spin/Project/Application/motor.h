#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f1xx_hal.h"
#include "tim.h"


#define MOTOR_TIM        (&htim1)
#define MOTOR_CHANNEL    TIM_CHANNEL_1     // PWMA = PA8


#define MOTOR_IN1_PORT   GPIOB
#define MOTOR_IN1_PIN    GPIO_PIN_15       // AIN1
#define MOTOR_IN2_PORT   GPIOB
#define MOTOR_IN2_PIN    GPIO_PIN_14       // AIN2


#define MOTOR_PWM_MAX    7199


void Motor_Init(void);
void Motor_SetSpeed(int16_t speed);        // -100 ~ +100

#endif
