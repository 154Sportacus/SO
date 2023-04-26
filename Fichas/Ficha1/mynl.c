#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

//Assinatura da função auxiliar readline
ssize_t myreadln(int fd, char *line, size_t size); 


//Funcao nl
void nl(int fd){
    size_t MAX = 1024;
    char lixo[MAX],prompt[24];
    ssize_t r =0;
    int j;
    for (int i = 0;(r=myreadln(fd,lixo,MAX))>0; i++){
        if(r==1){
            write(1,"\n",1);
            i--;
        }
        else{
            j = sprintf(prompt,"    %d  ",i);
            write(1,prompt,j);
            write(1,lixo,r);
        }
    }
}




int main(int argc, char **argv) {
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    nl(fd);
    close(fd);
    return 0;
}

//Função auxiliar myreadln

ssize_t myreadln(int fd, char *line, size_t size) {
    ssize_t i = 0, n, r = 0;
    char character;



while (i < size - 1) {  
                        
        if ((n = read(fd, &character, 1)) == 1) {
            r += n;
            line[i++] = character; 
            if (character == '\n') 
                break;
        } else if (n == 0) { 
            break;
        } else {
            return -1;  
        }
    }

    line[i] = '\0';


    lseek(fd, i - r, SEEK_CUR);

    return i;
}
