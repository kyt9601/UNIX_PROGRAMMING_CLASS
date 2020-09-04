#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void)
{
	char f[2][3] = {"f1", "f2"};
	int i, in, fd[2];

	fd[0] = open(f[0], O_RDONLY);
	fd[1] = open(f[1], O_WRONLY);

	while (1)
	{
                read(fd[0], &in, sizeof(int));
		printf("%d from client ...\n", in);
		if (in == -1) exit(0);
		in += 8;
		write(fd[1], &in, sizeof(int));
	}
}
