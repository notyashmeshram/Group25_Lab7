#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
uint8_t message = 0x00;


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

int main(void)
{
    SYSCTL_RCGC2_R |= 0x00000020;

    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_DEN_R = 0x1F;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_PUR_R = 0x11;

    NVIC_EN0_R = 0x40000000;
    GPIO_PORTF_IS_R = 0x00;
    GPIO_PORTF_IEV_R = 0x00;
    GPIO_PORTF_IM_R = 0x10;

    SYSCTL_RCGCUART_R |= 0x02;
    SYSCTL_RCGCGPIO_R |= 0x22;

    GPIO_PORTB_LOCK_R = 0x4C4F434B;
    GPIO_PORTB_CR_R = 0x03;
    GPIO_PORTB_DEN_R = 0x03;
    GPIO_PORTB_AFSEL_R = 0x03;
    GPIO_PORTB_PCTL_R = 0x11;
    GPIO_PORTB_DIR_R = 0x02;
    GPIO_PORTB_PUR_R = 0x02;




    UART1_IBRD_R = 104;
    UART1_FBRD_R = 11;
    UART1_LCRH_R |= 0x62;
    UART1_CC_R = 0x00;
    UART1_CTL_R |= 0x01;
    uint8_t rx_reg = 0x00;

    while(1){
        NVIC_EN0_R = 0x40000000;
        GPIO_PORTF_IM_R = 0x11;

        UART1_DR_R = message;
        while (UART1_FR_R & 0x08){
            ;
        }
        rx_reg = UART1_DR_R & 0xFFF;
        if ((rx_reg & 0xFF) == 0xF0){
            GPIO_PORTF_DATA_R &= 0x04;
            GPIO_PORTF_DATA_R ^= 0x04;

        }
        else if ((rx_reg & 0xFF) == 0xAA){
            GPIO_PORTF_DATA_R &= 0x08;
            GPIO_PORTF_DATA_R ^= 0x08;

        }

        else if (rx_reg & 0xF00){
            GPIO_PORTF_DATA_R &= 0x02;
            GPIO_PORTF_DATA_R ^= 0x02;
        }

        else{
            GPIO_PORTF_DATA_R &= 0x02;
            GPIO_PORTF_DATA_R ^= 0x02;
        }

    }

    return 0;
}
