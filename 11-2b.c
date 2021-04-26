#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 

int main(){
    int msqid; 
    char pathname[] = "11-2a.c";
    key_t key; 
    int len, maxlen, i; 

    struct mymsgbuf{ 
        long mtype;
	struct {
	    short sinfo;
            float finfo;
	} info; 
    } mybuf;

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    } 

    for (i = 0; i < 5; i++){
	mybuf.mtype = 2;
	mybuf.info.finfo = 3.51;
	len = sizeof(mybuf.info);

        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }
    
    maxlen = sizeof(mybuf.info);

    for(i = 0; i < 5; i++){
        if(len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0) < 0){
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
	while(mybuf.mtype == 2){}; //В случае первого запуска ожидает ответ, в случае зацикливания необходимо запустить вторую программу !Параллельно этой!
	printf("Proc 2 got message type of short = %ld, info = %d\n", mybuf.mtype, mybuf.info.sinfo);	
        
    }
    return 0;
}
