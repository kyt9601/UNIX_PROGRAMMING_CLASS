#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fds = open("data", O_RDONLY);
	char ch[2] = {0};

	for (int i=0; i<10; i++)
	{
		read(fds, &ch, sizeof(char));
		puts(ch);
		sleep(1);
	}

	return 3;
}
