#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int i, status;
    char *buffer = malloc(sizeof(char)*50);

    for(i=0; i<10; i++){
        if((pid=fork())==0){
            sprintf(buffer, "PID filho: %d  |  PID pai: %d\n", getpid(),getppid());
            write(0,buffer,strlen(buffer));
            _exit(i);
        }
        else{
            waitpid(pid,&status,0);       
            sprintf(buffer, "PAI SAYS: Filho \"%d\" terminado com codigo de saida \"%d\"\n",pid, WEXITSTATUS(status));
            write(0,buffer,strlen(buffer));
        }
    }

    return 0;
}