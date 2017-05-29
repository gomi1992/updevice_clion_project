/******************************************************************************** 
 * @file     UP_Woody_ColorAndCode.c
 * @brief    ��ɫʶ��������ʶ��
 * @details  1��Variable is not standardized��2��some fuction used last version ,
 *this version,We will not delete them,because of i am not sure if they will be use.
 *1:����汾�淶��ȫ�ֱ�����ʹ��2:Ԥ��δʹ�õ��ĺ���
 * @��ɫʶ��������ʶ������һ�£�������ܷ���һ��
 * @��ɫʶ��������ʶ��������̣���һ����ϵͳ��ʼ��֮ǰ����UP_Woody_Init();��ʼ��Woodyģ��
 * @��ɫʶ��������ʶ��������̣��ڶ���������UP_Woody_StartUp(0x08)��UP_Woody_StartUp(0x10);����Woody�������Ź���,(һ���ֽڵĵ��Ļ����λΪ1,0000 1000��0001 0000)
 * @��ɫʶ��������ʶ��������̣�������������UP_Woody_ColorRecLetIssue()(UP_Woody_BarCodeRecLetIssue());(����woody�·�ʶ����)
 * @��ɫʶ��������ʶ�����ʹ�����̣����Ĳ����ж�ʶ����UP_ColorId(��UP_ColorId)
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
#include "UP_Woody_ColorAndCode.h"

u8  UP_ColorId =0,UP_CodeId=0;
static u16 ID=0;
/****************************************************
��ͣ����
*****************************************************/

/**************************************
��������;UP_Woody_StartUp
��������:����woody����Ӧ����
�����������õĺ���:UP_USR232_Putc?UP_UART5_Putc
���ñ������ĺ�������
�������˵��:u32 command:��Χ0-255(ʵ�ʷ�Χ0-35,��鿴��ͣ���������)
�������˵������
��������ֵ����
����˵������
**************************************/
void UP_Woody_StartUp(u32 command){
	u32 sum=0x55+0xaa+0x06+0x02;
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x06);
	UP_UART5_Putc(0x02);
	UP_UART5_Putc(command);
	sum+=command;
	UP_UART5_Putc(sum);
	UP_delay_ms(DELAY_TIMES);
}
/******************************************************************
��ɫʶ��ģ�� 
******************************************************************/

/**************************************
�������ƣ�UP_Woody_ColorRecLetIssue
�������ܣ���ɫʶ�������·�
�����������õĺ�����UP_USR232_Putc��UP_UART5_Putc
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ����
����˵������
**************************************/
void UP_Woody_ColorRecLetIssue(){
	//char str[5]={0x55,0xAA,0x05,0x40,0x44};
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x05);
	UP_UART5_Putc(0x40);
	UP_UART5_Putc(0x44);
// 	for(lab=0;lab<5;lab++){
// 		UP_UART5_Putc(str[lab]);
// 	}
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
�������ƣ�UP_Woody_ColorRecLetIssue
�������ܣ���ɫʶ�������·�
�����������õĺ�����UP_USR232_Putc��UP_UART5_Putc
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ����
����˵������
**************************************/
void UP_Woody_ColorRecForbidIssue(){
	//char str[5]={0x55,0xAA,0x05,0x41,0x45};
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x05);
	UP_UART5_Putc(0x41);
	UP_UART5_Putc(0x45);
// 	for(lab=0;lab<5;lab++){
// 		UP_UART5_Putc(str[lab]);
// 	}
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
�������ƣ�UP_Woody_ColorRec_ID_Judge
�������ܣ���ɫʶ�𷵻ص�ID
�����������õĺ�����UP_USR232_Putc��UP_UART5_Putc
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ��ID��ʮ������ֵ��ʽ����Χ0~255
����˵������
**************************************/
u16 UP_Woody_ColorRec_ID_Judge(void)
{
	ID=(UP_Woody_ReturnData[4]&0X0F)+((UP_Woody_ReturnData[4]&0XF0)>>4)*16;
	return ID;
}
/*************************************************
�������ƣ�UP_Woody_ColorRec_ID_Judge
�������ܣ���ʾ��ɫʶ�𷵻ص�ID
�����������õĺ�������
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ����
����˵������
*************************************************/
 void UP_Woody_Display_ColorID(void)
  {
	 printf("(Pixel)%lud\n",UP_ColorId);
	 UP_delay_ms(3000);
	 UP_LCD_ClearScreen();
	}
/******************************************************************
����ʶ��ģ�� 
******************************************************************/

/**************************************
�������ƣ�UP_Woody_BarCodeRecLetIssue
�������ܣ�����ʶ�������·�
�����������õĺ�����UP_USR232_Putc��UP_UART5_Putc
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ����
����˵������
**************************************/
void UP_Woody_BarCodeRecLetIssue(){
// 	char str[5]={0x55,0xAA,0x05,0x50,0x54};
// 	for(lab=0;lab<5;lab++){
// 		UP_UART5_Putc(str[lab]);
// 	}
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x05);
	UP_UART5_Putc(0x50);
	UP_UART5_Putc(0x54);
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
�������ƣ�UP_Woody_BarCodeRecForbidIssue
�������ܣ�����ʶ�������·�
�����������õĺ�����UP_USR232_Putc��UP_UART5_Putc
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ����
����˵������
**************************************/
void UP_Woody_BarCodeRecForbidIssue(){
// 	char str[5]={0x55,0xAA,0x05,0x51,0x55};
// 	for(lab=0;lab<5;lab++){
// 		UP_UART5_Putc(str[lab]);
// 	}
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x05);
	UP_UART5_Putc(0x51);
	UP_UART5_Putc(0x55);
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
�������ƣ�UP_Woody_CodeRec_ID_Judge
�������ܣ�����ʶ�𷵻ص�ID
�����������õĺ�����UP_USR232_Putc��UP_UART5_Putc
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ��ID��ʮ������ֵ��ʽ����Χ0~255
����˵������
**************************************/
u16 UP_Woody_BarCodeRec_ID_Judge(void)
{
	ID=(UP_Woody_ReturnData[4]&0X0F)+((UP_Woody_ReturnData[4]&0XF0)>>4)*16;
	return ID;
}
/*************************************************
�������ƣ�UP_Woody_Display_BarCodeID
�������ܣ���ʾ����ʶ�𷵻ص�ID
�����������õĺ�������
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ����
����˵������
*************************************************/
 void UP_Woody_Display_BarCodeID(void)
  {
	 printf("(ID)%lu\n",UP_CodeId);
	 UP_delay_ms(2000);
	 UP_LCD_ClearScreen();
	}

/******************************************************************
��ά��ʶ��ģ�� 
******************************************************************/

/**************************************
�������ƣ�UP_Woody_QrCodeRecLetIssue
�������ܣ�����ʶ�������·�
�����������õĺ�����UP_USR232_Putc��UP_UART5_Putc
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ����
����˵������
**************************************/
void UP_Woody_QrCodeRecLetIssue(){
// 	char str[5]={0x55,0xAA,0x05,0x60,0x64};
// 	for(lab=0;lab<5;lab++){
// 		UP_UART5_Putc(str[lab]);
// 	}
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
�������ƣ�UP_Woody_QrCodeRecForbidIssue
�������ܣ�����ʶ�������·�
�����������õĺ�����UP_USR232_Putc��UP_UART5_Putc
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ����
����˵������
**************************************/
void UP_Woody_QrCodeRecForbidIssue(){
// 	char str[5]={0x55,0xAA,0x05,0x61,0x65};
// 	for(lab=0;lab<5;lab++){
// 		UP_UART5_Putc(str[lab]);
// 	}
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
�������ƣ�UP_Woody_QrCodeRec_ID_Judge
�������ܣ�����ʶ�𷵻ص�ID
�����������õĺ�����UP_USR232_Putc��UP_UART5_Putc
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ��ID��ʮ������ֵ��ʽ����Χ0~255
����˵������
**************************************/
u16 UP_Woody_QrCodeRec_ID_Judge(void)
{
	ID=(UP_Woody_ReturnData[4]&0X0F)+((UP_Woody_ReturnData[4]&0XF0)>>4)*16;
	return ID;
}
/**************************************
�������ƣ�UP_Woody_Display_QrCodeID
�������ܣ���ʾ��ά��ʶ�𷵻ص�ID
�����������õĺ�������
���ñ������ĺ�������
�������˵������
�������˵������
��������ֵ����
����˵������
*************************************************/
 void UP_Woody_Display_QrCodeID(void)
  {
	 printf("(Pixel)%lud\n",UP_Woody_BarCodeRec_ID_Judge());
	 UP_delay_ms(3000);
	 UP_LCD_ClearScreen();
	}
