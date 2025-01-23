#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdint.h>
#include "/inc/tm4c123gh6pm.h"

/*
    \brief Subroutine reads SW input and waits for signal to be low.
    If SW is already low, it returns right away.
    If SW is currently high, it will wait until it goes low.
    \param None
    \return None
*/
void WaitForSWLow(void);

/*
    \brief Subroutine reads SW input and waits for signal to be high.
    If SW is already high, it returns right away.
    If SW is currently low, it will wait until it goes high.
    \param None
    \return None
*/
void WaitForSWHigh(void);
  
/*
    \brief Subroutine sets Red LED (PF1) high
    \param portf_data_reg GPIO_PORTF_DATA_R register value
    \return ``portf_data_reg`` with PF1 set high
    \note Does not affect other bits in the port!
*/
uint32_t SetRed(uint32_t portf_data_reg);

/*
    \brief Subroutine clears Red LED (PF1) low
    \param portf_data_reg GPIO_PORTF_DATA_R register value
    \return ``portf_data_reg`` with PF1 set low
    \note Does not affect other bits in the port!
*/
uint32_t ClearRed(uint32_t portf_data_reg);

/*
    \brief Subroutine sets Green LED (PF3) high
    \param portf_data_reg GPIO_PORTF_DATA_R register value
    \return ``portf_data_reg`` with PF3 set high
    \note Does not affect other bits in the port!
*/
uint32_t SetGreen(uint32_t portf_data_reg);

/*
    \brief Subroutine clears Green LED (PF3) low
    \param portf_data_reg GPIO_PORTF_DATA_R register value
    \return ``portf_data_reg`` with PF3 set low
    \note Does not affect other bits in the port!
*/
uint32_t ClearGreen(uint32_t portf_data_reg);

#endif /* SEQUENCE_H */