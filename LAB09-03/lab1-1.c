#include <stdio.h>

int main(int argc, char* argv[])
{
	int i, data[10];

	for (i=0; i<10; i++)
		scanf("%d", data + i);

	for (i=0; i<10; i++)
		data[i] = *(data+i) + 5;

	for (i=0; i<10; i++)
		printf("%-5d", data[i]);
	
	puts("");

	return 0;
}
