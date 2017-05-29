/******************************************************************/
//	Copyright (C), 2011-2012, �������� 
//  Author   	  	: ����Ԫ  
//  Reviser				: �����
//  Update Date   : 2012/08/01
//  Version   	  : 1.3          
//  Description   : Transplant to v3.5 function library
/******************************************************************/ 

#include "STM32Lib/stm32f10x.h"
#include "UP_ADC.h"

u16 g_ADValue[16] = {0};			//��ǰADCֵ

/*************************************************
  ��������: UP_ADC_DMAConfiguration
  ��������: ADC��DMA������
  �����������õĺ���: ��        
  ���ñ������ĺ���: ��    
  �������˵��: ��    
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_ADC_DMAConfiguration(void)
{
	DMA_InitTypeDef DMA_InitStructure;																//����DMA��ʼ���ṹ�����
	// ʹ��DMA1ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// DMAͨ��1����
	DMA_DeInit(DMA1_Channel1); 																				//��λDMA1ͨ��1	
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)( &(ADC1->DR));		//ADC1���ݼĴ��� 	   ������ DMAͨ���������ַ=ADC1_DR��
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_ADValue;					  //��ȡADC������ 	   ������DMAͨ���洢����ַ��
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;								//Ƭ��������Դͷ	   ��ָ������ΪԴ��ַ��
	DMA_InitStructure.DMA_BufferSize = 16;														//ÿ��DMA16������	   ������DMA��������СΪ16��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ������ 	   ����ǰ����Ĵ�����ַ���䣩
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ��ַ����  	   ����ǰ�洢����ַ���䣩
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//���� 	   �������������ݿ��16λ��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//����	   ������洢�����ݿ��16λ��
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										//��ͨģʽ				   ��DMAͨ������ģʽλ���λ���ģʽ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;								//�����ȼ�				   ��DMAͨ�����ȼ��ߣ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;											//���ڴ浽�ڴ�		 	 ����ֹDMAͨ���洢�����洢�����䣩
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);											//����������DMA1ͨ��1����ʼ��DMAͨ��1��
	
//	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);									//DMAͨ��1��������ж�
	
	DMA_Cmd(DMA1_Channel1, ENABLE);																		//ʹ��DMA1ͨ��1
}

/*************************************************
  ��������: UP_ADC_Init
  ��������: ADC��س�ʼ��
  �����������õĺ���: ��        
  ���ñ������ĺ���: ��    
  �������˵��: ��    
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_ADC_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;																//����ADC��ʼ���ṹ�����
	GPIO_InitTypeDef  GPIO_InitStructure;															//����GPIO��ʼ���ṹ�����

	UP_ADC_DMAConfiguration();																				//ADC DMA����
  // ����ADC 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);								//ʹ��ADC1ʱ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);							//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);							//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);							//ʹ��GPIOCʱ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;											//ѡ��GPIOģʽΪģ������
	// PA0~7����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|
																 GPIO_Pin_2|GPIO_Pin_3|
																 GPIO_Pin_4|GPIO_Pin_5|
																 GPIO_Pin_6|GPIO_Pin_7;							//ѡ��PA0~PA7
	GPIO_Init(GPIOA, &GPIO_InitStructure);													  //����PA0~PA7Ϊģ������
	// PB0~1����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;							//ѡ��PB0~PB1
	GPIO_Init(GPIOB, &GPIO_InitStructure);														//����PB0~PB1Ϊģ������
	// PC0~5����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|
																 GPIO_Pin_2|GPIO_Pin_3|
																 GPIO_Pin_4|GPIO_Pin_5;							//ѡ��PC0~PC5
	GPIO_Init(GPIOC, &GPIO_InitStructure);														//����PC0~PC5Ϊģ������

	// ADC1����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;								//����ͬ��ģʽ   ��ADC1��ADC2�����ڶ���ģʽ��
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;											//������ͨ��ģʽ ��ɨ��ģʽ<��ͨ��>��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;								//ADCת������������ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 16;													//16��ͨ��
	ADC_Init(ADC1, &ADC_InitStructure);																//��ʼ��ADC1
	
	// ����ͨ��16�Ĳ����ٶ�
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);   //ADC1ѡ���ŵ�9,�������ȼ�1,����ʱ��239.5������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 4, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 5, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 7, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 8, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 9, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 10, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 11, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 12, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 13, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 14, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 15, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 16, ADC_SampleTime_239Cycles5);
	
	// ʹ��ADC1��DMAģʽ 
	ADC_DMACmd(ADC1, ENABLE);
	
	// ʹ��ADC1
	ADC_Cmd(ADC1, ENABLE);
	
	//����ADC1У׼�Ĵ���    
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));		//�ȴ�ADC1У׼�������
	
	//��ʼADC1У׼״̬
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));				//�ȴ�ADC1У׼���

	// �˹���ADCת��. 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);				//ʹ��ADC1�����ʼת��
}

/*************************************************
  ��������: UP_ADC_GetValue
  ��������: ��ȡĳ·AD����ֵ
  �����������õĺ���: ��        
  ���ñ������ĺ���: ��    
  �������˵��: ����ͨ��0~15   
  �������˵��: ��       
  ��������ֵ: ����ADCֵ0~4095��Ӧ0~5V
  ����˵��:��        
*************************************************/				
u16 UP_ADC_GetValue(u8 Channel)
{
	if(Channel < 16)
		return g_ADValue[Channel];
	else
		return 0;
}

/*************************************************
  ��������: UP_ADC_GetIO
  ��������: ��ȡĳ·IO����ֵ
  �����������õĺ���: ��        
  ���ñ������ĺ���: ��    
  �������˵��: Channel����ͨ��0~15   
  �������˵��: ��       
  ��������ֵ: ����ֵ0��1
  ����˵��:��        
*************************************************/
u8 UP_ADC_GetIO(u8 Channel)
{
	return 	(g_ADValue[Channel] > 500)?1:0;
}

/*************************************************
  ��������: UP_ADC_GetAllIO
  ��������: ��ȡ16·IO����ֵ
  �����������õĺ���: ��        
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ����16·IO״̬��16λ�޷�������ÿһλ��Ӧһ·
  ����˵��:��        
*************************************************/
u16 UP_ADC_GetAllIO(void)		
{
	u16 temp = 0;
	u8 i;
	for(i=0; i<16; i++)
	{
		if(g_ADValue[i] > 2048)
			temp |= (0x01<<i);
	}
	return temp;	
}
