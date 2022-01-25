#include "stm32f0xx.h"
#include "tim.h"

// #define SYSTEMCORECLOCK 48000000000
#define SYSTEMCOREFREQ 48000000U        // System Clock Frequency

void timerInit(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // TIM2 clock enable
	TIM3->PSC = 24000-1;                 // prescaler for 500us delay
	TIM3->CR1 |= TIM_CR1_OPM;           // one pulse mode
}

void delay_ms(uint16_t value) {
	TIM3->ARR = (value-1)*2;
	TIM3->CNT = 0;
	TIM3->CR1 |= TIM_CR1_CEN; // start timer
	while((TIM3->SR & TIM_SR_UIF)==0){} // wait
	TIM3->SR &= ~TIM_SR_UIF; // reset flag
}

void delay_us(uint32_t micros) {
	micros *= (SYSTEMCOREFREQ / 1000000) / 10;
	while (micros--);
}
