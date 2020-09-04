#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	execlp("test2", "test2", "abc", "def", "ghi", (char*)0);
	perror("not exsist ...");

	exit(0);
}
