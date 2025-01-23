#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/*
    \brief Initializes the Port E pin 2 for ADC0.
    Max sample rate: <=125,000 samples/second. SS3 triggering event: software trigger.
    SS3 1st sample source: channel 1. SS3 interrupts: enabled but not promoted to controller.
    \param None
    \return None
    \note Configure values for Port E initialization
*/
void ADC0Init(void);

/*
    \brief Collects a 12 bit ADC sample from PE2.
    \param None
    \return 12 bit ADC sample
*/
uint32_t ADC0In(void);

#endif /* ADC_H */