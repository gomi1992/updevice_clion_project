/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  	: ����Ԫ  
//  Reviser				: �����
//  Update Date   : 2012/08/01
//  Version   	  : 1.3          
//  Description   : Transplant to v3.5 function library
//  Update Date   : 2013/07/12
//  Version   	  : 1.3.1          
//  Description   : Modification for new controller
/******************************************************************/ 

#include "STM32Lib/stm32f10x.h"
#include "UP_UART.h"
#include "UP_ADC.h"
#include "UP_IOout.h"
#include "UP_PWM.h"
#include "UP_Timer.h"
#include "UP_System.h"
#include "UP_Globle.h"

#define MAX_FRAME_NUM	20			//�����е����֡��
#define MAX_BUF_NUM		20			//ÿ֡�����С
typedef struct _CDSFrame_Info {	   	//CDS֡���нṹ��
	u8 len;							//֡����
	u8 buf[MAX_BUF_NUM];			//֡����
} CDSFrame_Info;

CDSFrame_Info g_CDSFrames[MAX_FRAME_NUM];	//CDS֡����	
u8 g_CDSFrames_Tail = 0;		//CDS֡����βָ��
u8 g_CDSFrames_Head = 0;		//CDS֡����ͷָ��

void UP_UART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

void UP_UART1_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

void UP_UART4_Putc(unsigned char c)
{
    USART_SendData(UART4, c);
    while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET );
}

void UP_UART4_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(UART4, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
    }
}			

void UP_UART5_Putc(unsigned char c)
{
    USART_SendData(UART5, c);
    while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET );
}

void UP_UART5_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(UART5, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
    }
}

//����Ƿ��ж������֡��Ҫ���ͣ�ÿ1ms���һ�Σ�
void CheckCDSSend(void)
{
	//�ж϶����Ƿ�Ϊ��
	if(g_CDSFrames_Head == g_CDSFrames_Tail)
		return;
	//����һ�����ݷ���
	UART3_TXEN;
	DMA1_Channel2->CMAR = (u32)g_CDSFrames[g_CDSFrames_Tail].buf;	//Ҫ���͵��ڴ��ַ 
	DMA1_Channel2->CNDTR = g_CDSFrames[g_CDSFrames_Tail].len;		//Ҫ���͵��ֽ���
	DMA1_Channel2->CCR |= 0x01;										//����һ��DMA  // */
	g_CDSFrames_Tail++;
	if(g_CDSFrames_Tail == MAX_FRAME_NUM)
		g_CDSFrames_Tail = 0;	
}

//UART3��������֡�����ֶ��Э�飩
void UP_Uart3_SendCDSFrame(u8 id, u8 len, u8 cmd, u8* pData)
{
	unsigned char i;
 	unsigned char csum = 0;
	u16 pos = 0;
	//�жϻ����Ƿ�����
	if(g_CDSFrames_Head == g_CDSFrames_Tail-1 || 
		(g_CDSFrames_Tail == 0 && g_CDSFrames_Head == MAX_FRAME_NUM-1))
		return;
	//��Ҫ���͵�����֡ѹ�����
	g_CDSFrames[g_CDSFrames_Head].buf[pos++] = 0xff;	//֡ͷ
	g_CDSFrames[g_CDSFrames_Head].buf[pos++] = 0xff;	//֡ͷ
	g_CDSFrames[g_CDSFrames_Head].buf[pos++] = id;		//ID
	csum += id;
	g_CDSFrames[g_CDSFrames_Head].buf[pos++] = len;		//������
	csum += len;
	g_CDSFrames[g_CDSFrames_Head].buf[pos++] = cmd;		//����
	csum += cmd;		
	for(i=0; i<len-2; i++)
	{
		g_CDSFrames[g_CDSFrames_Head].buf[pos++] = pData[i];//����
	 	csum += pData[i];
	}
	csum = ~csum; 
	g_CDSFrames[g_CDSFrames_Head].buf[pos++] = csum;	//У���
	g_CDSFrames[g_CDSFrames_Head].len = pos;			//֡����
	g_CDSFrames_Head++;
	if(g_CDSFrames_Head == MAX_FRAME_NUM)
		g_CDSFrames_Head = 0;
}

//UART DMA������
void UP_UART_DMAConfiguration(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART3->DR));			//ԴͷBUF
	DMA_InitStructure.DMA_MemoryBaseAddr = 0;									//Ŀ��BUF
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;							//������Ŀ��
	DMA_InitStructure.DMA_BufferSize = 0;										//BUF��С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����ַ�Ĵ���������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�����ֽ�Ϊ��λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;			//�ڴ��ֽ�Ϊ��λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								//��ͨģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							//4���ȼ�֮һ��(������)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//���ڴ浽�ڴ�
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);								//DMA2��������ж�
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);								//����3��������DMA
}

void UP_UART_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
		USART_ClockInitTypeDef USART_ClockInitStructure;
	
	//ʹ�ܴ���1��PA��AFIO����
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | 
													 RCC_APB2Periph_AFIO   |
													 RCC_APB2Periph_USART1 , 
													 ENABLE);

    // A9 USART1_Tx 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�������-TX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // A10 USART1_Rx  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������-RX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = g_UP_BluetoothBaudRate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
		USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
		USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
		USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
		USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

	USART_ClockInit(USART1, &USART_ClockInitStructure);
    USART_Init(USART1, &USART_InitStructure);
    // Enable the USARTx 
    USART_Cmd(USART1, ENABLE);
	//���������UART�����жϣ�����1ʹ�ý����ж�
	if(g_UP_bBluetoothIT)	 
	{
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);// */
	}
	
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
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������-RX
   GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate =115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
// 	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
// 	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
// 	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
// 	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
//  USART_ClockInit(USART2, &USART_ClockInitStructure);
//  
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
/*����4	  19200  Zigbee */
	  //ʹ�ܴ���4ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	//ʹ��PC��AFIO����
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

	//PC10 ��T4X
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  //���츴�����-TX
    GPIO_Init(GPIOC, &GPIO_InitStructure);

  //PC11 ��R4X 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������-RX
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = g_UP_ZigbeeBaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

    USART_ClockInit(UART4, &USART_ClockInitStructure);
    USART_Init(UART4, &USART_InitStructure);
    
    USART_Cmd(UART4, ENABLE);
	//���������UART�����жϣ�����2ʹ�ý����ж�
	if(g_UP_bZigbeeIT)	 	
		USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);// */
	
	
	//ʹ�ܴ���5��PC,PD,AFIO����
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | 
													 RCC_APB2Periph_GPIOD | 
													 RCC_APB2Periph_AFIO  ,
													 ENABLE);
	  
    // C12 USART5_Tx 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�������-TX
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // D2 USART5_Rx  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������-RX
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = g_UP_USR232BaudRate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
		USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
		USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
		USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
		USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

		USART_ClockInit(UART5, &USART_ClockInitStructure);
		USART_ClearFlag(UART5,USART_FLAG_TC);
		USART_Init(UART5, &USART_InitStructure);
    // Enable the USARTx 
    USART_Cmd(UART5, ENABLE);	
	//���������UART�����жϣ�����5ʹ�ý����ж�
		if(g_UP_bUSR232IT)	
		{
			USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);// */	
		}
		
		
	//ʹ�ܴ���3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	// B10 ��T3X
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   //���츴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // B11 ��R3X 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	UART3_RXDS;
 	UART3_TXDS;
	//E14��TX3EN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //�������
    GPIO_Init(GPIOE, &GPIO_InitStructure);

	//E15��RX3EN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //�������
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 1000000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

    USART_ClockInit(USART3, &USART_ClockInitStructure);
    USART_Init(USART3, &USART_InitStructure);
    
    USART_Cmd(USART3, ENABLE);
	//����3ʹ�ý����ж�
 //	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);// */
	UP_UART_DMAConfiguration();
}

//ʹ������(UART1)���ݽ����ж�
void UP_EnableBluetoothIT(u32 BaudRate)
{
	g_UP_BluetoothBaudRate = BaudRate;
	g_UP_bBluetoothIT = TRUE;		
}
//ʹ��Zigbee(UART4)���ݽ����ж�
void UP_EnableZigbeeIT(u32 BaudRate)
{
	g_UP_ZigbeeBaudRate = BaudRate;
	g_UP_bZigbeeIT = TRUE;
}
/*//ʹ��USR232(UART5)���ݽ����ж�
void UP_EnableUSR232IT(u32 BaudRate)
{
	g_UP_USR232BaudRate = BaudRate;
	g_UP_bUSR232IT = TRUE;		
}*/
