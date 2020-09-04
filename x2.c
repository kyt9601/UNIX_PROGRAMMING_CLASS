#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

char *ftw_dirname;
pid_t pid_exec;
int in_backup;

void catchsigint(int signo)
{
	printf("execution takes too long\n");
	kill(pid_exec, SIGTERM);
}

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
                return;

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
        int i;
        int strlen_ftw_dirname = strlen(ftw_dirname);
        char new_name[512];
	char *args[4] = {"cp", NULL, NULL, NULL};

        if (type == FTW_F && strncmp(ftw_dirname, name+2, strlen(ftw_dirname)))
        {sleep(1);
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
        ftw_dirname = "TEMP";
        mkdir(ftw_dirname, 0755);

	puts("// backup started //");
        ftw(".", f_backup, 1);
	puts("// backup finished //");
}

void sigaction_backup(int signo)
{
	in_backup = 1;
	backup_m(NULL);
	in_backup = 0;
	alarm(5);
}

void sigaction_backup_term(int signo)
{
	while (in_backup == 1);
	exit(signo);
}

int main(void)
{
	char in[50], *res[20] = {0};
	char *inst[7] = {"cat", "cd", "cp", "mkdir", "ls", "vi", "backup"};
	void (*f[8])(char **) = {cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, backup_m, exec_m};
	int i;
	int status;
	pid_t pid_backup = -99;
	pid_exec = -99;
	in_backup = 0;

	while (1)
	{
		printf("> ");
		gets(in);

		i = 0;
		res[i] = strtok(in, " ");

		if (strcmp(res[0], "exit") == 0)
		{
			if (in_backup)
			{
				kill(pid_backup, SIGTERM);
				waitpid(pid_backup, &status, 0);
				puts("backup finished. exit.");
				exit(0);
			}
			else
				exit(0);
		}

		while (res[i])
		{
			i++;
			res[i] = strtok(NULL, " ");
		}

		for (i=0; i<7; i++)
			if (!strcmp(res[0], inst[i]))
				break;

		if (i == 6 && in_backup == 0)
		{
                pid_backup = fork();
                if (pid_backup == 0)
                {
						struct sigaction actback;
						actback.sa_handler = sigaction_backup;
						sigaction(SIGALRM, &actback, NULL);

						struct sigaction actbackterm;
						actbackterm.sa_handler = sigaction_backup_term;
						sigaction(SIGTERM, &actbackterm, NULL);

						sigaction_backup(SIGALRM);

                        while (1);
	  			}
				else
				{
						in_backup = 1;
				}
		}
        else if (i != 1)
        {
                pid_exec = fork();
                if (pid_exec == 0)
                {
						signal(SIGINT, SIG_IGN);

                       f[i](res);
                       exit(i);
                }
                else
                {
						struct sigaction act;
						act.sa_handler = catchsigint;
						sigaction(SIGINT, &act, NULL);
                       waitpid(pid_exec, &status, 0);
					   signal(SIGINT, SIG_DFL);
                }
        }
		else
		{
			f[i](res);
		}
	}
}
