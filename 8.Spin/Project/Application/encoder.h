#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f1xx_hal.h"

/* Encoder / gearbox parameters, used to convert raw counts into angular speed */
#define ENCODER_PPR      13                                    /* encoder pulses per revolution (single channel) */
#define ENCODER_MULT     4                                     /* TIM encoder mode TI12 = x4 decoding */
#define GEAR_RATIO       30                                    /* reduction ratio: motor shaft : output shaft = 30 : 1 */
#define ENCODER_CPR      (ENCODER_PPR * ENCODER_MULT * GEAR_RATIO)  /* counts per output revolution = 1560 */

void     Encoder_Init(void);            /* start TIM4 encoder counting and reset the counter */
int16_t  Encoder_ReadDelta(void);       /* read the signed count between two calls and reset it */
float    Encoder_GetSpeed(float dt);    /* output-shaft angular speed in rad/s, dt = sampling interval in seconds */

#endif
