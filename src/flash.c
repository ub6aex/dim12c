#include "stm32f0xx.h"
#include "flash.h"

#define STM32F0xx_PAGE_SIZE 0x400
#define STM32F0xx_FLASH_PAGE0_STARTADDR 0x8000000
#define STM32F0xx_FLASH_PAGE15_STARTADDR (STM32F0xx_FLASH_PAGE0_STARTADDR+15*STM32F0xx_PAGE_SIZE)
#define ADDR STM32F0xx_FLASH_PAGE15_STARTADDR // 0x08003C00

void FLASH_Unlock(void) {
    while ((FLASH->SR & FLASH_SR_BSY) != 0); // Wait till no operation is on going
    if ((FLASH->CR & FLASH_CR_LOCK) != 0) { // Check that the Flash is unlocked
        FLASH->KEYR = FLASH_FKEY1; // Perform unlock sequence
        FLASH->KEYR = FLASH_FKEY2;
    }
}

void FLASH_Lock(void) {
	FLASH->CR |= FLASH_CR_LOCK;
}

uint8_t flash_ready(void) {
	return !(FLASH->SR & FLASH_SR_BSY); // Wait until the BSY bit is reset in the FLASH_SR register
}

uint8_t check_EOP(void) {
	if(FLASH->SR & FLASH_SR_EOP) { // Check the 'End of operation' flag
		FLASH->SR |= FLASH_SR_EOP; //  Clear 'End of operation' flag by software
		return 1;
	}
	return 0;
}

uint32_t FLASH_Read(uint32_t flash_addr) {
    return (*(__IO uint32_t*)flash_addr);
}

uint8_t FLASH_Erase_Page(uint32_t page_addr) {
	while(!flash_ready()); // wait

	FLASH->CR|= FLASH_CR_PER; // enable page erasing
	FLASH->AR = page_addr; // set page address
	FLASH->CR|= FLASH_CR_STRT; // start the erasing
	while(!flash_ready());  // wait for page erase complete
	FLASH->CR &= ~FLASH_CR_PER; // disable the page erase

	return check_EOP(); // complete, clear flag
}

uint8_t FLASH_Write(uint32_t flash_addr, uint32_t data) {
	while(!flash_ready());
	
	FLASH->CR |= FLASH_CR_PG; // Set the PG bit in the FLASH_CR register to enable programming
	*(__IO uint16_t*)flash_addr = (uint16_t)data; // write two L bytes
	while(!flash_ready()); // wait
	if(!check_EOP()) return 0;

	flash_addr+=2;
	data>>=16;
	*(__IO uint16_t*)flash_addr = (uint16_t)data; // write two H bytes
	while(!flash_ready()); // wait
	FLASH->CR &= ~(FLASH_CR_PG); // Reset the PG Bit to disable programming

	return check_EOP();
}

uint32_t FLASH_Read_One() {
    return FLASH_Read(ADDR);
}

uint8_t FLASH_Write_One(uint32_t data) {
    FLASH_Unlock();
    FLASH_Erase_Page(ADDR);
    const uint8_t err = FLASH_Write(ADDR, data);
    FLASH_Lock();
    return err;
}
