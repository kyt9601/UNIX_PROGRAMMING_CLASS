#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(void)
{
	char fa[5] = "fifo", f[6][3] = {"f1", "f2", "f3", "f4", "f5", "f6"};
	int i, in, id, fda, fd[2];

	fda = open(fa, O_RDWR);
	read(fda, &id, sizeof(int));
	printf("id=%d\n", id+1);

	fd[1] = open(f[id], O_WRONLY);
	fd[0] = open(f[id+3], O_RDONLY);

	scanf("%d", &in);
	while (in != -1)
	{
		write(fd[1], &in, sizeof(int));
		read(fd[0], &in, sizeof(int));
		printf("%d\n", in);
		scanf("%d", &in);
	}

	write(fda, &id, sizeof(int));

	exit(0);
}
