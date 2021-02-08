#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
  printf("argument count = %d\n", argc);
  for(int i = 0; i < argc; i++){
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  while(*envp){
    printf("%s\n", *envp++);
  }	

  return 0;
}
