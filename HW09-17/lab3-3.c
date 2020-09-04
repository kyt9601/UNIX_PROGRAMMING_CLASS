#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
	int i, fd, data1[10], data2[10];

	fd = open("data3", O_CREAT | O_RDWR, 0600);

	for (i=0; i<10; i++)
		scanf("%d", &data1[i]);

	// data1 배열의 0-4번째 정수를 “data3" 파일의 0, 2, 4, 6, 8번째 위치에 저장
	for (i=0; i<=4; i++) {
		write(fd, data1+i, sizeof(int));
		lseek(fd, +1 * sizeof(int), SEEK_CUR);
	}

	// data1 배열의 5-9번째 정수를 “data3" 파일의 1, 3, 5, 7, 9번째 위치에 저장
//	lseek(fd, +1 * sizeof(int), SEEK_SET);
	lseek(fd, -9 * sizeof(int), SEEK_CUR);
        for (i=5; i<=9; i++) {
                write(fd, data1+i, sizeof(int));
                lseek(fd, +1 * sizeof(int), SEEK_CUR);
        }

	// "data3" 파일에 저장된 정수 10개를 읽어 data2 배열에 저장
/*	for (i=0; i<10; i++) {
		lseek(fd, +i * sizeof(int), SEEK_SET);
		read(fd, data2+i, sizeof(int));
	}
*/
	lseek(fd, 0, SEEK_SET);
	read(fd, data2, 10 * sizeof(int));

	for (i=0; i<10; i++)
		printf("%-5d", data1[i]);
	printf("\n");

	for (i=0; i<10; i++)
		printf("%-5d", data2[i]);
	printf("\n");

	return 0;
}
