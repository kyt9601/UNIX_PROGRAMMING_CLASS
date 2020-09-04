#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void change_directory(char *name) {
	// cd 명령 수행
	chdir(name);
}
void list() {
	// ls 명령 수행
	struct dirent *d;
	DIR *dp;

	dp = opendir(".");
	while (d = readdir(dp))
	{
		if (d->d_name[0]!='.')
		{
			printf("%s\n", d->d_name);
		}
	}
}

int main(int argc, char *argv[])
{
	char name[50];

	while (1)
	{
		// 현재 working directory 이름 표시
		printf("[%s] > ", getcwd(name, sizeof(name)));

		scanf("%s", name);

		if (strcmp(name, "cd")==0) {
			scanf("%s", name);
			change_directory(name);
		}
		else if (strcmp(name, "ls")==0)
			list();
		else
			break;
	}

	return 0;
}
