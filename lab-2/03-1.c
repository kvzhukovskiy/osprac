#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid, ppid;

  pid  = getpid();
  ppid = getppid();
  //ppid остается неизменным, pid постепенно растет

  printf("My pid = %d, my ppid = %d\n", (int)pid, (int)ppid);

  return 0;
}
