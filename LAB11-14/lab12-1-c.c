#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct q_entry
{
	long mtype;
	int num;
};

int main(int argc, char *argv[])
{
	int qid, i, id;
	key_t key = ftok("../temp.c", 4);
	struct q_entry msg;

	id = atoi(argv[1]);
	qid = msgget(key, 0600 | IPC_CREAT);
	for (i=0; i<5; i++)
	{
		scanf("%d", &msg.num);
		msg.mtype = id;
		msgsnd(qid, &msg, sizeof(int), 0);
		msgrcv(qid, &msg, sizeof(int), id + 3, 0);
		printf("%d\n", msg.num);
	}

	return 0;
}
