#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <unistd.h>

int main(void){
	int i, fd, data1[10], data2[10];

	for (i=0;i<10;i++){
		scanf("%d", &data1[i]);
	}

// 배열 data1[]에 저장 된 정수를 “data1" 파일에 저장한다.
	fd = open("data1", O_RDWR | O_CREAT, 0600);
	write(fd, data1, sizeof(int) * 10);

// “data1" 파일에 저장된 정수를 읽어 data2[] 배열에 저장한다.	
	lseek(fd, 0, SEEK_SET);
	read(fd, data2, sizeof(int) * 10);
	close(fd);

	for (i=0;i<10;i++){
		printf("%-5d", data1[i]);
	}
	printf("\n");

	for (i=0;i<10;i++){
		printf("%-5d", data2[i]);
	}

	printf("\n");
	return 0;
}
