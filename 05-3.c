#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
 
int main(){
  int ptc[2]; 
  int ctp[2]; 
  pid_t pid;
  ssize_t size;
  char buf[100]; 
  
  if(pipe(ptc) < 0){ 
    printf("Can't create first pipe\n");
    return -1;
  }
  if(pipe(ctp) < 0){ 
    printf("Can't create second pipe\n");
    close(ptc[0]); close(ptc[1]);
    return -1;
  }
  
  pid = fork(); 
  if(pid < 0){ 
    printf("Can't fork child\n");
    return -1;
  }else if (pid > 0){
    //parent code 
    close(ptc[0]);
    close(ctp[1]);

    size = write(ptc[1], "Hello world from child!", 23);
    if(size != 23){
      printf("Parent: Can't write full string\n");
      return -1;
    }

    size = read(ctp[0], buf, 24);
    if(size < 0){
      printf("Parent: Can\'t read string\n"); 
      return -1;
    } 
    printf("P: [%s]\n", buf);
    
    close(ptc[1]);
    close(ctp[0]);
    printf("Wait for child pid = %i\n", pid);
    waitpid(pid, NULL, 0); //waiting for child
    printf("Parent done\n");
  }else{ 
    //child code
    close(ptc[1]);
    close(ctp[0]);
    
    size = read(ptc[0], buf, 23);
    if(size < 0){
      printf("Child: Can\'t read string\n"); 
      return -1;
    } 
    printf("C: [%s]\n", buf);
    
    size = write(ctp[1], "Hello world from parent!", 24);
    if(size != 24){
      printf("Child: Can\'t write full string\n");
      return -1;
    }
    
    close(ptc[0]);
    close(ctp[1]);
    
    printf("Child done\n");
  }
  return 0; 
}
