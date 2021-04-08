#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// в таком варианте происходит утечка памяти
int main()
{
	char *name = (char*)malloc(strlen("Hello World!")+1);
	strcpy(name,"Hello World!");
	while(*name != 0) {
		printf("%c \n",*name);
		name++;
	}
	// потому, что в данном случае 
	// name указывает на последний символ строки "\0"
	free(name);
}
