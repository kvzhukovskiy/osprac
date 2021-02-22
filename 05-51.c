#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
//В консоли необходимо вызывать параллельно с 05-52.out
int main(){
  int fd;
  size_t size;
  char buffer[100];

  (void)umask(0);
  

  if(mknod("MYFIFO", S_IFIFO | 0666, 0) < 0){
	printf("\nCan't create FIFO");
	exit(-1);
  }

  if((fd = open("MYFIFO", O_WRONLY)) < 0){
	printf("\nCan't open FIFO");
	exit(-1);
  }

  size = write(fd, "Hello, world!", 14);
  if(size != 14){
	printf("\nCan't write all string");
	exit(-1);
  }
  close(fd);
  return 0;
}
