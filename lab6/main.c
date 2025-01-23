/*
    Lab6 - MC Debugging

    Navigate to the bits.c to finish the ToggledBits function.
    Complete DetectInputChange function.

    To launch the UART serial connection, open the terminal and run:
    'picocom -b 115200 -d 8 -p 1 -y n /dev/ttyACM0'.
    Don't forget to restart the launch board!
*/

#include "verify.h"
#include "bits.h"

#define ARR_SIZE 50

uint32_t time[ARR_SIZE] = {0}; // First data point is wrong, the other 49 will be correct
uint32_t data[ARR_SIZE] = {0}; // Leave the array defined as it is!
uint32_t i = 0;

uint32_t before;               // GPIO_PORTF_DATA_R value before the change
uint32_t last;                 // Last recorded time

void PortFInit(void);
void SysTickInit(void);
void RecordTimeAndData(void);
void DetectInputChange(void);
void Delay50ms(void);

int main(void) {
    PortFInit();
    SysTickInit();
    BESGrader();
    last = NVIC_ST_CURRENT_R;
    while (true) {
        before = GPIO_PORTF_DATA_R;
        DetectInputChange();
        RecordTimeAndData();
    }
}

/* 
    \brief Subroutine to initialize port F pins for input and output.
    PF0 and PF4 is SW1 and SW2 input respectively.
    PF1 is output to the LED.

    \param None
    \return None
*/
void PortFInit(void) {
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   // Activate clock for Port F
    delay = SYSCTL_RCGC2_R;                 // Allow time for clock to start
    GPIO_PORTF_LOCK_R = 0x4C4F434B;         // Unlock PF0
    GPIO_PORTF_CR_R = 0x1F;                 // Allow changes to PF4-0
    GPIO_PORTF_AMSEL_R = 0x00;              // Disable analog on PortF
    GPIO_PORTF_PCTL_R = 0x00000000;         // PCTL GPIO on PF4-0
    GPIO_PORTF_DIR_R |= 0x0E;               // PF4, PF0 in, PF1 out
    GPIO_PORTF_AFSEL_R = 0x00;              // Disable alt funct on PF7-0
    GPIO_PORTF_PUR_R |= 0x11;               // Enable pull-up on PF0 and PF4
    GPIO_PORTF_DEN_R |= 0x1F;               // Enable digital I/O on PF4-0      
}

/*
    \brief Subroutine to initialize SysTick with busy wait running at bus clock
    \param None
    \return None
    \note Runs at 16 MHz
*/
void SysTickInit(void) {
    NVIC_ST_CTRL_R = 0;                   // Disable SysTick during setup
    NVIC_ST_RELOAD_R = 0x00FFFFFF;        // Maximum reload value
    NVIC_ST_CURRENT_R = 0;                // Any write to current clears it             
    NVIC_ST_CTRL_R = 0x00000005;          // Enable SysTick with core clock
}

/* 
    \brief Subroutine to save time and data in the arrays only if: there is still space for new values; the values
    differ from the last.

    \param None
    \return None
*/
void RecordTimeAndData(void) {
    if (i < ARR_SIZE && ToggledBits(GPIO_PORTF_DATA_R, before)) {
        uint32_t now = NVIC_ST_CURRENT_R;      // get current clock cycles
        time[i] = (last - now) & 0x00FFFFFF;   // 24-bit time difference (gives values in ms)
        data[i] = GPIO_PORTF_DATA_R & 0x13;    // record PF0, PF1 and PF4
        last = now;
        i++;
    }
}

/* 
    \brief Subroutine to check whether any of the switches was pressed.

    \param None
    \return None
    \note If there is a switch press, toggle the red LED with a frequency of 10Hz. If no press,
    the LED is off. Delay50ms is called only when either switch is pressed.
*/
void DetectInputChange(void) {
    // Complete this function!
    uint32_t sw1 = GPIO_PORTF_DATA_R&0x01;
    uint32_t sw2 = GPIO_PORTF_DATA_R&0x10;
    if(!sw1 || !sw2)
    {
        GPIO_PORTF_DATA_R ^= 0x02;
    }
    else
    {
        GPIO_PORTF_DATA_R &= ~0x02;
    }
    Delay50ms();
    
}

#define DELAY_50MS 80500 // ~50ms

/*
    \brief Subroutine to delay 50 milliseconds
    \param None
    \return None
    \note Assumes 16 MHz clock
*/
void Delay50ms(void) {
    for (volatile uint32_t i = DELAY_50MS; i > 0; i--) {}
}
