#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    //criar buffer e signed size t;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    //criar ciclo para ler e escrever
    while((bytes_read = read(0,buffer, BUFFER_SIZE)) > 0){
        if( (bytes_written = write(1,buffer, bytes_read)) != bytes_read){
            perror("Failed to write to stdout");
            exit(1);
        }
    }

    //verificar se os bytes lidos são válidos; e.g.: ssize_t a ==-1 significa que é erro
    if (bytes_read < 0){
        perror("Failed to read from stdin");
        exit(1);
    }

    return 0;
}