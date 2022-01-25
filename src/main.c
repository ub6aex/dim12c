#include "stm32f0xx.h"

int main(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER4_0;

	for(;;){
        GPIOA->BSRR = GPIO_BSRR_BR_4;
        for(int i = 0; i < 60000; i++);
        GPIOA->BSRR = GPIO_BSRR_BS_4;
        for(int i = 0; i < 60000; i++);
	}
}
