#include "piano.h"

// RESERVED VARIABLE - DO NOT USE
extern uint32_t gpio_porte_data;

void PianoInit(void) {
    volatile uint32_t delay;
    // Complete functionality!
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_AMSEL_R &= ~0x0F;// disable analog
	GPIO_PORTE_PCTL_R &= 0x00000000;// GPIO clear
	GPIO_PORTE_AFSEL_R &= 0x00;// no alternate
	GPIO_PORTE_DIR_R &= ~0x0F;// PE0-3 in
	GPIO_PORTE_DEN_R |= 0x0F;// enable digital I/O on PE0-3
	GPIO_PORTE_DATA_R &= ~0x0F;// INITIAL VALUES ALL 0
}

uint32_t PianoInput(void) {
    // Complete functionality!
	uint32_t input = GPIO_PORTE_DATA_R&0x0F;
	return input;
}