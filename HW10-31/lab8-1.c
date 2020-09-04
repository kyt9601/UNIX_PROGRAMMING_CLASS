#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

union semun
{
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int main(int argc, char *argv[])
{
	ushort buf[3] = {0};
	int i, id, pid, semid;
	union semun arg;
	struct sembuf p_buf;

	id = atoi(argv[1]);
	semid = (0333, 3, 0600 | IPC_CREAT | IPC_EXCL);

	if (semid == -1)
		semid = semget(0333, 3, 0600);
	else
	{
		arg.array = buf;
		semctl(semid, 0, SETALL, arg);
	}

	if (id > 1)
	{
		p_buf.sem_num = id - 2;
		p_buf.sem_op = -1;
		p_buf.sem_flg = 0;
		semop(semid, &p_buf, 1);
	}

	pid = getpid();
	for (i=0; i<5; i++)
	{
		printf("pid = %d\n", pid);
		sleep(1);
	}

	
        if (id < 4)
        {
                p_buf.sem_num = id - 1;
                p_buf.sem_op = 1;
                p_buf.sem_flg = 0;
                semop(semid, &p_buf, 1);
        }

	return 0;
}
