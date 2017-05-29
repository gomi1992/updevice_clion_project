/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  : �����  
//  Update Date   : 2013/07/15
//  Version   	  : 1.0            
//  Reviser				: ��ΰ
//  Update Date   : 2014/08/18
//  Version   	  : 1.0.1          
//  Description   : The "UP_EnableUSR232IT(u32 BaudRate);" moves from UP_UART.c to UP_USR232.c
/******************************************************************/ 


#include "UP_USR232.h" 
#include "UP_UART.h"
#include "UP_Globle.h"
unsigned  char UP_IID=0;
unsigned  char UP_ID_len[20];
unsigned  char UP_RXlen[255]={
0x55 ,0xaa ,0x6f, 0x00, 0x04, 0x00, 0x72, 0x55 ,0xaa ,0x60, 0x00, 0x04, 0x00 ,0x63, 0x55, 0xaa ,	
0x61, 0x00, 0x04 ,0x00, 0x64 ,0x55, 0xaa, 0x62 ,0x00 ,0x04, 0x00 ,0x65, 0x55, 0xaa, 0x63 ,0x00, 	
0x04 ,0x00, 0x66, 0x55, 0xaa, 0x64, 0x00, 0x04, 0x00, 0x67, 0x55, 0xaa, 0x65,0x00, 0x04 ,0x00 ,	
0x68, 0x55, 0xaa, 0x66, 0x00, 0x04 ,0x00, 0x69, 0x55, 0xaa, 0x67, 0x00 ,0x04, 0x00, 0x6a, 0x55 ,	
0xaa, 0x68, 0x00, 0x04, 0x00, 0x6b, 0x55, 0xaa ,0x69 ,0x00, 0x04 ,0x00, 0x6c, 0x55 ,0xaa, 0x6a ,	
0x00, 0x04, 0x00, 0x6d, 0x55, 0xaa, 0x6b, 0x00, 0x04 ,0x00 ,0x6e, 0x55, 0xaa ,0x6c, 0x00, 0x04, 	
0x00, 0x6f, 0x55, 0xaa, 0x6d, 0x00 ,0x04, 0x00, 0x70, 0x55, 0xaa ,0x6e, 0x00, 0x04, 0x00 ,0x71, 	
0x55, 0xaa, 0x6f, 0x00, 0x04, 0x00, 0x72,};
unsigned  char UP_buffer_TX_ID_poling[64]={0x55,0xaa,0x6f,0x00,0x04,0x00,0x72,0x55,0xaa,0x60,0x00,0x04,0x00,0x63};
unsigned  char UP_ID_num[16]={0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f};
unsigned  char UP_ID_reset[20]={0x55, 0xAA, 0x00, 0x01, 0x04, 0x20, 0x00, 0x00  };
/*********************************�������Ĵ���ͨ�Ŵ�������*******************************/
void UP_UART2_Putc(unsigned char c)
{
	  //USART_ClearFlag(USART2,USART_FLAG_TC);
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC)== RESET );
}
void UP_UART2_length(unsigned char *buffer,u8 len)
{ 
		u8 i=0;
		for(i=0;i<len;i++)
		{
		 UP_UART2_Putc(buffer[i]);
		}

}
void   UP_USART2_long(unsigned  char  ID)
{
	UP_ID_len[0]=0x55;
	UP_ID_len[1]=0xAA;
	UP_ID_len[2]=ID;
	UP_ID_len[3]=0x00;
	UP_ID_len[4]=0x05;
	UP_ID_len[5]=0x00;
	UP_ID_len[6]=(UP_ID_len[0]+UP_ID_len[1]+UP_ID_len[2]+UP_ID_len[3]+UP_ID_len[4]+UP_ID_len[5])&0xff;
	//printf("%d\n",ID_len[6]);
	UP_UART2_length(UP_ID_len,7);//���ͳ���
}
void UP_UART2_polling(unsigned char *buffer,u8 len)
{
	u8  i=0;
	for(i=0;i<len;i++)
	{
	 UP_UART2_Putc(buffer[i]);
  }
}
void UP_UART2_set_ID(u8 i)
{
	UP_ID_reset[2]=UP_IID;
	UP_ID_reset[6]=UP_ID_num[i];
	UP_ID_reset[7]=(UP_ID_reset[0]+UP_ID_reset[1]+UP_ID_reset[2]+UP_ID_reset[3]+UP_ID_reset[4]+UP_ID_reset[5]+UP_ID_reset[6])&0xff;
//	printf("%d   %d\n",IID,ID_reset[7]);
 	UP_UART2_length(UP_ID_reset,8);
}

void   UP_USART2_type(u8  com, u8 type)//comȡֵΪ2��typeȡֵΪ1��2��3��
{
	if(com==2)
	{
		switch(type)
		{
		case 1:
			UP_UART2_polling(UP_RXlen,118);//��ѯID
		break;
		case 2:
			UP_USART2_long(UP_IID);//���;���
		break;
		}
	}
}
void   UP_USART2_coreect_pID(u8  P_ID)//ȡֵΪ0-15��Χ��
{
	UP_UART2_set_ID(P_ID);//����ID
}

/*************************************************
  ��������: UP_EnableUSR232IT
  ��������: ���ò�����
  �����������õĺ���: ��
  ���ñ������ĺ���: ��
  �������˵��: BaudRate������115200��
  �������˵��: ��       
  ��������ֵ:  ��
  ����˵��: ��        
***************************************************/
void UP_EnableUSR232IT(u32 BaudRate)
{
	g_UP_USR232BaudRate = BaudRate;
	g_UP_bUSR232IT = TRUE;		
}
/*************************************************
  ��������: UP_USR232_EnableIT
  ��������: ʹ��USR232���ݽ����ж�
  �����������õĺ���: ��
  ���ñ������ĺ���: ��
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ:  ��
  ����˵��: ��        
***************************************************/ 
void UP_USR232_EnableIT(void)
{
	g_UP_bUSR232IT = TRUE;		
}

/*************************************************
  ��������: UP_SetUSR232Hadler
  ��������: ����USR232���ݽ����ж���ں���
  �����������õĺ���: ��
  ���ñ������ĺ���: ��
  �������˵��: ApplicationAddress���Զ���ɽ����ж���ں�����ַ
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
***************************************************/ 
void UP_SetUSR232Hadler(void (*ApplicationAddress)(u32))
{
	if(ApplicationAddress != 0)
		g_UP_USR232ITAddress = (u32)ApplicationAddress;
}

/*************************************************
  ��������: UP_USR232_Putc
  ��������: USR232����һ���ַ�
  �����������õĺ���: UP_UART5_Putc(unsigned char c);
  ���ñ������ĺ���: ��
  �������˵��: c������Ƿ��͵��ַ�
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
***************************************************/ 
void UP_USR232_Putc(unsigned char c)
{
	UP_UART5_Putc(c);
} 

/*************************************************
  ��������: UP_USR232_Puts
  ��������: USR232����һ���ַ�
  �����������õĺ���: UP_UART5_Puts(str);
  ���ñ������ĺ���: ��
  �������˵��: ���Է����ַ���
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
***************************************************/ 	
void UP_USR232_Puts(char * str)
{
	UP_UART5_Puts(str);
}		
