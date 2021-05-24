#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

//Start value is 0
//Parent: for{write() => A(+2) => Z => read}
//Child: for{D(-1) => read() => write() => D(-1)}

int A(int id, struct sembuf *buffer, int val){
	buffer->sem_op = val;
	buffer->sem_flg = 0;
	buffer->sem_num = 0;
	return semop(id, buffer, 1);
}

int D(int id, struct sembuf *buffer, int val){
	buffer->sem_op = val;
	buffer->sem_flg = 0;
	buffer->sem_num = 0;
	return semop(id, buffer, 1); 
}

int Z(int id, struct sembuf *buffer){
	buffer->sem_op = 0;
	buffer->sem_flg = 0;
	buffer->sem_num = 0;
	return semop(id, buffer, 1); 
}

int main(){
	int ptc[2], semid, pid;
	key_t key;
	size_t N, size;
	char buf[15];
	char path[] = "09-3.c";
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

	printf("Input N > 1\n");
	scanf("%d", &N);
	if(N < 2){
		printf("N must be 2 or higher\n");
		return -1; 
	}

  	pid = fork();
 
	if(pid < 0){ 
		printf("Can't create fork\n");
		return -1;
	}else if (pid > 0){
		//Parent code 
		for(int i = 0; i < N; i++){
			size = write(ptc[1], "Hello, child!", 14);
			if(size != 14){
				printf("Parent: Can't write full string\n");
				return -1;
			}

			A(semid, &buffer, 2);
			if(semop(semid, &buffer, 1) < 0){
				printf("Can't increase value\n");
				return -1;
			}

			Z(semid, &buffer);
			if(semop(semid, &buffer, 1) < 0){
				printf("Can't Z\n");
				return -1;
			}

			size = read(ptc[0], buf, 15);
       		if(size < 0){
           		printf("Parent: Can't read string\n"); 
           		return -1;
        	} 
			printf("Parent got message: [%s]\n", buf);
    	}
		printf("Parent finished working");
	}else{ 
		//Child code  
		for(int i = 0; i < N; i++){
			D(semid, &buffer, -1);
			if(semop(semid, &buffer, 1) < 0){
				printf("Can't decrease value\n");
				return -1;
			}

      		size = read(ptc[0], buf, 14);
      		if(size < 0){
        		printf("Child: Can't read string\n"); 
        		return -1;
      		}
      		printf("Child got message: [%s]\n", buf);
   
      		size = write(ptc[1], "Hello, parent!", 15);
      		if(size != 15){
        		printf("Child: Can\'t write full string\n");
        		return -1;
      		}

      		D(semid, &buffer, -1);
			if(semop(semid, &buffer, 1) < 0){
				printf("Can't increase value\n");
				return -1;
			}
    	} 
		printf("Child finished working");
  	}
	close(ptc[0]);
	close(ptc[1]);	
	return 0; 
}
