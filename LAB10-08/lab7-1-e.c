#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *args[] = {"test2", "abc", "def", "ghi", (char*)0};

	execvp("test2", args);
	perror("not exsist ...");

	exit(0);
}
