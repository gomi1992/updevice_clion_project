/******************************************************************/
//	Copyright (C), 2011-2012, �������� 
//  Author   	  : ����Ԫ  
//  Update Date   : 2011/06/12
//  Version   	  : 1.2            
//  Description   :  
//  Reviser				: �����
//  Update Date   : 2013/09/24
//  Version   	  : 1.3          
//  Description   : Change EXTI line Settings.
/******************************************************************/

#ifndef EXTI_H
#define EXTI_H

#include "STM32Lib/stm32f10x.h"

//�ⲿ�ж�ͨ���궨��
#define EXTI_DOWN		EXTI_Line13
#define EXTI_UP			EXTI_Line14
#define EXTI_BACK		EXTI_Line12
#define EXTI_OK			EXTI_Line15
#define EXTI_AD4		EXTI_Line7
#define EXTI_AD5		EXTI_Line6
#define EXTI_AD6		EXTI_Line5
#define EXTI_AD7		EXTI_Line4
#define EXTI_AD12		EXTI_Line3
#define EXTI_AD13		EXTI_Line2
#define EXTI_AD14		EXTI_Line1
#define EXTI_AD15		EXTI_Line0

//�ⲿ�ж���غ���
extern void UP_Exti_Init(void);				//�ⲿ�жϳ�ʼ��
extern void UP_Exti_EnableIT(u32 Channel, EXTITrigger_TypeDef TriggerMode);	//ʹ���ⲿ�ж�ͨ�����������ж�ģʽ
extern void UP_Exti_DisableIT(u32 Channel); 	//�����ⲿ�ж�
extern void UP_Exti_SetHadler(void (*ApplicationAddress)(u32));	//�����ⲿ�ж���ں���

#endif
