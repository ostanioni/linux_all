#include <stdio.h>
#include <unistd.h>

void main()
{
	long sz = sysconf(_SC_PAGESIZE);
	printf("page_size = %li \n", sz);
}
