#ifndef _PWM_
#define _PWM_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

void PWM_Init(int arr,int psc);
void PWM_Move(u8 id);

extern float pwm_ratio[4];

//#define Use_PWM

#endif
