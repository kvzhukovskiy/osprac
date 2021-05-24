#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LAST_MESSAGE 255 

//В программе 11-3а.с пользователь вводит число
//Программа 11-3b.с представляет из себя сервер
//Cервер будет работать до закрытия консоли или до 
//Вызова программы kill, которая посылает сообщение с типом, используемым
//Для завершения работы

int main(){
    int msqid; 
    char pathname[] = "11-3b.c"; 
    key_t key; 
    int i, len, maxlen; 

    struct sendbuf{ 
        long mtype;
	struct{
	    pid_t pid;
	    float mess;
	} info;	 
    } sbuf;

    struct getbuf{ 
        long mtype;
	float mess;
    } gbuf;    

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    } 

    sbuf.info.pid = getpid();
    sbuf.mtype = 1;
    printf("Input float number: ");
    scanf("%f", &sbuf.info.mess);
    len = sizeof(sbuf.info);

    if (msgsnd(msqid, (struct msgbuf *) &sbuf, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    maxlen = sizeof(gbuf.mess);

    if(len = msgrcv(msqid, (struct msgbuf *) &gbuf, maxlen, sbuf.info.pid, 0) < 0){
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }
    printf("Server answered result = %f\n", gbuf.mess);	
        
    return 0;
}
