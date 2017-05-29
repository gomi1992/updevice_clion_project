/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  	: ����Ԫ  
//  Reviser				: �����
//  Update Date   : 2012/08/01
//  Version   	  : 1.3          
//  Description   : Transplant to v3.5 function library
//  Reviser				: �����
//  Update Date   : 2013/07/18
//  Version   	  : 1.3.1         
//  Description   : Change GPIO configuration for keys.
/******************************************************************/ 

#ifndef KEYS_H
#define KEYS_H

#include "STM32Lib/stm32f10x.h"

//������ʼ��
extern void UP_Key_Init(void);

//��������궨��
#define UP_Key_Down()	(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13))	//��
#define UP_Key_Up()		(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14))	//��
#define UP_Key_BACK()	(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12))	//����
#define UP_Key_OK()		(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15))	//ȷ��

#endif
