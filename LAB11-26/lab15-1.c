#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd, i, num;
	struct flock lock;

	fd=open("data", O_RDWR | O_CREAT, 0600);
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(int);

	scanf("%d", &num);
	write(fd, &num, sizeof(int));

	for (i=0; i<3; i++)
	{
		if (fork() == 0)
		{
	                lock.l_type = F_RDLCK;
	                lock.l_start = 0;
	                fcntl(fd, F_SETLKW, &lock);

			read(fd, &num, sizeof(int));
			sleep(5);
			num += 10;
			write(fd, &num, sizeof(int));

			exit(getpid());
		}

		lock.l_type = F_WRLCK;
		lock.l_start = 0;
		fcntl(fd, F_SETLKW, &lock);

		read(fd, &num, sizeof(int));
		num += 10;

		sleep(1);
		lseek(fd, -sizeof(int), SEEK_CUR);
		write(fd, &num, sizeof(int));

		lock.l_type = F_UNLCK;
		lock.l_start = -sizeof(int);
		fcntl(fd, F_SETLK, &lock);
	}

	lseek(fd, 0, SEEK_SET);
	for (i=0; i<10; i++)
	{
		read(fd, &num, sizeof(int));
		printf("%d\n", num);
	}

	return 0;
}
