#include "STM32Lib/stm32f10x.h"

#include "UP_Globle.h"
#include "UP_Timer.h"
#include "UP_UART.h"
#include "UP_USR232.h"

u16 RF_RecTimeCount = 0;
u8 RF_Start = 0;
//u8 RF_ReadSign = 0;
u8 RF_RecCount = 0;
u8 RF_RecBuf[10];
u32 RF_Add = 0;
u8 UP_RFID_ReadSign = 0;

/*************************************************
  ��������: RFIDParser 
  ��������: �˺����û�����ʹ�ã���void UP_RFID_Init(void){};���Ѿ����á�
            ��Ҫ���ڰѽ��յĿ��ŵȸ���RF_RecBuf�Լ������õȡ�
  �����������õĺ���: ��  
  ���ñ������ĺ���: ��
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void RFIDParser(u32 rec)
{	
	RF_Start = 1;
	RF_RecTimeCount = 0; 
	RF_RecBuf[RF_RecCount++] = rec;
	RF_Add++;
}

/*************************************************
  ��������: UP_RFID_ReadCard 
  ��������: ��Ҫ�����жϿ������յ����ֽ��������Ƿ�ʱ
  �����������õĺ���: ��  
  ���ñ������ĺ���: ��
  �������˵��:  null
                     ��in��������
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void RFID_Timer_Handler(u32 null)
{
	if(RF_Start)
	{
		if(10 < RF_RecTimeCount++)//�ж��Ƿ�ʱ
		{
			RF_Start = 0;
			RF_RecTimeCount = 0;
			
			if(5 == RF_RecCount)
			{
				UP_RFID_ReadSign = 1;//�յ�5���ֽڵĿ��ź�˱�־��1��ֻ�е�ִ��UP_RFID_ReadCard��ȡ���ź�˱�־������
			}
			RF_RecCount = 0;
		}
	}	
}

/*************************************************
  ��������: UP_RFID_Init 
  ��������: ʹ��RFID�Ĵ��ڼ���ʱ���жϣ����ж����жϴ��ڽ��������Ƿ�ʱ��
  �����������õĺ���: void UP_EnableUSR232IT(u32 BaudRate); 
                      void UP_SetUSR232Hadler(void (*ApplicationAddress)(u32)); 
                      void UP_Timer_EnableIT(u32 TimerChannel, u32 Time_us); 
                      void UP_Timer_SetHadler(u32 channel,void (*ApplicationAddress)(u32));
                      void RFID_Timer_Handler(u32 null);void RFIDParser(u32 rec)��    
  ���ñ������ĺ���: ��
  �������˵��:  ��  
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_RFID_Init(void)
{
	UP_EnableUSR232IT(9600);
	UP_SetUSR232Hadler(RFIDParser);
	UP_Timer_EnableIT(TIMER_CHANNEL1,10000);//ʹ�ܶ�ʱ�ж�
	UP_Timer_SetHadler(TIMER_CHANNEL1,RFID_Timer_Handler);//���ö�ʱ�жϵ���ں���	
}

/*************************************************
  ��������: UP_RFID_ReadCard 
  ��������: ��RF_RecBuf[5]��ȡ�Ŀ�����Ϣ��ֵ��buf[5]
  �����������õĺ���: ��  
  ���ñ������ĺ���: ��
  �������˵��:  ��RF_RecBuf[5]��ȡ�Ŀ�����Ϣ��ֵ��buf[5] 
  �������˵��: ��       
  ��������ֵ: 1��0
  ����˵��:��        
*************************************************/
u8 UP_RFID_ReadCard(u8 buf[5])
{
	if(UP_RFID_ReadSign)
	{	
		UP_RFID_ReadSign = 0;
		buf[0] = RF_RecBuf[0];
		buf[1] = RF_RecBuf[1];
		buf[2] = RF_RecBuf[2];
		buf[3] = RF_RecBuf[3];
		buf[4] = RF_RecBuf[4];
		RF_RecBuf[0]=RF_RecBuf[1]=RF_RecBuf[2]=RF_RecBuf[3]=RF_RecBuf[4]=0;
		return 1;	
	}
	else
	{
		buf[0]=buf[1]=buf[2]=buf[3]=buf[4]=0;
		return 0;
	}
}

/*************************************************
  ��������: UP_RFID_CheckID 
  ��������: �ȶԶ����Ŀ���
  �����������õĺ���: ��  
  ���ñ������ĺ���: ��
  �������˵��:  ID1
                   ��in�������Ŀ���
                 ID2
                   ��in���ȶԵĿ��� 
  �������˵��: ��
  ��������ֵ: 1��0
  ����˵��:��        
*************************************************/
u8 UP_RFID_CheckID(u8 *ID1,u8 *ID2)//ID1Ϊ�����Ŀ��ţ�ID2ΪҪ�ȶԵĿ���
{
	u8 i;
	for(i=0;i<5;i++)
	{
		if(ID1[i] != ID2[i])
		{
			return 0;
		}
	}
	return 1;
}



