#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include<sys/types.h>
#include<sys/ipc.h>

union semun
{
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int main(int argc, char *argv[])
{
	int i, semid, pr;
	union semun arg;
	ushort buf[3];
	struct sembuf p_buf;

	pr = atoi(argv[1]);
	semid = semget((key_t)(3303 + pr), 1, 0600 | IPC_CREAT | IPC_EXCL);
	arg.array = pr;
	semctl(semid, 0, SETVAL, arg);

	p_buf.sem_num=0;
	p_buf.sem_op=-1;
	semop(semid, &p_buf, 1);
	for (i=0; i<5; i++)
		printf("process %d in critical section\n", getpid());
	p_buf.sem_num=0;
	p_buf.sem_op=1;
	semop(semid, &p_buf, 1);

	return 0;
}
