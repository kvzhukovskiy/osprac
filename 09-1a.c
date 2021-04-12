#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
	int semid;
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

	if ((semctl(semid, 0, GETVAL, 0) < 5) && (semctl(semid, 0, GETVAL, 0) >= 0)){
		mybuf.sem_op = 0;
        	mybuf.sem_flg = 0;
       		mybuf.sem_num = 0;
		printf("Not enough of launch second programm\n");
 		exit(-1);	
	       if (semop(semid, &mybuf, 1) < 0){
	     	  printf("Can\'t wait for condition\n");
	          exit(-1);
	        }
	}

	printf("Condition is present\n");
	//В задании не указано сбрасывать ли флаг после успешного запуска
	//Первой программы, строка ниже делает это
	//semctl(semid, 0, SETVAL, 0); 
	return 0;
}
