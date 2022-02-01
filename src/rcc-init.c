#include "stm32f0xx.h"
#include "rcc-init.h"

void rccInit(void)
{
	__IO uint32_t StartUpCounter = 0, HSEStatus = 0, HSEStartUp_TimeOut = 0x500, RESET = 1;

	// enable HSE
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);
	
	// wait for HSE ready flag or timeout
	do {
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;  
	} while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

	if ( (RCC->CR & RCC_CR_HSERDY) != RESET) {
		HSEStatus = (uint32_t)0x01;
	} else {
		HSEStatus = (uint32_t)0x00;
	}  
 
	// If HSE started ok
	if ( HSEStatus == (uint32_t)0x01) {
		// Flash prefetch buffer enable
		FLASH->ACR |= FLASH_ACR_PRFTBE;
	
		// Flash latency
		FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY;

		// set PLL source HSE
		RCC->CFGR |= RCC_CFGR_PLLSRC;


		RCC->CFGR |= RCC_CFGR_PLLMULL6;

		// Enable PLL
		RCC->CR |= RCC_CR_PLLON;

		// Wait for PLL ready flag
		while((RCC->CR & RCC_CR_PLLRDY) == 0) {
		}

		// Choose PLL as a system clock
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
		RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

		// Wait for PLL is system clock
		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) {
		} 
	}  else {
		for(;;) {
			// rcc init failed. Wait for WDG reset.
		};
	}
}
