#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd;

	fd = open("./data", O_APPEND);
	if (fd == -1)
	{
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}