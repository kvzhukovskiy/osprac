#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LAST_MESSAGE 255 

int main(){
    int msqid, type; 
    char pathname[] = "11-3b.c";
    key_t key; 
    float mess;
    int len, maxlen, i; 

    struct sendbuf{ 
        long mtype;
	float mess; 
    } sbuf;

    struct getbuf{ 
        long mtype;
	struct{
	    pid_t pid;
	    float mess;	
	} info;
    } gbuf;


    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    } 

    while(1){
        maxlen = sizeof(gbuf.info);

        if(len = msgrcv(msqid, (struct msgbuf *) &gbuf, maxlen, 0, 0) < 0){
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

	if(gbuf.mtype == LAST_MESSAGE){
	    printf("Killer programm have been executed, bye\n");
	    exit(-1);
	}
	
	printf("Server got number = %f, from client pid: %ld\n", gbuf.info.mess, 			gbuf.info.pid);
        mess = gbuf.info.mess * gbuf.info.mess; 

        sbuf.mtype = gbuf.info.pid;
        sbuf.mess = mess;
        len = sizeof(sbuf.mess);

        if (msgsnd(msqid, (struct msgbuf *) &sbuf, len, 0) < 0){
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
   }

    	
        
    return 0;
}
