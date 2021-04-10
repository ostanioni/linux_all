#include <stdio.h>
#include <dlfcn.h>

int (*someSecretFunctionPrototype)(int);

int main(int argc, char * argv[])
{
	void *hdl = dlopen(argv[1], RTLD_LAZY);
	someSecretFunctionPrototype = (void (*)(const char*))dlsym(hdl, "someSecretFunctionPrototype");
	
	int result = someSecretFunctionPrototype(argv[2]);
	printf("\n%d", result);
	
	dlclose(hdl);	
	return 0;
}