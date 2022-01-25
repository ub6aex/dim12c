#include "stm32f0xx.h"
#include "rcc-init.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "stm32_tm1637.h"

int main(void) {
    USART1_Init();
    USART1_Send_String("\n\rStarting");
    rccInit();
    gpioInit();
    timerInit();
    tm1637Init();
    tm1637SetBrightness(1);

    uint32_t ctr = 0;

	for(;;) {
        status_led_on();
        tm1637DisplayDecimal(ctr++, 0);
        USART1_Send_String("\n\rA");
        delay_ms(1000);

        status_led_off();
        tm1637DisplayDecimal(ctr++, 0);
        USART1_Send_String("-");
        delay_ms(1000);
	}
}
