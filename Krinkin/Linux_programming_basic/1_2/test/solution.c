#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

//argv = "libShared.so someSecretFunctionPrototype 8"
int (*someSecretFunctionPrototype)(int);

int main(int argc, char * argv[])
{
	void *hdl = dlopen(argv[1], RTLD_LAZY);
	someSecretFunctionPrototype = (int (*)(int))dlsym(hdl, argv[2]);
	
	int result = someSecretFunctionPrototype( atoi(argv[3]) );
	printf("\n%d\n", result);
	
	dlclose(hdl);
	return 0;
}
