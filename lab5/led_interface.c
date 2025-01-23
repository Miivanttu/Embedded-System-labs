#include "led_interface.h"

uint32_t SetOrToggleLED(uint32_t sw, uint32_t led_val) {
    // Complete this function!
    if(sw)
    {
        return led_val ^= 0x02;
    }
    return led_val |= 0x02;
}