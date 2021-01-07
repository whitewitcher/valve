#ifndef __CAN1_H
#define __CAN1_H

#include "stdbool.h"
#include "stm32f10x.h"
#include "stm32f10x_can.h"
#include "stm32f10x_rcc.h"

#define valve1ON  GPIO_SetBits(GPIOB, GPIO_Pin_1);
#define valve1OFF GPIO_ResetBits(GPIOB, GPIO_Pin_1);
#define valve2ON  GPIO_SetBits(GPIOB, GPIO_Pin_0);
#define valve2OFF GPIO_ResetBits(GPIOB, GPIO_Pin_0);
#define valve3ON  GPIO_SetBits(GPIOA, GPIO_Pin_3);
#define valve3OFF GPIO_ResetBits(GPIOA, GPIO_Pin_3);
#define valve4ON  GPIO_SetBits(GPIOA, GPIO_Pin_2);
#define valve4OFF GPIO_ResetBits(GPIOA, GPIO_Pin_2);
#define valve5ON  GPIO_ResetBits(GPIOA, GPIO_Pin_1);
#define valve5OFF GPIO_SetBits(GPIOA, GPIO_Pin_1);
#define valve6ON  GPIO_ResetBits(GPIOA, GPIO_Pin_6);
#define valve6OFF GPIO_SetBits(GPIOA, GPIO_Pin_6);
#define valve7ON  GPIO_ResetBits(GPIOA, GPIO_Pin_7);
#define valve7OFF GPIO_SetBits(GPIOA, GPIO_Pin_7);

void CAN1_Init(void);

extern bool valve[7];
void valveCtrl(u8 ID,bool statue);


#endif
