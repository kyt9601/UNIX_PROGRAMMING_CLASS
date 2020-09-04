#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	int num;

	if ( (num = access("data1", F_OK)) < 0)
		perror("not exsist...");
	
	printf("%d\n", access("data1", R_OK | W_OK));

	return 0;
}
