#include "STM32Lib/stm32f10x.h"
#include "UP_LCD.h"

void UART2Init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
		
	//ʹ�ܴ���2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//ʹ��PD��AFIO����
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
	//���ж˿���ӳ��
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);	//URT2�ܽ���ӳ��

	//PD5 ��T2X
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  //���츴�����-TX
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //PD6 ��R2X 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������-RX
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

    USART_ClockInit(USART2, &USART_ClockInitStructure);
    USART_Init(USART2, &USART_InitStructure);
    
    USART_Cmd(USART2, ENABLE);
  
}

void UART2Putc(unsigned char c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

void UART2Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART2, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}	

/*************************************************
  ��������: UP_MP3_Init  
  ��������: ����ģ�鴮�ڳ�ʼ��
  �����������õĺ���: void UART2Init(u32 baud);     
  ���ñ������ĺ���: ��
  �������˵��:  ��   
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_MP3_Init(void)
{
	UART2Init(9600);	
	UP_delay_ms(5000);
}

/*************************************************
  ��������: UP_MP3_PlaySound 
  ��������: MP3����վ����
  �����������õĺ���: void UART2Putc(unsigned char c);     
  ���ñ������ĺ���: ��
  �������˵��:  num
                     ��in�����ֵ~0x1e   
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_MP3_PlaySound(u16 num)
{
	UART2Putc(0x7E);
	UART2Putc(0x12);
	UART2Putc((num&0xff00)>>8);
	UART2Putc(num&0x00ff);
	UART2Putc(0xEF);
}

/*************************************************
  ��������: UP_MP3_Pause 
  ��������: MP3��ͣ����
  �����������õĺ���: void UART2Putc(unsigned char c);     
  ���ñ������ĺ���: ��
  �������˵��:  ��   
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_MP3_Pause(void)
{
	UART2Putc(0x7E);
	UART2Putc(0x0E);
	UART2Putc(0x00);
	UART2Putc(0x00);
	UART2Putc(0xEF);	
}

/*************************************************
  ��������: UP_MP3_SetVolume  
  ��������: ������������ģ��������С
  �����������õĺ���: void UART2Putc(unsigned char c);     
  ���ñ������ĺ���: ��
  �������˵��:  volume
                        ��in�����ֵ~0x1e   
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_MP3_SetVolume(u8 volume)//���Ϊ0x1E
{
	UART2Putc(0x7E);
	UART2Putc(0x06);
	UART2Putc(0x00);
	UART2Putc(volume);
	UART2Putc(0xEF);	
}













