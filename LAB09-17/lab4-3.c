#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	char str1[100], str2[100];

	scanf("%s%s", str1, str2);

	link(str1, str2);

	return 0;
}
