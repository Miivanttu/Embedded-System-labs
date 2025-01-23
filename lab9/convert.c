#include "convert.h"
#include <string.h>

void ConvertDec(uint32_t num, char* str) {
	// Complete this function!
	int i = 0;
	int c = 0;
	if(num > 9999)
	{
		while (c != 4) 
		{
			str[i++] = '*';
			c++;
		}
	}
	else
	{
		while (num > 0) 
		{
			str[i++] = num % 10 + '0';
			num /= 10;
		}
		if (strlen(str) < 5)
		{
			int len = 4 - strlen(str);
			for (len; len > 0; len--)
			{
				str[i++] = ' ';
			}
		}
		for (int j = 0, k = i - 1; j < k; j++, k--) 
		{
			char num = str[j];
			str[j] = str[k];
			str[k] = num;
		}
	}
	str[i++] = ' ';
	str[i++] = '\0';
}

void ConvertDistance(uint32_t num, char* str) {
	// Complete this function!
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
