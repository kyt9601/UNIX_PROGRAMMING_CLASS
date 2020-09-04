#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int status;
	pid_t pid[3];
	char *func[3] = {"./lab7-2-a", "./lab7-2-b", "./lab7-2-c"};
	char *fname[3] = {"lab7-2-a", "lab7-2-b", "lab7-2-c"};

	for (int i=0; i<3; i++)
	{
		if ((pid[i]=fork()) == 0)
		{
			execl(func[i], fname[i], "3", "4", "5", "6", "7", (char*)0);
		}
	}

	waitpid(pid[2], &status, 0);
	printf("exit : %d\n", WEXITSTATUS(status));

        waitpid(pid[1], &status, 0);
        printf("exit : %d\n", WEXITSTATUS(status));

        waitpid(pid[0], &status, 0);
        printf("exit : %d\n", WEXITSTATUS(status));

	return 0;
}
