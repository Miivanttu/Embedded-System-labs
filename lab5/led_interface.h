#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H

#include <stdint.h>

/*

    The function checks wheter the switch is pressed or not.
    If the switch is not pressed, LED is on. If the switch is pressed, toggle the LED.

    \param sw Current value of the switch
    \param led_val Current LED value

    \return LED value (either toggled or remains the same)

    \note The switch uses positive logic
*/
uint32_t SetOrToggleLED(uint32_t sw, uint32_t led_val);

#endif /* LED_INTERFACE_H */