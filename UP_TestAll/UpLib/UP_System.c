/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  	: ����Ԫ  
//  Reviser				: �����
//  Update Date   : 2012/08/01
//  Version   	  : 1.3          
//  Description   : Transplant to v3.5 function library
/******************************************************************/ 

#include "STM32Lib/stm32f10x.h"
#include "UP_System.h"
#include "UP_NVIC.h"

/*************************************************
  ��������: UP_System_Init   
  ��������: ��ʼ��ϵͳ����
  �����������õĺ���: SystemInit();UP_NVIC_Configuration();UP_Key_Init();UP_ADC_Init();UP_UART_Init();
                      UP_LCD_Init();UP_IOout_Init();UP_PWM_Init();UP_Exti_Init();UP_Timer_Init();UP_Bluetooth_Init();	
  ���ñ������ĺ���: ��    
  �������˵��: ��       
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_System_Init(void)
{
	
	SystemInit();							//��ʼ����
	UP_NVIC_Configuration();	//��ʼ���ж�
	UP_Key_Init();						//��ʼ������	
	UP_ADC_Init();						//��ʼ��ADC
	UP_UART_Init(); 					//��ʼ������
	UP_LCD_Init();						//��ʼ��LCD
	UP_IOout_Init();					//��ʼ��IO�����
	UP_PWM_Init();						//��ʼ��PWM
	UP_Exti_Init();						//��ʼ���ⲿ�ж�
	UP_Timer_Init();					//��ʼ����ʱ��
	UP_Bluetooth_Init();			//��ʼ������ģ��
}

/*************************************************
  ��������: UP_ITConfig   
  ��������: ʹ������
  �����������õĺ���: ��
  ���ñ������ĺ���: ��    
  �������˵��: ��       
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_ITConfig(u8 channel, u8 flag)
{
	
	
}


