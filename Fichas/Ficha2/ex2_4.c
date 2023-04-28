#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int i, status;
    char *buffer = malloc(sizeof(char)*50);

    for (i = 1; i <= 10; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
        else if (pid == 0) {
            // child process
            sprintf(buffer,"Deu merda\n");
            write(0,buffer,strlen(buffer));
            exit(i);
        }
    }

    // parent process
    for (i = 1; i <= 10; i++) {
        wait(&status);
        if (WIFEXITED(status)) {
            sprintf(buffer,"Child %d exited with status %d\n", i, WEXITSTATUS(status));
            write(0,buffer,strlen(buffer));
        }
        
        
    }

    free(buffer);

    return 0;
}