#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// в таком варианте происходит утечка памяти
int main()
{
	char *name = (char*)malloc(strlen("Hello World!")+1);
	strcpy(name,"Hello World!");
	while(*name != '\0') {
		printf("%c",*name);
		name++;
	}
	// free(name); 
	// не сработает
	// потому, что в данном случае 
	// name указывает на последний символ строки "\0"
	printf("\n символ, на который указывает name = '\\%d'", *((int*)name) );
	// те на конец строки
}
// пример на С++ показывающий конец строки
/*
#include <iostream>
 
int main()
{
    char str[] = "Hello World!";
    std::cout << str << " has " << sizeof(str) << " characters.\n";
    for (int idx = 0; idx < sizeof(str); ++idx)
        std::cout << static_cast<int>(str[idx]) << " ";
 
    return 0;
}
// Output: 
// Hello, World! has 14 characters.                                                                                                                     
// 72 101 108 108 111 44 32 87 111 114 108 100 33 0   
// Нуль в конце является ASCII-кодом нуль-терминатора,
// который был добавлен в конец строки.
*/
