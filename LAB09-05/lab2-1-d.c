#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fds = open("test1", O_WRONLY | O_APPEND);
	char ch = 'Z';

	for (int i=0; i<5; i++)
		write(fds, &ch, sizeof(char));

	close(fds);

	return 0;
}
