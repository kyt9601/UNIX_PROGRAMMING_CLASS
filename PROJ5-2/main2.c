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
#define SHMKEY_MESSAGE 849305
#define SHMKEY_MANAGE 790394

struct message_buffer
{
	int read_count;
    int sender_id;
    char mtext[BUFSIZE];
};

struct manage_buffer
{
    int total_user;
    int id_buffer[4];
	int counter;
};

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

void receiver(int id, int semid, struct message_buffer message[], struct manage_buffer *manage)
{
	int counter = 0;
	struct sembuf wait = {id+2, -1, 0};
	struct sembuf signal = {id+2, 1, 0};
//	srand(time(NULL));
    while (1)
	{
		wait.sem_num = id+2;
        semop(semid, &wait, 1);
//		if (rand() % 2) sleep(5);
		if (message[counter].sender_id != id)
	        printf("%d : %s\n", message[counter].sender_id, message[counter].mtext);

		wait.sem_num = 2;
		semop(semid, &wait, 1);
        message[counter].read_count -= 1;
		if (message[counter].read_count == 0)
		{
			signal.sem_num = 1;
			semop(semid, &signal, 1);
		}
        signal.sem_num = 2;
        semop(semid, &signal, 1);

		if (message[counter].sender_id == id &&
				!strcmp(message[counter].mtext, "talk_quit"))
			break;

		counter = (counter + 1) % 4;
	}

	exit(0);
}

void sender(int id, int semid, struct message_buffer message[], struct manage_buffer *manage)
{
	int i;
    char buffer[BUFSIZE];
    struct sembuf wait = {1, -1, 0};
    struct sembuf signal = {1, 1, 0};

    while (gets(buffer))
	{
	    semop(semid, &wait, 1);

        message[manage->counter].sender_id = id;
        strcpy(message[manage->counter].mtext, buffer);
        message[manage->counter].read_count = manage->total_user;
		for (i=3; i<7; i++)
		{
			if (manage->id_buffer[i-3])
			{
				signal.sem_num = i;
				semop(semid, &signal, 1);
			}
		}
		manage->counter = (manage->counter + 1) % 4;

		if (!strcmp(buffer, "talk_quit"))
			break;
	}

    return;
}

int main(int argc, char *argv[])
{
    int i, id, semid, shmid_message, shmid_manage;
    struct message_buffer *message;
	struct manage_buffer *manage;
    union semun arg;
    ushort init_value[7] = {1, 4, 1, 0, 0, 0, 0};
	struct sembuf wait = {0, -1, 0};
	struct sembuf signal = {0, 1, 0};

    shmid_message = shmget(SHMKEY_MESSAGE, sizeof(struct message_buffer) * 4, 0600 | IPC_CREAT);
    message = (struct message_buffer*)shmat(shmid_message, 0, 0);
    shmid_manage = shmget(SHMKEY_MANAGE, sizeof(struct manage_buffer), 0600 | IPC_CREAT);
    manage = (struct manage_buffer*)shmat(shmid_manage, 0, 0);

    semid = semget(SEMKEY, 7, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid == -1)
	{
        semid = semget(SEMKEY, 7, 0600);
    }
    else
	{
        arg.array = init_value;
        semctl(semid, 0, SETALL, arg);

	    manage->total_user = 0;
	    for (i=0; i<4; i++)
	        manage->id_buffer[i] = 0;
		manage->counter = 0;
    }

   //***login
	// sem
	semop(semid, &wait, 1);
    for (i=0; i<4; i++)
	{
		if (manage->id_buffer[i] == 0)
		{
			id = i + 1;
		    printf("id : %d\n", id);
			manage->id_buffer[i] = 1;
			break;
		}
	}
    manage->total_user += 1;
	semop(semid, &signal, 1);

    if (!fork())
        receiver(id, semid, message, manage);
    else
        sender(id, semid, message, manage);

    waitpid(-1, NULL, 0);

    //***logout
	semop(semid, &wait, 1);
    manage->id_buffer[id-1] = 0;
    manage->total_user -= 1;
	semop(semid, &signal, 1);

    if (manage->total_user == 0)
	{
        semctl(semid, IPC_RMID, 0);
        shmctl(shmid_message, IPC_RMID, 0);
		shmctl(shmid_manage, IPC_RMID, 0);
    }

    exit(0);
}
