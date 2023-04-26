#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024




/*  Comandos a implementar:
-i - Acrescentar pessoas a um ficheiro de dados bin ́ario.
-u - Atualizar a idade de uma determinada pessoa no ficheiro de dados.

    Exemplos de execucao:
$ pessoas -i "Jos ́e Mourinho" 59
$ pessoas -u "Jos ́e Mourinho" 60
*/

//Criar uma struct para pessoas
    // nome, idade, morada, ...
typedef struct pessoa{
    char name[50];
    int age;
}Pessoa;

//Funcao responsavel pela insercao de uma pessoa no ficheiro: "-i"
int insert(char *name, char* age){
    int fd;
    Pessoa person;
    
    if((fd = open("pessoas_testfiles/pessoas", O_CREAT | O_WRONLY | O_APPEND, 0600))>=0){
        strcpy(person.name, name);
        person.age=atoi(age);

        write(fd,&person,sizeof(Pessoa));
        printf("Pessoa \"%s\" inserida corretamente\n",person.name);
    }
    else{
        perror("O Ficheiro nao foi aberto correctamente");
        return -1;
    }

    return (int) lseek(fd,0,SEEK_CUR)/sizeof(Pessoa);
}

//Funcao responsavel pela insercao de uma pessoa no ficheiro: "-i"
void update(){
    printf("Update function is being updated, 0 updates were made to the file.\n");
}


int main(int argc, char **argv){



    if(argc == 4 && !strcmp(argv[1], "-i")){
        printf("Registo: %d\n", insert(argv[2], argv[3]));
    } 
    else if(argc == 4 && !strcmp(argv[1],"-u")){
        update();
    }
    else{
        printf("escachou\n");
    }

    return 0;
}