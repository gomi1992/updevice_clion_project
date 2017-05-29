/********************************************************************************                                         
 * @file     UP_Woody_VoicePlaying.c
 * @brief    �������Ź���
 * @details  1��Variable is not standardized��2��some fuction used last version ,
 *this version,We will not delete them,because of i am not sure if they will be use.
 *1:����汾�淶��ȫ�ֱ�����ʹ��2:Ԥ��δʹ�õ��ĺ���
 * @�������Ź���ʹ�û������̣���һ����ϵͳ��ʼ��֮ǰ����UP_Woody_Init();��ʼ��Woodyģ��
 * @�������Ź���ʹ�û������̣��ڶ���������UP_Woody_StartUp(0x02);����Woody�������Ź���,(һ���ֽڵĵڶ�λΪ1,0000 0010)
 * @�������Ź���ʹ�û������̣�������������UP_Woody_PlayingMusic(0);(�������Ϊ0��wav�ļ����������޸ķ�ΧΪ0-254)
 * @author   nealwang
 * @date     18/12/2015 
 * @version  v1.0 
 * @par Copyright (c): UpTech 
 * @par History: version: v0.8 
 ********************************************************************************/ 
#include"UP_System.h"
#include "UP_USR232.h" 
#include "UP_UART.h"
#include "UP_Globle.h"
#include "UP_Woody_VoicePlaying.h"
 /*************************************
 *��������
 **************************************/
 
 static u32 w[2]={0x00,0x00};
 static u32 data1;
/*************************************************
	��������: UP_Woody_PlayingMusic
  ��������: ����woody��������
  �����������õĺ���: void UP_USR232_Putc(unsigned char c);
  ���ñ������ĺ���: ��    
  �������˵��: u32 data1����Χ0-255�����Ŷ�Ӧ����id 0x00~0x31
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��  
*************************************************/
void UP_Woody_PlayingMusic(u32 data1)
{	 
	u32 sum=0x55+0xaa+0x07+0x20;
	w[0]=0x00;
	w[1]=data1;
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x07);
	UP_UART5_Putc(0x20);
	UP_UART5_Putc(w[0]);
	UP_UART5_Putc(w[1]);
	sum+=data1;
	UP_UART5_Putc(sum);
	UP_delay_ms(DELAY_TIMES);
}


