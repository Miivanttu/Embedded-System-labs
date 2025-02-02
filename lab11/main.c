/*
    Lab11 - ADC: Slide potentiometer, screen

    Complete functions in this file and initialization in adc.c.

    To launch the UART serial connection, open the terminal and run:
    'picocom -b 115200 -d 8 -p 1 -y n /dev/ttyACM0'.
    Don't forget to restart the launch board!
*/

#include "verify.h"
#include "i2c_helper.h"
#include "ssd1306.h"
#include "adc.h"
#include <string.h>

#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))
#define NVIC_SYS_PRI3_R         (*((volatile uint32_t *)0xE000ED20))

// Calibration 
#define A 740
#define B 3

#define DISTANCE_LEN 8

void ConvertDistance(uint32_t num);
void SysTickInit(void);
void SysTickHandler(void);
void ConfigureDisplay(void);
void OLEDprint(const char* unit, const char* str);

char str[DISTANCE_LEN + 1]; // + 1 for '\0'
uint32_t distance,adc;
uint32_t flag = 0;

int main(void) {
    ADC0Init();
    SysTickInit();
    //ConfigureDisplay(); // Comment out if using UART
    BESGrader();
    while (true) {
    //Complete main function.
        if (flag == 1)
        {
            ConvertDistance(distance);
            UARTprintf("Distance ~ %s\n", str); // Comment out if using display
            flag = 0;
        }
    }
}

/*
    \brief Converts the decimal number to a formatted distance string.
    4       to "0.004 cm", 
    31      to "0.031 cm",
    102     to "0.102 cm",
    2210    to "2.210 cm",
    10000   to "*.*** cm",
    >9999   to "*.*** cm".
    \param num Decimal numerical value to be converted to a string
    \return None
    \note Remember to set the last character as '\0'!
*/
void ConvertDistance(uint32_t num) {
    // Copy here your solution from Lab 9!
    int i = 0;
	memset(str,0,strlen(str));
	if(num > 9999)
	{
		while (strlen(str) != 5) 
		{
    		str[i++] = '*';
			if (strlen(str) == 3)
				str[i++] = '.';
    	}
	}
	else
	{
		while (num > 0) 
		{
			str[i++] = num % 10 + '0';
			if (strlen(str) == 3)
				str[i++] = '.';
			num /= 10;
		}
		if (strlen(str) <= 4)
		{
			int len = strlen(str);
			for (len; len < 5;len= strlen(str))
			{
				if (strlen(str) == 3)
					str[i++] = '.';
				str[i++] = '0';
			}
		}
	}
	for (int j = 0, k = i - 1; j < k; j++, k--) 
	{
		char num = str[j];
		str[j] = str[k];
		str[k] = num;
	}
	const char end[] = " cm\0";
	strcat(str, end);
}

/* 
    \brief Subroutine initializes SysTick periodic interrupts.
    \param None
    \return None
    \note Initialize SysTick interrupts to trigger at 40 Hz, 25 ms.
*/
void SysTickInit(void) {
    NVIC_ST_CTRL_R = 0;                 // disable SysTick during setup
    NVIC_ST_RELOAD_R = 400000 - 1;      // reload value
    NVIC_ST_CURRENT_R = 0;              // any write to current clears it
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000; // priority 2 (What priority should be?)        
    NVIC_ST_CTRL_R = 0x07;              // enable SysTick with core clock and interrupts
}

/*
    \brief Collects an 12-bit ADC sample, converts it to 32-bit unsigned
    fixed-point distance (resolution 0.001 cm) and stores in mailbox.
    Calibrate the A and B constants to achieve maximum linearity.
    \param None
    \return None
    \note Handler executes every 25 ms.
*/
void SysTickHandler(void) {
    // Collect a 12-bit 0 to 4095 ADC sample
	// Calibrate to units 0.001 cm
    //Complete main function.
    adc = ADC0In();
    distance = A * adc/1024 + B;
    flag = 1;
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
    \param unit Unit name, like "Distance ~ " 
    \param str_distance String to be displayed on the screen
    \return None
*/
void OLEDprint(const char* unit, const char* str_distance) {
    SetCursor(0, 0);
    DrawStr(unit);
    DrawStr(str_distance);
}
