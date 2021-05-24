#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
	int semid, i;
	key_t key;
	char pathname[] = "09-1a.c";
	struct sembuf mybuf;
	if((key = ftok(pathname,0)) < 0){
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
		printf("Can\'t get semid\n");
		exit(-1);
	}

	mybuf.sem_op = 1;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;
	if(semop(semid, &mybuf, 1) < 0){
		printf("Can\'t wait for condition\n");
		exit(-1);
	}
	i = semctl(semid, 0, GETVAL, 0);
	printf("Condition is set %d\n", i);
	return 0;
}
