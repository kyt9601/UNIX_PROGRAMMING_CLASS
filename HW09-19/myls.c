#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	int n;
	char fname[100], mtime[100];
        struct stat st;

//	scanf("%s", fname);
	n = read(0, fname, 100);
	fname[n-1] = '\0';

        if (stat(fname, &st) < 0) {
		perror("not exsist... ");
		return 0;
	}

	strcpy(mtime, ctime(&st.st_mtime));
	mtime[strlen(mtime) - 1] = '\0';

        printf("%04o %d %d %d %ld %s %s\n", st.st_mode&0777, st.st_nlink, st.st_uid,
				st.st_gid, st.st_size, mtime, fname);	

	return 0;
}
