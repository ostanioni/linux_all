#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	char ar[444096];
	char *buf = ar;
	size_t len = sizeof(ar);
	ssize_t ret;
	
	int fd;
	fd = open("./data", O_RDONLY);
	
	while (len != 0 && (ret = read (fd, buf, len)) != 0)
	{
		if (ret == -1) {
			if (errno == EINTR) continue;
			perror ("read");
			break;
		}
		
		len -= ret;
		buf += ret;
	}
	printf("%s \n", ar);
}

