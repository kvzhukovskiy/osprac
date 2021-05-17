#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 

int main(){
    int msqid, type; 
    char pathname[] = "11-3b.c";
    key_t key; 
    float mess;
    int len, maxlen, i; 

    struct mymsgbuf{ 
        long mtype;
	float info; 
    } mybuf;

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    } 

    while(1){
        maxlen = sizeof(mybuf.info);

        if(len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0) < 0){
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

	if(mybuf.mtype == LAST_MESSAGE){
	    printf("Killer programm have been executed, bye\n");
	    exit(-1);
	}
	
	if(mybuf.mtype == 2){
	    printf("There are more then one server online\n");
	    mess = mybuf.info;
	    type = 2;
	}else{
	    printf("Server got number = %f\n", mybuf.info);
            mess = mybuf.info * mybuf.info; 
	}

        mybuf.mtype = 2;
        mybuf.info = mess;
        len = sizeof(mybuf.info);

        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }

	if(type == 2){
	    exit(-1);	
	} 
   }

    	
        
    return 0;
}
