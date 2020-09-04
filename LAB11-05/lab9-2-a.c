#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
	int i, fd, len = 0;
	char *addr;

	fd = open("temp", O_RDWR | O_CREAT, 0600);
	addr = mmap(NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	ftruncate(fd, 512);
	for (i=0; i<3; i++)
	{
		len = len + read(0, addr+len, 512);
		if (512 < len)
			break;
	}

	munmap(addr, 512);

	return 0;
}
