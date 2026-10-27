#include "encoder.h"
#include "tim.h"

void Encoder_Init(void)
{
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
}

int16_t Encoder_ReadDelta(void)
{
    int16_t delta = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    return delta;
}

float Encoder_GetSpeed(float dt)
{
    if (dt <= 0.0f)
        return 0.0f;

    /* speed = (delta / CPR) * 2*pi / dt   ->   rad/s of the output shaft */
    return (float)Encoder_ReadDelta() * 2.0f * 3.14159265f / ((float)ENCODER_CPR * dt);
}
