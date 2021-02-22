#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
  int size = 0;
  int fd;
  while(write(fd, "h", 1) == 1)
  	size++;
  printf("Size of pipe =  %d\n");
  close(fd);
  return 0;
}
