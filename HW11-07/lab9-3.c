#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

void do_child(int i, int *turn)
{
	int j, pid;

	if (i < 2)
		do {} while(i != *turn);

	pid = getpid();
	for (j=0; j<5; j++)
	{
		printf("child %d .... \n", pid);
		sleep(1);
	}

	if (0 < i) *turn = i - 1;

	exit(0);
}

int main()
{
        int fd, i, *addr;
	pid_t pid;

        fd = open("temp", O_RDWR | O_CREAT | O_TRUNC, 0600);
        addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        ftruncate(fd, sizeof(int));
	*addr = 2;

        for (i=0; i<3; i++)
		if (!fork())
			do_child(i, addr);

	for (i=0; i<3; i++)
		wait(0);

        munmap(addr, sizeof(int)*10);

        return 0;
}


