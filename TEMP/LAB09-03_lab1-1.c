#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <ftw.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/errno.h>

#define BUFSIZE 512
#define SEMKEY 328949
#define SHMKEY 849305

struct message_buffer
{
	int total_user;
	int id_buffer[4];
	int read_count;

    int sender_id;
    char mtext[BUFSIZE];
};

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

void receiver(int id, int semid, struct message_buffer *message)
{
	struct sembuf wait = {id+2, -1, 0};
	struct sembuf signal = {id+2, 1, 0};

    while (1)
	{
		wait.sem_num = id+2;
        semop(semid, &wait, 1);
		if (message->sender_id != id)
	        printf("%d : %s\n", message->sender_id, message->mtext);

		wait.sem_num = 2;
		semop(semid, &wait, 1);
        message->read_count -= 1;
        signal.sem_num = 2;
		semop(semid, &signal, 1);
//		printf("[%d] read_count : %d\n", id, message->read_count);
		if (message->read_count == 0)
		{
			signal.sem_num = 1;
			semop(semid, &signal, 1);
			union semun arg;
//			printf("getval : %d\n", semctl(semid, 1, GETVAL, arg));
		}

		if (message->sender_id == id &&
				!strcmp(message->mtext, "talk_quit"))
			break;
	}

	exit(0);
}

void sender(int id, int semid, struct message_buffer *message)
{
	int i;
    char buffer[BUFSIZE];
    struct sembuf wait = {1, -1, 0};
    struct sembuf signal = {1, 1, 0};

    while (1)
	{
		gets(buffer);
        semop(semid, &wait, 1);

        message->sender_id = id;
        strcpy(message->mtext, buffer);
        message->read_count = message->total_user;
		for (i=3; i<7; i++)
		{
			if (message->id_buffer[i-3])
			{
				signal.sem_num = i;
				semop(semid, &signal, 1);
			}
		}

		if (!strcmp(buffer, "talk_quit"))
			break;
	}

    return;
}

int main(int argc, char *argv[])
{
    int i, id, semid, shmid;
    struct message_buffer *message;
    union semun arg;
    ushort init_value[7] = {1, 1, 1, 0, 0, 0, 0};
	struct sembuf wait = {0, -1, 0};
	struct sembuf signal = {0, 1, 0};

    shmid = shmget(SHMKEY, sizeof(struct message_buffer), 0600 | IPC_CREAT);
    message = (struct message_buffer*)shmat(shmid, 0, 0);

    semid = semget(SEMKEY, 7, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid == -1)
	{
        semid = semget(SEMKEY, 7, 0600);
    }
    else
	{
        arg.array = init_value;
        semctl(semid, 0, SETALL, arg);

	    message->total_user = 0;
	    for (i=0; i<4; i++)
	        message->id_buffer[i] = 0;
    }

   //***login
	// sem
	semop(semid, &wait, 1);
    for (i=0; i<4; i++)
	{
		if (message->id_buffer[i] == 0)
		{
			id = i + 1;
		    printf("id : %d\n", id);
			message->id_buffer[i] = 1;
			break;
		}
	}
    message->total_user += 1;
	semop(semid, &signal, 1);

    if (!fork())
        receiver(id, semid, message);
    else
        sender(id, semid, message);

    waitpid(-1, NULL, 0);

    //***logout
	semop(semid, &wait, 1);
    message->id_buffer[id-1] = 0;
    message->total_user -= 1;
	semop(semid, &signal, 1);

    if (message->total_user == 0)
	{
        semctl(semid, IPC_RMID, 0);
        shmctl(shmid, IPC_RMID, 0);
    }

    exit(0);
}
