#include "STM32Lib/stm32f10x.h"
#include "UP_Beer.h"
#include "UP_Timer.h"
#include "UP_IOout.h"
u32 g_Timer2Count= 0;		//��ʱ��2�ļ���ֵ

/*����Timer�ж���ں���*/

void TimerHadler2(u32 timerchannel)
{
	UP_IOout_SetALLIO(0);
	g_Timer2Count++;	
}
void UP_Beer_Delay(u32 nCount){
	u16 TIMCounter = nCount;
  TIM_Cmd(TIM2, ENABLE);
  TIM_SetCounter(TIM2, TIMCounter);
  while (TIMCounter>1)
  {
    TIMCounter = TIM_GetCounter(TIM2);
  }
  TIM_Cmd(TIM2, DISABLE);
}
//Rate ֵΪҪ���õ�ʱ������channel Ϊio�˿ںŷ�Χ0-5��Value �ߵ͵�ƽ���ã�
void UP_Beer_Set(u8 Channel,u32 Rate,u8 Value){
	UP_IOout_SetIO(Channel,Value);//���ö�ӦIO�����
	UP_Beer_Delay(Rate);
	UP_IOout_SetIO(Channel,0);//���ö�ӦIO�����Ϊ0
	
	//UP_Timer_EnableIT(TIMER_CHANNEL2,Rate);//ʹ�ܼ�ʱ��2����ʱʱ��rate
}