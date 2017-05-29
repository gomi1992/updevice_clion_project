/******************************************************************/
//	Copyright (C), 2011-2013, �������� 
//  Author   	  	: �����  
//  Update Date   : 2013/07/12
//  Version   	  : 1.3          
//  Description   : Modification for new LCD
//  Update Date   : 2013/09/24
//  Version   	  : 1.3.1          
//  Description   : Modification for using "printf"
/******************************************************************/ 

#include "STM32Lib/stm32f10x.h"
#include "UP_LCD.h"

#define L_DELAY	12

u8 g_LCD_X = 0;
u8 g_LCD_Y = 0;

void Delay_us(u16 us)
{
	u16 i;
	while(us)
	{
		us--;
		for(i=0;i<8;i++);
	}
}

void Delay_ms(u16 ms)
{
	u16 i;
	while(ms)
	{
		ms--;
		for(i=0;i<8000;i++);
	}
}
/*************************************************
  ��������: Lcds_Config
  ��������: Lcd��ʼ��
  �����������õĺ���: ��        
  ���ñ������ĺ���: ��    
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void Lcds_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//ʹ�ܶ˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	CS_H;
	//GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50Mʱ���ٶ�
	//PC6��� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//PD9,8,10,11���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*************************************************
  ��������: SPIByte(�û�����)
  ��������: ��дSPI���ߣ���LCD����������ݡ�������д
  �����������õĺ���: ��        
  ���ñ������ĺ���: LCD_WriteCmd(u8 addr,u8 cmd);LCD_WriteData(u8 dat);
  �������˵��: byte��ָ����һ���ֽڳ��ȵ�����
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void SPIByte(u8 byte)
{
	u8 i;
	u8 temp = byte;

	for(i=0;i<8;i++)
	{
		SCL_L;
		if(temp&0x80)
			SDA_H;
		else
			SDA_L;


		SCL_H;
		temp<<=1;


	}
	SCL_H;
	SDA_H;
}

/*************************************************
  ��������: LCD_WriteCmd
  ��������: ��LCD��Ӧ�Ĵ���������Ӧ���
  �����������õĺ���: SPIByte(u8 byte);    
  ���ñ������ĺ���:   UP_LCD_Init(void);UP_LCD_OnOff(u8 onoff);UP_LCD_Fill(u8 dat);
                      UP_LCD_ClearLine(u8 y);UP_LCD_ShowCharacter(u8 x, u8 y, char cha[2]);
                      UP_LCD_ShowCharacter(u8 x, u8 y, char cha[2]);UP_LCD_ShowString(u8 x, u8 y, char *pstring);
                      UP_LCD_ShowProgressBar(u8 y,u8 percent);
  �������˵��: addr��Ҫ������LCD�ļĴ����ĵ�ַ;cmd��Ҫ�·���LCD�����
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
*************************************************/
void LCD_WriteCmd(u8 addr,u8 cmd)
{
	CS_L;

	RS_L;

	
	SCL_L;
	SDA_L;

	SCL_H;

	SPIByte(addr);

	SPIByte(cmd);

	CS_H;
}
	
/*************************************************
  ��������: LCD_WriteData
  ��������: д���ݵ�LCD
  �����������õĺ���: SPIByte(u8 byte);       
  ���ñ������ĺ���:  UP_LCD_Fill(u8 dat);UP_LCD_ClearLine(u8 y);UP_LCD_ShowLetter(u8 x, u8 y, u8 ch);
                     UP_LCD_ShowCharacter(u8 x, u8 y, char cha[2]);UP_LCD_ShowString(u8 x, u8 y, char *pstring);
                     UP_LCD_ShowCharacterString(u8 x, u8 y, char *pstring);UP_LCD_ShowProgressBar(u8 y,u8 percent);
  �������˵��: ����һ���ֽڵ����ݵ�LCD��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
*************************************************/
void LCD_WriteData(u8 dat)
{
	CS_L;

	RS_H;


	SCL_L;
	SDA_L;

	SCL_H;

	SPIByte(dat);

	CS_H;
}
/*************************************************
  ��������: UP_LCD_Init
  ��������: LCD������ʾ����
  �����������õĺ���: Lcds_Config();LCD_WriteData(u8 dat);LCD_WriteCmd(u8 addr,u8 cmd);UP_LCD_ClearScreen();	       
  ���ñ������ĺ���: ��
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
*************************************************/
void UP_LCD_Init(void)
{
	Lcds_Config();			//��ʼ��LCD�ܽ�

	RST_L;
	Delay_ms(100);
	RST_H;
	Delay_ms(100);
	
	LCD_WriteCmd(0x00,0x00);
	LCD_WriteCmd(0x01,0x00);

	LCD_WriteCmd(0x02,0x7b);

	LCD_WriteCmd(0x03,0x00);
	LCD_WriteCmd(0x04,0x00);
	LCD_WriteCmd(0x05,0x00);
	LCD_WriteCmd(0x06,0x00);
	LCD_WriteCmd(0x07,0x00);
	LCD_WriteCmd(0x08,0x00);
	LCD_WriteCmd(0x09,0x00);
	LCD_WriteCmd(0x0a,0x00);
	LCD_WriteCmd(0x0b,0x00);
	LCD_WriteCmd(0x0c,0x00);
	LCD_WriteCmd(0x0d,0x00);
	LCD_WriteCmd(0x0e,0x00);
	LCD_WriteCmd(0x0f,0x00);

	LCD_WriteCmd(0x13,0x00);
	LCD_WriteCmd(0x14,0x00);
	LCD_WriteCmd(0x15,0x00);
	LCD_WriteCmd(0x16,0x00);
	LCD_WriteCmd(0x17,0x00);
	LCD_WriteCmd(0x18,0x00);

	LCD_WriteCmd(0x10,0x2C);
	LCD_WriteCmd(0x12,0x24);
	LCD_WriteCmd(0x11,0xf8);

    LCD_WriteCmd(0x01,0x02);
	UP_LCD_Turn(0);              //�����Ƕ�Ϊ0��
	UP_LCD_ClearScreen();		  //����

	LCD_BK_ON;//������
}

/*************************************************
  ��������: UP_LCD_OnOff
  ��������: ����LCD
  �����������õĺ���: LCD_WriteCmd(u8 addr,u8 cmd);     
  ���ñ������ĺ���: ��
  �������˵��: onoff�⼴����״̬��0��1����
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��:��        
***************************************************/
void UP_LCD_OnOff(u8 onoff)
{
     if(onoff>0) 
		 LCD_WriteCmd(0x01,0x02);       //����ʾ
     else 
		 LCD_WriteCmd(0x01,0x00);   	//����ʾ
}

/*************************************************
  ��������: UP_LCD_Turn
  ��������: LCD��ʾ����ת dir=0������ʾ dir=1 180�ȷ�ת
  �����������õĺ���: LCD_WriteCmd(u8 addr,u8 cmd);     
  ���ñ������ĺ���: UP_LCD_Init(void);
  �������˵��: dec�����жϱȽϵ�ֵ
  �������˵��: ��          
  ��������ֵ: ��   
  ����˵��: ��         
***************************************************/ 
void UP_LCD_Turn(u8 dec)
{
     if(dec) 
		 LCD_WriteCmd(0x11,0xfb);
     else 
		 LCD_WriteCmd(0x11,0xf8);
}

/*************************************************
  ��������: UP_LCD_Fill
  ��������: ���LCD.
  �����������õĺ���: LCD_WriteCmd(u8 addr,u8 cmd);LCD_WriteData(u8 dat);     
  ���ñ������ĺ���: ��
  �������˵��: ����һ���ֽڵ����ݵ�LCD
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
***************************************************/ 
void UP_LCD_Fill(u8 dat)
{
    u8 i,j;
	LCD_WriteCmd(0x03,0x40);
    for(i=0;i<64;i++)
    {  
	    LCD_WriteCmd(0x05,0);
		LCD_WriteCmd(0x06,i); 
		for(j=0;j<16;j++)
		{
			LCD_WriteData(dat);
			Delay_us(350); 
		}
    }			
}

/*************************************************
  ��������: UP_LCD_ClearScreen
  ��������: ����
  �����������õĺ���: UP_LCD_Fill(u8 dat);    
  ���ñ������ĺ���: ��
  �������˵��: ��
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
***************************************************/ 
void UP_LCD_ClearScreen(void)
{
	 UP_LCD_Fill(0);
	 g_LCD_X = 0;
	 g_LCD_Y = 0;
}

/*************************************************
  ��������: UP_LCD_ClearLine
  ��������: ���ĳ��
  �����������õĺ���: LCD_WriteData(u8 dat);LCD_WriteCmd(u8 addr,u8 cmd);
  ���ñ������ĺ���: ��
  �������˵��: ����һ�����ֵ�LCD�����ĳһ�У���Χ0����3
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
***************************************************/ 
void UP_LCD_ClearLine(u8 y)
{
     u8 i,j;
     for(i=0;i<17;i++)
     {
	     LCD_WriteCmd(0x03,0x40);
		 LCD_WriteCmd(0x05,0);
	     LCD_WriteCmd(0x06,y*16+i);
		 for(j=0;j<16;j++)
		 {
			 LCD_WriteData(0);
			 Delay_us(350); 
		 }
     }
}// */

/*************************************************
  ��������: UP_LCD_ShowLetter
  ��������: ��ʾ����Ӣ���ַ�
  �����������õĺ���: LCD_WriteData(u8 dat);LCD_WriteCmd(u8 addr,u8 cmd);Delay_us(u16 us)��
  ���ñ������ĺ���: ��
  �������˵��: x������(0-15)��y������(0-3)��ch����Ӣ���ַ���
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
***************************************************/ 
void UP_LCD_ShowLetter(u8 x, u8 y, u8 ch)
{
	 LCD_WriteCmd(0x03,0x42);
	 LCD_WriteCmd(0x05,x);
	 LCD_WriteCmd(0x06,y*16);
	 LCD_WriteData(ch);
	 Delay_us(350); 
}

/*************************************************
  ��������: UP_LCD_ShowCharacter
  ��������: ��ʾ��������
  �����������õĺ���: LCD_WriteData(u8 dat);LCD_WriteCmd(u8 addr,u8 cmd);Delay_us(u16 us)��
  ���ñ������ĺ���: ��
  �������˵��: x������(0-15)��y������(0-3)��cha[i]��������ĺ��֡�
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
***************************************************/ 
void UP_LCD_ShowCharacter(u8 x, u8 y, char cha[2])
{ 
	 LCD_WriteCmd(0x03,0x43);
	 LCD_WriteCmd(0x05,x*2);
	 LCD_WriteCmd(0x06,y*16);
	 LCD_WriteData(cha[0]);
	 Delay_us(350);
	 LCD_WriteData(cha[1]);
	 Delay_us(350);
}

/*************************************************
  ��������: UP_LCD_ShowString
  ��������: ��ʾ�ַ���
  �����������õĺ���: LCD_WriteData(u8 dat);LCD_WriteCmd(u8 addr,u8 cmd);Delay_us(u16 us)��
  ���ñ������ĺ���: ��
  �������˵��: x������(0-15)��y������(0-3),*pstring�����ַ���.
  �������˵��: ��       
  ��������ֵ:  result����д��LCD���ַ��ĸ���, 0�������ַ���û��д��
  ����˵��: ��        
***************************************************/ 
u8 UP_LCD_ShowString(u8 x, u8 y, char *pstring)
{
   u8 result = 0;
   if(y > 3 || x > 16)
		return 0;
   LCD_WriteCmd(0x03,0x42);
   LCD_WriteCmd(0x05,x);
   LCD_WriteCmd(0x06,y*16);

   while(*pstring != 0)
   {
         LCD_WriteData(*pstring);
		 Delay_us(350);
		 pstring++;
		 result++;
   }
   return result;
}

/*************************************************
  ��������: UP_LCD_ShowCharacterString
  ��������: ��ʾ�����ַ���
  �����������õĺ���: LCD_WriteData(u8 dat);LCD_WriteCmd(u8 addr,u8 cmd);Delay_us(u16 us)��
  ���ñ������ĺ���: ��
  �������˵��: x������(0-15)��y������(0-3),*pstring�����ַ���.
  �������˵��: ��       
  ��������ֵ:  result����д��LCD���ַ��ĸ���
  ����˵��: ��        
***************************************************/ 
u8 UP_LCD_ShowCharacterString(u8 x, u8 y, char *pstring)
{
   u8 result = 0;
   if(y > 3 || x > 16)
		return 0;
   LCD_WriteCmd(0x03,0x43);
   LCD_WriteCmd(0x05,x*2);
   LCD_WriteCmd(0x06,y*16);

   while(*pstring != 0)
   {
         LCD_WriteData(*pstring);
		 Delay_us(400);
		 pstring++;
		 LCD_WriteData(*pstring);
		 Delay_us(400);
		 pstring++;
		 result++;
   }
   return result;
}

/*************************************************
  ��������: UP_LCD_ShowInt
  ��������: ��ʾ��������
  �����������õĺ���:UP_LCD_ShowString(u8 x, u8 y, char *pstring);
  ���ñ������ĺ���: ��
  �������˵��: x������(0-15)��y������(0-3),n������������
  �������˵��: ��       
  ��������ֵ: ����������������ĸ���
  ����˵��: ��        
***************************************************/ 
u8 UP_LCD_ShowInt(u8 x, u8 y, int n)
{
   s8 i; 
   u8 bf = 0;
   char display_buffer[8] = {0};

   if(n < 0)
   {
	  n = -n;
	  bf = 1;
   }

   for(i=6; i>=0; i--)
   {
      display_buffer[i] = '0'+n%10;
      n = n/10.0f;
      if(n == 0)
      break;
   }
   if(bf)
   {
	  i--;
	  display_buffer[i] = '-';
   }
   UP_LCD_ShowString(x,y,&display_buffer[i]);// */
   return 7-i;
}
void UP_LCD_ShowHex(u8 x,u8 y,u8 hex)
{
	char H,L;
	
	if((hex/16)<10)	H = '0'+hex/16;
	else H = 'A'+hex/16-10;
	
	if((hex%16)<10)	L = '0'+hex%16;
	else L = 'A'+hex%16-10;
	UP_LCD_ShowLetter(x,y,H);
	UP_LCD_ShowLetter(x+1,y,L);
}
/*************************************************
  ��������: UP_LCD_ShowProgressBar
  ��������: ��ʾ�ٷֱ� 0-112 step = 8 8*14 = 112 
  �����������õĺ���: LCD_WriteData(u8 dat);LCD_WriteCmd(u8 addr,u8 cmd);Delay_us(u16 us)��
  ���ñ������ĺ���: ��
  �������˵��: y��ʾ��,percent*0.125��ʾ����ֵ��С����ֵ���112.
  �������˵��: ��       
  ��������ֵ: ��
  ����˵��: ��        
***************************************************/ 
void UP_LCD_ShowProgressBar(u8 y,u8 percent)
{
	u8 i,j;
	LCD_WriteCmd(0x03,0x40);
	//show form
	for(i=(y*16+2);i<(y*16+13);i++)
	{
	    LCD_WriteCmd(0x05,1);
		LCD_WriteCmd(0x06,i);
		LCD_WriteData(0x80);
		Delay_us(50);
		LCD_WriteCmd(0x05,14);
		LCD_WriteCmd(0x06,i);
		LCD_WriteData(0x01);
		Delay_us(50);
	}
	LCD_WriteCmd(0x06,y*16+2);
	for(i=1;i<15;i++)
	{
	    LCD_WriteCmd(0x05,i);
		LCD_WriteData(255);
		Delay_us(50);
	}
	LCD_WriteCmd(0x06,y*16+13);
	for(i=1;i<15;i++)
	{
	    LCD_WriteCmd(0x05,i);
		LCD_WriteData(255);
		Delay_us(50);
	}
	//show percent
	for(j=(y*16+2);j<(y*16+13);j++)
	{
		LCD_WriteCmd(0x06,j);
		for(i=1;i<=percent;i++)
	    {
	       LCD_WriteCmd(0x05,i);
		   LCD_WriteData(255);
		   Delay_us(50);
	    }
	}
}	

/*************************************************/
//���� fputc �˺���Ϊprintf����
/*************************************************/
int fputc(int ch) 
{ 
	static u8 s_bClearScreen = 0;
	if(s_bClearScreen)
	{
		s_bClearScreen = 0;
		UP_LCD_ClearScreen();
	}
	if(ch == '\n')
	{
		g_LCD_Y++;
		g_LCD_X = 0;
	}
  if(g_LCD_X > 15)
	{	
		g_LCD_Y++;
		g_LCD_X = 0;
	}
	if(g_LCD_Y > 3)
	{
		s_bClearScreen = 1;
	}
	if(ch != '\r' && ch != '\n')
	{
		UP_LCD_ShowLetter(g_LCD_X,g_LCD_Y,(u8)ch);
		g_LCD_X++;
	}
    return ch; 
} 
