#include "stm32f0xx.h"
#include "rcc-init.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "tm1637.h"

int main(void) {
    rccInit();
    gpioInit();
    timerInit();
    tm1637Init();
    tm1637SetBrightness(1);
    USART1_Init();

    uint8_t *dmxBuf = get_Dmx_Bufer_ptr();
    set_Dmx_Address(1);

	for(;;) {
        tm1637DisplayDecimal(dmxBuf[11], 0);
	}
}
