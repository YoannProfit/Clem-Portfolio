#include "stdlib.h"

char* itoa(int val, char* str, int base)
{
    uint8 i = 0;

    while (val)
    {
        str[i++] = (val % base) + '0';
        val = val / base;
    }

	str[i] = '\0';
 
	return str;
}
	