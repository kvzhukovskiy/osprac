#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0; 

void *mythread(void *dummy){
    pthread_t mythid; 
    mythid = pthread_self();
    a = a+1;
    printf("Thread %d, Calculation result = %d\n", mythid, a);
    return NULL;
}

int main(){
    pthread_t thid, mythid, secThid;
    int result, secResult;

    result = pthread_create( &thid, 
        (pthread_attr_t *)NULL, mythread, NULL);
    secResult = pthread_create( &secThid, 
        (pthread_attr_t *)NULL, mythread, NULL);

    if(result != 0){
    printf ("Error on first thread create, return value = %d\n", result);
    exit(-1);
    }

    if(secResult != 0){
    printf ("Error on second thread create, return value = %d\n", secResult);
    exit(-1);
    }

    printf("Firsr thread created, thid = %d\n", thid);
    printf("Second thread created, thid = %d\n", secThid);

    mythid = pthread_self();
    a = a+1; 
    printf("Thread %d, Calculation result = %d\n", mythid, a);
    pthread_join(thid, (void **)NULL);
    pthread_join(secThid, (void **)NULL);
    return 0;
}
