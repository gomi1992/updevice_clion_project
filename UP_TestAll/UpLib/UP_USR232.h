/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  : �����  
//  Update Date   : 2013/07/15
//  Version   	  : 1.0            
//  Description   :  
/******************************************************************/

#ifndef USR232_H
#define USR232_H

#include "STM32Lib/stm32f10x.h"
extern unsigned  char UP_IID;
extern unsigned  char UP_size;
extern unsigned  char UP_ID_len[20];
extern unsigned  char UP_RXlen[255];
extern unsigned  char UP_buffer_TX_ID_poling[64];
extern unsigned  char UP_ID_num[16];
extern unsigned  char UP_ID_reset[20];
//USR232?��1?o����y
void UP_USR232_EnableIT(void);					//��1?��USR232��y?Y?����??D??
void UP_SetUSR232Hadler(void (*ApplicationAddress)(u32));		//��?��YUSR232��y?Y?����??D??��??��o����y
void UP_USR232_Putc(unsigned char c);		//USR232����?����???��?��?
void UP_USR232_Puts(char * str);				//USR232����?����?��?��?��?	


#endif
