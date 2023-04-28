#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_STRINGS 10
#define MAX_LENGTH 50

ssize_t myreadln(int fd, char *line, size_t size) {
    ssize_t i = 0, n, r = 0;
    char character;


// testList = [1,2,3] = 1:2:3:[]
//escreve os bytes do fd no buffer até encontrar um final de linha ou o size acabar;

while (i < size - 1) {  // i < size -1 : o size é referente ao buffer.  size-1 é a ultima posicao do buffer que contem o \'0'.
                        //Como este spot está resrvado, percorremos o ciclo no máximno até ao penultimo spot do buffer.
        if ((n = read(fd, &character, 1)) == 1) {
            r += n; //contar quantos merdas foram escritos no buffer (line)
            line[i++] = character; //actually escrever as merdas no buffer. escrita é feita caracter a caracter.
            if (character == '\n') // se encontrarmos o ultimo caracter paramos tudo.
                break;
        } else if (n == 0) { // se o read chegar ao final do ficheiro retorna 0 e paramos.
            break;
        } else {
            return -1;  // O n é um ssize_t therefore se retornar um numero negativo encontramo-nos numa situaçcao de erro de leitura, retornamos entao um -1;
        }
    }
// escreve '\0' no final do buffer para fechar o mesmo e.g. x:[] no haskell [] == '\0'
    line[i] = '\0';

// Volta á pos
    lseek(fd, i - r, SEEK_CUR);

    return i;
}


int main() {
    char **list = (char **) malloc(MAX_STRINGS * sizeof(char *));
    int ret,status,j,i=0;
    ssize_t bytes_read;
    pid_t pid;

    if (list == NULL) {
        perror("Erro ao alocar memória para a lista");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_STRINGS; i++) {
        list[i] = (char *) malloc((MAX_LENGTH + 1) * sizeof(char));
        if (list[i] == NULL) {
            perror("Erro ao alocar memória para a string");
            exit(EXIT_FAILURE);
        }
    }

    // Use the list here...
    while((bytes_read = myreadln(0,list[i],sizeof(char *)))>0 && strcmp(list[i],"stop\n") && i<MAX_STRINGS){
        list[i][bytes_read-1]='\0';
        printf("read:%s\n", list[i]);
        i++;
    }
    

    for(j=0;j<i;j++){
        if (pid = fork()==0){
            if((ret = execlp(list[j],list[j],NULL))==-1){
                printf("List[j]= %s\n", list[j]);
                perror("Falha na leitura do comando por parte do filho\n");
                exit(EXIT_FAILURE);
            }
            _exit(j+1);
        }
    }

    for(int k=0; k<j; k++){
        wait(&status);
        printf("Bom trabalho meo puto!\n");
    }


    for (int i = 0; i < MAX_STRINGS; i++) {
        free(list[i]);
    }
    free(list);

    return 0;
}