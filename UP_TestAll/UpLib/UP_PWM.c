/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  : ����Ԫ  
//  Update Date   : 2011/06/12
//  Version   	  : 1.2            
//  Description   :  
/******************************************************************/ 

#include "STM32Lib/stm32f10x.h"
#include "UP_PWM.h"

//�����궨��
#define UP_PWM_NUM		4					//PWMͨ����Ŀ

//����궨�壨ע���и�����ת��
#define UP_PWM0_L		(GPIOB->BSRR = GPIO_Pin_6)
#define UP_PWM0_H		(GPIOB->BRR = GPIO_Pin_6)
#define UP_PWM1_L		(GPIOB->BSRR = GPIO_Pin_7)
#define UP_PWM1_H		(GPIOB->BRR = GPIO_Pin_7)
#define UP_PWM2_L		(GPIOB->BSRR = GPIO_Pin_8)
#define UP_PWM2_H		(GPIOB->BRR = GPIO_Pin_8)	
#define UP_PWM3_L		(GPIOB->BSRR = GPIO_Pin_9)
#define UP_PWM3_H		(GPIOB->BRR = GPIO_Pin_9)	

u32 g_UP_PWMPeriod = 24000;					//PWM���ڣ�����ֵ��
u16 g_UP_PWMDutyTime[UP_PWM_NUM] = {2048,2048,2048,2048};		//PWMռ�ձ�
u8  g_UP_bPWMMode[UP_PWM_NUM] = {0};		//�Ƿ���PWMģʽ
TIM_OCInitTypeDef  g_TIM_OCInitStructure; 	//��ʼ���Ƚ����õı���


//PWM��س�ʼ��
void UP_PWM_Init(void)	
{
	//Timer4��4������Ƚ���4·PWM
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//ʹ�ܶ˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//ʹ�ܶ�ʱ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	UP_PWM0_L;
	UP_PWM1_L;
	UP_PWM2_L;
	UP_PWM3_L;
	//GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//���츴�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50Mʱ���ٶ�
	//PB6,7,8,9��� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Timer4��������   
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Prescaler = 2;						//3��Ƶ��24MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
	TIM_TimeBaseStructure.TIM_Period = 24000;						//����ֵ24000��1KHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

	//�Ƚ����ͨ������
	g_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				//PWMģʽ1
	g_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//����ͨ����Ч
	g_TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//����ͨ����Ч
	g_TIM_OCInitStructure.TIM_Pulse = 12000; 							//50%ռ��ʱ��
	g_TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//�������
	g_TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;     //�����˵ļ���  
	g_TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	g_TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
 
//	TIM_OC1Init(TIM4,&g_TIM_OCInitStructure); 						//ͨ��1
//	TIM_OC2Init(TIM4,&g_TIM_OCInitStructure); 						//ͨ��2
//	TIM_OC3Init(TIM4,&g_TIM_OCInitStructure); 						//ͨ��3
//	TIM_OC4Init(TIM4,&g_TIM_OCInitStructure); 						//ͨ��4
	//TIM4����ʹ��
	TIM_Cmd(TIM4,ENABLE);
	//TIM4 PWM���ʹ��
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
}

//����ռ��ʱ��0~4095
void UP_PWM_SetDutyTime(u8 Channel, u16 DutyTime)	
{
	if(Channel > UP_PWM_NUM || DutyTime > 4095)
		return;
	g_UP_PWMDutyTime[Channel] = DutyTime;
	switch(Channel)
	{
	case 0:
		TIM4->CCR1 = (g_UP_PWMPeriod*DutyTime)/4095; break;
	case 1:
		TIM4->CCR2 = (g_UP_PWMPeriod*DutyTime)/4095; break;
	case 2:
		TIM4->CCR3 = (g_UP_PWMPeriod*DutyTime)/4095; break;
	case 3:
		TIM4->CCR4 = (g_UP_PWMPeriod*DutyTime)/4095; break;
	default: 
		break;
	}
}

//����Ƶ��
void UP_PWM_SetFrequency(u32 Frequency)
{
	u16 temp = 0;
	if(Frequency != 0)
	{
		TIM4->PSC = temp = 2000/Frequency;		//��Ƶֵ��Ŀ��Ƶ��Խ�ͷ�ƵԽ��
		TIM4->ARR = g_UP_PWMPeriod = 72000000/(temp+1)/Frequency;	//����ֵ
		temp = g_UP_PWMPeriod>>1;
		TIM4->CCR1 = temp;			//ռ��ʱ��Ĭ��50%
		TIM4->CCR2 = temp;
		TIM4->CCR3 = temp;
		TIM4->CCR4 = temp;
	}
}

//����IO���
void  UP_PWM_SetIO(u8 Channel, u8 Value)
{
	if(Channel > UP_PWM_NUM || g_UP_bPWMMode[Channel] == 1)
		return;
	switch(Channel)								//����Ӧ�Ķ˿����ó���Ҫ�ĵ�ƽ
	{
		case 0: Value?UP_PWM0_H:UP_PWM0_L;break;
		case 1: Value?UP_PWM1_H:UP_PWM1_L;break;
		case 2: Value?UP_PWM2_H:UP_PWM2_L;break;
		case 3: Value?UP_PWM3_H:UP_PWM3_L;break;	
		default: break;
	}
}

//ʹ��PWMģʽ���
void  UP_PWM_Enable(u8 Channel, FunctionalState NewState)
{  
	if(Channel > UP_PWM_NUM)
		return;
	if(NewState == ENABLE)
	{
		g_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//����ͨ����Ч
		g_TIM_OCInitStructure.TIM_Pulse = (g_UP_PWMPeriod*g_UP_PWMDutyTime[Channel])/4095; 	//ռ��ʱ��
		g_UP_bPWMMode[Channel] = 1;
	}
	else
	{
		g_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; 	//����ͨ����Ч
		g_UP_bPWMMode[Channel] = 0;
		UP_PWM_SetIO(Channel,0);
	}
	switch(Channel)
	{
	case 0:
		TIM_OC1Init(TIM4,&g_TIM_OCInitStructure); break;
	case 1:
		TIM_OC2Init(TIM4,&g_TIM_OCInitStructure); break;
	case 2:
		TIM_OC3Init(TIM4,&g_TIM_OCInitStructure); break;
	case 3:
		TIM_OC4Init(TIM4,&g_TIM_OCInitStructure); break;
	default: 
		break;
	}
}					

