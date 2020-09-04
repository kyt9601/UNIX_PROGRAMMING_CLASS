#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void do_child(int id, int p[2][2]);

int main()
{
	int i, in, pid, p[2][2];

	for (i=0; i<2; i++)
		pipe(p[i]);

	for (i=0; i<3; i++)
		if (fork() == 0)
			do_child(i, p);

        for (i=0; i<2; i++)
	{
                close(p[i][0]);
		close(p[i][1]);
	}

        for (i=0; i<3; i++)
                wait(0);

	return 0;
}

void do_child(int id, int p[2][2])
{
	char buf = 'a';
	int i, pid = getpid();

	if (id < 2) read(p[id][0], &buf, 1);

	for (i=0; i<5; i++)
	{
		sleep(1);
		printf("%ld\n", pid);
	}

	if (0 < id) write(p[id-1][1], &buf, 1);

	exit(0);
}
