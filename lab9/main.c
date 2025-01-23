/*
    Lab9 - UART

    Navigate to the convert.c to finish the ConvertDec() and ConvertDistance() functions.

    To launch the UART serial connection, open the terminal and run:
    'picocom -b 115200 -d 8 -p 1 -y n /dev/ttyACM0'.
    Don't forget to restart the launch board!
*/

#include "verify.h"
#include "convert.h"
#include "i2c_helper.h"
#include "ssd1306.h"

#define BUF_SIZE 100

void ConfigureDisplay(void);
void OLEDprint(const char* unit, const char* str, uint8_t page);

// Should remain untouched!
int main(void) {
    ConfigureUART();
    ConfigureDisplay();                   // Uncomment when OLED is connected!
    char str[BUF_SIZE];
    uint32_t num;
    UARTprintf("\nConversion program\n");
    while (true) {
        DisplayClear();                   // Uncomment when OLED is connected!
        UARTprintf("\nInput: ");
        UARTgets(str, BUF_SIZE);
        num = atoi(str);
        ConvertDec(num, str);
        UARTprintf("Dec = %s, ", str);
        OLEDprint("Dec = ", str, 0);      // Uncomment when OLED is connected!
        ConvertDistance(num, str);
        UARTprintf("Distance ~ %s", str);
        OLEDprint("Distance ~ ", str, 2); // Uncomment when OLED is connected!
        DisplayUpdate();                  // Uncomment when OLED is connected!
    }
}

/*
    \brief Configures the OLED screen display for use.
    PD0 (PUR by PB6) is SCL (Clk pin on screen), and PD1 (PUR by PB7) is SDA (Data pin on screen).
    Connect +3.3V and GND to 3Vo and GND on screen.
    \param None
    \return None
*/
void ConfigureDisplay(void) {
    I2C3Config();
    DisplayInit();
}

/*
    \brief Prints the unit and the string on the OLED screen
    \param unit Either "Dec = " or "Distance ~ " 
    \param str String to be displayed on the screen
    \param page OLED screen page to print text on
    \return None
*/
void OLEDprint(const char* unit, const char* str, uint8_t page) {
    SetCursor(0, page);
    DrawStr(unit);
    DrawStr(str);
}