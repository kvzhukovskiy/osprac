#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <time.h>

//Function A is increment func for sem
//Function D is decrement func for sem
//Start value is 0

int A(int id, struct sembuf *buffer){
	buffer->sem_op = 1;
	buffer->sem_flg = 0;
	buffer->sem_num = 0;
	return semop(id, buffer, 1);
}

int D(int id, struct sembuf *buffer){
	buffer->sem_op = -1;
	buffer->sem_flg = 0;
	buffer->sem_num = 0;
	return semop(id, buffer, 1); 
}

int main(){
	int ptc[2], semid, pid;
	key_t key;
	size_t N, size;
	char path[] = "09-3.c";
	char buf[22]; 
	struct sembuf buffer;

	if(pipe(ptc) < 0){ 
		printf("Can't create pipe\n");
		return -1;
	}

	if((key = ftok(path, 0)) < 0){
		printf("Can't get key\n");
		return -1;
  	}


	if(semid = semget(key, 1, 0666 | IPC_CREAT) < 0){
	  	printf("Can't create semid\n");
	  	return -1;
	}

  	pid = fork(); 
	if(pid < 0){ 
		printf("Can't create fork\n");
		return -1;
	}else if (pid > 0){
		//Parent code 
		printf("Input N > 1\n");
		scanf("%d", &N);
		if(N < 2){
			A(semid, &buffer);
			printf("N must be 2 or higher\n");
			return -1; 
			}
    
		for(int i = 0; i < N; i++){
			size = write(ptc[1], "Hello world to child!!", 22);
			if(size != 22){
				printf("Parent: Can't write full string\n");
				return -1;
			}
			//Writing a message for child
			//And give him rules
			A(semid, &buffer);
			D(semid, &buffer);

			size = read(ptc[0], buf, 22);
       		if(size < 0){
           		printf("Parent: Can\'t read string\n"); 
           		return -1;
        	} 
			printf("Parent got message: [%s]\n", buf);
    	}
    	close(ptc[0]);
	}else{ 
		//Child code  
		for(;;){
			//Waiting for parent message
			D(semid, &buffer);
      		size = read(ptc[0], buf, 22);
      		if(size < 0){
        		printf("Child: Can\'t read string\n"); 
        		return -1;
      		}
      		printf("Child got message: [%s]\n", buf);
   
      		size = write(ptc[1], "Hello world to parent!", 22);
      		if(size != 22){
        		printf("Child: Can\'t write full string\n");
        		return -1;
      		}
			//Writing a meesage for parent
			//And give back rules
      		A(semid, &buffer);
    	} 
  	}
	//Return start value for new launch
	A(semid, &buffer);
	close(ptc[0]);
	close(ptc[1]);	
	return 0; 
}
