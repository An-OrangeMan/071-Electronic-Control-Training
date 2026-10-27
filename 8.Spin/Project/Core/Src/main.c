/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "stdio.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Speed closed-loop control parameters */
#define SPEED_TARGET       0.4f     /* target angular speed, rad/s */
#define CONTROL_PERIOD_MS  200      /* control / sampling period, ms */
#define SPEED_KP           20.0f    /* proportional gain */
#define SPEED_KI           0.0f     /* no integral -> no slow-then-fast ramp */
#define SPEED_KD           0.0f     /* derivative gain */
#define SPEED_FILTER       0.2f     /* low-pass coefficient 0~1, smaller = smoother */
#define SPEED_FF           30.0f    /* feedforward duty %: the speed knob, adjust to hit 0.4 */
#define ENCODER_DIR        1.0f     /* 1 = normal, -1 = swap encoder A/B if spd reads negative */
#define MAX_PWM            50       /* hard cap, prevents high-speed runaway */
#define DISPLAY_DIV        3        /* refresh OLED every N control cycles */
/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t display_buf[20];

PID_TypeDef speed_pid;
float speed_measured = 0.0f;   /* raw encoder speed, rad/s */
float speed_filtered = 0.0f;   /* filtered speed, rad/s */
int16_t motor_pwm = 0;
uint32_t last_tick = 0;
uint32_t disp_cnt  = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();

  /* USER CODE BEGIN 2 */
  Motor_Init();
  Encoder_Init();
  PID_Init(&speed_pid, SPEED_KP, SPEED_KI, SPEED_KD, -100.0f, 100.0f);
  last_tick = HAL_GetTick();
  Motor_SetSpeed((int16_t)(SPEED_FF + SPEED_KP * SPEED_TARGET));   /* start at operating duty, no slow crawl */

  OLED_Init();
  OLED_Clear();
  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
    /* Fixed-period speed loop: measure -> low-pass filter -> PID -> motor PWM */
    if (HAL_GetTick() - last_tick >= CONTROL_PERIOD_MS)
    {
        float dt = (HAL_GetTick() - last_tick) / 1000.0f;
        last_tick = HAL_GetTick();

        speed_measured = ENCODER_DIR * Encoder_GetSpeed(dt);
        speed_filtered += SPEED_FILTER * (speed_measured - speed_filtered);

        motor_pwm = (int16_t)PID_Update(&speed_pid, SPEED_TARGET, speed_filtered, dt);
        motor_pwm += (int16_t)SPEED_FF;   /* feedforward keeps motor out of the friction dead-zone */
        if (motor_pwm < 0)       motor_pwm = 0;        /* always forward at this setpoint */
        if (motor_pwm > MAX_PWM) motor_pwm = MAX_PWM;  /* no high-speed runaway */
        Motor_SetSpeed(motor_pwm);

        if (++disp_cnt >= DISPLAY_DIV)
        {
            disp_cnt = 0;
            sprintf((char*)display_buf, "spd:%.3f", speed_filtered);
            OLED_ShowString(0, 0, display_buf, 16);
            sprintf((char*)display_buf, "pwm:%d", motor_pwm);
            OLED_ShowString(0, 2, display_buf, 16);
        }
    }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
