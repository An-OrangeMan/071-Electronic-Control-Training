#include "servo.h"


void Servo_Init(void)
{
    HAL_TIM_PWM_Start(SERVO_TIM, SERVO_CHANNEL);
}


void Servo_SetAngle(uint8_t angle)
{
    uint16_t ccr;

    if (angle > SERVO_MAX_ANGLE)
        angle = SERVO_MAX_ANGLE;

    // 线性映射
    ccr = SERVO_MIN_PULSE + (uint16_t)((uint32_t)angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE) / SERVO_MAX_ANGLE);
    //angle 是 uint8_t，最大 255 当 angle = 180 时：180 × 2000 = 360000 超过 16 位最大值 65535 
    __HAL_TIM_SET_COMPARE(SERVO_TIM, SERVO_CHANNEL, ccr);
}
