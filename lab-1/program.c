#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
	printf ("User ID: %d\n", getuid());
	printf ("Group ID: %d\n\n", getgid());

	return 0;
}
