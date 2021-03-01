#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
 
int main(){
  int *ptr;
  int md, fd, flag;
  int i = 0;
  key_t key;
  if((key = ftok("./key", 0)) < 0){
    printf("Error of generate key\n");
    exit(-1);
  }
  if((md = shmget(key, 8192*sizeof(int), 0777|IPC_CREAT|IPC_EXCL)) < 0){
  if(errno != EEXIST){
    printf("Error of create shared memory\n");
    exit(-1);
  }
  else {
    if((md = shmget(key, 8192*sizeof(int), 0)) < 0){
      printf("Error of find shared memory\n");
      exit(-1);
      }
    }
  }
  if((ptr = (int *)shmat(md, NULL, 0)) == (int *)(-1)){
    printf("Error attach shared memory\n");
    exit(-1);
  }
  fd = open ("./readingFile", O_RDONLY); //Читаем текст из файла
  while ((flag = read (fd, &ptr[i], 1)) > 0){ //Записывам его в массив
    i++;
  }
  ptr[i+1]=EOF;
  if (flag < 0){
    printf ("Can not read file\n");
    exit (1);
  }  
  if(shmdt(ptr) < 0){  
    printf("Can't detach shared memory\n");
    exit(-1);
  }
  return 0;
}
