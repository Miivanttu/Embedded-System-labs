#include "dac.h"

// RESERVED VARIABLE - DO NOT USE
extern uint32_t gpio_portb_data;

void DACInit(void) {
    volatile uint32_t delay;
    // Complete functionality!
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTB_DR8R_R |= 0x0F;// 8 mA pin activation
	GPIO_PORTB_DIR_R |= 0x0F;// PB0-3 out
	GPIO_PORTB_DEN_R |= 0x0F;// enable digital I/O on PB0-3
	GPIO_PORTB_DATA_R &= ~0x0F;// INITIAL VALUES ALL 0
}

void DACOut(uint32_t data) { 
    // Complete functionality!
	GPIO_PORTB_DATA_R = data; 
}