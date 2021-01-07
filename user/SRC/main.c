#include "main.h"

void IO_Init(void);

int main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    IO_Init();
	//PWM_Init(8999,159);
    CAN1_Init();

  for(int i=0;i<7;i++) valve[i]=0;
	while(1)
	{
    if(!valve[0]) valve1ON else valve1OFF
    if(!valve[1]) valve2ON else valve2OFF
    if(!valve[2]) valve3ON else valve3OFF
    if(!valve[3]) valve4ON else valve4OFF
#ifdef Use_PWM
	PWM_Move(1);
	PWM_Move(2);
	PWM_Move(3);
	PWM_Move(4);
#else	
    if(!valve[4]) valve5ON else valve5OFF
    if(!valve[5]) valve6ON else valve6OFF
    if(!valve[6]) valve7ON else valve7OFF
#endif
	delay_ms(100);
	}
} 

void IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
