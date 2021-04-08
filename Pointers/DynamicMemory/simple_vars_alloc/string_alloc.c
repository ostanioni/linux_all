#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	char *name = (char*)malloc(strlen("Hello World!")+1);
	strcpy(name,"Hello World!");
	while(*name != 0) {
		printf("%c \n",*name);
		name++;
	}
	free(name);
}
