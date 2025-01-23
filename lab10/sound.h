#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

#ifndef NVIC_ST_RELOAD_R
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#endif

#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))
#define NVIC_SYS_PRI3_R         (*((volatile uint32_t *)0xE000ED20))


/* 
    \brief Subroutine initializes SysTick periodic interrupts.

    \param None
    \return None
    \note Calls DACInit(). When setting priority, only bits 32, 31 and 30 are changed.
*/
void SoundInit(void) ;

/* 
    \brief Subroutine changes SysTick interrupt period to start sound output.

    \param period Interrupt period reload value
    \return None
    \note Use only RELOAD register.
*/
void SoundPlay(uint32_t period);

/* 
    \brief Subroutine to handle the SysTick interrupt. Outputs to DAC
    with a value from the sinewave.

    \param None
    \return None
    \note Don't forget to increase the index.
*/
void SysTickHandler(void);

/*
    \brief Chooses what note would be depending on the current input.

    \param input Input value from the pressed switch
    \return Note period for the interrupt reload value
    \note Multiple presses/no input - Note is Off
*/
uint32_t ChooseNote(uint32_t input);

#endif /* SOUND_H */
