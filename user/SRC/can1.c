#include "can1.h"

bool valve[7];

void CAN1_Init(void)
{
	CAN_InitTypeDef CAN_InitStruct;
	GPIO_InitTypeDef GPIO_InitStucture;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);					//使能PORTA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);											//使能CAN1时钟

	GPIO_InitStucture.GPIO_Mode=GPIO_Mode_AF_PP;												//复用推挽
	GPIO_InitStucture.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStucture);
	
	GPIO_InitStucture.GPIO_Mode=GPIO_Mode_IPU;													//上拉输入
	GPIO_InitStucture.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStucture);
	
	//GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
	
	CAN_InitStruct.CAN_Mode=CAN_Mode_Normal;													//模式设置：0，普通模式；1，回回环模式
	CAN_InitStruct.CAN_Prescaler=4;																//分频系数(Fdiv)为brp+1 ???
	CAN_InitStruct.CAN_BS1=CAN_BS1_4tq;															//时间段1占用时间单位
	CAN_InitStruct.CAN_BS2=CAN_BS2_4tq;															//时间段2占用时间单位
	CAN_InitStruct.CAN_SJW=CAN_SJW_1tq;															//重新同步跳跃宽度
	CAN_InitStruct.CAN_ABOM=DISABLE;															//软件自动离线管理
	CAN_InitStruct.CAN_AWUM=DISABLE;															//睡眠模式通过软件唤醒
	CAN_InitStruct.CAN_NART=ENABLE;															//禁止报文自动传送
	CAN_InitStruct.CAN_RFLM=DISABLE;															//报文不锁定，新的覆盖旧的
	CAN_InitStruct.CAN_TTCM=DISABLE;															//非时间触发通信模式
	CAN_InitStruct.CAN_TXFP=DISABLE;															//优先级由报文标识符确定
	CAN_Init(CAN1,&CAN_InitStruct);
	
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;										//激活过滤器0
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;							//FIFO0
	CAN_FilterInitStructure.CAN_FilterIdHigh=((0x00030101<<3)&0xffff0000)>>16;					//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow=(0x00030101<<3)&0xffff;														
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(0xffff00<<3)>>16;								//32位MASK
	CAN_FilterInitStructure.CAN_FilterIdLow=(0xffff00<<3)&0xffff;														
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;     
	CAN_FilterInitStructure.CAN_FilterNumber=0;													//过滤器0
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;								//32位
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     								// 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;								            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);		
}

static CanRxMsg rx_message;
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) != RESET)
	{
	  CAN_Receive(CAN1,CAN_FIFO0,&rx_message);
	  CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
		CAN_ClearFlag(CAN1, CAN_IT_FMP0);
		
		if((rx_message.IDE == CAN_Id_Extended)&&(rx_message.RTR == CAN_RTR_Data))
		{
      if(rx_message.Data[0]=='V' && rx_message.Data[1]=='L')
      {        
          valve[rx_message.Data[3]]=rx_message.Data[2];
	    }
      }
	}
}

void valveCtrl(u8 ID,bool statue){
	CanTxMsg tx_message;
	tx_message.ExtId = 0x00030101;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.IDE = CAN_Id_Extended;
	tx_message.DLC = 4;
	tx_message.Data[0] = 'V';
	tx_message.Data[1] = 'L';
	tx_message.Data[2] = statue;
	tx_message.Data[3] = ID;
	CAN_Transmit(CAN1,&tx_message);
}
