#include "sound.h"
#include "dac.h"

// RESERVED VARIABLE - DO NOT USE
extern uint32_t nvic_st_reload;

const uint32_t sinewave[] = {
    7, 8, 9, 10, 11, 12, 13, 14,
    15, 15, 15, 14, 13, 12, 11, 10,
    9, 8, 7, 6, 5, 4, 3, 2,
    1, 1, 1, 2, 3, 4, 5, 6
};
uint32_t i = 0;

void SoundInit(void) {
    // Complete functionality!
	NVIC_ST_CTRL_R = 0;// disable SysTick during setup
	NVIC_ST_RELOAD_R = 0;// reload value (if doesnt work change from 0 to any number)
	NVIC_ST_CURRENT_R = 0;// any write to current clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000; // priority 1
	NVIC_ST_CTRL_R = 0x07;// enable SysTick with core clock and interrupts
    DACInit();
}

void SoundPlay(uint32_t period) {
    // Complete functionality!
    NVIC_ST_RELOAD_R = period-1;
}

void SysTickHandler(void) {
    // Complete functionality!
    DACOut(sinewave[i]);
    i = (i + 1) % 32;
}

uint32_t ChooseNote(uint32_t input) {
    // Complete functionality!
    uint32_t sysVal;
    int N = 32;
    switch (input)
    {
        case 0x01:
            sysVal = 16000000/(523.251*N)+0.5;
            break;
        case 0x02:
            sysVal = 16000000/(587.330*N)+0.5;
            break;
        case 0x04:
            sysVal = 16000000/(659.255*N)+0.5;
            break;
        case 0x08:
            sysVal = 16000000/(783.991*N)+0.5;
            break;
        default:
            sysVal = 1;
            break;
    }
    return sysVal;
}