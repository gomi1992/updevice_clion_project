/******************************************************************/
//	Copyright (C), 2011-2013, 北京博创 
//  Author   	  : 乔潇楠  
//  Update Date   : 2013/07/15
//  Version   	  : 1.0            
//  Reviser				: 王伟
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
/*********************************超声波的串口通信传输数据*******************************/
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
	UP_UART2_length(UP_ID_len,7);//发送长度
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

void   UP_USART2_type(u8  com, u8 type)//com取值为2，type取值为1，2，3，
{
	if(com==2)
	{
		switch(type)
		{
		case 1:
			UP_UART2_polling(UP_RXlen,118);//查询ID
		break;
		case 2:
			UP_USART2_long(UP_IID);//发送距离
		break;
		}
	}
}
void   UP_USART2_coreect_pID(u8  P_ID)//取值为0-15范围内
{
	UP_UART2_set_ID(P_ID);//设置ID
}

/*************************************************
  函数名称: UP_EnableUSR232IT
  函数功能: 设置波特率
  被本函数调用的函数: 无
  调用本函数的函数: 无
  输入参数说明: BaudRate可以是115200等
  输出参数说明: 无       
  函数返回值:  无
  其它说明: 无        
***************************************************/
void UP_EnableUSR232IT(u32 BaudRate)
{
	g_UP_USR232BaudRate = BaudRate;
	g_UP_bUSR232IT = TRUE;		
}
/*************************************************
  函数名称: UP_USR232_EnableIT
  函数功能: 使能USR232数据接收中断
  被本函数调用的函数: 无
  调用本函数的函数: 无
  输入参数说明: 无
  输出参数说明: 无       
  函数返回值:  无
  其它说明: 无        
***************************************************/ 
void UP_USR232_EnableIT(void)
{
	g_UP_bUSR232IT = TRUE;		
}

/*************************************************
  函数名称: UP_SetUSR232Hadler
  函数功能: 传递USR232数据接收中断入口函数
  被本函数调用的函数: 无
  调用本函数的函数: 无
  输入参数说明: ApplicationAddress可以定义成接收中断入口函数地址
  输出参数说明: 无       
  函数返回值: 无
  其它说明: 无        
***************************************************/ 
void UP_SetUSR232Hadler(void (*ApplicationAddress)(u32))
{
	if(ApplicationAddress != 0)
		g_UP_USR232ITAddress = (u32)ApplicationAddress;
}

/*************************************************
  函数名称: UP_USR232_Putc
  函数功能: USR232发送一个字符
  被本函数调用的函数: UP_UART5_Putc(unsigned char c);
  调用本函数的函数: 无
  输入参数说明: c代表的是发送的字符
  输出参数说明: 无       
  函数返回值: 无
  其它说明: 无        
***************************************************/ 
void UP_USR232_Putc(unsigned char c)
{
	UP_UART5_Putc(c);
} 

/*************************************************
  函数名称: UP_USR232_Puts
  函数功能: USR232发送一串字符
  被本函数调用的函数: UP_UART5_Puts(str);
  调用本函数的函数: 无
  输入参数说明: 可以发送字符串
  输出参数说明: 无       
  函数返回值: 无
  其它说明: 无        
***************************************************/ 	
void UP_USR232_Puts(char * str)
{
	UP_UART5_Puts(str);
}		
