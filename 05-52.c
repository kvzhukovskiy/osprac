#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
  int fd;
  size_t size;
  char readbuffer[14];

  if((fd = open("MYFIFO", O_RDONLY)) < 0){
	printf("\nCant't open FIFO");
	exit(-1);
  }

  size = read(fd, readbuffer, 14);

  if(size < 0){
	printf("\nCant't read all string");
	exit(-1);
  }

  printf("\n%s", readbuffer);

  close(fd);
  return 0;
}
