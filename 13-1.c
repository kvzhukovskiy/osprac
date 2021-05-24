#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

void move(char *path, int len){
   path[len - 1] += 1;
   if(path[len - 1] > 'z'){
	path[len - 1] = 'a';
	move(path, len - 2);
   }
}

void func(char *path, int len, int *count){
   char *next = malloc(sizeof(char) * (len + 1));

   strcpy(next, path);
   move(next, len);

   if(symlink(path, next) == -1){//Если не удается создать связь => выход
	return;
   } 

   func(next, len, count);//Рекурсивный вызов
   
   (*count)++;//Увеличение счетчика
   unlink(next);//Удаление файла
   free(next);//Освобождение памяти
}

int main(){
    FILE *file;
    int recCount = 0;
    
    if(mkdir("folder", 0777) < 0){
	printf("Can't create folder");
	exit(-1);
    }

    file = fopen("folder/a", "w");

    if(file == NULL){
	printf("Can't open the file\n");
	exit(-1);
    }

    if(fclose(file) == -1){
	printf("Can't close the file\n");
	exit(-1);
    }

    func("folder/a", 8, &recCount);
    printf("Depth = %d\n", recCount);
}
