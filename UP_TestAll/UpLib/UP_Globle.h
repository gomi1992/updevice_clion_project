/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  	: ����Ԫ  
//  Reviser				: �����
//  Update Date   : 2012/08/01
//  Version   	  : 1.3          
//  Description   : Transplant to v3.5 function library
/******************************************************************/

#ifndef GLOBLE_H
#define GLOBLE_H

#include "STM32Lib/stm32f10x.h"

//������ر���	����1
extern bool g_UP_bBluetoothIT;			//�Ƿ������������ݽ����ж�
extern u32 g_UP_BluetoothBaudRate;	//�������ڲ�����
extern u32 g_UP_BluetoothITAddress;	//�����ж���ں���

//Zigbee��ر���	����4
extern bool g_UP_bZigbeeIT;					//�Ƿ�����Zigbee���ݽ����ж�
extern u32 g_UP_ZigbeeBaudRate;			//Zigbee���ڲ�����
extern u32 g_UP_ZigbeeITAddress;		//Zigbee�ж���ں���

//USR232��ر���	����5
extern bool g_UP_bUSR232IT;					//�Ƿ�����USR232���ݽ����ж�
extern u32 g_UP_USR232BaudRate;			//USR232���ڲ�����
extern u32 g_UP_USR232ITAddress;		//USR232�ж���ں���

//UP_Loader��ز���
extern bool g_UP_LoaderIT;					//�Ƿ�����UP_Loader���ݽ����ж�
extern u32  g_UP_LoaderBaudRate;			//UP_Loader������
extern u32  g_UP_LoaderITAddress;			//UP_Loader�ж���ں���

//��ʱ����ر���
extern bool g_UP_bTimerIT[4];				//�Ƿ����ô������ݽ����ж�
extern u32 g_UP_iTimerTime[4];			//��ʱ����
extern u32 g_UP_TimerITAddress;			//�����ж���ں���
extern u32 g_UP_Timer0ITAddress;
extern u32 g_UP_Timer1ITAddress;
extern u32 g_UP_Timer2ITAddress;
extern u32 g_UP_Timer3ITAddress;

//�ⲿ�ж���ز���
extern u32 g_UP_ExtiFlag;						//�ⲿ�ж�ʹ�ܵ�ͨ��
extern u32 g_UP_ExtiITAddress;			//�ⲿ�ж���ں���

//����
extern volatile u32 g_SysTickTimer;			//1ms��ȷϵͳʱ�Ӽ�ʱ�����������û������޸ģ�

//���ȫ�ֺ�������	
extern void UP_delay_us(u32 us);		//��usΪ��λ��ʱ
extern void UP_delay_ms(u32 ms);		//��msΪ��λ��ʱ
extern u32 abs(s32 i);							//����������ֵ
extern double fabs(double i);				//�󸡵�������ֵ

#endif

