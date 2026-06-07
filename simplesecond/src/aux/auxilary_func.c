#include "aux.h"
#include <stdio.h>

unsigned long hash_djb2(const char *str)
{
	unsigned long hash = 5381;
	int c;
	while((c = *str++))
		hash = ((hash << 5) + hash)	+ c;
	return hash;
}

int ConvertToUInt(char *number){
	if(number == NULL) return -1;

	int res = 0;
	int i = 0;
	while(number[i] != '\0'){
		if(number[i] < '0' || number[i] > '9')
			return -1;
		res = res * 10 + (number[i] - '0');
		i++;
	}

	return res;
}



