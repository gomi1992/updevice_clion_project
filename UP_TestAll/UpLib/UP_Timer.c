/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  	: ����Ԫ  
//  Reviser				: �����
//  Update Date   : 2012/08/01
//  Version   	  : 1.3          
//  Description   : Transplant to v3.5 function library
/******************************************************************/ 

#include "STM32Lib/stm32f10x.h"
#include "UP_Timer.h"
#include "UP_Globle.h"

u32 g_UP_iTimerCount = 0;				//��ʱ���ܼ���ֵ
u32 g_UP_iTimerTime[4] = {0};			//��ʱʱ�䣬usΪ��λ
u16 g_UP_TimerCCRVal_L16[4] = {0};		//Timer�Ƚ�ͨ���Ƚ�ֵ��16λ
u16 g_UP_TimerCCRVal_H16[4] = {0};		//Timer�Ƚ�ͨ���Ƚ�ֵ��16λ

u16 g_UP_TempCCRVal_L16[4] = {0};		//Timer�Ƚ�ͨ���Ƚ�ֵ��16λ
u16 g_UP_TempCCRVal_H16[4] = {0};		//Timer�Ƚ�ͨ���Ƚ�ֵ��16λ

//��ʼ����ز���
void UP_Timer_InitParameters(void)
{

}

/*************************************************
  ��������: UP_Timer_Init  
  ��������: ��ʱ����س�ʼ��
  �����������õĺ���: ��       
  ���ñ������ĺ���: ��    
  �������˵��: ��         
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_Timer_Init(void)
{
	//Timer3��4������Ƚ���4·��ʱ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	UP_Timer_InitParameters();

	if(!g_UP_bTimerIT[0] && !g_UP_bTimerIT[1] && !g_UP_bTimerIT[2] && !g_UP_bTimerIT[3])
		return;
	//ʹ�ܶ�ʱ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//Timer3��������   
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Prescaler = 71;						//72��Ƶ��1us
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
	TIM_TimeBaseStructure.TIM_Period = 0xffff;						//����ֵ65535
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

	//�Ƚ�ͨ������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;       	//����ȽϷ�����ģʽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//����Ϊ��

	TIM_OCInitStructure.TIM_Pulse = g_UP_iTimerTime[0]>65535?65535:g_UP_iTimerTime[0]; 	//�Ƚ�ʱ��
	if(g_UP_bTimerIT[0]) 
		TIM_OC1Init(TIM3,&TIM_OCInitStructure);							//ͨ��1				
	TIM_OCInitStructure.TIM_Pulse = g_UP_iTimerTime[1]>65535?65535:g_UP_iTimerTime[1]; 	//�Ƚ�ʱ��
	if(g_UP_bTimerIT[1]) 
		TIM_OC2Init(TIM3,&TIM_OCInitStructure); 						//ͨ��2
	TIM_OCInitStructure.TIM_Pulse = g_UP_iTimerTime[2]>65535?65535:g_UP_iTimerTime[2]; 	//�Ƚ�ʱ��
	if(g_UP_bTimerIT[2]) 
		TIM_OC3Init(TIM3,&TIM_OCInitStructure); 						//ͨ��3
	TIM_OCInitStructure.TIM_Pulse = g_UP_iTimerTime[3]>65535?65535:g_UP_iTimerTime[3]; 	//�Ƚ�ʱ��
	if(g_UP_bTimerIT[3]) 
		TIM_OC4Init(TIM3,&TIM_OCInitStructure); 						//ͨ��4

	//Ԥ����������ж�λ
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update);

	//�Ƚ��ж�ʹ��
	if(g_UP_bTimerIT[0]) 
		TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	if(g_UP_bTimerIT[1]) 
		TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
	if(g_UP_bTimerIT[2]) 
		TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
	if(g_UP_bTimerIT[3]) 
		TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);	  
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//TIM3����ʹ��
	TIM_Cmd(TIM3,ENABLE);
}

/*************************************************
  ��������: UP_Timer_Pro  
  ��������: ��ʱ��3�жϴ���
  �����������õĺ���: ��       
  ���ñ������ĺ���: ��    
  �������˵��: ��         
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_Timer_Pro(void)
{
	//��ʱ������ж�
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		g_UP_iTimerCount += 0xffff;
	}
	//�Ƚ�ͨ��1�ж�
	else if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
	    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
		if(g_UP_TempCCRVal_H16[0] == 0 && g_UP_TempCCRVal_L16[0] == 0)
		{
			g_UP_TempCCRVal_L16[0] = g_UP_TimerCCRVal_L16[0];
			g_UP_TempCCRVal_H16[0] = g_UP_TimerCCRVal_H16[0];
			if(g_UP_Timer0ITAddress)
				(*((void(*)(u8))g_UP_Timer0ITAddress))(TIMER_CHANNEL0);
		}
		if(g_UP_TempCCRVal_H16[0] != 0)
		{
			TIM3->CCR1 += 65535;
			g_UP_TempCCRVal_H16[0]--;
		}
		else if(g_UP_TempCCRVal_L16[0] != 0)
		{
			TIM3->CCR1 += g_UP_TempCCRVal_L16[0];
			g_UP_TempCCRVal_L16[0] = 0;
		}
	}
	//�Ƚ�ͨ��2�ж�
	else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{
	    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		if(g_UP_TempCCRVal_H16[1] == 0 && g_UP_TempCCRVal_L16[1] == 0)
		{
			g_UP_TempCCRVal_L16[1] = g_UP_TimerCCRVal_L16[1];
			g_UP_TempCCRVal_H16[1] = g_UP_TimerCCRVal_H16[1];
			if(g_UP_Timer1ITAddress)
				(*((void(*)(u8))g_UP_Timer1ITAddress))(TIMER_CHANNEL1);
		}
		if(g_UP_TempCCRVal_H16[1] != 0)
		{
			TIM3->CCR2 += 65535;
			g_UP_TempCCRVal_H16[1]--;
		}
		else if(g_UP_TempCCRVal_L16[1] != 0)
		{
			TIM3->CCR2 += g_UP_TempCCRVal_L16[1];
			g_UP_TempCCRVal_L16[1] = 0;
		}
	}
	//�Ƚ�ͨ��3�ж�
	else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
	{
	    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
		if(g_UP_TempCCRVal_H16[2] == 0 && g_UP_TempCCRVal_L16[2] == 0)
		{
			g_UP_TempCCRVal_L16[2] = g_UP_TimerCCRVal_L16[2];
			g_UP_TempCCRVal_H16[2] = g_UP_TimerCCRVal_H16[2];
			if(g_UP_Timer2ITAddress)
				(*((void(*)(u8))g_UP_Timer2ITAddress))(TIMER_CHANNEL2);
		}
		if(g_UP_TempCCRVal_H16[2] != 0)
		{
			TIM3->CCR3 += 65535;
			g_UP_TempCCRVal_H16[2]--;
		}
		else if(g_UP_TempCCRVal_L16[2] != 0)
		{
			TIM3->CCR3 += g_UP_TempCCRVal_L16[2];
			g_UP_TempCCRVal_L16[2] = 0;
		}
	}
	//�Ƚ�ͨ��4�ж�
	else if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
	{
	    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
		if(g_UP_TempCCRVal_H16[3] == 0 && g_UP_TempCCRVal_L16[3] == 0)
		{
			g_UP_TempCCRVal_L16[3] = g_UP_TimerCCRVal_L16[3];
			g_UP_TempCCRVal_H16[3] = g_UP_TimerCCRVal_H16[3];
			if(g_UP_Timer3ITAddress)
				(*((void(*)(u8))g_UP_Timer3ITAddress))(TIMER_CHANNEL3);
		}
		if(g_UP_TempCCRVal_H16[3] != 0)
		{
			TIM3->CCR4 += 65535;
			g_UP_TempCCRVal_H16[3]--;
		}
		else if(g_UP_TempCCRVal_L16[3] != 0)
		{
			TIM3->CCR4 += g_UP_TempCCRVal_L16[3];
			g_UP_TempCCRVal_L16[3] = 0;
		}
	}
}

/*************************************************
  ��������: UP_Timer_EnableIT
  ��������: ʹ�ܼ�ʱ���ж�
  �����������õĺ���: ��       
  ���ñ������ĺ���: ��    
  �������˵��: TimerChannel  Time_us     
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_Timer_EnableIT(u32 TimerChannel, u32 Time_us)
{
	if(TimerChannel > 3 || Time_us == 0)
		return;
	g_UP_bTimerIT[TimerChannel] = TRUE;
	g_UP_iTimerTime[TimerChannel] = Time_us;
	g_UP_TempCCRVal_L16[TimerChannel] = g_UP_TimerCCRVal_L16[TimerChannel] = Time_us;
	g_UP_TempCCRVal_H16[TimerChannel] = g_UP_TimerCCRVal_H16[TimerChannel] = Time_us>>16;
}

/*************************************************
  ��������: UP_Timer_DisableIT
  ��������: ���ܼ�ʱ���ж�
  �����������õĺ���: ��       
  ���ñ������ĺ���: ��    
  �������˵��: TimerChannel     
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_Timer_DisableIT(u32 TimerChannel)
{
	if(TimerChannel > 3)
		return;
	g_UP_bTimerIT[TimerChannel] = FALSE;
	//�Ƚ��жϽ���
	if(!g_UP_bTimerIT[0]) 
		TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
	if(!g_UP_bTimerIT[1]) 
		TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE);
	if(!g_UP_bTimerIT[2]) 
		TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);
	if(!g_UP_bTimerIT[3]) 
		TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);	
}

/*************************************************
  ��������: UP_Timer_SetHadler
  ��������: ���ö�ʱ���ж���ں���
  �����������õĺ���: ��       
  ���ñ������ĺ���: ��    
  �������˵��: channel   ApplicationAddress    
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
*************************************************/
void UP_Timer_SetHadler(u32 channel,void (*ApplicationAddress)(u32))
{
	if(ApplicationAddress != 0)
	{
		//g_UP_TimerITAddress = (u32)ApplicationAddress;
		switch(channel)
		{
			case 0:
				g_UP_Timer0ITAddress = (u32)ApplicationAddress;
				break;
			case 1:
				g_UP_Timer1ITAddress = (u32)ApplicationAddress;
				break;		
			case 2:
				g_UP_Timer2ITAddress = (u32)ApplicationAddress;
				break;
			case 3:
				g_UP_Timer3ITAddress = (u32)ApplicationAddress;
				break;
			default:
				break;
		}
		
	}
}

/*************************************************
  ��������: UP_Timer_GetCount
  ��������: ��ȡ��ʱ������ֵ��32λ��
  �����������õĺ���: ��       
  ���ñ������ĺ���: ��    
  �������˵��: ApplicationAddress    
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
u32 UP_Timer_GetCount(void)
{
	return (g_UP_iTimerCount+TIM3->CNT);
}

/*************************************************
  ��������: UP_Timer_CleanCount
  ��������: �����ʱ������ֵ
  �����������õĺ���: ��       
  ���ñ������ĺ���: ��    
  �������˵��: �� 
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_Timer_CleanCount(void)
{
	g_UP_iTimerCount = 0;
	TIM3->CNT = 0;
}
														
