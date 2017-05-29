#include"UPLib/UP_System.h"
#include"user.h"

void id2_handle(void)
{
	zigbee_tx_buffer[0] = 2;
	UP_Zigbee_SendData(DEST_ID_1,1,zigbee_tx_buffer);
}
void id3_handle(void)
{
  	zigbee_tx_buffer[0] = 3;
	UP_Zigbee_SendData(DEST_ID_1,1,zigbee_tx_buffer);
}
void id4_handle(void)
{
	zigbee_tx_buffer[0] = 4;
	UP_Zigbee_SendData(DEST_ID_1,1,zigbee_tx_buffer);
}
void id5_handle(void)
{
   	zigbee_tx_buffer[0] = 5;
	UP_Zigbee_SendData(DEST_ID_1,1,zigbee_tx_buffer);
}
void id6_handle(void)
{
    zigbee_tx_buffer[0] = 6;
	UP_Zigbee_SendData(DEST_ID_1,1,zigbee_tx_buffer);
}

void id7_2_handle(void)
{
   	zigbee_tx_buffer[0] = 2;
	UP_Zigbee_SendData(DEST_ID_2,1,zigbee_tx_buffer);
}
void id7_3_handle(void)
{
	zigbee_tx_buffer[0] = 3;
	UP_Zigbee_SendData(DEST_ID_2,1,zigbee_tx_buffer);
}
void id8_handle(void)
{
    zigbee_tx_buffer[0] = 8;
	UP_Zigbee_SendData(DEST_ID_2,1,zigbee_tx_buffer);
}
void id9_handle(void)
{
    zigbee_tx_buffer[0] = 9;
	UP_Zigbee_SendData(DEST_ID_2,1,zigbee_tx_buffer);
}
void id10_handle(void)
{
    zigbee_tx_buffer[0] = 10;
	UP_Zigbee_SendData(DEST_ID_2,1,zigbee_tx_buffer);    
}
void id11_2_handle(void)
{
   	zigbee_tx_buffer[0] = 2;
	UP_Zigbee_SendData(DEST_ID_5,1,zigbee_tx_buffer);
}
void id11_3_handle(void)
{
	zigbee_tx_buffer[0] = 3;
	UP_Zigbee_SendData(DEST_ID_5,1,zigbee_tx_buffer);
}

















