#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(){

    int ret;

    /* posso usar assim;
     if ((ret=execlp("/usr/bin/ls","usr/bin/ls",NULL)) == -1){
        perror("Erro na execucao");
        exit(EXIT_FAILURE);
    }    
    */

    /*usar assim;
     if ((ret=execl("/bin/ls","/bin/ls",NULL)) == -1){
        perror("Erro na execucao");
        exit(EXIT_FAILURE);
    }
    */
    // ou assim.
    if ((ret=execlp("ls","ls",NULL)) == -1){
        perror("Erro na execucao");
        exit(EXIT_FAILURE);
    }

    return ret;
}