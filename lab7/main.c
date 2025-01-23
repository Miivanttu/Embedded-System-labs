/*
    Lab7 - Interrupts and buzzer

    Finish the SysTickHandler() function.
    Complete PortAInit function.
    set NVIC_ST_RELOAD_R value.

    To launch the UART serial connection, open the terminal and run:
    'picocom -b 115200 -d 8 -p 1 -y n /dev/ttyACM0'.
    Don't forget to restart the launch board!
*/

#include "verify.h"

#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))

#define GPIO_PORTA_AMSEL_R      (*((volatile uint32_t *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile uint32_t *)0x4000452C))
#define GPIO_PORTA_DIR_R        (*((volatile uint32_t *)0x40004400))
#define GPIO_PORTA_DR8R_R       (*((volatile uint32_t *)0x40004508))
#define GPIO_PORTA_AFSEL_R      (*((volatile uint32_t *)0x40004420))
#define GPIO_PORTA_DEN_R        (*((volatile uint32_t *)0x4000451C))
#define GPIO_PORTA_DATA_R       (*((volatile uint32_t *)0x400043FC))
#define SYSCTL_RCGC2_GPIOA      0x00000001  // Port A Clock Gating Control

#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))
#define NVIC_SYS_PRI3_R         (*((volatile uint32_t *)0xE000ED20))

uint32_t wbefore = 0;
uint32_t whatDo = 0;

void PortAInit(void);
void SysTickInit(void);
void SysTickHandler(void);

// Should remain untouched!
int main(void) {
    PortAInit();
    SysTickInit();
    BESGrader();
    while (true) {}
}

/* 
    \brief Subroutine to initialize port A pins for input and output.
    PA3 is external switch input.
    PA2 is output to the buzzer.

    \param None
    \return None
    \note No analog, regular function, no alt function, digital enabled, 8-mA drive select for output
*/
void PortAInit(void) {
    // Complete this function!
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;   // Turn on the clock for Port E
	delay = SYSCTL_RCGC2_R;                 // Allow time for clock to start
	GPIO_PORTA_AMSEL_R &= 0x0C;             // Disable analog on PA3 and PA2 AMSEL
	GPIO_PORTA_PCTL_R &= 0x220011;        
    GPIO_PORTA_DR8R_R |= 0x04;
	GPIO_PORTA_DIR_R |= 0x04;               // PA2 in, PA3 out
	GPIO_PORTA_AFSEL_R &= 0x03;             // Clear PA3 and PA2 bits AFSEL to disable alternate functions
	GPIO_PORTA_DEN_R |= 0x0C;               // Set PA3 and PA2 bits DEN to enable digital
    GPIO_PORTA_DATA_R &= ~0x08;
}

/*
    \brief Subroutine to initialize SysTick. 
    \param None
    \return None
    \note Runs at 16 MHz. 
*/
void SysTickInit(void) {
    NVIC_ST_CTRL_R = 0;           // disable SysTick during setup
    NVIC_ST_RELOAD_R = 18181 - 1;     // reload value for 1.13636ms (assuming 16MHz)
    NVIC_ST_CURRENT_R = 0;        // any write to current clears it
    NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R & 0x00FFFFFF; // priority 0               
    NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts
}

/*
    \brief Subroutine to handle the SysTick interrupt. When switch is pressed, the buzzer
    is emmiting a tone at 440 Hz. When the switch is pressed again, the buzzer turns off.
    \param None
    \return None
    \note Interrupt at 1/880Hz = 1.13636ms. 
    When the switch is held down, the buzzer tone is toggled on/off once!
*/
void SysTickHandler(void) {
    // Complete this function!
    uint32_t sw = GPIO_PORTA_DATA_R&0x08;
    
    if (!wbefore && sw)
    {
        whatDo = !whatDo;
    }
        if (whatDo)
        {
            GPIO_PORTA_DATA_R ^= 0x04;
        }
        else if(!whatDo)
        {
            GPIO_PORTA_DATA_R &= ~0x04;
        }
        
    wbefore = sw;
    
}