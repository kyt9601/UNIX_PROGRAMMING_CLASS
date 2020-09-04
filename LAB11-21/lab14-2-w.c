#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/mman.h>

struct databuf
{
        char d_read;
        int d_num;
};

int main()
{
	int i;
	int shmid;
	struct databuf *arr;
	
	shmid = semget((key_t)44009, sizeof(struct databuf) * 10, 0600 | IPC_CREAT | IPC_EXCL);
	arr = (struct databuf*)semat(shmid, NULL, 0);

	for (i=0; i<10; i++)
	{
		scanf("%d", &(arr[i].d_num));
		arr[i].d_read = 1;
	}

	return 0;
}
