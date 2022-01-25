#ifndef __STM32_tm1637_H
#define __STM32_tm1637_H

#include "stm32f0xx.h"

void tm1637Init(void);
void tm1637DisplayDecimal(uint16_t value, uint8_t displaySeparator);
void tm1637SetBrightness(uint8_t brightness);

#endif
