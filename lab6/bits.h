#ifndef BITS_H
#define BITS_H

#include <stdint.h>
#include <stdbool.h>

/* 
    \brief Subroutine to return only PF4, PF1 and PF0 bits if they were toggled.

    \param data_reg Current GPIO_PORTF_DATA_R passed value
    \param before_toggle GPIO_PORTF_DATA_R value before the toggling of any bits
    \return All the toggled bits from the register
    \note Think how one bitwise operation can help check whether it toggled or not.
*/
uint32_t ToggledBits(uint32_t data_reg, uint32_t before_toggle);

#endif /* BITS_H */