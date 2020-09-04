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

int f_backup(const char *name, const struct stat *status, int type)
{
        int i, n;
        int fds1, fds2;
        int strlen_ftw_dirname = strlen(ftw_dirname);
        char buf[512];
        char new_name[512];
	char *args[4] = {"cp", NULL, NULL, NULL};

        if (type == FTW_F && strncmp(ftw_dirname, name+2, strlen(ftw_dirname)))
        {
		printf("backup %s ...\n", name);
		sleep(1);
                strcpy(new_name, ftw_dirname);
                strcat(new_name, "/");
                strcat(new_name, name+2);
                for (i=strlen_ftw_dirname+1; i<strlen(new_name); i++)
                        if (new_name[i] == '/')
                                new_name[i] = '_';

		args[1] = name;
		args[2] = new_name;
                cp_m(args);
        }

        return 0;
}

void backup_m(char **s)
{
        ftw_dirname = s[1];
        mkdir(ftw_dirname, 0755);

        ftw(".", f_backup, 1);
}

int main(void)
{
	char in[50], *res[20] = {0};
	char *inst[7] = {"cat", "cd", "cp", "mkdir", "ls", "vi", "backup"};
	void (*f[8])(char **) = {cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, backup_m, exec_m};
	int i;
	int status;
	pid_t pid_backup, pid_exec;

	while (1)
	{
		printf("> ");
		gets(in);

		i = 0;
		res[i] = strtok(in, " ");

		if (strcmp(res[0], "exit") == 0)
		{
			if (0 < waitpid(pid_backup, &status, WNOHANG))
				exit(0);
			else
				puts("backup in progress or waitpid error ...");
		}

		while (res[i])
		{
			i++;
			res[i] = strtok(NULL, " ");
		}

		for (i=0; i<7; i++)
			if (!strcmp(res[0], inst[i]))
				break;

		if (i == 6)
		{
                        pid_backup = fork();
                        if (pid_backup == 0)
                        {
                                f[i](res);
                                exit(6);
                        }

		}
                if (i == 7)
                {
                        pid_exec = fork();
                        if (pid_exec == 0)
                        {
                                f[i](res);
                                exit(7);
                        }
                        else
                        {
                                waitpid(pid_exec, &status, 0);
                        }
                }
		else
		{
			f[i](res);
		}
	}
}
