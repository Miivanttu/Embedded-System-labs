#include "sequence.h"

// Complete these subroutines!

void WaitForSWLow(void) {
    uint32_t in = GPIO_PORTF_DATA_R&0x10;
    while(in)
    {
        in = GPIO_PORTF_DATA_R & 0x10;
    }
}

void WaitForSWHigh(void) {
    uint32_t in = GPIO_PORTF_DATA_R&0x10;
    while (!in)
    {
        in = GPIO_PORTF_DATA_R&0x10;
    }
}

uint32_t SetRed(uint32_t portf_data_reg) {
    return portf_data_reg |= 0x02;
}

uint32_t ClearRed(uint32_t portf_data_reg) {
    return portf_data_reg &= 0x02;
}

uint32_t SetGreen(uint32_t portf_data_reg) {
    return portf_data_reg |= 0x08;
}

uint32_t ClearGreen(uint32_t portf_data_reg) {
    return portf_data_reg &= 0x08;
}
