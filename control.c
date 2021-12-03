#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <sys/shm.h>

#define SEMKEY 5423508
#define MEMKEY 4580924

union semun {
	int val;                  //used for SETVAL
    struct semid_ds *buf;     //used for IPC_STAT and IPC_SET
    unsigned short  *array;   //used for SETALL
    struct seminfo  *__buf;
};

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: ./control [-c create] [-r remove]\n");
		return 0;
	} else {
		if (!strcmp(argv[1],"-c")) {
			int semd = semget(SEMKEY,1,IPC_CREAT | IPC_EXCL | 0600);
			int memd = shmget(MEMKEY,sizeof(int), IPC_CREAT | IPC_EXCL | 0600);
	
			union semun u;
			u.val = 1;
			semctl(semd,0,SETVAL,u);
			int fd = open("telephone.txt",O_CREAT | O_WRONLY | O_TRUNC, 0644);
			close(fd);
		} else if (!strcmp(argv[1],"-r")) {
			int semd = semget(SEMKEY,0,0);
			int memd = shmget(MEMKEY,0,0);
			shmctl(memd,IPC_RMID,NULL);
			semctl(semd,0,IPC_RMID);
						
			int fd = open("telephone.txt",O_RDONLY);
			struct stat stp;
			stat("telephone.txt",&stp);
			char *buffer = malloc(stp.st_size + 1);
			buffer[stp.st_size] = '\0';
			read(fd,buffer,stp.st_size);
			printf("The Full Story:\n%s",buffer);
			free(buffer);
			close(fd);
		} else {
			printf("Invalid flag '%s'\n",argv[1]);
			return 1;
		}
	}
}