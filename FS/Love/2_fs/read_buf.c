#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

int main()
{
	char arr[1024];
	

	FILE * fp; 

	fp = fopen("./data", "r");
		
	if (fp == NULL)
    {
        fprintf(stderr," Error: fopen() failed in "
	   		"file \"%s\" at line # %d \n", __FILE__,__LINE__);
        exit(EXIT_FAILURE);
    }



	if (fclose(fp) != 0) // ​0​ on success, EOF otherwise
	{
		fprintf(stderr," Error: fclose() failed in "
	   		"file \"%s\" at line # %d \n", __FILE__,__LINE__);
        exit(EXIT_FAILURE);
	}

    printf("Normal Return\n");
	return EXIT_SUCCESS;
}

long long get_file_size(FILE * fp)
{
	fseek(fp, 0L, SEEK_END);
	sz = ftell(fp);

	fseek(fp, 0L, SEEK_SET);

	rewind(fp);

}

