#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE * fp; // If successful, returns a pointer to the new file stream.
	// On error, returns a non-zero error 
	// code and writes the null pointer to *fp

	fp = fopen("./data", "r");
	/**************************************************************
	* "r"- read (read from start, error if not exist)             *
	* "w"- write (destroy contents, create new if not exist)      *
	* "a"- append (write to end, create new if not exist)         *     
	* "r+"- read from start extended(open a file for read/write)  *
	* "w+"- write  extended (create  a file for  read/write)      *
	* "a+"- append extended(Open a file for read/write to end)    *
	***************************************************************/
	
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
