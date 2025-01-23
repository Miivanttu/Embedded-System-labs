#include "game_logic.h"
#include "i2c_helper.h"
#include "ssd1306.h"
#include "/inc/tm4c123gh6pm.h"
#include "/utils/ustdlib.h"

#define GAME_SEED 222745 // You can enter your student code

#define DELAY 500

#define BUF_SIZE 4

#define PF0 0x01
#define PF3 0x08
#define PF4 0x10

char scoreStr[BUF_SIZE];

/*
    \brief Debounce switches with a short delay.
    \param None
    \return None
*/
void Debounce(void) {
    for (volatile uint32_t i = DELAY; i > 0; i--) {}
}

/*
    \brief Gives a random usigned number in the range.
    \param max Maximum number that can be returned (included)
    \param min Minimum number that can be returned (included)
    \return Random number in the range including both lower and upper limits
*/
uint8_t RandInRange(uint8_t max, uint8_t min) {
    int rand = urand(); 
    rand *= rand < 0 ? -1 : 1;
    rand = rand % (max + 1 - min) + min;
    return rand;
}


/*
    \brief SysTick interrupt handler that makes the buzzer pin PF3 output sound.
    \param None
    \return None
    \note To disable handler set REALOD to 0.
*/
void SysTickHandler(void) {
    //Complete this function
    GPIO_PORTF_DATA_R ^= 0x08;
}

void GameLoop(void) {
    //Complete this function
    uint8_t basketPosition = 3;
    uint32_t ballCount = 0;
    uint32_t firstBallSpeed = 0;
    uint32_t secondBallSpeed = 0;
    uint32_t firstCursorXPos = 0;
    uint32_t secondCursorXPos = 0;
    uint32_t firstCursorYPos = 0;
    uint32_t secondCursorYPos = 0;
    usrand(GAME_SEED); // Sets the random seed
    uint32_t score = 0;
    while(true) {
        DisplayClear();
        // ...
        drawFigures(basketPosition, firstCursorXPos, firstCursorYPos, secondCursorXPos, secondCursorYPos, score);
        NVIC_ST_RELOAD_R = 0;
        GPIO_PORTF_DATA_R &= ~0x08;
        if (GPIO_PORTF_DATA_R&0x10)
            basketPosition++;
        if (GPIO_PORTF_DATA_R&0x01)
            basketPosition--;
        if (basketPosition > 7)
            basketPosition = 7;
        Debounce();
        if(ballCount == 0)
        {
            firstCursorYPos = RandInRange(7,0);
            firstBallSpeed = RandInRange(7,5);
            do
            {
                secondCursorYPos = RandInRange(7,0);
            } while (firstCursorYPos == secondCursorYPos);
            secondBallSpeed = RandInRange(5,3);
            ballCount = 2;
        }
        else if(ballCount == 1)
        {
            if (!secondCursorXPos)
            {
                do
                {
                    secondCursorYPos = RandInRange(7,0);
                } while (firstCursorYPos == secondCursorYPos);
                secondBallSpeed = RandInRange(5,3);
            }
            else if (!firstCursorXPos)
            {
                do
                {
                    firstCursorYPos = RandInRange(7,0);
                } while (firstCursorYPos == secondCursorYPos);
                firstBallSpeed = RandInRange(7,5);
            }
            ballCount = 2;
        }
        if(firstCursorXPos >= 108)
            firstCursorXPos = 0;
        if(secondCursorXPos >= 108)
            secondCursorXPos = 0;
        if(firstCursorXPos >= 94 && firstCursorYPos == basketPosition)
        {
            score++;
            firstCursorXPos = 0 - firstBallSpeed;
            ballCount--;
            NVIC_ST_RELOAD_R = 18181 - 1;
        }
        if(secondCursorXPos >= 94 && secondCursorYPos == basketPosition)
        {
            score++;
            secondCursorXPos = 0 - secondBallSpeed;
            ballCount--;
            NVIC_ST_RELOAD_R = 18181 - 1;
        }
        DisplayUpdate();
        firstCursorXPos += firstBallSpeed;
        secondCursorXPos += secondBallSpeed;
    }
}

    //Functions below were added by me

/*
    \brief Converts score number to string.
    \param Score
    \return None
    \note None
*/
void ConvertDec(uint32_t num) {
    int i = 0;
    if (num <= 99)
    {
        while (num > 0) 
        {
            scoreStr[i++] = num % 10 + '0';
            num /= 10;
        }
        for (int j = 0, k = i - 1; j < k; j++, k--) 
        {
            char num = scoreStr[j];
            scoreStr[j] = scoreStr[k];
            scoreStr[k] = num;
        }
        scoreStr[i++] = '\0';
    }
    else if (num > 99)
    {
        scoreStr[i++] = '9';
        scoreStr[i++] = '9';
        scoreStr[i++] = '\0';
    }
}


/*
    \brief Draws balls, line, basket and score on the screen.
    \param ball coordinates, basket coordinates, score
    \return None
    \note None
*/
void drawFigures(uint8_t basketPosition, uint32_t firstCursorXPos, 
uint32_t firstCursorYPos, uint32_t secondCursorXPos, uint32_t secondCursorYPos, uint32_t score)
{
    DrawLine(112,112,0,63,true);  
    SetCursor(100,basketPosition);
    DrawChar(']');
    ConvertDec(score);
    SetCursor(115, 0);
    DrawStr(scoreStr);
    SetCursor(firstCursorXPos, firstCursorYPos);
    DrawChar('o');
    SetCursor(secondCursorXPos, secondCursorYPos);
    DrawChar('o');
}

