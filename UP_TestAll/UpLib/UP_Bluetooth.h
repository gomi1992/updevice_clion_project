/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  : ����Ԫ  
//  Update Date   : 2011/06/12
//  Version   	  : 1.2            
//  Description   :  
/******************************************************************/

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "STM32Lib/stm32f10x.h"

//������غ���
extern void UP_Bluetooth_Init(void);				//������ʼ��
extern void UP_Bluetooth_EnableIT(void); 			//ʹ���������ݽ����ж�
extern void UP_Bluetooth_SetHadler(void (*ApplicationAddress)(u8));	//�����������ݽ����ж���ں���
extern void UP_Bluetooth_Putc(unsigned char c); 	//��������һ���ַ�
extern void UP_Bluetooth_Puts(char * str);			//��������һ���ַ�

#endif
