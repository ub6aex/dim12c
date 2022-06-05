#include "stm32f0xx.h"
#include "tim.h"
#include "i2c.h"

#define I2C1_OWN_ADDRESS 0x5A
// #define I2C_MODE_I2C 0x00000000U

// #define SLAVE_OWN_ADDRESS                       0xD0 // real address is 0x68
// #define I2C_REQUEST_WRITE                       0x00
// #define I2C_REQUEST_READ                        0x01

__IO uint32_t tmpreg;

void I2C1_Init(void) {
    // enable port A
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // open drain for I2C signals
    GPIOA->OTYPER |= GPIO_OTYPER_OT_9; // SCL
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR9_0; // OD + PU

    GPIOA->OTYPER |= GPIO_OTYPER_OT_10; // SDA
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR10_0; // OD + PU

    // AF4 for I2C signals
    GPIOA->AFR[1] = (GPIOA->AFR[1] & ~(GPIO_AFRH_AFRH1 | GPIO_AFRH_AFRH2)) \
                  | (4 << (1 * 4)) | (4 << (2 * 4));

    // Select AF mode (10) on PA9 and PA10
    GPIOA->MODER &= ~GPIO_MODER_MODER9_0;
    GPIOA->MODER |= GPIO_MODER_MODER9_1;
    GPIOA->MODER &= ~GPIO_MODER_MODER10_0;
    GPIOA->MODER |= GPIO_MODER_MODER10_1;

    // Enable the peripheral clock I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Configure I2C1, master
    /* Timing register value is computed with the AN4235 xls file,
    fast Mode @400kHz with I2CCLK = 48MHz, rise time = 140ns, fall time = 40ns */
    I2C1->TIMINGR = (uint32_t)0x00B01A4B;

    // Periph enable
    I2C1->CR1 |= I2C_CR1_PE;
    // I2C1->ISR |= I2C_ISR_TXE;

    // Slave address = 0x5A, read transfer, 1 byte to receive, autoend
    // I2C1->CR2 =  I2C_CR2_AUTOEND | (1 << 16) | I2C_CR2_RD_WRN | (I2C1_OWN_ADDRESS << 1);
    I2C1->CR2 |= I2C_CR2_AUTOEND | (1 << 16) | (I2C1_OWN_ADDRESS << 1);
}

uint8_t I2C1_TX_empty() {
    return ((I2C1->ISR & I2C_ISR_TXE) == (I2C_ISR_TXE));
}

void I2C1_WriteBytes(uint16_t addr, uint8_t *buf, uint16_t bytes_count) {
    // Write data
    uint32_t i = 0;
    for(i = 0; i < bytes_count; i++) {
        while (!I2C1_TX_empty()) {};
        I2C1->TXDR = buf[0];
        I2C1->CR2 |= I2C_CR2_START;
    }
}

void I2C1_ReadBytes(uint16_t addr, uint8_t *buf, uint16_t bytes_count) {
}
