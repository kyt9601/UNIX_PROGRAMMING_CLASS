#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	int fd;
	char *addr, str[512];

        ushort buf[3] = {0};
        int i, id, pid, semid;
        union semun arg;
        struct sembuf p_buf;

	fd = open("data", O_RDWR);
	addr = mmap(NULL, 512, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        if (semid == -1)
                semid = semget(0333, 3, 0600);
        else
        {
                arg.array = buf;
                semctl(semid, 0, SETALL, arg);
        }


	for (i=0; i<3; i++)
	{
		scanf("%s", str);
		
	}

	munmap(addr, 512);

	return 0;
}
