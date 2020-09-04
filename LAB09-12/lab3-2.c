#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	int fds = open("data2", O_EXCL | O_CREAT | O_WRONLY, 0644);

	for (int i=0; i<10; i++)
		write(fds, "X", sizeof(char));

	lseek(fds, 0, SEEK_SET);
	write(fds, "Y", sizeof(char));

	lseek(fds, 0, SEEK_END);
	write(fds, "Y", sizeof(char));

	lseek(fds, +1, SEEK_SET);
	for (int i=0; i<4; i++) {
		write(fds, "Z", sizeof(char));
		lseek(fds, +1, SEEK_CUR);
	}

	lseek(fds, +14, SEEK_SET);
	write(fds, "T", sizeof(char));

	lseek(fds, -2, SEEK_END);
	write(fds, "S", sizeof(char));

	lseek(fds, -2, SEEK_CUR);
	write(fds, "W", sizeof(char));

	close(fds);

	return 0;
}
