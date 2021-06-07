#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int sendPid;
int ind = 0;
int res = 0;
int flag = 0;

void sigusr1_handler(int nsig){
    res |= (1 << ind);
    ind++;
    kill(sendPid, SIGUSR1);
}

void sigusr2_handler(int nsig){
    ind++;
    kill(sendPid, SIGUSR1);
}

void sigchild_handler(int nsig){
    flag = 1;
}

int main(){
    printf("Receiver pid = %d\n", (int)getpid());
    printf("Enter sender pid: ");
    scanf("%d", &sendPid);
    
    printf("\nWaiting\n");

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    signal(SIGCHLD, sigchild_handler);

    while(!flag);

    printf("Number %d has been recieved\n", res);
    return 0;
}

