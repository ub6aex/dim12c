#include "stm32f0xx.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

void USART1_Init() {
	//PINs init
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // port A enable

    // RX
	GPIOA->MODER &= ~GPIO_MODER_MODER3_0;
	GPIOA->MODER |= GPIO_MODER_MODER3_1;

    // TX
	GPIOA->MODER &= ~GPIO_MODER_MODER2_0;
	GPIOA->MODER |= GPIO_MODER_MODER2_1;

	// AF1 for USART1 signals
    GPIOA->AFR[0] = (GPIOA->AFR[0] &~ (GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3)) | (1 << (2 * 4)) | (1 << (3 * 4));

	// HSI clock as USART1 clock
	RCC->CFGR3 |= RCC_CFGR3_USART1SW;

    // Enable the peripheral clock USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	// baudrate
    USART1->BRR = 8*1000000/115200;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void USART1_Send_Byte(unsigned char ucSend_Data) {
    USART1->TDR = ucSend_Data & 0x01FF;
	while(((USART1->ISR & 0x80) >> 7)!=1);
}

unsigned char USART1_Receive_Byte(void) {
    unsigned char ucTemp = 0;
	while(((USART1->ISR & 0x20) >> 5)!=1);
	ucTemp = USART1->RDR;	
	return ucTemp;
}

void USART1_Send_String(char *pucString) {
   while(*pucString!='\0') {
       USART1_Send_Byte(*pucString);
	   pucString++;
   }
}

void USART1_Send_uInt(uint32_t number) {
  char str[8] = "";
  sprintf(str, "%lu", number);
  USART1_Send_String(str);
}
