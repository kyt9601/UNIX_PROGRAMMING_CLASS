#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	for (int i=1; i<=5; i++)
	{
		puts(argv[i]);
		sleep(1);
	}

	return 1;
}
