#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

void move(char *path, int *len){
   if(path[*len - 1] != 'z'){
	path[*len - 1] += 1;//Меняем последнюю букву на следующую по алфавиту
   }else{
	strcat(path, "a"); //Добавляем в конец пути a
	(*len)++;
   }
}

void func(char *path, int len, int *count){
   int op;

   char *next = malloc(sizeof(char) * (len + 1));
   strcpy(next, path);

   move(next, &len);

   if(symlink(path, next) == -1){//Если не удается создать связь => выход
	return;
   } 

   op = open(next, O_RDWR | O_CREAT, 0666);//Пробуем открыть

   if(op < 0){
	if(remove(next) < 0) { //Пытаемся удалить файл
	    printf("Can't delete file\n");
	    exit(-1);
	}
	return; //Выход из рекурсии
   }

   if(close(op) < 0){
	printf("Can't close the file\n");
	exit(-1);
   }  

   func(next, len, count);//Рекурсивный вызов
   
   (*count)++;//Увеличение счетчика
   unlink(next);//Удаление файла
   free(next);//Освобождение памяти
}

int main(){
    FILE *file;
    int recCount = 0;

    file = fopen("a", "w"); // Создаем файл

    if(file == NULL){
	printf("Can't open the file\n");
	exit(-1);
    }

    if(fclose(file) == -1){
	printf("Can't close the file\n");
	exit(-1);
    }

    func("a", 1, &recCount);
    printf("Max Recursive Depth = %d\n", recCount);
    return 0;
}
