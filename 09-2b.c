#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main(){
    int *array; 
    int shmid, semid; 
    int new = 1;
    char pathname[] = "09-2a.c"; 
    key_t key; 
    long i; 
    struct sembuf mybuf; 

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
		printf("Can\'t get semid\n");
		exit(-1);
	}

    if((shmid = shmget(key, 3*sizeof(int), 
        0666|IPC_CREAT|IPC_EXCL)) < 0){
        if(errno != EEXIST){
            printf("Can\'t create shared memory\n");
            exit(-1);
        } else {
            if((shmid = shmget(key, 
                3*sizeof(int), 0)) < 0){
                printf("Can\'t find shared memory\n");
                exit(-1);
            }
            new = 0;
        }
    }
    if((array = (int *)shmat(shmid, NULL, 0)) == 
        (int *)(-1)){
        printf("Can't attach shared memory\n");
        exit(-1);    
    }

    if(new){
	semctl(semid, 0, SETVAL, 0);
	semctl(semid, 0, SETVAL, 1);
    }

    while(semctl(semid, 1, GETVAL, 0) == 0){}
    semctl(semid, 0, SETVAL, 0);

    if(new){
	array[0] = 0;
	array[1] = 1;
	array[2] = 1;
    }else{
	array[1] += 1;
	for(int j = 0; j < 1000000000l; j++){}
	array[2] += 1;
    }	
    printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
        array[0], array[1], array[2]);

    semctl(semid, 0, SETVAL, 1);
	
        if(shmdt(array) < 0){ 
            printf("Can't detach shared memory\n");
            exit(-1);
        }
        return 0;
}
