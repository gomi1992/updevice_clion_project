/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  	: ����Ԫ  
//  Reviser				: �����
//  Update Date   : 2012/08/01
//  Version   	  : 1.3          
//  Description   : Transplant to v3.5 function library
//  Update Date   : 2013/07/09
//  Version   	  : 1.3.1          
//  Description   : modify the UP_Sev_SetALLIO(u8 Value) 
//  Update Date   : 2013/09/24
//  Version   	  : 1.4          
//  Description   : modify to IO out function
/******************************************************************/ 

#include "STM32Lib/stm32f10x.h"
#include "UP_IOout.h"

//��ر�������

u8  g_UP_IOVal[UP_IOout_NUM];					//IO�˿ڵ�ƽ

u8 UP_Buzzer_State=0;
u8 UP_Buzzer_Period = 0;
void UP_Set_Beer(u16 ms,u8 Value)	/***����������ʱ��***/
{
 UP_Buzzer_State=Value;
 UP_Buzzer_Period = ms;
}
/*************************************************
  ��������: UP_IOout_AllLow    
  ��������: ���ж˿�����͵�ƽ
  �����������õĺ���: ��        
  ���ñ������ĺ���: void UP_IOout_GPIOInit(void);   
  �������˵��: ��         
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_IOout_AllLow(void)
{
	UP_IOout0_L; UP_IOout1_L; UP_IOout2_L;
	UP_IOout3_L; UP_IOout4_L; UP_IOout5_L; 
}

/*************************************************
  ��������: UP_IOout_GPIOInit    
  ��������: ��ʼ��IO
  �����������õĺ���: void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
                      void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState);
                      void UP_IOout_AllLow(void);void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
  ���ñ������ĺ���: ��    
  �������˵��: ��         
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_IOout_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;			//����GPIO��ʼ���ṹ�����

	//��������CLOCK,��ʹ��GPIO֮ǰ����������Ӧ�˵�ʱ��
	RCC_APB2PeriphClockCmd(
		RCC_APB2Periph_GPIOA|
		RCC_APB2Periph_GPIOD|
		RCC_APB2Periph_GPIOE|
		RCC_APB2Periph_AFIO,
		ENABLE);

	//���ж˿���ӳ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);	//���Զ˿ڲ�����

	//��ʼ��IO���Ϊ0
	UP_IOout_AllLow();

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�

	//PA14,15��� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//PD0~1,PD3~4,���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*************************************************
  ��������: UP_IOout_Init    
  ��������: ��ʼ��IOout
  �����������õĺ���: void UP_IOout_GPIOInit(void)��       
  ���ñ������ĺ���: void UP_System_Init(void)��
  �������˵��: ��         
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_IOout_Init(void)
{
	UP_IOout_GPIOInit();		//��ʼ��IO
}

/*************************************************
  ��������: UP_IOout_SetIO    
  ��������: ����IO���
  �����������õĺ���: ��      
  ���ñ������ĺ���: ��
  �������˵��: Channel����Ӧ��6���˿�0~5
				        Value  �����ö˿ڵ������ƽ0/1       
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void  UP_IOout_SetIO(u8 Channel, u8 Value)
{
	if(Channel > UP_IOout_NUM || Value > 1)
		return;
	switch(Channel)								//����Ӧ�Ķ˿����ó���Ҫ�ĵ�ƽ
	{
		case 0: Value?UP_IOout0_H:UP_IOout0_L;break;
		case 1: Value?UP_IOout1_H:UP_IOout1_L;break;
		case 2: Value?UP_IOout2_H:UP_IOout2_L;break;
		case 3: Value?UP_IOout3_H:UP_IOout3_L;break;
		case 4: Value?UP_IOout4_H:UP_IOout4_L;break;
		case 5: Value?UP_IOout5_H:UP_IOout5_L;break;
		default: break;
	}
}

/*************************************************
  ��������: UP_IOout_SetALLIO   
  ��������: ͬʱ����6·ģ�����˿�IO�����ƽ��ֵ0x00~0x3F
  �����������õĺ���: ��      
  ���ñ������ĺ���: ��
  �������˵��: Value  �����ö˿ڵ������ƽ0/1       
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void  UP_IOout_SetALLIO(u8 Value)
{
	u8 i;
	for(i=0; i<8; i++)
	{
		g_UP_IOVal[i] = Value&0x01;
		switch(i)								//����Ӧ�Ķ˿����ó���Ҫ�ĵ�ƽ
		{
			case 0: Value?UP_IOout0_H:UP_IOout0_L;break;
			case 1: Value?UP_IOout1_H:UP_IOout1_L;break;
			case 2: Value?UP_IOout2_H:UP_IOout2_L;break;
			case 3: Value?UP_IOout3_H:UP_IOout3_L;break;
			case 4: Value?UP_IOout4_H:UP_IOout4_L;break;
			case 5: Value?UP_IOout5_H:UP_IOout5_L;break;	
			default: break;
		}
		Value >>= 1;	
	}
}						
