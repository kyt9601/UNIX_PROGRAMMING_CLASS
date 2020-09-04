#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct q_entry
{
	long type;
	int data;
};

int main(int argc, char *argv[])
{
	int qid, i, id;
	pid_t pid;
	key_t key;
	struct q_entry msg;

	id = atoi(argv[1]);
	pid = getpid();
	key = ftok("../temp.c", 8);
	qid = msgget(key, 0600 | IPC_CREAT);

	if (1 < id)
		msgrcv(qid, &msg, sizeof(int), id, 0);

	for (i=0; i<5; i++)
	{
		sleep(1);
		printf("%d. pid %d\n", id, pid);
	}

	if (id < 4)
	{
		msg.type = id + 1;
		msg.data = id;
		msgsnd(qid, &msg, sizeof(int), 0);
	}

	return 0;
}
