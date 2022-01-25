#ifndef _USART_H
#define _USART_H

void USART1_Init();
void USART1_Send_Byte(unsigned char ucSend_Data);
unsigned char USART1_Receive_Byte(void);
void USART1_Send_String(char *pucString);
void USART1_Send_uInt(uint32_t number);

#endif
