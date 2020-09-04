#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fds = open("test3", O_EXCL | O_CREAT | O_WRONLY, 0644);
	char ch = 'K';

	if (fds < 0) puts("can't open file");

	for (int i=0; i<10; i++)
		write(fds, &ch, sizeof(char));

	close(fds);

	return 0;
}
