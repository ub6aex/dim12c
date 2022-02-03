#include "stm32f0xx.h"
#include "wdg.h"
#include "rcc-init.h"
#include "gpio.h"
#include "tim.h"
#include "flash.h"
#include "usart.h"
#include "tm1637.h"

int main(void) {
    WDGinit(2000);
    rccInit();
    gpioInit();
    timerInit();
    tm1637Init();
    tm1637SetBrightness(2);
    USART1_Init();

    set_Dmx_Address(FLASH_Read_One());
    uint8_t *dmxBuf = get_Dmx_Bufer_ptr();

    // FLASH_Write_One(5);

	for(;;) {
        tm1637DisplayDecimal(dmxBuf[0], 0);
        WDGreset();
	}
}
