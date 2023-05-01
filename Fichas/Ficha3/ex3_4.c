#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int mysystem(char *command) {
    pid_t pid;
    int pd[2], status;

    if (pipe(pd) == -1) {
        perror("Error creating pipe");
        return -1;
    }

    pid = fork();

    if (pid == -1) {
        perror("Error forking");
        return -1;
    } else if (pid == 0) { // child process
        close(pd[0]); // close read end of pipe
        dup2(pd[1], STDOUT_FILENO); // redirect stdout to pipe
        close(pd[1]); // close write end of pipe
        execlp(command, command, (char*) NULL); // execute command
        perror("Error executing command");
        _exit(EXIT_FAILURE);
    } else { // parent process
        close(pd[1]); // close write end of pipe
        waitpid(pid, &status, 0); // wait for child process to finish
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }
}
