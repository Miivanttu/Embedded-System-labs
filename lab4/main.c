/*
    Lab4 - Loop sequence, subroutines, blocking functions

    Navigate to the sequence.c to finish subroutines. Complete main function.

    To launch the UART serial connection, open the terminal and run:
    'picocom -b 115200 -d 8 -p 1 -y n /dev/ttyACM0'.
    Don't forget to restart the launch board!
*/

#include "verify.h"
#include "sequence.h"

void PortFInit(void);
void Delay1ms(uint32_t msec);

int main(void) {
    PortFInit();        // Initialize Port F
    BESGrader();
    uint32_t out = GPIO_PORTF_DATA_R&0x0A;       // output for PF3 and PF1
    while (true) {
        // Complete this functionality!
        GPIO_PORTF_DATA_R = SetGreen(out);// It is allowed to go on the slide (green)
        WaitForSWLow();
        GPIO_PORTF_DATA_R = ClearGreen(out);
        GPIO_PORTF_DATA_R = SetRed(out); // It is disallowed to go on the slide (red)
        Delay1ms(20);// Debounce the switch
        WaitForSWHigh();// Wait for a person to leave the slide
        Delay1ms(500);// Delay 500ms
        GPIO_PORTF_DATA_R = SetGreen(out) + SetRed(out);// Prepare to go down the slide (yellow)
        Delay1ms(500);// Delay 500ms
        // Repeat
    }
}

/* 
    \brief Subroutine to initialize port F pins for input and output.
    PF4 is SW1 input.
    PF3 and PF1 is output LEDs.

    \param None
    \return None
*/
void PortFInit(void){ 
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   // Turn on the clock for Port F
    delay = SYSCTL_RCGC2_R;                 // Delay to allow clock to stabilize     
    GPIO_PORTF_AMSEL_R &= 0x00;             // Disable analog function
    GPIO_PORTF_PCTL_R &= 0x00000000;        // GPIO clear bit PCTL  
    GPIO_PORTF_DIR_R &= ~0x10;              // PF4 input,
    GPIO_PORTF_DIR_R |= 0x0A;               // PF3, PF1 output  
    GPIO_PORTF_AFSEL_R &= 0x00;             // No alternate function
    GPIO_PORTF_PUR_R |= 0x10;               // Enable pullup resistor on PF4       
    GPIO_PORTF_DEN_R |= 0x1A;               // Enable digital pins PF4, PF3, PF1
}

#define DELAY_1MS 1600 // Enter value that is close to ~0.9-1ms

/*
    \brief Subroutine to delay in units of milliseconds
    \param msec Number of milliseconds to delay
    \return None
    \note Uses internal 16 MHz oscillator
*/
void Delay1ms(uint32_t msec) {
    for (; msec > 0; msec--) {
        for (volatile uint32_t i = DELAY_1MS; i > 0; i--) {}
    }
}
