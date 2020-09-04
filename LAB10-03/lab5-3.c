#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <ftw.h>
#include <sys/stat.h>
#include <sys/types.h>

int list(const char *name, const struct stat *st, int type) {
/*        char mtime[100];

	if (type!=FTW_D && !(type==FTW_F && (S_IXUSR & st->st_mode || S_IXGRP & st->st_mode || S_IXOTH & st->st_mode)))
		return 0;

        strcpy(mtime, ctime(&st->st_mtime));
        mtime[strlen(mtime) - 1] = '\0';

        printf("%c %04o %d %d %d %ld %s %s\n", (S_ISREG(st->st_mode) != 0 ? 'F' : 'D'), st->st_mode&0777, st->st_nlink, st->st_uid,
                                st->st_gid, st->st_size, mtime, name);
*/

	if (type == FTW_D || type==FTW_DNR)
		printf("%s : %d\n", name, st->st_size);
	else if (type== FTW_F){
		if (S_IXUSR & st->st_mode ||
			S_IXGRP & st->st_mode ||
			S_IXOTH & st->st_mode)
			printf("%s : %d\n", name, st->st_size);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	ftw(".", list, 1);

	return 0;
}
