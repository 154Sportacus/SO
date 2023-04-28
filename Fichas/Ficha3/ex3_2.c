#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(){

    int ret,status;
    pid_t pid;


    if((pid=fork())==0){
        if((ret=execlp("ls","ls",NULL))==-1){
            perror("Falha ao executar o comando \"ls\"\n");
            exit(EXIT_FAILURE);
        }
        _exit(0);
    }
    else{
        wait(&status);
        printf("Jogas-te bem meu filho\n");

    }


    return 0;
}