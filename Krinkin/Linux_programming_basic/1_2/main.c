#include "say_hello.h"
#include <stdio.h>

int main()
{
	char name[20];
	
	printf("Enter your name: ");
	scanf("%20s", name);
	say_hello(name);
	
	return 0;
}