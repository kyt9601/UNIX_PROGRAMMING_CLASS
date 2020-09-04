#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	char *fname = argv[1];
	char buf[3] = {0};
	int num_process = atoi(argv[2]);
	int fds = 0;

	fds = open(fname, O_RDONLY);

	for (int i=0; i<num_process; i++)
		if (!fork()) {
			read(fds, buf, 2);
			printf("%s, %ld\n", buf, getpid());
			exit(0);
		}

	return 0;
}
