/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  : ����Ԫ  
//  Update Date   : 2011/06/12
//  Version   	  : 1.2            
//  Description   :  
//  Update Date   : 2013/09/24
//  Version   	  : 1.2.1            
//  Description   :  temp modify UP_CDS_SetMode()'s delay
/******************************************************************/ 

#include "STM32Lib/stm32f10x.h"
#include "UP_CDS5500.h"
#include "UP_UART.h"
#include "UP_Globle.h"

#define CMD_READ			0x02		//���ֶ��������
#define CMD_WRITE			0x03		//���ֶ��д����
#define CMD_SWRITE		0x83		//���ֶ��ͬ��д����

/*************************************************
  ��������: UP_CDS_SetMode
  ��������: ���ģʽ���ã�ID�����ã���Χ1����253.
  �����������õĺ���: UP_Uart3_SendCDSFrame(u8 id, u8 len, u8 cmd, u8* pData);        
  ���ñ������ĺ���: ��    
  �������˵��: id�����ö����ID��mode��ָ���ö������ģʽ��
	              �������ó�CDS_SEVMODE��CDS_MOTOMODEģʽ��      
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_CDS_SetMode(u8 id, u8 mode)
{
	u8 buf[5];
	if(mode > 1)
		return;
	buf[0] = 0x06;
	if(mode == CDS_SEVMODE)
	{
		buf[1] = 0;		
		buf[2] = 0;
		buf[3] = 0xff;
		buf[4] = 0x03;
	}
	else if(mode == CDS_MOTOMODE)
	{
		buf[1] = 0;		
		buf[2] = 0;
		buf[3] = 0;
		buf[4] = 0;
	}
	UP_Uart3_SendCDSFrame(id,0x07,CMD_WRITE,buf);
	UP_delay_ms(30);
}

/*************************************************
  ��������: UP_CDS_SetAngle
  ��������: ���ö�ӦID�ŵĶ��ת��(0~1023��Ӧ0~300��)���ٶȣ�0~1023��Լ1Ȧ1s����
  �����������õĺ���: UP_Uart3_SendCDSFrame(u8 id, u8 len, u8 cmd, u8* pData);        
  ���ñ������ĺ���: ��    
  �������˵��: id�����ö����ID��mode��ָ���ö������ģʽ��
	              �������ó�CDS_SEVMODE��CDS_MOTOMODEģʽ��
                speed���ö���ٶȣ�0~1023��Լ1Ȧ1s��
  �������˵��: ��       
  ��������ֵ: �� 
  ����˵��:��        
*************************************************/
void UP_CDS_SetAngle(u8 id, u16 angle, u16 speed)
{
	u8 buf[5];
	if(angle > 1023 || speed > 1023)
		return;
	buf[0] = 0x1E;
	buf[1] = angle;		
	buf[2] = angle>>8;
	buf[3] = speed;
	buf[4] = speed>>8;
	UP_Uart3_SendCDSFrame(id,0x07,CMD_WRITE,buf);
}
/*************************************************
  ��������: UP_CDS_SetAngleTimes
  ��������: ���ö�ӦID�ŵĶ��ת��(0~1023��Ӧ0~300��)���ٶȣ�0~1023��Լ1Ȧ1s����Ȧ��times 0-254
  �����������õĺ���: UP_Uart3_SendCDSFrame(u8 id, u8 len, u8 cmd, u8* pData);        
  ���ñ������ĺ���: ��    
  �������˵��: id�����ö����ID��mode��ָ���ö������ģʽ��
	              �������ó�CDS_SEVMODE��CDS_MOTOMODEģʽ��
                speed���ö���ٶȣ�0~1023��Լ1Ȧ1s��
  �������˵��: ��       
  ��������ֵ: �� 
  ����˵��:��        
*************************************************/
void UP_CDS_SetAngleTimes(u8 id, u16 angle, s16 speed,u8 times)
{
	u8 buf[3],buf1[2],buf2[5];
	u16 temp;
	if(angle > 1023 || speed<-1023||speed > 1023)
		return;
	temp = speed>0?speed:-speed;
	buf[0] = 0x20;
	buf[1] = temp;
	buf[2] = temp>>8;
	if(speed < 0)
	buf[2] |= 0x04;
	UP_Uart3_SendCDSFrame(id,0x05,CMD_WRITE,buf);
	UP_delay_ms(8);
	UP_Uart3_SendCDSFrame(id,0x05,CMD_WRITE,buf);
	UP_delay_ms(8);
	if(times != 0)
	{	
		buf1[0] = 0x2d; 
		buf1[1] = times;
		UP_Uart3_SendCDSFrame(id,0x04,CMD_WRITE,buf1);
		UP_delay_ms(8);	
		buf[0] = 0x1E;
		buf[1] = angle;		
		buf[2] = angle>>8;
		UP_Uart3_SendCDSFrame(id,0x05,CMD_WRITE,buf);
	}
	else
	{
	 	if(angle > 1023 || speed > 1023)
		return;
		buf2[0] = 0x1E;
		buf2[1] = angle;		
		buf2[2] = angle>>8;
		buf2[3] = speed;
		buf2[4] = speed>>8;
		UP_Uart3_SendCDSFrame(id,0x07,CMD_WRITE,buf2); 
	}
	 UP_delay_ms(3);
}

/*************************************************
  ��������: UP_CDS_SetSpeed
  ��������: ���ö�ӦID�ŵĵ���ٶȣ�0~1023��Լ1Ȧ1s����
  �����������õĺ���: UP_Uart3_SendCDSFrame(u8 id, u8 len, u8 cmd, u8* pData);        
  ���ñ������ĺ���: ��    
  �������˵��: id�����õ��ģʽ��ID��
	              speed���ö���ٶȣ�0~1023��Լ1Ȧ1s��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_CDS_SetSpeed(u8 id, s16 speed)
{
	u8 buf[5];
	u16 temp;
	if(speed > 1023 || speed < -1023)
		return;
	temp = speed>0?speed:-speed;
	buf[0] = 0x20;
	buf[1] = temp;
	buf[2] = temp>>8;
	if(speed < 0)
		buf[2] |= 0x04;
	UP_Uart3_SendCDSFrame(id,0x05,CMD_WRITE,buf);
}

/*************************************************
  ��������: UP_CDS_Set4MotoSpeed
  ��������: ͬʱ���ö�ӦID��4��������ٶȣ����Ե��ô˺�������������ǰ�������ˡ���ת����ת�Ķ������ԡ�
  �����������õĺ���: �� 
  ���ñ������ĺ���: ��    
  �������˵��:  id[4]��������4�����ģʽ��ID��ͬ,  speed[4]���ö�ӦID��4��������ٶȡ�  
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
*************************************************/
void UP_CDS_Set4MotoSpeed(u8 id[4], s16 speed[4])
{
	u8 i;
	u8 pos = 2;
	u8 buf[20];
	u16 temp;
	for(i=0; i<4; i++)
	{
		if(speed[i] > 1023 || speed[i] < -1023)
			return;
	}
	buf[0] = 0x20; 		//��ַ
	buf[1] = 0x02; 		//����
	for(i=0; i<4; i++)
	{
		temp = speed[i]>0?speed[i]:-speed[i];
		buf[pos++] = id[i];	//ID
		buf[pos++] = temp;
		buf[pos++] = temp>>8;
		if(speed[i] < 0)
			buf[pos-1] |= 0x04;
	}
	UP_Uart3_SendCDSFrame(0xFE,pos+2,CMD_SWRITE,buf);
}
