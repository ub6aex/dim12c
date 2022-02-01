#include "stm32f0xx.h"
#include "gpio.h"

void gpioInit(void) {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER4_0;
	status_led_off();
}

void status_led_on(void) {
	GPIOA->BSRR = GPIO_BSRR_BR_4;
}

void status_led_off(void) {
	GPIOA->BSRR = GPIO_BSRR_BS_4;
}
