#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

char *ftw_dirname;

void cp_m(char *src, char *dest)
{
        char *fname1 = src, *fname2 = dest;
        int fds1, fds2;
        char buf[512];
        int n;

        if ( (fds1 = open(fname1, O_RDONLY)) < 0 )
                return 0;

	fds2 = open(fname2, O_TRUNC | O_CREAT | O_WRONLY, 0755);

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

int f_backup(const char *name, const struct stat *status, int type)
{
	int i, n;
	int fds1, fds2;
	int strlen_ftw_dirname = strlen(ftw_dirname);
	char buf[512];
	char new_name[512];
	
	if (type == FTW_F && strncmp(ftw_dirname, name+2, strlen(ftw_dirname)))
	{
		strcpy(new_name, ftw_dirname);
		strcat(new_name, "/");
		strcat(new_name, name+2);
		for (i=strlen_ftw_dirname+1; i<strlen(new_name); i++)
			if (new_name[i] == '/')
				new_name[i] = '_';

		cp_m(name, new_name);
	}
	
	return 0;
}

void backup_m(char **s)
{
	ftw_dirname = s[1];
	mkdir(ftw_dirname, 0755);

	ftw(".", f_backup, 1);
}

int f_rmall(const char *name, const struct stat *status, int type)
{
        if (type == FTW_F)
		remove(name);

        return 0;
}

void rmall_m(char **s)
{
	ftw(s[1], f_rmall, 1);
	remove(s[1]);
}

void vi_m(char **s)
{
	char *fname = s[1];
        int fds = open(fname, O_CREAT | O_RDWR | O_APPEND, 0644);
	char buf[512];
	int n;

	do {
		n = read(0, buf, 512);
		if (buf[0] == 'q' && buf[1] == 'u' && buf[2] == 'i' && buf[3] == 't' && buf[4] == '\n')
			break;
		write(fds, buf, n);
	} while (0 < n);
}

int main(void)
{
	char in[50], *res[20] = {0};
	char *inst[3] = {"mkdir", "vi", "backup"};
	void (*f[4])(char **) = {mkdir_m, vi_m, backup_m, rmall_m};
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

		for (i=0; i<3; i++)
			if (!strcmp(res[0], inst[i]))
				break;

		f[i](res);
	}
}
