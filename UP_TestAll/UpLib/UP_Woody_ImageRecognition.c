/********************************************************************************                                         
 * @file     UP_Woody_ImageRecognition.c
 * @brief     ͼ��׽����
 * @details  1��Variable is not standardized��2��some fuction used last version ,
 *this version,We will not delete them,because of i am not sure if they will be use.
 *1:����汾�淶��ȫ�ֱ�����ʹ��2:Ԥ��δʹ�õ��ĺ���
 * @ͼ��׽����ʹ�û������̣���һ����ϵͳ��ʼ��֮ǰ����UP_Woody_Init();��ʼ��Woodyģ��
 * @ͼ��׽��ʹ�û������̣��ڶ���������UP_Woody_StartUp(0x04);����Woody�������Ź���,(һ���ֽڵĵ���λΪ1,0000 0100)
 * @ͼ��׽��ʹ�û������̣�������������UP_Woody_SpeechRecLetIssue();(����woody�·�ʶ����)
 * @ͼ��׽��ʹ�û������̣����Ĳ�������ID��HSI��ʽ������ֵ
 * @ͼ��׽��ʹ�û������̣����岽���Զ��壨ͼ��׽���յ���׽�������ص�����X��Y���꣬�ɸ����Լ���Ҫ���,���磺���ص���>1000��ִ����Ӧ������
 * @author   nealwang
 * @date     18/12/2015 
 * @version  v1.0 
 * @par Copyright (c): UpTech 
 * @par History: version: v0.8 
 ********************************************************************************/ 
#include "STM32Lib/stm32f10x.h"
#include"UP_System.h"
#include "UP_USR232.h" 
#include "UP_UART.h"
#include "UP_Globle.h"
#include "UP_Woody_ImageRecognition.h"


 static u8 i;
 static u8 lab;	
 static u8 flam;
 static u8 number;
 static u8 num;
 u8 Send[2];
 u32 Pixel_0,Pixel_1,Pixel_2,Pixel_3,Pixel_4,Pixel_5,Pixel_6,Pixel_7;
 u32 Pixel;
 u32 X_0, X_1, X_2, X_3,Y_0,Y_1,Y_2,Y_3,X,Y;
 
 //static u8 revoic_flage= 0xac;/********oxac ��ʾ��woody ���Ͷ�����ָ��*********/
char  UP_Woody_ReturnData1[20]={0x00};//������з��ص�����
 
char  UP_Woody_StartImageRecognition[5]={0x55,0xAA,0x05,0x30,0x34};//����ͼ��ʶ��ָ��

char  UP_Woody_ClosingImageRecognition[5]={0x55,0xAA,0x05,0x31,0x35};//�ر�ͼ��ʶ��ָ��

char  UP_Woody_ReturnImageRecognition[20]={0x00};//���ͼ��ʶ�𷵻ص���Ч���ص�

char  UP_Woody_White[4]={0x00,0x01,0x11,0x50};//��Ӧ���ص�69968

char  UP_Woody_Pink[4]={0x00,0x00,0xc1,0x88};//��Ӧ���ص�42516



/*************************************************
  ��������: UP_Woody_WhiteRecognize
  ��������: ���ð�ɫHSI��������
  �����������õĺ���: void UP_UART5_Putc(unsigned char c);
  ���ñ������ĺ���: ��    
  �������˵��: data:������
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��     
*************************************************/
void UP_Woody_WhiteRecognize(u8 data)
{
  number=0;
	Send[0] = (data)&0xff;
	 
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x0D);
	UP_UART5_Putc(0x33);
	UP_UART5_Putc(0x00);//H����
	UP_UART5_Putc(0xAF);//H����
	UP_UART5_Putc(0x01);//H����
  UP_UART5_Putc(0x5E);//H����
  UP_UART5_Putc(0xA9); //S����
	UP_UART5_Putc(0x00); //S����
  UP_UART5_Putc(0xFF); //I����
  UP_UART5_Putc(0x00); //I����
  UP_UART5_Putc(0x15); //У���
	UP_delay_ms(DELAY_TIMES);
	}

/*************************************************
  ��������: UP_Woody_PinkRecognize
  ��������: ���÷ۺ�ɫHSI��������
  �����������õĺ���: void UP_UART5_Putc(unsigned char c);
  ���ñ������ĺ���: ��    
  �������˵��: data:������
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��     
*************************************************/	
void UP_Woody_PinkRecognize(u8 data)
{
  number=0;
	Send[0] = (data)&0xff;
	 
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x0D);
	UP_UART5_Putc(0x33);
	UP_UART5_Putc(0x00);//H����
	UP_UART5_Putc(0x43);//H����
	UP_UART5_Putc(0x01);//H����
  UP_UART5_Putc(0x44);//H����
  UP_UART5_Putc(0xE2); //S����
	UP_UART5_Putc(0x39); //S����
  UP_UART5_Putc(0xFF); //I����
  UP_UART5_Putc(0x00); //I����
  UP_UART5_Putc(0xE1); //У���
	UP_delay_ms(DELAY_TIMES);
	}
	
/*************************************************
  ��������: UP_Woody_RedRecognize
  ��������: ���ú�ɫHSI��������
  �����������õĺ���: void UP_UART5_Putc(unsigned char c);
  ���ñ������ĺ���: ��    
  �������˵��: data:������
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��     
*************************************************/
void UP_Woody_RedRecognize(u8 data)
{
  number=0;
	Send[0] = (data)&0xff;
	 
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x0D);
	UP_UART5_Putc(0x33);
	UP_UART5_Putc(0x00);//H����
	UP_UART5_Putc(0x29);//H����
	UP_UART5_Putc(0x00);//H����
  UP_UART5_Putc(0x04);//H����
// 	UP_UART5_Putc(0x00);//H����
// 	UP_UART5_Putc(0x64);//H����
// 	UP_UART5_Putc(0x00);//H����
//   UP_UART5_Putc(0x50);//H����
	
  UP_UART5_Putc(0xA9); //S����
	UP_UART5_Putc(0x44); //S����
  UP_UART5_Putc(0xFF); //I����
  UP_UART5_Putc(0x00); //I����
  UP_UART5_Putc(0x58); //У���
	UP_delay_ms(DELAY_TIMES);
	}
	
	/*************************************************
  ��������: UP_Woody_BlueRecognize
  ��������: ������ɫHSI��������
  �����������õĺ���: void UP_UART5_Putc(unsigned char c);
  ���ñ������ĺ���: ��    
  �������˵��: data:������
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��     
*************************************************/
void UP_Woody_BlueRecognize(u8 data)
{
  number=0;
	Send[0] = (data)&0xff;
	 
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x0D);
	UP_UART5_Putc(0x33);
	UP_UART5_Putc(0x01);//H����
	UP_UART5_Putc(0x06);//H����
	UP_UART5_Putc(0x00);//H����
  UP_UART5_Putc(0xD3);//H����
  UP_UART5_Putc(0xB1); //S����
	UP_UART5_Putc(0x3F); //S����
  UP_UART5_Putc(0xFF); //I����
  UP_UART5_Putc(0x00); //I����
  UP_UART5_Putc(0x08); //У���
	UP_delay_ms(DELAY_TIMES);
	}
	
/*************************************************
  ��������: UP_Woody_YellowRecognize
  ��������: ���û�ɫHSI��������
  �����������õĺ���: void UP_UART5_Putc(unsigned char c);
  ���ñ������ĺ���: ��    
  �������˵��: data:������
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��     
*************************************************/
void UP_Woody_YellowRecognize(u8 data)
{
  number=0;
	Send[0] = (data)&0xff;
	 
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xAA);
	UP_UART5_Putc(0x0D);
	UP_UART5_Putc(0x33);
	UP_UART5_Putc(0x00);//H����
	UP_UART5_Putc(0x50);//H����
	UP_UART5_Putc(0x00);//H����
  UP_UART5_Putc(0x30);//H����
  UP_UART5_Putc(0xCF); //S����
	UP_UART5_Putc(0x54); //S����
  UP_UART5_Putc(0xDF); //I����
  UP_UART5_Putc(0x82); //I����
  UP_UART5_Putc(0x43); //У���
	UP_delay_ms(DELAY_TIMES);
	}	
/*************************************************
  ��������: UP_Woody_Display_ReturnImageRecognize
  ��������: ��ʾͼ��ʶ�𷵻ص�����
  �����������õĺ���: void UP_LCD_ShowHex(u8 x,u8 y,u8 hex);
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��     
*************************************************/
	void  UP_Woody_Display_ReturnImageRecognize(void)
 {
	 

	 if(flam==0x01)
	 {
	  UP_delay_ms(1000);
	  UP_LCD_ShowHex(0,0,UP_Woody_ReturnImageRecognition[0]);
		
		UP_LCD_ShowHex(2,0,UP_Woody_ReturnImageRecognition[1]);
		
		UP_LCD_ShowHex(4,0,UP_Woody_ReturnImageRecognition[2]);
		
		UP_LCD_ShowHex(6,0,UP_Woody_ReturnImageRecognition[3]);
		
		UP_LCD_ShowHex(0,1,UP_Woody_ReturnImageRecognition[4]);
		
		UP_LCD_ShowHex(2,1,UP_Woody_ReturnImageRecognition[5]);

		UP_LCD_ShowHex(4,1,UP_Woody_ReturnImageRecognition[6]);
		
		UP_LCD_ShowHex(6,1,UP_Woody_ReturnImageRecognition[7]);
		
		UP_LCD_ShowHex(0,2,UP_Woody_ReturnImageRecognition[8]);
		
		UP_LCD_ShowHex(2,2,UP_Woody_ReturnImageRecognition[9]);
		
		UP_LCD_ShowHex(4,2,UP_Woody_ReturnImageRecognition[10]);
		
		UP_LCD_ShowHex(6,2,UP_Woody_ReturnImageRecognition[11]);
		
		UP_LCD_ShowHex(0,3,UP_Woody_ReturnImageRecognition[12]);
		
		flam=0;	
		UP_delay_ms(1000);
		//UP_LCD_ClearScreen();
		}
}
	
/*************************************************
  ��������: UP_Woody_ImagePixel
  ��������: ���㲢�洢����ͷ���ص����ص��ֵ
  �����������õĺ���: ��
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: Pixel:�洢����ͷ���ص����ص��ֵ
  ����˵��: ��     
*************************************************/
u32 UP_Woody_ImagePixel(void)
{
	if(flam==0x01)
	{
	Pixel_0=(UP_Woody_ReturnImageRecognition[11]&0X0F);
	Pixel_1=((UP_Woody_ReturnImageRecognition[11]&0XF0)>>4)*16;//10
	
	Pixel_2=((UP_Woody_ReturnImageRecognition[10]&0X0F))*256;//100
	Pixel_3=((UP_Woody_ReturnImageRecognition[10]&0XF0)>>4)*4096;//1000
	
	Pixel_4=((UP_Woody_ReturnImageRecognition[9]&0X0F))*65536;//10000
	Pixel_5=((UP_Woody_ReturnImageRecognition[9]&0XF0)>>4)*1048576;//100000
	
	Pixel_6=((UP_Woody_ReturnImageRecognition[8]&0X0F))*16777216;//1000000
	Pixel_7=((UP_Woody_ReturnImageRecognition[8]&0XF0)>>4)*268435456;//10000000
	
	Pixel=Pixel_0+Pixel_1+Pixel_2+Pixel_3+Pixel_4+Pixel_5+Pixel_6+Pixel_7;//���ص�ֵ
	return Pixel;
	}
	flam=0;	
//	memset(UP_Woody_ReturnImageRecognition,0,18);
}

/*************************************************

/*************************************************
  ��������: UP_Woody_Display_Pixel
  ��������: ��ʾ����ͷ���ص����ص��ֵ
  �����������õĺ���: u32 UP_Woody_ImagePixel(void);
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: �˺�����ʾ��LCD�ĵ�һ�У�Լ3s��ʾһ�Σ��û��ɸ�����ʾʱ�䡣    
*************************************************/
 void UP_Woody_Display_Pixel(void)
  {
	 printf("(Pixel)%lud\n",UP_Woody_ImagePixel());
	 UP_delay_ms(3000);
	 UP_LCD_ClearScreen();
	}

/*************************************************
  ��������: UP_Woody_X_Coordinates
  ��������: ���㲢�洢����ͷ���ص�X����ֵ
  �����������õĺ���: ��
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: X:ʶ�������Xλ�ã�Xmax=320
  ����˵��: ��     
*************************************************/
u32 UP_Woody_X_Coordinates(void)
{
	if(flam==0x01)
	{
	X_0=(UP_Woody_ReturnImageRecognition[5]&0X0F);
	X_1=((UP_Woody_ReturnImageRecognition[5]&0XF0)>>4)*16;//10
	
	X_2=((UP_Woody_ReturnImageRecognition[4]&0X0F))*256;//100
	X_3=((UP_Woody_ReturnImageRecognition[4]&0XF0)>>4)*4096;//1000
	
	X=X_0+X_1+X_2+X_3;//X����ֵ
	return X;
	}
	flam=0;	
}

/*************************************************

  ��������: UP_Woody_Y_Coordinates
  ��������: ���㲢�洢����ͷ���ص�Y����ֵ
  �����������õĺ���: ��
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: Y:ʶ�������Yλ�ã�Ymax=240
  ����˵��: ��     
*************************************************/
u32 UP_Woody_Y_Coordinates(void)
{
	if(flam==0x01)
	{
	Y_0=(UP_Woody_ReturnImageRecognition[7]&0X0F);
	Y_1=((UP_Woody_ReturnImageRecognition[7]&0XF0)>>4)*16;//10
	
	Y_2=((UP_Woody_ReturnImageRecognition[6]&0X0F))*256;//100
	Y_3=((UP_Woody_ReturnImageRecognition[6]&0XF0)>>4)*4096;//1000
	
	Y=Y_0+Y_1+Y_2+Y_3;//Y����ֵ
	return Y;
	}
	flam=0;	
}

/*************************************************

/*************************************************
  ��������: UP_Woody_Display_X_Y
  ��������: ��ʾ����ͷ���ص������X��Y������ֵ
  �����������õĺ���: u32 UP_Woody_X_Coordinates(void);u32 UP_Woody_Y_Coordinates(void);void UP_LCD_ClearScreen(void);
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: �˺�����ʾ��LCD�ĵڶ��У�Լ3s��ʾһ�Σ��û��ɸ�����ʾʱ�䡣    
*************************************************/
 void UP_Woody_Display_X_Y(void)
  {
	 printf("\n");
	 printf("(X)(%lud)\n",(UP_Woody_X_Coordinates()));
	 printf("(Y)(%lud)\n",UP_Woody_Y_Coordinates());
   //printf("(X,Y)(%lud,%lud)\n",(UP_Woody_X_Coordinates(),UP_Woody_Y_Coordinates()));
	 UP_delay_ms(3000);
	 UP_LCD_ClearScreen();
	}
	
	
/*************************************************
  ��������: UP_Clear_ImageRecognize_Data
  ��������: UP_Woody_ReturnImageRecognition[20]��0
  �����������õĺ���: ��
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��     
*************************************************/
void UP_Woody_Clear_ImageRecognize_Data(void)
{
	for(i=0;i<13;i++)
    {
			UP_Woody_ReturnImageRecognition[i]=0x00;
		}
}

/*************************************************
  ��������: USR232_Hadler
  ��������: ����ͼ��ʶ���ж���ں���
  �����������õĺ���: ��
  ���ñ������ĺ���: ��    
  �������˵��: TempData:�洢uart5���ܼĴ���������
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��     
*************************************************/	
void USR232_Hadler(u32 TempData)
{   
	UP_Woody_ReturnImageRecognition[number++] = TempData;
	UP_Woody_ReturnData[num++]= TempData;
	UP_Timer_EnableIT(TIMER_CHANNEL2,9000);		 	 //ʹ�ܼ�ʱ��2����ʱʱ��9ms
	UP_Timer_Init();
	 if(num>=6)
	{
		num=0;
  }
		if(number>=13)
	{
		number=0;
		num=0;
  }
}

/*************************************************
  ��������: TimeHadler
  ��������: ���嶨ʱ���ж���ں���
  �����������õĺ���: UP_Timer_DisableIT; memset
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��     
*************************************************/
void TimeHadler(u32 timechannel)
{ 
  	
   if(timechannel == TIMER_CHANNEL2)
   {	
     UP_Timer_DisableIT(TIMER_CHANNEL2);		   /*******��ֹ��ʱ��3ͨ��2�ж�*********/
		 number = 0;
		 num = 0;
     flam=0x01;
     Flage=0x01;	
     UP_VoiceId = UP_Woody_Speech_ID_Judge(); 
		 UP_ColorId = UP_Woody_ColorRec_ID_Judge();
	   UP_CodeId = UP_Woody_BarCodeRec_ID_Judge();
			//UP_LCD_ShowInt(5,3,5);
		 memset(UP_Woody_ReturnData,0,6);        /**********�������********/
   };
   
	 
}	

/*************************************************
  ��������: UP_Woody_Init
  ��������: ͼ��ʶ���ʼ��
  �����������õĺ���: UP_EnableUSR232IT(u32 BaudRate);UP_SetUSR232Hadler(void (*ApplicationAddress)(u32));
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��     
*************************************************/	
void UP_Woody_Init(void)
 {   
//����ʹ�������ϵͳ��ʼ��֮ǰ����Ϊʹ�ܹ����趨�˴��ڵ��ж���ںͲ�����
	//ʹ��UART5�����ж� (USR232����)
	UP_EnableUSR232IT(19200);
	UP_SetUSR232Hadler(USR232_Hadler);
	UP_Timer_SetHadler(TIMER_CHANNEL2,TimeHadler);			 			 //���ݼ�ʱ���ж���ں���ָ�� 
 } 
 /**************************************
	��������: UP_Woody_ImageRecLetIssue
  ��������: ͼ��ʶ�����·�
  �����������õĺ���: UP_USR232_Putc��UP_UART5_Putc
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��   
**************************************/
void UP_Woody_ImageRecLetIssue(){
// 	char str[5]={0x55,0xAA,0x05,0x30,0x34};
// 	for(lab=0;lab<5;lab++){
// 		UP_UART5_Putc(str[lab]);
// 	}
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x05);
	UP_UART5_Putc(0x30);
	UP_UART5_Putc(0x34);
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
	��������: UP_Woody_ImageRecForbidIssue
  ��������: ͼ��ʶֹͣ�·�
  �����������õĺ���: UP_USR232_Putc��UP_UART5_Putc
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��   
**************************************/
void UP_Woody_ImageRecForbidIssue(){
// 	char str[5]={0x55,0xAA,0x05,0x31,0x35};
// 	for(lab=0;lab<5;lab++){
// 		UP_UART5_Putc(str[lab]);
// 	}
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x05);
	UP_UART5_Putc(0x31);
	UP_UART5_Putc(0x35);
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
	��������: UP_Woody_ImageRecIDSet
  ��������: ͼ��ʶ�����id����
  �����������õĺ���: UP_USR232_Putc��UP_UART5_Putc
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��   
**************************************/
void UP_Woody_ImageRecIDSet(u32 id){
	u8 sum=0x55+0xaa+0x06+0x32;
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x06);
	UP_UART5_Putc(0x32);
	UP_UART5_Putc(id);
	sum+=id;
	UP_UART5_Putc(sum);
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
	��������: UP_Woody_ImageRecHSISet
  ��������: ͼ��ʶ�����hsi����
  �����������õĺ���: UP_USR232_Putc��UP_UART5_Putc
  ���ñ������ĺ���: ��    
  �������˵��: HMax,HMin,SMax,SMin,IMax,IMin
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��  
**************************************/
void UP_Woody_ImageRecHSISet(u32 HMax,u32 HMin,u32 SMax,u32 SMin,u32 IMax,u32 IMin){
	u8 sum=0x55+0xaa+0x0D+0x33;
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x0D);
	UP_UART5_Putc(0x33);
	UP_UART5_Putc(HMax>>8&0xff);
	UP_UART5_Putc(HMax&0xff);sum+=HMax;
	UP_UART5_Putc(HMin>>8&0xff);
	UP_UART5_Putc(HMin&0xff);sum+=HMin;
	UP_UART5_Putc(SMax&0xff);sum+=SMax;
	UP_UART5_Putc(SMin&0xff);sum+=SMin;
	UP_UART5_Putc(IMax&0xff);sum+=IMax;
	UP_UART5_Putc(IMin&0xff);sum+=IMin;
	UP_UART5_Putc(sum&0xff);
// 	printf("===>%d\n",sum);
// 	UP_UART5_Putc(0x55);
// 	UP_UART5_Putc(0xaa);
// 	UP_UART5_Putc(0x0D);
// 	UP_UART5_Putc(0x33);
// 	UP_UART5_Putc(173&0xff);
// 	UP_UART5_Putc(80&0xff);
// 	UP_UART5_Putc(211&0xff);
// 	UP_UART5_Putc(41&0xff);
// 	UP_UART5_Putc(177&0xff);
// 	UP_UART5_Putc(17&0xff);
// 	UP_UART5_Putc(250&0xff);
}
