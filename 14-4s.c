#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int recPid;
int flag = 1;

void receiver_handler(int nsig){
    flag = 1;
}

int main(){
    int value;
    signal(SIGUSR1, receiver_handler);
    printf("Sender pid = %d\n", (int)getpid());
    printf("Enter receiver pid: ");
    scanf("%d", &recPid);

    printf("\nEnter a value to send: ");
    scanf("%d", &value);

    int lenght = sizeof(int)*8;
    
    for(int i = 0; i < lenght; i++){
	while(!flag);

	if((value & (1 << i)) != 0){
	    kill(recPid, SIGUSR1);
	}else{
   	    kill(recPid, SIGUSR2);
        }
	
	flag = 0;
    }
    kill(recPid, SIGCHLD);
    return 0;
}

