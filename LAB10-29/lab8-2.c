#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

void catchusr(int signo)
{
        printf("......... cacth SIGUSR1 : %d\n", signo);
}

void do_child(int n, pid_t pids[])
{
	int i, pid;
	static struct sigaction act;

	// SIGUSR1 signal 처리가 가능 하도록 설정
	act.sa_handler = catchusr;
	sigaction(SIGUSR1, &act, NULL);

	printf("%d-th child is created...\n", n);

	if (n != 4)
		pause();

	// signal을 받으면, process id 세 번 출력
	pid = getpid();
	for (i=0; i<5; i++)
	{
		sleep(1);
		printf("child process pid = %d\n", pid);
	}
	
	if (n != 0)
		kill(pids[n-1], SIGUSR1);

	exit(0);
}

int main(void)
{
	int i, status;
	pid_t pid[5];

	for (i=0; i<5; i++)
	{
		pid[i] = fork();
		if (pid[i] == 0)
			do_child(i, pid);
	}

	for (i=0; i<5; i++)
		wait(&status);
//		k = wait(&status);
//		printf("child %d is terminated with exit %d\n", k, WEXITSTATUS(status));

	exit(0);
}
