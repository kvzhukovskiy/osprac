#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 

//В программе 11-3а.с пользователь вводит число
//Программа 11-3b.с представляет из себя сервер
//При запуске нескольких серверов, дублирующие сервера будут прокидывать
//Результат дальше, не меняя его, после чего завершать работу
//Начальный же сервер будет работать до закрытия консоли или до 
//Вызова программы kill, которая посылает сообщение с типом, используемым
//Для завершения работы

int main(){
    int msqid; 
    char pathname[] = "11-3b.c"; 
    key_t key; 
    int i, len, maxlen; 

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


    mybuf.mtype = 1;
    printf("Input float number: ");
    scanf("%f", &mybuf.info);
    len = sizeof(mybuf.info);

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    maxlen = sizeof(mybuf.info);

    if(len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 2, 0) < 0){
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }
    printf("Server answered result = %f\n", mybuf.info);	
        
    return 0;
}
