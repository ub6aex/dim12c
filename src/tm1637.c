#include "tm1637.h"
#include "stm32f0xx.h"
#include "tim.h"

void _tm1637Start(void);
void _tm1637Stop(void);
void _tm1637ReadResult(void);
void _tm1637WriteByte(unsigned char b);
void _tm1637DelayUsec(unsigned int i);
void _tm1637ClkHigh(void);
void _tm1637ClkLow(void);
void _tm1637DioHigh(void);
void _tm1637DioLow(void);

// Configuration.

const char segmentMap[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, // 0-7
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, // 8-9, A-F
    0x00
};

void tm1637Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // CLK
    // GP output open-drain
    GPIOA->MODER |= GPIO_MODER_MODER0_0;
    GPIOA->MODER &= ~GPIO_MODER_MODER0_1;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR0;

    // DIO
    // GP output open-drain
    GPIOA->MODER |= GPIO_MODER_MODER1_0;
    GPIOA->MODER &= ~GPIO_MODER_MODER1_1;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR1;

    tm1637SetBrightness(8);
}

void tm1637DisplayDecimal(uint16_t value, uint8_t displaySeparator) {
    unsigned char digitArr[4];
    uint32_t v = value;
    for (int i = 0; i < 4; ++i) {
        digitArr[i] = segmentMap[v % 10];
        if (i == 2 && displaySeparator) {
            digitArr[i] |= 1 << 7;
        }
        v /= 10;
    }
    if(value < 1000) {digitArr[3] = segmentMap[16];}
    if(value < 100) {digitArr[2] = segmentMap[16];}
    if(value < 10) {digitArr[1] = segmentMap[16];}

    _tm1637Start();
    _tm1637WriteByte(0x40);
    _tm1637ReadResult();
    _tm1637Stop();

    _tm1637Start();
    _tm1637WriteByte(0xc0);
    _tm1637ReadResult();

    for (int i = 0; i < 4; ++i) {
        _tm1637WriteByte(digitArr[3 - i]);
        _tm1637ReadResult();
    }

    _tm1637Stop();
}

// Valid brightness values: 0 - 8.
// 0 = display off.
void tm1637SetBrightness(uint8_t brightness) {
    // Brightness command:
    // 1000 0XXX = display off
    // 1000 1BBB = display on, brightness 0-7
    // X = don't care
    // B = brightness
    _tm1637Start();
    _tm1637WriteByte(0x87 + brightness);
    _tm1637ReadResult();
    _tm1637Stop();
}

void _tm1637Start(void) {
    _tm1637ClkHigh();
    _tm1637DioHigh();
    delay_us(2);
    _tm1637DioLow();
}

void _tm1637Stop(void) {
    _tm1637ClkLow();
    delay_us(2);
    _tm1637DioLow();
    delay_us(2);
    _tm1637ClkHigh();
    delay_us(2);
    _tm1637DioHigh();
}

void _tm1637ReadResult(void) {
    _tm1637ClkLow();
    delay_us(5);
    // while (dio); // We're cheating here and not actually reading back the response.
    _tm1637ClkHigh();
    delay_us(2);
    _tm1637ClkLow();
}

void _tm1637WriteByte(unsigned char b) {
    for (int i = 0; i < 8; ++i) {
        _tm1637ClkLow();
        if (b & 0x01) {
            _tm1637DioHigh();
        }
        else {
            _tm1637DioLow();
        }
        delay_us(3);
        b >>= 1;
        _tm1637ClkHigh();
        delay_us(3);
    }
}

void _tm1637ClkHigh(void) {
    GPIOA->BSRR = GPIO_BSRR_BS_0;
}

void _tm1637ClkLow(void) {
    GPIOA->BSRR = GPIO_BSRR_BR_0;
}

void _tm1637DioHigh(void) {
    GPIOA->BSRR = GPIO_BSRR_BS_1;
}

void _tm1637DioLow(void) {
    GPIOA->BSRR = GPIO_BSRR_BR_1;
}
