#include "analog_temp.h"
#include "/inc/tm4c123gh6pm.h"

void TempSensorInit(void) {
    //Complete main function.
    SYSCTL_RCGCADC_R |= 0x0001;             // 1) Activate ADC0
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;   // 2) Activate clock for Port E
    while (!(SYSCTL_RCGC2_R & SYSCTL_RCGC2_GPIOE)) {} // 3) Wait until Port E clock ready
    GPIO_PORTE_DIR_R &= ~0x04;              // 4) Make PE2 input
    GPIO_PORTE_AMSEL_R |= 0x04;             // 5) Enable alternate function on PE2
    GPIO_PORTE_DEN_R &= ~0x04;              // 6) Disable digital I/O on PE2
    GPIO_PORTE_AMSEL_R |= 0x04;             // 7) Enable analog functionality on PE2
    while (!(SYSCTL_PRADC_R & 0x0001)) {}   // 8) Wait until ADC0 ready for access 
    ADC0_PC_R &= ~0xF;
    ADC0_PC_R |= 0x1;                       // 9) Configure for 125K samples/sec
    ADC0_SSPRI_R = 0x0123;                  // 10) Sequencer 3 is highest priority
    ADC0_ACTSS_R &= ~0x0008;                // 11) Disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000;                 // 12) Seq3 is software trigger
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 1;                     // 13) Set channel 1
    ADC0_SSCTL3_R = 0x000E;                 // 14) No D0 (bit 0), yes TS0 IE0 END0 (bits 3-1)
    ADC0_IM_R &= ~0x0008;                   // 15) Disable SS3 interrupts
    ADC0_ACTSS_R |= 0x0008;                 // 16) Enable sample sequencer 3
}

/*
    \brief Collects a 12 bit ADC sample from PE2.
    \param None
    \return 12 bit ADC sample
*/
uint32_t ADC0In(void) { 
    ADC0_PSSI_R = 0x0008;                       // 1) Initiate SS3
    while (!(ADC0_RIS_R & 0x08)) {}             // 2) Wait for conversion done
    uint32_t result = ADC0_SSFIFO3_R & 0xFFF;   // 3) Read result
    ADC0_ISC_R = 0x0008;                        // 4) Acknowledge completion
    return result;
}

/*
    \brief Convert analog value to Celsius.
    Function uses expression from Tiva C Series TM4C123GH6PM datasheet (p. 813).
    \param sample ADC data sample
    \return Temperature of CPU in Celsius
*/
float ConvertToCel(uint32_t sample) {
	return (147.5 - ((75 * 3.3 * sample) / 4096));
}

/*
    \brief Prints the decimal temperature through the UART in the format of 1 place after comma.
    \param None
    \return None
*/
void PrintTemp(float temperature) {
    int32_t integer = (int32_t)temperature;
    int32_t fraction = (int32_t)(temperature * 10.0f);
    fraction = fraction - (integer * 10);
    fraction *= fraction < 0 ? -1 : 1;
	UARTprintf("Temperature in C: %d.%d\n", integer, fraction);
}

void GetTemp(void) {
    //Complete main function
    GPIO_PORTF_DATA_R |= 0x02;// turn red LED ON
	uint32_t sample = ADC0In();// collect ADC data
	GPIO_PORTF_DATA_R &= ~0x02;// turn red LED OFF
	GPIO_PORTF_DATA_R |= 0x08;// green LED ON
	uint32_t convrtSample = ConvertToCel(sample);// convert ADC data to celsius temperature
	GPIO_PORTF_DATA_R &= ~0x08;// turn green LED OFF
    UARTprintf("Temp = ~ %d\n", convrtSample);// print the temperature
}