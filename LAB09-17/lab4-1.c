#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	struct stat buf;

	stat("p1.c", &buf);
	printf("%o %d %ld\n", buf.st_mode&0777, buf.st_nlink, buf.st_size);

	return 0;
}
