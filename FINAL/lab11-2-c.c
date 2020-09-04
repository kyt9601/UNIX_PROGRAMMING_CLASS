#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// Usage : ./runfile <fifo1> <fifo2>
// Example : ./runfile t1in t1out

int main(int argc, char *argv[])
{
	int i;
	int num;
	int fd[2];
	char *f[2] = {argv[1], argv[2]};

	fd[0] = open(f[1], O_RDONLY);
	fd[1] = open(f[0], O_WRONLY);

	for (i=0; i<5; i++)
	{
		scanf("%d", &num);
		write(fd[1], &num, sizeof(int));
		read(fd[0], &num, sizeof(int));
		printf("%d from server\n", num);
	}

	return 0;
}
