#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fds = open("test2", O_RDONLY);
	int data[10] = {0};

	read(fds, data, sizeof(int) * 5);

	for (int i=0; i<10; i++) {
		printf("%d\n", data[i]);
	}

	close(fds);

	return 0;
}
