#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	printf("pid=%ld, pgid=%ld, sid=%ld\n", getpid(), getpgid(0), getsid(getpid()));

	return 0;
}
