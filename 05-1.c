#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
int main(){
    FILE * file;
    char n;
    file = fopen("myfile","rt");
    if(file == NULL){
      printf("Can't open the file");
      exit(-1);
    }  
    while(!feof(file))
    {
      fscanf(file,"%c",&n);
      printf("%c",n);
    }
    fclose(file);
    return 0; 
}
