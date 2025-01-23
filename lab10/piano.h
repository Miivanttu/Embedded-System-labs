#ifndef PIANO_H
#define PIANO_H

#include <stdint.h>

#ifndef GPIO_PORTE_DATA_R
#define GPIO_PORTE_DATA_R       (*((volatile uint32_t *)0x400243FC))
#endif

#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))
#define SYSCTL_RCGC2_GPIOE      0x00000010  // Port E Clock Gating Control
#define GPIO_PORTE_DIR_R        (*((volatile uint32_t *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile uint32_t *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile uint32_t *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile uint32_t *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile uint32_t *)0x4002452C))

/* 
    \brief Piano keys initialization. Subroutine initializes port E pins for input.
    PE0-3 is input.

    \param None
    \return None
    \note Activate the Port E and use AMSEL, PCTL, AFSEL, DIR, DEN registers. Set data register to 0.
*/
void PianoInit(void);

/* 
    \brief Input from piano keys. Subroutine returns the value of each PE0-3 pin.

    \param None
    \return PE0-3 switch status
    \note We do not consider multiple key presses.
*/
uint32_t PianoInput(void);

#endif /* PIANO_H */