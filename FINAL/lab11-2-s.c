#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int i, n;
	int num, end, result;
	int fd[3][2];
	char *f[6] = {"f1", "f2", "f3", "f4", "f5", "f6"};
	fd_set set, master;

	for (i=0; i<6; i++)
		mkfifo(f[i], 0600);
	for (i=0; i<3; i++)
                fd[i][0] = open(f[i], O_RDONLY);
	for (i=0; i<3; i++)
		fd[i][1] = open(f[i+3], O_WRONLY);

	FD_ZERO(&master);
	for (i=0; i<3; i++)
		FD_SET(fd[i][0], &master);

	while (set = master, n = select(fd[2][0]+1, &set, NULL, NULL, NULL) > 0)
	{
		end = 0;
		for (i=0; i<3; i++)
			if (FD_ISSET(fd[i][0], &set))
				if (result = read(fd[i][0], &num, sizeof(int)) > 0)
				{
					printf("%d send %d, n=%d\n", i, num, n);
					num += 8;
					write(fd[i][1], &num, sizeof(int));
				}
				else if (result == 0)
				{
					end++;
				}
		if (end == 3)
			break;
	}

	return 0;
}
