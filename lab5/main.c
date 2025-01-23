/*
    Lab5 - Breadboard circuit building

    Navigate to the led_interface.c to finish the SetOrToggleLED() function. 
    Complete main function.

    To launch the UART serial connection, open the terminal and run:
    'picocom -b 115200 -d 8 -p 1 -y n /dev/ttyACM0'.
    Don't forget to restart the launch board!
*/

#include "led_interface.h"
#include "verify.h"

#define GPIO_PORTE_DATA_R       (*((volatile uint32_t *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile uint32_t *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile uint32_t *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile uint32_t *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile uint32_t *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile uint32_t *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile uint32_t *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))
#define SYSCTL_RCGC2_GPIOE      0x00000010  // Port E Clock Gating Control

void PortEInit(void);
void Delay100ms(uint32_t times);

int main(void){
    PortEInit(); // Student submitted subroutine
    BESGrader();
    uint32_t sw;   // input from PE0
    uint32_t out;  // output for PE1
    while (true) {
        // Complete this functionality!
        sw = GPIO_PORTE_DATA_R&0x01;
        out = GPIO_PORTE_DATA_R&0x02;
        Delay100ms(1);
        GPIO_PORTE_DATA_R = SetOrToggleLED(sw, out);
    }
}

/* 
    \brief Subroutine to initialize port E pins for input and output.
    PE0 is switch input.
    PE1 is output to the LED.

    \param None
    \return None
    \note Set the LED to be initially ON at the end of the initialization. Bit setting
    doesn't affect other bits.
*/
void PortEInit(void) {
    // Complete this function!
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;   // Turn on the clock for Port E
	delay = SYSCTL_RCGC2_R;                 // Allow time for clock to start
	GPIO_PORTE_AMSEL_R &= 0x03;             // Disable analog on PE1 and PE0 AMSEL
	GPIO_PORTE_PCTL_R &= 0x03;              // Clear PE1 and PE0 bit fields PCTL to configure as GPIO
	GPIO_PORTE_DIR_R |= 0x02;               // PE0 in, PE1 out
	GPIO_PORTE_AFSEL_R &= 0x03;             // Clear PE1 and PE0 bits AFSEL to disable alternate functions
	GPIO_PORTE_PUR_R &= ~0x03;              // Clear PE1 and PE0 PUR
	GPIO_PORTE_DEN_R |= 0x03;               // Set PE1 and PE0 bits DEN to enable digital
	GPIO_PORTE_DATA_R |= 0x02;              // Set PE1 so LED is initially ON      
}

#define DELAY_100MS 146500 // ~100ms

/*
    \brief Subroutine to delay 100 milliseconds N times
    \param times Number of times to delay 100 ms
    \return None
    \note Assumes 16 MHz clock
*/
void Delay100ms(uint32_t times) {
    for (; times > 0; times--) {
        for (volatile uint32_t i = DELAY_100MS; i > 0; i--) {}
    }
}
