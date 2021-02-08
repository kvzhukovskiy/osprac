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
		//Запуск программы 03-03.exe без входных параметров
		execl("./03-3.exe", NULL);
		exit(0);
	}
}
