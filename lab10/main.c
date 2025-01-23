/*
    Lab10 - DAC

    Navigate to the dac.c/piano.c/sound.c to finish functions.
    Complete main function.

    To launch the UART serial connection, open the terminal and run:
    'picocom -b 115200 -d 8 -p 1 -y n /dev/ttyACM0'.
    Don't forget to restart the launch board!
*/

#include "verify.h"
#include "sound.h"
#include "piano.h"
#include "dac.h"

#define DELAY_20MS 27000

void Delay20ms(void);

int main(void) {
    SoundInit();
    PianoInit();
    BESGrader();
    while (true) {
        // Complete functionality!
        uint32_t inputData = PianoInput();
        uint32_t note = ChooseNote(inputData);
        SoundPlay(note);
    }
}

/*
    \brief Subroutine to delay 20 milliseconds
    \param None
    \return None
    \note Assumes 16 MHz clock
*/
void Delay20ms(void) {
    for (volatile uint32_t i = DELAY_20MS; i > 0; i--) {}
}
