/*
    Lab8 - FSM

    Complete port initializations and main code with FSM in the main.c file.
    Navigate to fsm.h to add states.

    To launch the UART serial connection, open the terminal and run:
    'picocom -b 115200 -d 8 -p 1 -y n /dev/ttyACM0'.
    Don't forget to restart the launch board!
*/

#include "verify.h"
#include "fsm.h"

#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))

#define GPIO_PORTF_DATA_R       (*((volatile uint32_t *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile uint32_t *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile uint32_t *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile uint32_t *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile uint32_t *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile uint32_t *)0x4002552C))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // Port F Clock Gating Control
	
#define GPIO_PORTB_DATA_R       (*((volatile uint32_t *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile uint32_t *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile uint32_t *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile uint32_t *)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile uint32_t *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile uint32_t *)0x4000552C))
#define SYSCTL_RCGC2_GPIOB      0x00000002  // Port B Clock Gating Control
	
#define GPIO_PORTE_DATA_R       (*((volatile uint32_t *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile uint32_t *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile uint32_t *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile uint32_t *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile uint32_t *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile uint32_t *)0x4002452C))
#define SYSCTL_RCGC2_GPIOE      0x00000010  // Port E Clock Gating Control
	
#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count Flag

void SysTickWait100ms(uint32_t times);
void SysTickInit(void);
void PortBInit(void);
void PortEInit(void);
void PortFInit(void);

#define GoCar 0 
#define WtCar 1 
#define StpCar 2 
#define GoPed 3
#define StpPed 4 
#define Off1 5 
#define Blnk1 6
#define Off2 7
#define Blnk2 8
#define Off3 9

const FSMStateData_t fsm[10] = {
	// Complete this!
    {0x04, 0x02, 50,{WtCar, GoCar, WtCar, WtCar}},     //GoCar
    {0x02, 0x02, 15,{StpCar, StpCar, StpCar, StpCar}}, //WtCar
    {0x01, 0x02, 30,{GoPed, GoCar, GoPed, GoPed}},     //StpCar
    {0x01, 0x08, 50,{Off1, Off1, GoPed, Off1}},        //GoPed
    {0x01, 0x02, 30,{GoCar, GoCar, GoPed, GoCar}},     //StpPed
    {0x01, 0x00, 5,{Blnk1, Blnk1, Blnk1, Blnk1}},      //Off1
    {0x01, 0x08, 5,{Off2, Off2, Off2, Off2}},          //Blnk1
    {0x01, 0x00, 5,{Blnk2, Blnk2, Blnk2, Blnk2}},      //Off2
    {0x01, 0x08, 5,{Off3, Off3, Off3, Off3}},          //Blnk2
    {0x01, 0x00, 5,{StpPed, StpPed, StpPed, StpPed}}   //Off3
};


int main(void) {
	PortBInit();
	PortEInit();
	PortFInit();
    SysTickInit();
	BESGrader();
    uint32_t input;           // input combination from sensors on PE0-1
    FSMState_t state; // current state index
    while (true) {
        // Complete this function!
        GPIO_PORTB_DATA_R = fsm[state].portb_out;
		GPIO_PORTF_DATA_R = fsm[state].portf_out;
	    SysTickWait100ms(fsm[state].times);
		input = GPIO_PORTE_DATA_R&0x03;
		state = fsm[state].next[input];
    }
}

/* 
    \brief Subroutine to delay for a specifiad amount of time.

    \param delay  
    \return The delay parameter is in units of the 16 MHz core clock (62.5 ns).
*/
void SysTickWait(uint32_t delay) {
    NVIC_ST_RELOAD_R = delay - 1;                           // number of counts to wait
    NVIC_ST_CURRENT_R = 0;                                  // any value written to CURRENT clears it
    while ((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT) == 0) {}   // wait for count flag
}

/* 
    \brief Subroutine to delay exactly 100ms X times.

    \param times How many times to delay 100ms
    \return None
	\note Use this function for delays in the main program.
*/
void SysTickWait100ms(uint32_t times) {
    for (uint32_t i = 0; i < times; i++) {
        SysTickWait(1600000); // 16 MHz - 1s, ? - 100ms
    }
}

/*
    \brief Subroutine to initialize SysTick. 
    \param None
    \return None
    \note Runs at 16 MHz. 
*/
void SysTickInit(void) {
    NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
    NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}

/* 
    \brief Subroutine to initialize port B pins for output.
    PB0 is car red external LED.
    PB1 is car yellow external LED.
	PB2 is car green external LED.

    \param None
    \return None
*/
void PortBInit(void) {
    // Complete this function!
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;   // Turn on the clock for Port E
	delay = SYSCTL_RCGC2_R;                 // Allow time for clock to start   
    GPIO_PORTB_AMSEL_R &= ~0x07;        //  disable analog function
    GPIO_PORTB_PCTL_R &= ~0x00000FFF;   //  GPIO clear bit PCTL  
    GPIO_PORTB_DIR_R |= 0x07;          //  PB0-2 output  
    GPIO_PORTB_AFSEL_R &= 0x00;        //  no alternate function       
    GPIO_PORTB_DEN_R |= 0x07;          //  enable digital pins PB0-PB2
}

/* 
    \brief Subroutine to initialize port E pins for input.
    PE0 is car sensor (external switch).
    PE1 is pedestrian sensor (external switch).

    \param None
    \return None
*/
void PortEInit(void) {
    // Complete this function!
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;   // Turn on the clock for Port E
	delay = SYSCTL_RCGC2_R;                 // Allow time for clock to start
	GPIO_PORTE_AMSEL_R &= ~0x03;        // disable analog function
    GPIO_PORTE_PCTL_R &= 0x00000000;   // GPIO clear bit PCTL  
    GPIO_PORTE_DIR_R &= ~0x03;         // PE0-1 input 
    GPIO_PORTE_AFSEL_R &= 0x00;        // no alternate function       
    GPIO_PORTE_DEN_R |= 0x03;          // enable digital pins PE0-PE1
}

/* 
    \brief Subroutine to initialize port F pins for input.
    PF1 is pedestrian internal red LED.
    PF3 is pedestrian internal green LED.

    \param None
    \return None
*/
void PortFInit(void) { 
    // Complete this function!
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   // Turn on the clock for Port E
	delay = SYSCTL_RCGC2_R;                 // Allow time for clock to start
    GPIO_PORTF_AMSEL_R &= ~0x0A;        // disable analog on PF
    GPIO_PORTF_PCTL_R = 0x00000000;   // GPIO clear bit PCTL  
    GPIO_PORTF_DIR_R |= 0x0A;          // PF3, PF1 out
    GPIO_PORTF_AFSEL_R &= 0x00;        // disable alt funct on PF
	GPIO_PORTF_PUR_R = 0x00;          // enable pull-up on PF1 and PF3
    GPIO_PORTF_DEN_R |= 0xA;          // enable digital pins PF1, PF3
}
