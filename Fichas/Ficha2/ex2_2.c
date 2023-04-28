#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    pid_t pid;
    int status;

    if((pid = fork()) == 0){
        printf("Eu sou o filho, o meu pid é: %d.O pid do meu pai é:%d\n",getpid(),getppid());
        _exit(0);
    }
    else{
        wait(&status);
        printf("Eu sou o pai, o meu pid é: %d. O pid do meu filho é: %d.E o pid do meu pai é:%d\n",getpid(),pid,getppid());
    }
    

    return 0;
}