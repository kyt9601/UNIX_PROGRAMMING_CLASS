#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 512

int main(int argc, char *argv[])
{
	char fname1[100], fname2[100], buf[BUF_SIZE];
	int fds1, fds2, num;

	scanf("%s", fname1);
	scanf("%s", fname2);

	if ( (fds1 = open(fname1, O_RDONLY)) < 0 ) {
		perror("fds1 open error... ");
		return 0;
	}

	if ( (fds2 = open(fname2, O_EXCL | O_CREAT | O_WRONLY, 0644)) < 0 ) {
		int num;
		printf("file exsist... : ");
		scanf("%d", &num);
		if (num != 1) {
			return 0;
		} else {
			close(fds2);
			fds2 = open(fname2, O_TRUNC | O_CREAT | O_WRONLY, 0644);
		}
	}

	do {
		num = read(fds1, buf, sizeof(char) * BUF_SIZE);
		write(fds2, buf, num);
	} while (0 < num);

	return 0;
}
