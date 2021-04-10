#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

//argv = "libShared.so someSecretFunctionPrototype 8"
int (*someSecretFunctionPrototype)(int);
const char* concat(const char *s1, const char *s2);

int main(int argc, char * argv[])
{
	const char *path = concat("./", argv[1]);
	void *hdl = dlopen(path, RTLD_LAZY);
	someSecretFunctionPrototype = (int (*)(int))dlsym(hdl, argv[2]);
	
	int result = someSecretFunctionPrototype( atoi(argv[3]) );
	printf("\n%d\n", result);
	
	dlclose(hdl);
	return 0;
}

const char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}