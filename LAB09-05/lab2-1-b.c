#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fds = open("test2", O_CREAT | O_WRONLY, 0640);
	int data = 35;

	for (int i=0; i<10; i++)
		write(fds, &data, sizeof(int));

	close(fds);

	return 0;
}
