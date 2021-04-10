#include <stdio.h>
#include <dlfcn.h>

void (*say_hello)(const char*);

int main()
{
	void *hdl = dlopen("./libHello.so", RTLD_LAZY);
	say_hello = (void (*)(const char*))dlsym(hdl, "say_hello");
	
	char name[20];
	
	printf("Enter your name: ");
	scanf("%19s", name);
	
	say_hello(name);
	
	dlclose(hdl);
	return 0;
}