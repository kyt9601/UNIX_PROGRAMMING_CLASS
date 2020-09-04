#include <stdio.h>

void add_five(int *pArr);

int main(int argc, char* argv[])
{
	int i, data[10];

	for (i=0; i<10; i++)
		scanf("%d", data + i);

	add_five(data);

	for (i=0; i<10; i++)
		printf("%-5d", data[i]);
	
	puts("");

	return 0;
}

void add_five(int *pArr)
{
	for (int i=0; i<10; i++)
		pArr[i] = pArr[i] + 5;
}
