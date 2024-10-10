#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"


uint8_t message = 0x00;
uint8_t PORTF_Interrupt = 0x00;

void GPIOPortF_Handler();

int main(void)
{
    SYSCTL_RCGC2_R |= 0x00000020;       

    // GPIO_PORTF_REGISTERS
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     
    GPIO_PORTF_CR_R = 0x1F;             
    GPIO_PORTF_DEN_R = 0x1F;            
    GPIO_PORTF_DIR_R = 0x0E;           
    GPIO_PORTF_PUR_R = 0x11;           

    NVIC_EN0_R = 0x40000000; 
    GPIO_PORTF_IS_R = 0x00; 
    GPIO_PORTF_IEV_R = 0x00; 
    GPIO_PORTF_IM_R = 0x10;

    // enabling to UART module 1
    SYSCTL_RCGCUART_R |= 0x02; 
    SYSCTL_RCGCGPIO_R |= 0x22; 
	
    // enabling to PORTB registers
    GPIO_PORTB_LOCK_R = 0x4C4F434B;    
    GPIO_PORTB_CR_R = 0x03;            
    GPIO_PORTB_DEN_R = 0x03;
    GPIO_PORTB_AFSEL_R = 0x03; 
    GPIO_PORTB_PCTL_R = 0x11; 
    GPIO_PORTB_DIR_R = 0x02;
    GPIO_PORTB_PUR_R = 0x02;



    // UART module 1 registers
    UART1_IBRD_R = 104;
    UART1_FBRD_R = 11;
    UART1_LCRH_R |= 0x62;
    UART1_CC_R = 0x00;
    UART1_CTL_R |= 0x01; 
    uint8_t rx_reg = 0x00;

    while(1){
        NVIC_EN0_R = 0x40000000; // 30th bit controls PORTF GPIO interrupts
        GPIO_PORTF_IM_R = 0x11; 

        UART1_DR_R = message; 
        while (UART1_FR_R & 0x08){
            ;
        }
        rx_reg = UART1_DR_R & 0xFFF; 
        if ((rx_reg & 0xFF) == 0xF0){
            GPIO_PORTF_DATA_R &= 0x04;
            GPIO_PORTF_DATA_R ^= 0x04;
            delay(500000);
        }
        else if ((rx_reg & 0xFF) == 0xAA){
            GPIO_PORTF_DATA_R &= 0x08;
            GPIO_PORTF_DATA_R ^= 0x08;
            delay(500000);
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

void delay(int us){
    NVIC_ST_RELOAD_R = 16*us;
    NVIC_ST_CTRL_R = 0x00000005;
    while( (NVIC_ST_CTRL_R & 0x00010000) != 0x00010000 ){;} 
    NVIC_ST_CTRL_R = 0x00000000;
}



void GPIOPortF_Handler(void){
    PORTF_Interrupt = GPIO_PORTF_RIS_R & 0x11; 

    
    NVIC_EN0_R = 0x00000000; 
    GPIO_PORTF_IM_R = 0x00; 
    if (PORTF_Interrupt == 0x01){ 
        GPIO_PORTF_ICR_R = 0x11; 
        message = 0xAA;
    }
    if (PORTF_Interrupt == 0x10){ 
        GPIO_PORTF_ICR_R = 0x11; 
        message = 0xF0;
    }

}
