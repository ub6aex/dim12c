#include "stm32f0xx.h"
#include "wdg.h"

void WDGinit(uint16_t ms) {
    IWDG->KR=0x5555; // access key
    IWDG->PR=7; // prescaler divider 256
    IWDG->RLR=ms*40/256; // counter
    IWDG->KR=0xAAAA; // reset timer
    IWDG->KR=0xCCCC; // enable wdg
}

void WDGreset() {
    IWDG->KR=0xAAAA; // reset timer
}
