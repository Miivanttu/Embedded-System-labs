#include <stdint.h>
#include <stdbool.h>
#include "/inc/tm4c123gh6pm.h"
#include "i2c_helper.h"
#include "ssd1306.h"
#include "game_logic.h"

/*
    \brief Initialisation of the PF0, PF3 and PF4 of the Port F.
    \param None
    \return None
    \note Remember to unlock PortF to make changes.
*/
void PortFInit(void) {
    //Complete this function
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   // Turn on the clock for Port F
    delay = SYSCTL_RCGC2_R;                 // Delay to allow clock to stabilize     
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x19;
    GPIO_PORTF_AMSEL_R &= 0x00;             // Disable analog function
    GPIO_PORTF_PCTL_R &= 0x00000000;        // GPIO clear bit PCTL  
    GPIO_PORTF_DIR_R &= ~0x11;              // PF4, PF0 input
    GPIO_PORTF_DIR_R |= 0x08;               // PF3 output  
    GPIO_PORTF_AFSEL_R &= 0x00;             // No alternate function
    GPIO_PORTF_PUR_R |= 0x11;               // Enable pullup resistor on PF4, PF0       
    GPIO_PORTF_DEN_R |= 0x19;               // Enable digital pins PF4, PF3, PF0
}

/* 
    \brief Subroutine initializes SysTick periodic interrupts.
    \param None
    \return None
    \note RELOAD should be 0, priority is 0, enable with core clock and interrupts.
*/
void SysTickInit(void) {
    //Complete this function
    NVIC_ST_CTRL_R = 0;           // disable SysTick during setup
    NVIC_ST_RELOAD_R = 0;
    NVIC_ST_CURRENT_R = 0;        // any write to current clears it
    NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R & 0x00FFFFFF; // priority 0               
    NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts
}

int main(void) {
    PortFInit();   // SW1 and SW2 switch, PF3 Green LED + connected Buzzer
    SysTickInit();
    I2C3Config();
    DisplayInit();
    GameLoop();
}