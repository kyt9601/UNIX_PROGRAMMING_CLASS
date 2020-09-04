#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct q_entry
{
	long mtype;
	int num;
};

int main()
{
	int i, qid;
	key_t key = ftok("../temp.c", 4);
	struct q_entry msg;

	qid = msgget(key, 0600 | IPC_CREAT);
	for (i=0; i<15; i++)
	{
		msgrcv(qid, &msg, sizeof(int), -3, 0);
		msg.mtype += 3;
		msg.num += 8;
		msgsnd(qid, &msg, sizeof(int), 0);
	}

	return 0;
}
