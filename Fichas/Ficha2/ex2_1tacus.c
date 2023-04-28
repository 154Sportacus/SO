#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    pid_t pid;
    int status;


    if((pid=fork())==0){
        char *buffer = malloc(sizeof(char)*50); //multiplicar por 50 só para garantir que há espaço suficiente para escrever a string e os pid's
        sprintf(buffer,"Parent id: %d\nChild id: %d\n",getppid(),getpid());
        write(0,buffer,strlen(buffer));
        free(buffer);
        _exit(0);
    }
    else{
        wait(&status);
        printf("Parent process (pid= %d) is closing...\n", getpid());
    }

    return 0;
}