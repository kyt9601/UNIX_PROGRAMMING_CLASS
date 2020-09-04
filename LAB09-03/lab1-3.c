#include <stdio.h>

void add_five(int *pNum);

int main(int argc, char* argv[])
{
	int i, data[10];

	for (i=0; i<10; i++)
		scanf("%d", data + i);

	for (i=0; i<10; i++)
		add_five(data+i);

	for (i=0; i<10; i++)
		printf("%-5d", data[i]);
	
	puts("");

	return 0;
}

void add_five(int *pNum)
{
	*pNum = *pNum + 5;
}
