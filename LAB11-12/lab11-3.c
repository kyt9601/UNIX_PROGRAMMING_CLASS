#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	char buf, f[3][3] = {"f1", "f2", "f3"};
	int i, j, k, fd[2];

	for (i=0; i<3; i++)
		mkfifo(f[i], 0600);

	k = atoi(argv[1]);
	if (0 < k) fd[0] = open(f[k-1], O_RDONLY);
	if (k < 3) fd[1] = open(f[k], O_WRONLY);

	if (0 < k) read(fd[0], &buf, 1);

	j = getpid();
	for (i=0; i<5; i++)
	{
		printf("[%d] %ld\n", k, j);
		sleep(1);
	}

	if (k < 3) write(fd[1], "a", 1);

	return 0;
}
