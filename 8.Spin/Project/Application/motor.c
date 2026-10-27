#include "motor.h"


void Motor_Init(void)
{

    HAL_TIM_PWM_Start(MOTOR_TIM, MOTOR_CHANNEL);
    HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);

 
    __HAL_TIM_SET_COMPARE(MOTOR_TIM, MOTOR_CHANNEL, 0);
}


void Motor_SetSpeed(int16_t speed)
{
    uint16_t ccr;

    //限幅
    if (speed > 100)  speed = 100;
    if (speed < -100) speed = -100;

    if (speed > 0)          // 正转
    {
        HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
        ccr = (uint16_t)((uint32_t)speed * MOTOR_PWM_MAX / 100);
    }
    else if (speed < 0)     // 反转
    {
        HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_SET);
        ccr = (uint16_t)((uint32_t)(-speed) * MOTOR_PWM_MAX / 100);
    }
    else                    // 停止
    {
        HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
        ccr = 0;
    }


    __HAL_TIM_SET_COMPARE(MOTOR_TIM, MOTOR_CHANNEL, ccr);
}
