#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int i, n;
	int num, result;
	int fd[3][2];
	char *f[6] = {"f1", "f2", "f3", "f4", "f5", "f6"};
	fd_set set, master;
	struct timeval timeout, origin;

	for (i=0; i<6; i++)
		mkfifo(f[i], 0600);
	for (i=0; i<3; i++)
		fd[i][0] = open(f[i], O_RDWR);
	for (i=0; i<3; i++)
		fd[i][1] = open(f[i+3], O_RDWR);

	FD_ZERO(&master);
	for (i=0; i<3; i++)
		FD_SET(fd[i][0], &master);
	origin.tv_sec = 60;
	origin.tv_usec = 0;

	while (set = master, timeout = origin, n = select(fd[2][0]+1, &set, NULL, NULL, &timeout) > 0)
	{
		for (i=0; i<3; i++)
			if (FD_ISSET(fd[i][0], &set))
				if (result = read(fd[i][0], &num, sizeof(int)) > 0)
				{
					printf("%d send %d, n=%d\n", i, num, n);
					num += 8;
					write(fd[i][1], &num, sizeof(int));
				}
	}

	return 0;
}
