#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int num_process = atoi(argv[1]);

	for (int i=0; i<num_process; i++)
		if (!fork()) {
			printf("pid=%ld, pgid=%ld, sid=%ld\n", getpid(), getpgrp(), getsid(getpid()));
			exit(0);
		}

	return 0;
}
