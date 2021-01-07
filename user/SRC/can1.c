#include "can1.h"

bool valve[7];

void CAN1_Init(void)
{
	CAN_InitTypeDef CAN_InitStruct;
	GPIO_InitTypeDef GPIO_InitStucture;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);					//ʹ��PORTAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);											//ʹ��CAN1ʱ��

	GPIO_InitStucture.GPIO_Mode=GPIO_Mode_AF_PP;												//��������
	GPIO_InitStucture.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStucture);
	
	GPIO_InitStucture.GPIO_Mode=GPIO_Mode_IPU;													//��������
	GPIO_InitStucture.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStucture);
	
	//GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
	
	CAN_InitStruct.CAN_Mode=CAN_Mode_Normal;													//ģʽ���ã�0����ͨģʽ��1���ػػ�ģʽ
	CAN_InitStruct.CAN_Prescaler=4;																//��Ƶϵ��(Fdiv)Ϊbrp+1 ???
	CAN_InitStruct.CAN_BS1=CAN_BS1_4tq;															//ʱ���1ռ��ʱ�䵥λ
	CAN_InitStruct.CAN_BS2=CAN_BS2_4tq;															//ʱ���2ռ��ʱ�䵥λ
	CAN_InitStruct.CAN_SJW=CAN_SJW_1tq;															//����ͬ����Ծ���
	CAN_InitStruct.CAN_ABOM=DISABLE;															//����Զ����߹���
	CAN_InitStruct.CAN_AWUM=DISABLE;															//˯��ģʽͨ���������
	CAN_InitStruct.CAN_NART=ENABLE;															//��ֹ�����Զ�����
	CAN_InitStruct.CAN_RFLM=DISABLE;															//���Ĳ��������µĸ��Ǿɵ�
	CAN_InitStruct.CAN_TTCM=DISABLE;															//��ʱ�䴥��ͨ��ģʽ
	CAN_InitStruct.CAN_TXFP=DISABLE;															//���ȼ��ɱ��ı�ʶ��ȷ��
	CAN_Init(CAN1,&CAN_InitStruct);
	
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;										//���������0
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;							//FIFO0
	CAN_FilterInitStructure.CAN_FilterIdHigh=((0x00030101<<3)&0xffff0000)>>16;					//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow=(0x00030101<<3)&0xffff;														
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(0xffff00<<3)>>16;								//32λMASK
	CAN_FilterInitStructure.CAN_FilterIdLow=(0xffff00<<3)&0xffff;														
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;     
	CAN_FilterInitStructure.CAN_FilterNumber=0;													//������0
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;								//32λ
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     								// �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;								            // �����ȼ�Ϊ0
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
