/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  	: ����Ԫ  
//  Reviser				: �����
//  Update Date   : 2012/08/01
//  Version   	  : 1.3          
//  Description   : Transplant to v3.5 function library
/******************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "STM32Lib/stm32f10x.h"
#include "UP_Timer.h"
#include "UP_UART.h"
#include "UP_Globle.h"
#include "UP_IOout.h"
extern unsigned  int   UP_RXlen[64];
extern  unsigned  char  UP_IID;
unsigned  int UP_size=0;
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
extern void UP_Zigbee_Rec(u32 data);
u32 count=0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NMIException
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMIException(void)
{
}

/*******************************************************************************
* Function Name  : HardFaultException
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFaultException(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManageException
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManageException(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFaultException
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFaultException(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFaultException
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFaultException(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMonitor(void)
{
}

/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVCHandler(void)
{
}

/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSVC(void)
{
}

/*******************************************************************************
* Function Name  : SysTickHandler
* Description    :ϵͳʱ�ӣ�һ����̵�1MS�ж�һ��
*******************************************************************************/
extern void CheckCDSSend(void);
extern u8 UP_Buzzer_State;
extern u8 UP_Buzzer_Period;
static u8 freq=0;
void SysTick_Handler(void)		//����Ƿ��ж������֡��Ҫ���ͣ�ÿ1ms���һ�Σ�
{
	CheckCDSSend();
	if(UP_Buzzer_State == 1)
	 { 
	  if(UP_Buzzer_Period-->0)
	  {
	   if(++freq>=2)
	    {
		   buzzer_io_H;
		   freq=0;
		}
		else
		{
		   buzzer_io_L;
		}
	   }
	   else
	   {
	      UP_Buzzer_State = 0;
	   }
	 }
	 if(UP_Buzzer_State == 0)
	 { 
	     buzzer_io_L;
	 }
}

void EXTI_IRQHandler(u32 Channel)
{
	if(g_UP_ExtiITAddress != 0)
		(*((void(*)(u32))g_UP_ExtiITAddress))(Channel);
	EXTI->PR = Channel;		//�����־
}

void EXTI0_IRQHandler(void)
{
	EXTI_IRQHandler(EXTI_Line0);
}

void EXTI1_IRQHandler(void)
{
	EXTI_IRQHandler(EXTI_Line1);
}

void EXTI2_IRQHandler(void)
{
	EXTI_IRQHandler(EXTI_Line2);
}

void EXTI3_IRQHandler(void)
{
	EXTI_IRQHandler(EXTI_Line3);
}

void EXTI4_IRQHandler(void)
{
	EXTI_IRQHandler(EXTI_Line4);
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
		EXTI_IRQHandler(EXTI_Line5);
	else if(EXTI_GetITStatus(EXTI_Line6) != RESET)
		EXTI_IRQHandler(EXTI_Line6);
	else if(EXTI_GetITStatus(EXTI_Line7) != RESET)
		EXTI_IRQHandler(EXTI_Line7);
	else if(EXTI_GetITStatus(EXTI_Line8) != RESET)
		EXTI_IRQHandler(EXTI_Line8);
	else if(EXTI_GetITStatus(EXTI_Line9) != RESET)
		EXTI_IRQHandler(EXTI_Line9);
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
		EXTI_IRQHandler(EXTI_Line10);
	else if(EXTI_GetITStatus(EXTI_Line11) != RESET)
		EXTI_IRQHandler(EXTI_Line11);
	else if(EXTI_GetITStatus(EXTI_Line12) != RESET)
		EXTI_IRQHandler(EXTI_Line12);
	else if(EXTI_GetITStatus(EXTI_Line13) != RESET)
		EXTI_IRQHandler(EXTI_Line13);
	else if(EXTI_GetITStatus(EXTI_Line14) != RESET)
		EXTI_IRQHandler(EXTI_Line14);
	else if(EXTI_GetITStatus(EXTI_Line15) != RESET)
		EXTI_IRQHandler(EXTI_Line15);
}

//extern volatile bool ADC_Ok;
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1))
 	{
		DMA_ClearITPendingBit(DMA1_IT_GL1);	//���ȫ���жϱ�־
	//	ADC_Ok=TRUE;
	}
}
/*
extern void UP_Sev_TimerPro(void);
void TIM2_IRQHandler(void)
{
	UP_Sev_TimerPro();
}
*/
extern void UP_Timer_Pro(void);
void TIM3_IRQHandler(void)
{	
	UP_Timer_Pro();
}
/*
extern void UP_PWM_TimerPro(void);
void TIM4_IRQHandler(void)
{	
	UP_PWM_TimerPro();
}  // */

void USART2_IRQHandler(void)
{
	static unsigned  char  p=0;
  //UP_size=0;
 	//�����ж�
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{
		 USART_ClearITPendingBit(USART2,USART_IT_RXNE);
     UP_RXlen[p]=USART_ReceiveData(USART2);				//��DR
		  p++;
		if(p>=255){p=0;}	
		if((p==119)||(p==9)||(p==10))
		{	
		 p=0;
		if((UP_RXlen[0]==0x55)&&(UP_RXlen[1]==0xAA))
		 {
      UP_IID=UP_RXlen[2];
			UP_size=UP_RXlen[7]*256+UP_RXlen[8];	
			 if(UP_size<500)
			 {
			 //UP_LCD_Show(5,2,UP_size);
			 //UP_LCD_ShowString(0,0,UP_IID);
			 //UP_LCD_ShowString(0,1,UP_size);
					//printf("%d  %d\n",UP_IID,UP_size);
			 }
			
	    }
	   }
}
}
void USART1_IRQHandler(void)
{
	//�����ж�
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		if(g_UP_BluetoothITAddress != 0)
			(*((void(*)(u8))g_UP_BluetoothITAddress))(USART_ReceiveData(USART1));
	}
	
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);	//��SR
		USART_ReceiveData(USART1);				//��DR
	}
}

void UART5_IRQHandler(void)
{
	//�����ж�
	if(USART_GetITStatus(UART5,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
		if(g_UP_USR232ITAddress != 0)
			(*((void(*)(u8))g_UP_USR232ITAddress))(USART_ReceiveData(UART5));
	}
	
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
	if(USART_GetFlagStatus(UART5,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(UART5,USART_FLAG_ORE);	//��SR
		USART_ReceiveData(UART5);				//��DR
	}
}




void UART4_IRQHandler(void)
{
	//�����ж�
	if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
 		if(g_UP_ZigbeeITAddress != 0)
 			(*((void(*)(u8))g_UP_ZigbeeITAddress))(USART_ReceiveData(UART4));
		//UP_Zigbee_Rec((u32)USART_ReceiveData(UART4));
		//count++;
	}
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
	if(USART_GetFlagStatus(UART4,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(UART4,USART_FLAG_ORE);	//��SR
		USART_ReceiveData(UART4);				//��DR
	}
}

void DMA1_Channel2_IRQHandler(void)
{
	if((DMA1->ISR & DMA1_IT_TC2) != (uint32_t)RESET)
 	{	
		DMA1->IFCR = DMA1_IT_GL2;				//����жϱ�־
		DMA1_Channel2->CCR &= ~((u32)0x01);		//����DMA
		UP_delay_us(100);
		UART3_TXDS;
	//	DMA1_Channel2->CNDTR = 15;				//����װ��Ҫ���͵��ֽ���	// */
	}
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
