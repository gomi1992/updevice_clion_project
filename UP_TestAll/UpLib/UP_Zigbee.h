/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  : ����Ԫ  
//  Update Date   : 2011/06/12
//  Version   	  : 1.2            
//  Description   :  
/******************************************************************/

#ifndef ZIGBEE_H
#define ZIGBEE_H

#include "STM32Lib/stm32f10x.h"
extern u8 UP_Zigbee_RecBuf[80];
/*************************************************************
*			 ����zigbee�豸��ز���	   ��Ҫ�û��������������޸�
*************************************************************/
#define  PANID_L			 0x07		 /**zigbee�豸���������� ���ֽ� **/
#define	 PANID_H	         0x02		 /**zigbee�豸���������� ���ֽ�  ׿Խ֮�ǲ��Բ��� 12 **/

#define	 DEVICE_ID			  0       /******�����豸 ID ����ʱ����ע�� ��0x00��ַ��Ӧzigbee���豸����ΪЭ������·����������Ϊ�� 0x00 ���������*****/

#define  DEST_ID_1			  1		 /******�������ݵ�Ŀ�ĵ�ַ*****/
#define  DEST_ID_2			  2	
#define  DEST_ID_3			  3	
#define  DEST_ID_4			  4	
#define  DEST_ID_5			  5	
#define  DEST_ID_6			  6	
#define  DEST_ID_7			  7	

/**+++++++++++++++++++++++++++++++++++++++**/
#if(DEVICE_ID==0x00)
	#define 	DEVICE_TYPE   0x00	/**�豸����ΪЭ����**/
#else 
  #define 	    DEVICE_TYPE   0x01	/**�豸����Ϊ·����**/
#endif


typedef struct _UP_ZIGBEE_FRAME
{
	u8 frame_type;
	u8 frame_len;
	u8 *frame_data;
	u8 rec_sign;
	u32 rec_count;
}UP_ZIGBEE_FRAME;



extern u8 zigbee_rec_count;
extern u8 zigbee_receive_num;
extern UP_ZIGBEE_FRAME UP_Zigbee_RecFrame;
extern u8 zigbee_tx_buffer[5]; /***zigbee �������ݻ�����**/

//Zigbee��غ���
void zigbee_init(void);
void UP_Zigbee_EnableIT(void);					//ʹ��Zigbee���ݽ����ж�
void UP_SetZigbeeHadler(void (*ApplicationAddress)(u32));		//����Zigbee���ݽ����ж���ں���
void UP_Zigbee_Putc(unsigned char c);		//Zigbee����һ���ַ�
void UP_Zigbee_Puts(char * str);				//Zigbee����һ���ַ�	
void UP_Zigbee_Rec(u32 data);
void UP_Zigbee_SetID(u8 ID);
void UP_Zigbee_SetDeviceType(u8 type);
extern void UP_Zigbee_RequireSensor(u8 S_ID,u8 D_ID);
void UP_Zigbee_SendData(u8 D_ID,u8 len,u8 *data);
void UP_Zigbee_GetConfig(void);
void UP_Zigbee_GetID(void);
extern void Zigbee_TIMER_handler(u32 null);
extern void UP_Push_TxData(u8 S_ID,u8 D_ID,u8 len,u8 cmd,u8 *data);
#endif
