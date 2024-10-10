#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

void GPIOPortF_Handler(void) {

    if (GPIO_PORTF_RIS_R & (1 << 4)) {
        GPIO_PORTF_ICR_R |= (1 << 4);
        UART1_CTL_R &= ~(0x01);
        while (UART1_FR_R & (0x08)){};
        UART1_LCRH_R &= ~(0x10);
        UART1_CTL_R |= (0x80);
        UART1_DR_R = (0xF0);
        UART1_CTL_R |= (0x01);
    }


    if (GPIO_PORTF_RIS_R & (1 << 0)) {
        GPIO_PORTF_ICR_R |= (1 << 0);
        UART1_CTL_R &= ~(0x01);
        while (UART1_FR_R & (0x08)){};
        UART1_LCRH_R &= ~(0x10);
        UART1_CTL_R |= (0x80);
        UART1_DR_R = (0xAA);
        UART1_CTL_R |= (0x01);
    }
}

