#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

void list(char *fname) {
        char mtime[100];
        struct stat st;

        if (stat(fname, &st) < 0) {
                perror("not exsist... ");
                return;
        }

        strcpy(mtime, ctime(&st.st_mtime));
        mtime[strlen(mtime) - 1] = '\0';

        printf("%c %04o %d %d %d %ld %s %s\n", (S_ISREG(st.st_mode) != 0 ? 'F' : 'D'), st.st_mode&0777, st.st_nlink, st.st_uid,
                                st.st_gid, st.st_size, mtime, fname);
}

int main(int argc, char *argv[])
{
	DIR *dp = opendir(".");
	struct dirent *d;

	while ( (d = readdir(dp)) != NULL )
	{
		list(d->d_name);
	}

	return 0;
}
