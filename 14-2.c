#include <signal.h>
#include <stdio.h>

void my_handler(int nsig){
    if(nsig == SIGINT){
    	printf("Buttons Ctrl + C were pushed\n");
	return;	
    }
    if(nsig == SIGQUIT){
    	printf("Buttons Ctrl + 4 were pushed\n");
	return;
    }
    printf("Some other buttons were pushed\n");
}

int main(){
    (void)signal(SIGINT, my_handler);
    (void)signal(SIGQUIT, my_handler);
    while(1);
    return 0;
}

