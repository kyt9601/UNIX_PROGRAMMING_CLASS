#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

void catchusr(int signo)
{
        printf(".........cacth SIGUSR1 : %d\n", signo);
}

void do_child(int n)
{
	int i, pid;
	static struct sigaction act;

	// SIGUSR1 signal 처리가 가능 하도록 설정
	act.sa_handler = catchusr;
	sigaction(SIGUSR1, &act, NULL);

	printf("%d-th child is created...\n", n);
	pause();

	// signal을 받으면, process id 세 번 출력
	pid = getpid();
	for (i=0; i<3; i++)
		printf("pid = %d\n", pid);		

	exit(0);
}

int main(void)
{
	int i, k, status;
	pid_t pid[3];

	for (i=0; i<3; i++)
	{
		pid[i] = fork();
		if (pid[i] == 0)
			do_child(i);
	}

	// 1초씩 sleep()하면서, child들에게 SIGUSR1 signal 보내기
	for (i=0; i<3; i++)
	{
		sleep(1);
		kill(pid[i], SIGUSR1);
	}

	for (i=0; i<3; i++)
	{
		k = wait(&status);
		printf("child #%d is terminated...\n", k);
	}

	exit(0);
}
