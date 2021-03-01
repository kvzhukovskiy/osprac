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
  int md, flag;
  int i = 0;
  key_t key;
  int fd;
  if((key = ftok("./key", 0)) < 0){
    printf("Can\'t generate key\n");
    exit(-1);
  }
  if((md = shmget(key, 8192*sizeof(int), 0)) < 0){
    printf("Can\'t find shared memory\n");
    exit(-1);
  }
  if((ptr = (int *)shmat(md, NULL, 0)) == (int *)(-1)){
    printf("Can't attach shared memory\n");
    exit(-1);
  }
  while (ptr[i] != EOF ){//Выводим информацию из сегмента
    putchar(ptr[i]);
    i++;
  }
  if(shmctl(md, IPC_RMID, NULL) < 0){ //Пробуем отчистить сегмент
    printf("Can't free shared memory\n");
    exit(-1);
  }
  return 0;
}
