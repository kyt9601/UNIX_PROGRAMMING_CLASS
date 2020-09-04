#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void do_child(int i, int p[][2]);

int main()
{
	int i, in, pid, p[3][2];

	for (i=0; i<3; i++)
		pipe(p[i]);

	for (i=0; i<3; i++)
		if (fork() == 0)
			do_child(i, p);

	for (i=0; i<3; i++)
		close(p[i][0]);

	for (i=0; i<12; i++)
	{
		scanf("%d", &in);
		write(p[i%3][1], &in, sizeof(int));
	}

	in = -1;
	for (i=0; i<3; i++)
		write(p[i][1], &in, sizeof(int));

	for (i=0; i<3; i++)
		wait(0);

	return 0;
}

void do_child(int i, int p[][2])
{
	int out;

	while (1)
	{
		read(p[i][0], &out, sizeof(int));
		if (out == -1)
			break;
		printf("child process %d received %d ...\n", getpid(), out);
	}

	exit(i);
}
