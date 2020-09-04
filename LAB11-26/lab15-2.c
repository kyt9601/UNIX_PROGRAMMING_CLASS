#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd, i;
	struct flock lock;

	if ((fd = open("turn1", O_RDWR | O_CREAT | O_EXCL, 0600)) < 0 )
		fd = open("turn1", O_RDWR | O_CREAT, 0600);
	else
		fd = open("turn1", O_RDWR, 0600);

	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(int);

        lock.l_type = F_WRLCK;
        lock.l_start = 0;
        fcntl(fd, F_SETLKW, &lock);

	for (i=0; i<5; i++)
	{
		sleep(1);
		printf("%d\n", getpid());
	}

        lock.l_type = F_UNLCK;
        lock.l_start = -sizeof(int);
        fcntl(fd, F_SETLK, &lock);

	return 0;
}
