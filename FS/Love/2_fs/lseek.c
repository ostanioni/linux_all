#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{	// off_t lseek (int fd, off_t pos, int origin);
	// origin : SEEK_CUR, SEEK_END, SEEK_SET
	if (fd == -1)
	{
		perror("Error: ");
		fprintf(stderr,"syscall open() failed in file \"%s\" "
					"at line # %d \n", __FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}

}
