#ifndef DAC_H
#define DAC_H

#include <stdint.h>

#ifndef GPIO_PORTB_DATA_R
#define GPIO_PORTB_DATA_R       (*((volatile uint32_t *)0x400053FC))
#endif

#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))
#define SYSCTL_RCGC2_GPIOB      0x00000002  // Port B Clock Gating Control
#define GPIO_PORTB_DIR_R        (*((volatile uint32_t *)0x40005400))
#define GPIO_PORTB_DR8R_R       (*((volatile uint32_t *)0x40005508))
#define GPIO_PORTB_DEN_R        (*((volatile uint32_t *)0x4000551C))

/* 
    \brief 4-bit DAC initialization. Subroutine initializes port B pins for output.
    PB0-3 is output.

    \param None
    \return None
    \note Activate the Port B and use DR8R, DIR, DEN registers. Set data register to 0.
*/
void DACInit(void);

/* 
    \brief Subroutine to ouput data to the Port B data register.

    \param data 4-bit data (0-15)
    \return None
*/
void DACOut(uint32_t data);

#endif /* DAC_H */