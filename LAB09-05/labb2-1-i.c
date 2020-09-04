#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void cacthalarm(int signo);

int main(int argc, char *argv[])
{
	int i;
	int num, sum = 0;
	static struct sigaction act;

	act.sa_handler = cacthalarm;
	sigaction(SIGALRM, &act, NULL);

	for (i=0; i<10; i++)
	{
		do {
			alarm(10);
		} while (scanf("%d", &num) < 0);
		alarm(0);
		sum += num;
	}

        printf("sum = %d\n", sum);

	return 0;
}

void cacthalarm(int signo)
{
	printf("input !!\n");
}
