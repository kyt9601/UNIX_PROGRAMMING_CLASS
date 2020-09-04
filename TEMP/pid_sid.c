#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	for (int i=0; i<5; i++)
	{
		printf("pid=%d, gid=%d, sid=%d\n", getpid(), getpgid(0), getsid(getpid()));
		sleep(1);
	}

	return 2;
}
