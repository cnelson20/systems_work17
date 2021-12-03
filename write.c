#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

#define SEMKEY 5423508
#define MEMKEY 4580924

int main() {
	int semd = semget(SEMKEY,0,0);
	int memd = shmget(MEMKEY,0,0);
	char input[1024];
	
	struct sembuf sbuf = {-1,0,0};
	semop(semd,&sbuf,1);
	
	int *shrdmem = shmat(memd,NULL,0);
	if (shrdmem == (void *)-1) {
		printf("%s\n",strerror(errno));
	}
	struct stat stp;
	stat("telephone.txt",&stp);
	int fd;
	*shrdmem;
	printf("if check\n");
	if (stp.st_size != 0) {
		fd = open("telephone.txt",O_RDONLY);
		lseek(fd,*shrdmem * -1,SEEK_END);
		read(fd,input,*shrdmem);
		close(fd);
		printf("Last line: %s",input);
	} else {
		printf("You're starting a story!\n");
	}
	
	fgets(input,1024,stdin);
	fd = open("telephone.txt",O_WRONLY | O_APPEND);
	write(fd,input,strlen(input));
	close(fd);
	
	*shrdmem = strlen(input);
	shmdt(shrdmem);
	
	sbuf.sem_op = 1; // make it {1,0,0}
	semop(semd,&sbuf,1);
	
}