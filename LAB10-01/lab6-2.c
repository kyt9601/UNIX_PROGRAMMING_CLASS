#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	for (int i=1; i<argc; i++)
		puts(argv[i]);

	return 0;
}
