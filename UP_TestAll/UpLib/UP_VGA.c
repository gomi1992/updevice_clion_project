#include "STM32Lib/stm32f10x.h"
#include "UP_CDS5500.h"
#include "UP_Globle.h"
#include "UP_VGA.h"
#include "UP_UART.h"
#include "UP_Timer.h"
#include "UP_LCD.h"
#include "UP_ADC.h"


u16  speed = 800;

s16 Speed1=0,Speed2=0,Speed3=0,Speed4=0;		//�ĸ�������ٶ�
u8 CDS_LEN;								//����ѭ����ش����ݵ���Ч���ݳ���
u8 CDS_DATA[5];							//ѭ���巵�����ݵĻ���
u8 UP_VGA_TrackSign;							//ѭ����7�����״̬
u32 rec_num;

u8 UP_VGA_ForwardSign = 0;
u8 UP_VGA_StopSign = 0;
u8 UP_VGA_TurnSign = 0;

u8 UP_VGA_CrossSign = 0;			//��·��־0-�޲�·������ʮ��·�ڣ�1-����в�· 2-�Ҳ��в�· 3-T�ֲ�·,�˱�־ֻ���ֶ�����
u8 UP_VGA_CrossSign2 = 0;			//��·��־0���˱�־�Զ�����
u8 iLeft,iMid,iRight,iLeft2,iLeft3,iRight2,iRight3;
s16 spdLeft,spdRight;
s16 sLast = 0;


u8 UP_VGA_ProAdcChannel;
u16 UP_VGA_ProDis;


void USART3_NVIC(void)					//��������3�ж�
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);			   // */	
}

void CDS_UARTHadler(u8 Data)			//���ڽ����յ���ѭ���崫�ص�����
{//��������ʾ��FF FF 00 03 00 03 FB--��ͷ1 ��ͷ2 ID ���� ������Ϣ ���� У��
	u8 temp;							//�յ�������
	static u8 sum = 0;					//У��
	static u8 count = 0;				
	static u8 data_count = 0;
	temp = Data;	 
	switch(count)
	{
		case 0:
		{
			if(temp == 0xFF)			//�ж���ͷ1
				count = 1;
			else
				sum = 0;
		}
		break;
		case 1:
		{
			if(temp != 0xFF)			//�ж���ͷ2
			{
			 	count = 0;
				sum = 0;
			}
			else
			{	
				count = 2;
			}
		}
		break;
		case 2:
		{	
			if(temp != 0x00)			//ѭ����ID���̶�Ϊ0x00
			{
			 	count = 0;
				sum = 0;
			}
			else
			{
			 	sum = 0x00;
				count = 3;
			}
		}
		break;
		case 3:
		{	
			if(temp != 0x03)			//��Ч���ݳ���
			{
			 	count = 0;
				sum = 0;
			}
			else
			{
				CDS_LEN = 0x03;
				sum += CDS_LEN;
				count = 4;
				data_count = 0x00;
			}			
		}
		break;
		case 4:
		{	
			if(data_count < (CDS_LEN-1))//�ش����ݵ���Ч���ݴ��뻺��
			{	
				CDS_DATA[data_count] = temp;
				sum += temp;	 
				data_count += 1;
			}	
			else if(data_count == ((CDS_LEN-1)))
			{	
				sum = ~sum;
				if(sum == temp)				//�ж�У��λ�Ƿ���ȷ
				{		
					if(CDS_DATA[0] == 0x00)	//�ж�״̬λ0x00Ϊ����
					{
						 UP_VGA_TrackSign = CDS_DATA[1] ;//ѭ����״̬�����־������
						 rec_num++;
					}
				}
			 	sum = 0;
				data_count = 0;
				count = 0;
			}
		}
		break;
		default:
			break;					
	}
	
}
void USART3_IRQHandler(void)//����3�жϺ���
{
	//�����ж�
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)//�ж��Ƿ�Ϊ�����ж�
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//���жϱ�־
		CDS_UARTHadler(USART_ReceiveData(USART3));	  //�����յ�������
		
	}
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART3,USART_FLAG_ORE);	//��SR
		USART_ReceiveData(USART3);				//��DR
	}
}

void UP_UART3_Putc(unsigned char c)
{
    USART_SendData(USART3, c);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
}

/*************************************************
  ��������: CDS_Read_Sign  
  ��������: ��ȡѭ����״̬����
  �����������õĺ���: void CDS_Read_Sign(void);      
  ���ñ������ĺ���: ��
  �������˵��:  ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void CDS_Read_Sign(void)
{
	
	UART3_RXDS;			//�رն�����ߵĽ����ź���
	UART3_TXEN;			//�򿪶�����ߵķ����ź���
	UP_UART3_Putc(0xff);//��ͷ1
	UP_UART3_Putc(0xff);//��ͷ2
	UP_UART3_Putc(0x00);//ѭ����ID
	UP_UART3_Putc(0x04);//���ݳ���
	UP_UART3_Putc(0x02);//��ָ��
	UP_UART3_Putc(0x32);//��ȡ�ڴ��ַ
	UP_UART3_Putc(0x01);//��ȡ���ݳ���
	UP_UART3_Putc(0xc6);//У��
	UP_delay_us(10);	//��ʱ10us�ȴ����ݱ��������
	//ѭ����ͨѶ�ӿڲ�����CDSϵ�����ֶ���ĵ����߽ӿڣ����պͷ��������Ǹ���һ����
	UART3_TXDS;			//�رն�����ߵķ����ź���
	UART3_RXEN;			//�򿪶�����ߵĽ����ź���
}

void SetSpeed(s16 left,s16 right)
{
	Speed1 = left;//
	Speed2 = 0-right;//
	Speed3 = left;//
	Speed4 = 0-right;//
}
extern void MAIN_TIMER_PRO(u32 null);

/*************************************************
  ��������: VGA_Timer_Handler  
  ��������: 1ms��ʱ�жϣ���ʱ�������ѭ���巢��ָ��
  �����������õĺ���: void CDS_Read_Sign(void);      
  ���ñ������ĺ���: ��
  �������˵��:  null:������   
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void VGA_Timer_Handler(u32 null)
{
	static u32 Count;
	static u32 Count2;
	static u32 Count3;//�����ϰ��뿪����ʱ����

	if(UP_ADC_GetValue(UP_VGA_ProAdcChannel)>UP_VGA_ProDis)
	{
		Count3 = 1000;//��ʱ1s
	}
	else
	{
		if(Count3 != 0)
			Count3--;
	}
	
	if(0 == Count)
	{
		CDS_Read_Sign();
	}
	else if(2 == Count)
	{//����CountΪ0ʱ���˶�����ߵĽ��գ��ڷ���������ǰҪ�ر�
		UART3_RXDS;		//�رն�����ߵĽ����ź���,����ٶ����ú����а�����UART3_TXEN
		if(Count3!=0)//if(UP_ADC_GetValue(UP_VGA_ProAdcChannel)>UP_VGA_ProDis)
			UP_CDS_SetSpeed(0x01,0);
		else		
			UP_CDS_SetSpeed(0x01,Speed1);		
	}
	else if(3 == Count)
	{
		if(Count3!=0)//if(UP_ADC_GetValue(UP_VGA_ProAdcChannel)>UP_VGA_ProDis)
			UP_CDS_SetSpeed(0x02,0);	
		else
			UP_CDS_SetSpeed(0x02,Speed2);	
	}
	else if(4 == Count)	
	{
		if(Count3!=0)//if(UP_ADC_GetValue(UP_VGA_ProAdcChannel)>UP_VGA_ProDis)
			UP_CDS_SetSpeed(0x03,0);	
		else
			UP_CDS_SetSpeed(0x03,Speed3);	
	}
	else if(5 == Count)
	{
		if(Count3!=0)//if(UP_ADC_GetValue(UP_VGA_ProAdcChannel)>UP_VGA_ProDis)
			UP_CDS_SetSpeed(0x04,0);
		else
			UP_CDS_SetSpeed(0x04,Speed4);
	}		
	if(6 == Count) 
		Count = 0;
	else
		Count += 1;
	
	if(6 == Count2)
	{
		Count2 = 0;
		
		if(UP_VGA_ForwardSign)
		{
			
			iLeft = (UP_VGA_TrackSign&0x04)>>2;
			iMid = (UP_VGA_TrackSign&0x08)>>3;
			iRight = (UP_VGA_TrackSign&0x10)>>4;
			iLeft2 = (UP_VGA_TrackSign&0x02)>>1;
			iLeft3 = UP_VGA_TrackSign&0x01;
			iRight2 = (UP_VGA_TrackSign&0x20)>>5;
			iRight3 = (UP_VGA_TrackSign&0x40)>>6;
			
			if(iMid == 1)
			{
				spdLeft = speed;
				spdRight = speed;
			}
			
			if(iLeft == 1|| iLeft2 == 1)
			{
				spdLeft = speed*0.8;
				spdRight = speed*1.2;
				sLast = 1;	//�������
			}
			
			if(iRight == 1 || iRight2 == 1)
			{
				spdLeft = speed*1.2;
				spdRight = speed*0.8;
				sLast = 2; 	//�����Ҳ�
			}
			
			if(iMid==0 && iLeft==0 && iRight==0)
			{
				if(sLast == 1)
				{
					//��ȥǰ���������
					spdLeft = 0-speed;
					spdRight = speed;
				}
				else if(sLast == 2)
				{
					//��ȥǰ�������Ҳ�
					spdLeft = speed;
					spdRight = 0-speed;
				}
				else
				{
					if(iLeft3==1)
						sLast = 1;
					else if(iRight3 == 1)
						sLast = 2;	
				}
			}
			SetSpeed(spdLeft,spdRight);
 			
		}
		else
		{
			iLeft = (UP_VGA_TrackSign&0x04)>>2;
			iMid = (UP_VGA_TrackSign&0x08)>>3;
			iRight = (UP_VGA_TrackSign&0x10)>>4;
			iLeft2 = (UP_VGA_TrackSign&0x02)>>1;
			iLeft3 = UP_VGA_TrackSign&0x01;
			iRight2 = (UP_VGA_TrackSign&0x20)>>5;
			iRight3 = (UP_VGA_TrackSign&0x40)>>6;
		}
	
		if(iLeft2 == 1 && iLeft3 == 1)
		{
			if((iLeft==1||iMid==1||iRight==1) && (iRight2==0 && iRight3==0))//�����󽻲���UP_VGA_CrossSign��1
				UP_VGA_CrossSign2 = UP_VGA_CrossSign = 1;
			else
				UP_VGA_CrossSign2 = UP_VGA_CrossSign = 0;
		}
		else if(iRight2 == 1 && iRight3 == 1)
		{
			if((iLeft==1||iMid==1||iRight==1) && (iLeft2==0 && iLeft3==0))//�����ҽ�����UP_VGA_CrossSign��2
				UP_VGA_CrossSign2 = UP_VGA_CrossSign = 2;
			else
				UP_VGA_CrossSign2 = UP_VGA_CrossSign = 0;
		}
		else
			UP_VGA_CrossSign2 = UP_VGA_CrossSign = 0;
		
		if(iLeft2&iLeft3&iRight2&iRight3)//����T�ֽ�����UP_VGA_CrossSign��3
		{
			UP_VGA_CrossSign2 = UP_VGA_CrossSign = 3;
		}	
		
	}
	else
		Count2 += 1;
	//MAIN_TIMER_PRO(0);
}

/*************************************************
  ��������: UP_VGA_Timer  
  ��������: ʹ�ܶ�ʱ���жϣ����ж�ΪС����Ѱ�߽��̡�
  �����������õĺ���: void UP_Timer_EnableIT(u32 TimerChannel, u32 Time_us);
                      void VGA_Timer_Handler(u32 null);void CDS_Read_Sign(void);    
  ���ñ������ĺ���: ��
  �������˵��:  ��   
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_VGA_Timer(void)
{
	UP_Timer_EnableIT(TIMER_CHANNEL0,1000);//ʹ�ܶ�ʱ�ж�
	UP_Timer_SetHadler(TIMER_CHANNEL0,VGA_Timer_Handler);//���ö�ʱ�жϵ���ں���	
}

/*************************************************
  ��������: UP_VGA_OPEN_USART3  
  ��������: �򿪴���3����������ڣ�����3�����ж�ʹ�ܡ�
  �����������õĺ���: void USART3_NVIC(void);
                      void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);    
  ���ñ������ĺ���: ��
  �������˵��:  ��   
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void UP_VGA_OPEN_USART3(void)
{
	USART3_NVIC();
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//����3�����ж�ʹ��	
}

void UP_VGA_SetSpeed(s16 left,s16 right)
{
	Speed1 = left;//
	Speed2 = 0-right;//
	Speed3 = left;//
	Speed4 = 0-right;//
}


void UP_VGA_DIS_TRACKSIGN(u8 line)
{
	u8 i;
	for(i=0;i<7;i++)				//��ʾѭ�����7�����״̬
	{
		if((UP_VGA_TrackSign & (1<<i)))
			UP_LCD_ShowInt(2*i+1,line,1);
		else
			UP_LCD_ShowInt(2*i+1,line,0);
	}	
}

void UP_VGA_MOTION_FORWARD(void)
{
	UP_VGA_ForwardSign = 1;
}
void UP_VGA_MOTION_STOP(void)
{
	UP_VGA_ForwardSign = 0;
	UP_VGA_SetSpeed(0,0);
}
void UP_VGA_MOTION_TURN(u8 dir)
{
	u8 temp;//,i;
	temp = UP_VGA_ForwardSign;
	UP_VGA_ForwardSign = 0;
	UP_VGA_SetSpeed(speed,speed);
	
	while(iLeft2|iLeft3|iRight2|iRight3)
	{
		UP_delay_ms(1);
		UP_VGA_SetSpeed(speed,speed);
	}
	UP_VGA_SetSpeed(0,0);
	UP_delay_ms(200);
	if(dir==1)
	{
		UP_VGA_MOTION_STOP();
		UP_VGA_SetSpeed(0-speed,speed);
		UP_delay_ms(50);
		while(iLeft3 != 1)
		{		
			UP_delay_ms(1);//UP_VGA_DIS_TRACKSIGN(2);
			UP_VGA_SetSpeed(0-speed,speed);
		}
		UP_VGA_SetSpeed(0,0);
		UP_delay_ms(200);
		while(!((iLeft==1|iMid==1|iRight==1)&&(iLeft2|iLeft3|iRight2|iRight3)==0))//while(!((iMid==1)&&(iLeft|iLeft2|iLeft3|iRight|iRight2|iRight3)==0))
		{
			UP_delay_ms(1);//UP_VGA_DIS_TRACKSIGN(2);	
			UP_VGA_SetSpeed(0-speed,speed);			
		}	
  }
	else if(dir==2)
	{
		UP_VGA_MOTION_STOP();		
		UP_VGA_SetSpeed(speed,0-speed);
		UP_delay_ms(50);
		while(iRight3 != 1)
		{		
			UP_delay_ms(1);//UP_VGA_DIS_TRACKSIGN(2);
			UP_VGA_SetSpeed(speed,0-speed);
		}
		UP_VGA_SetSpeed(0,0);
		UP_delay_ms(200);
		while(!((iLeft==1|iMid==1|iRight==1)&&(iLeft2|iLeft3|iRight2|iRight3)==0))//while(!((iMid==1)&&(iLeft|iLeft2|iLeft3|iRight|iRight2|iRight3)==0))
		{
			UP_delay_ms(1);//UP_VGA_DIS_TRACKSIGN(2);		
			UP_VGA_SetSpeed(speed,0-speed);
		}	

	}

	UP_VGA_SetSpeed(0,0);
  UP_VGA_ForwardSign = temp;
}

void UP_VGA_PROTECTSET(u8 ADCChannel,u16 ProtectDis)
{
	UP_VGA_ProAdcChannel = ADCChannel;
	UP_VGA_ProDis = ProtectDis;
}
