#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd;

	fd = open("./data", O_APPEND); // O_APPEND in <fcntl.h>
	
	if (fd == -1)
	{
		perror("Error: ");
		fprintf(stderr,"syscall open() failed in file \"%s\" "
					"at line # %d \n", __FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}

	/***************************************
	 * close() returns zero on success.    *
	 * On error, -1 is returned, and       *
     * errno is set to indicate the error. *
	 ***************************************/

	if ( close(fd) != 0 ) // close() in <unistd.h>
	{
		perror("Error: ");
		fprintf(stderr,"syscall open() failed in file \"%s\" "
				"at line # %d \n", __FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}

    printf("Normal Return\n");	
	
	return EXIT_SUCCESS; // EXIT_FAILURE, EXIT_SUCCESS in <stdlib.h>
}