#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *args[] = {"test1", "abc", "def", "ghi", (char*)0};

	execv("./test1", args);
	perror("not exsist ...");

	exit(0);
}
