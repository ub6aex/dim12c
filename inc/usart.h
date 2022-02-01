#ifndef _USART_H
#define _USART_H

#define DMX_START_CODE 0 // DMX512 start code to react to (0 is for dimmers)
#define DMX_CHANNELS_NUM 12 // maximum number of channels supported by hardware

void USART1_Init();
void USART1_Send_Byte(unsigned char ucSend_Data);
// uint8_t USART1_Receive_Byte(void);
void USART1_Send_String(char *pucString);
void USART1_Send_uInt(uint32_t number);
uint8_t* get_Dmx_Bufer_ptr(void);
void set_Dmx_Address(uint8_t addr);

#endif
