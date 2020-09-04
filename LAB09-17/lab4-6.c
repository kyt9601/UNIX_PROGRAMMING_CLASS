#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	char str[100], str2[100];
	struct stat st1, st2;

	scanf("%s", str);
	lstat(str, &st1);
	stat(str, &st2);
	readlink(str, str2, 100);

	printf("%o %ld %s\n", st1.st_mode&0777, st1.st_size, str);
	printf("%o %ld %s\n", st2.st_mode&0777, st2.st_size, str2);

	return 0;
}
