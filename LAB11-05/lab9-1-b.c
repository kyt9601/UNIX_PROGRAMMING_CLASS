#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
	int i;
	int fd, *addr;

	fd = open("temp", O_RDWR | O_CREAT, 0600);
	addr = mmap(NULL, sizeof(int)*10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	sleep(5);
	for (i=0; i<5; i++)
		printf("%d\n", addr[i]);
        sleep(5);
        for (i=5; i<10; i++)
                printf("%d\n", addr[i]);

	munmap(addr, sizeof(int)*10);

	return 0;
}
