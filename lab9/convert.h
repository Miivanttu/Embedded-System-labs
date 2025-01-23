#ifndef CONVERT_H
#define CONVERT_H

#include <stdint.h>

/*
    \brief Converts the decimal number to a formatted string.
    4       to "   4 ", 
    31      to "  31 ",
    102     to " 102 ",
    2210    to "2210 ",
    10000   to "**** ", 
    >9999   to "**** ".
    \param num Decimal numerical value to be converted to a string
    \param str Converted string is manipulated and saved directly into this variable
    \return None
    \note Remember to set the last character as '\0'!
*/
void ConvertDec(uint32_t num, char* str);

/*
    \brief Converts the decimal number to a formatted distance string.
    4       to "0.004 cm", 
    31      to "0.031 cm",
    102     to "0.102 cm",
    2210    to "2.210 cm",
    10000   to "*.*** cm",
    >9999   to "*.*** cm".
    \param num Decimal numerical value to be converted to a string
    \param str Converted string is manipulated and saved directly into this variable
    \return None
    \note Remember to set the last character as '\0'!
*/
void ConvertDistance(uint32_t num, char* str);

#endif /* CONVERT_H */