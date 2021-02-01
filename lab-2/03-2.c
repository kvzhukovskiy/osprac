#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
	int pid = fork();

	if (pid == -1) {
   		printf("\nПроизошла ошибка");
		exit(1);
	} else if (pid == 0) {
    		printf("\nЭто процесс ребенка");
		printf("\npid = %d", getppid());
		exit(0);
	} else {
    		printf("\nЭто процесс родителя");
		printf("\npid = %d", getpid());
		printf("\nЗдесь происходят другие действия:)\n");
	}
}
