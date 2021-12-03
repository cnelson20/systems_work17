#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>

#define SEMKEY 5423508
#define MEMKEY 4580924

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: ./control [-c create] [-r remove]\n");
		return 0;
	} else {
		if (!strcmp(argv[1],"-c")) {
			int semd = semget(SEMKEY,1,IPC_CREAT | IPC_EXCL | 0600);
			semctl(semd,0,SETVAL,1);
			int fd = open("telephone.txt",O_WRONLY | O_TRUNC);
			close(fd);
		} else if (!strcmp(argv[1],"-r")) {
			int semd = semget(SEMKEY,0,0);
			semctl(semd,0,IPC_RMID);
		} else {
			printf("Invalid flag '%s'\n",argv[1]);
			return 1;
		}
	}
}