#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void A(char *argv[])
{
}

void B(char *argv[])
{
}

void C(char *argv[])
{
	int i=0;
	for (i=0; i<20; i++)
		printf("%sz\n", argv[i]);
}

void D(char *argv[])
{
}

void E(char *argv[])
{
}

void F(char *argv[])
{
}

void G(char *argv[])
{
}

int main(void)
{
	char in[50], *res[20] = {0};
	char *inst[6] = {"A", "B", "C", "D", "E", "F"};
	void (*f[7])(char **) = {A, B, C, D, E, F, G};
	int i;

	while (1)
	{
		printf("> ");
		gets(in);

		i = 0;
		res[i] = strtok(in, " ");

		if (strcmp(res[0], "exit") == 0)
			exit(0);

		while (res[i])
		{
			i++;
			res[i] = strtok(NULL, " ");
		}

        for (i=0; i<20; i++)
                printf("%sz\n", res[i]);

		for (i=0; i<6; i++)
			if (!strcmp(res[0], inst[i]))
				break;

		f[i](res);
	}
}
