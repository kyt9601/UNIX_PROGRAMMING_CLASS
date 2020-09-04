#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void cat_m(char **s)
{
	char *fname = s[1];
	int fds = open(fname, O_RDONLY);
	char buf[512];
	int n;

        do {
                n = read(fds, buf, sizeof(char) * 512);
		write(1, buf, n);
        } while (0 < n);

	puts("");
}

void cd_m(char **s)
{
	char *dname = s[1];
	chdir(dname);
}

void cp_m(char **s)
{
        char *fname1 = s[1], *fname2 = s[2];
        int fds1, fds2;
        char buf[512];
        int n;

        if ( (fds1 = open(fname1, O_RDONLY)) < 0 )
                return 0;

	fds2 = open(fname2, O_TRUNC | O_CREAT | O_WRONLY, 0644);

        do {
                n = read(fds1, buf, sizeof(char) * 512);
                write(fds2, buf, n);
        } while (0 < n);
}

void mkdir_m(char **s)
{
	char *dname = s[1];

	mkdir(dname, 0755);
}

void ls_m(char **s)
{
        struct dirent *d;
        DIR *dp;

        dp = opendir(".");
        while (d = readdir(dp))
                if (d->d_name[0]!='.')
                        printf("%s\n", d->d_name);
}

void vi_m(char **s)
{
	char *fname = s[1];
        int fds = open(fname, O_CREAT | O_RDWR | O_APPEND, 0644);
	char buf[512];
	int n;

	lseek(fds, 0, SEEK_SET);
        do {
                n = read(fds, buf, sizeof(char) * 512);
                write(1, buf, n);
        } while (0 < n);

	do {
		n = read(0, buf, 512);
		if (buf[0] == 'q' && buf[1] == 'u' && buf[2] == 'i' && buf[3] == 't' && buf[4] == '\n')
			break;
		write(fds, buf, n);
	} while (0 < n);
}

void exec_m(char **s)
{
	char *fname1 = s[0];
	char fname2[512] = {0};

	strcpy(fname2, &s[0][2]);
	s[0] = fname2;

	execv(fname1, s);
}

int main(void)
{
	char in[50], *res[20] = {0};
	char *inst[6] = {"cat", "cd", "cp", "mkdir", "ls", "vi"};
	void (*f[7])(char **) = {cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, exec_m};
	int i;
	pid_t pid;

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

		for (i=0; i<6; i++)
			if (!strcmp(res[0], inst[i]))
				break;

		if (i == 6)
		{
			pid = fork();
			if (pid == 0)
			{
				f[i](res);
				exit(0);
			}
			else
			{
				wait(0);
			}
		}
		else
		{
			f[i](res);
		}
	}
}
