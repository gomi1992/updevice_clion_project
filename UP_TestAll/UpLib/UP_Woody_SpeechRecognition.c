/******************************************************************************** 
 * @file     UP_Woody_SpeechRecognition.c
 * @brief    ����ʶ����ģ�� 
 * @details  1��Variable is not standardized��2��some fuction used last version ,
 *this version,We will not delete them,because of i am not sure if they will be use.
 *1:����汾�淶��ȫ�ֱ�����ʹ��2:Ԥ��δʹ�õ��ĺ���
 * @����ʶ����ʹ�û������̣���һ����ϵͳ��ʼ��֮ǰ����UP_Woody_Init();��ʼ��Woodyģ��
 * @����ʶ����ʹ�û������̣��ڶ���������UP_Woody_StartUp(0x01);����Woody�������Ź���,(һ���ֽڵĵ�һλΪ1,0000 0001)
 * @����ʶ����ʹ�û������̣�������������UP_Woody_SpeechRecLetIssue();(����woody�·�ʶ����)
 * @����ʶ���ܵĻ���ʹ�����̣����Ĳ����ж�ʶ����UP_VoiceId
 * @author   nealwang
 * @date     18/12/2015 
 * @version  v1.0 
 * @par Copyright (c): UpTech 
 * @par History: version: v0.8 
 ********************************************************************************/  
#include"UP_System.h"

#include "UP_Globle.h"
#include "UP_LCD.h"
#include "UP_UART.h"
u8 UP_VoiceId=0;
u8 Flage = 0;				//���ձ�־
u8 Tab=0;
u8 Var=0;
static u8 num=0;
static u16 ID;
char UP_Woody_ComparisonTables[7]={0X01,0X02,0X03,0X04,0X05,0X06,0X07};
char TestCommand[5] = {0x55,0xAA,0x05,0x01,0x05};		
char  UP_Woody_StartSpeech[5]={0x55,0xAA,0x05,0x10,0x14};
char  UP_Woody_Read[5]={0x55,0xAA,0x05,0x12,0x16};
char  UP_Woody_Closing[5]={0x55,0xAA,0x05,0x11,0x15};
char  UP_Woody_ReturnData[6]={0x00};

/*************************************************
  ��������: UP_Woody_Speech_ClearData
  ��������: ����UP_Woody_ReturnData[6]
  �����������õĺ���: ��
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
*************************************************/	
void UP_Woody_Speech_ClearData(void)
{
	for(Var=0;Var<6;Var++)
    {
			UP_Woody_ReturnData[Var]=0x00;
		}
}

/*************************************************
  ��������: UP_Display_Return_Text
  ��������: ��ʾ����ͨ��ʱ���ص�����
  �����������õĺ���: void UP_LCD_ShowHex(u8 x,u8 y,u8 hex);UP_delay_ms(u32 ms);
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
*************************************************/	
void  UP_Display_Return_Text(void)
 {
	 
if(Flage==0x01)
{

	  UP_delay_ms(500);
	  UP_LCD_ShowHex(1,0,UP_Woody_ReturnData[0]);
		UP_LCD_ShowHex(3,0,UP_Woody_ReturnData[1]);
	
		UP_LCD_ShowHex(5,0,UP_Woody_ReturnData[2]);
		
		UP_LCD_ShowHex(7,0,UP_Woody_ReturnData[3]);

		UP_LCD_ShowHex(9,0,UP_Woody_ReturnData[4]);
		
		Flage=0;
   	UP_delay_ms(500);
	
   }		
}

/*************************************************
  ��������: UP_Display_Return_Read
  ��������: ��ʾ������ʱ���ص�����
  �����������õĺ���: void UP_LCD_ShowHex(u8 x,u8 y,u8 hex);UP_delay_ms(u32 ms);
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
*************************************************/
 void  UP_Display_Return_Read(void)
 {
	 if(Flage==0x01)
	 {
	  UP_delay_ms(1000);
	  UP_LCD_ShowHex(0,1,UP_Woody_ReturnData[0]);
		
		UP_LCD_ShowHex(2,1,UP_Woody_ReturnData[1]);
		
		UP_LCD_ShowHex(4,1,UP_Woody_ReturnData[2]);
		
		UP_LCD_ShowHex(6,1,UP_Woody_ReturnData[3]);
		
		UP_LCD_ShowHex(8,1,UP_Woody_ReturnData[4]);
		
		UP_LCD_ShowHex(10,1,UP_Woody_ReturnData[5]);
	 
		Flage=0;	
		UP_delay_ms(1000);
  }
}

/*************************************************
  ��������: UP_Woody_Speech_ID_Judge
  ��������: ����ID�ż��㴦��
  �����������õĺ���: ��
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ID,ʮ������ֵ��ʽ����Χ1~255��
  ����˵��: ��        
*************************************************/
u16 UP_Woody_Speech_ID_Judge(void)
{
  ID=(UP_Woody_ReturnData[4]&0X0F)+((UP_Woody_ReturnData[4]&0XF0)>>4)*16;
	return ID;
}

/*************************************************
  ��������: UP_Woody_Display_Speech_ID
  ��������: ��ʾ����ʶ�𷵻ص�ID
  �����������õĺ���: UP_LCD_ClearScreen();
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: �˺�����ʾ��LCD�ĵ����У�Լ3s��ʾһ�Σ��û��ɸ�����ʾʱ�䡣    
*************************************************/
 void UP_Woody_Display_Speech_ID(void)
  {
	 printf("\n");
	 printf("\n");
   printf("(S_ID)(%lud)\n",UP_VoiceId);
	 UP_delay_ms(3000);
	 UP_LCD_ClearScreen();
	}
	
	
/*************************************************
/*************************************************
  ��������: UP_Woody_TestCommand
  ��������: ����LUBY��Woody֮���ͨ���Ƿ�ɹ�
  �����������õĺ���: void UP_USR232_Putc(unsigned char c);
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
*************************************************/
void UP_Woody_Speech_TestCommand(void)
{
	for(Tab=0;Tab<5;Tab++)
  {
	   UP_USR232_Putc(TestCommand[Tab]);
  }	
}



/*************************************************
  ��������: UP_Woody_Speech_ReadData����ʱԤ���ú���
  ��������: ���Ͷ�����Э��
  �����������õĺ���: void UP_USR232_Putc(unsigned char c);
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��     
*************************************************/
void UP_Woody_Speech_ReadData(void)
{
	
//   for(Tab=0;Tab<5;Tab++)
//    {
// 	   UP_USR232_Putc(UP_Woody_Read[Tab]);
//    }
//    UP_delay_ms(DELAY_TIMES);
//    revoic_flage =0; 	 
}
/**************************************
	��������: UP_Woody_SpeechRecLetIssue
  ��������: ���Ͷ�����Э��
  �����������õĺ���: void UP_USR232_Putc(unsigned char c);
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��   
**************************************/
void UP_Woody_SpeechRecLetIssue(void){
// 	char str[5]={0x55,0xAA,0x05,0x10,0x14};
// 	for(lab=0;lab<5;lab++){
// 		UP_UART5_Putc(str[lab]);
// 	}
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x05);
	UP_UART5_Putc(0x10);
	UP_UART5_Putc(0x14);
	
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
	��������: UP_Woody_SpeechRecForbidIssue
  ��������: ���Ͷ�����Э��
  �����������õĺ���: void UP_USR232_Putc(unsigned char c);
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��  
**************************************/
void UP_Woody_SpeechRecForbidIssue(void){
	//char str[5]={0x55,0xAA,0x05,0x11,0x15};
	//for(lab=0;lab<5;lab++){
	//	UP_UART5_Putc(str[lab]);
	//}
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x05);
	UP_UART5_Putc(0x10);
	UP_UART5_Putc(0x14);
	UP_delay_ms(DELAY_TIMES);
}
