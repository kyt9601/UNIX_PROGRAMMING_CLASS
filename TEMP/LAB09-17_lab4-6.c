#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fds = open("test1", O_CREAT | O_WRONLY, 0600);
	char ch = 'X';

	for (int i=0; i<10; i++)
		write(fds, &ch, sizeof(char));

	close(fds);

	return 0;
}
