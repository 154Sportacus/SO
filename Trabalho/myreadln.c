#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

//Readline recebe um fd -> file descriptor; *line -> buffer; size -> buffer size in bytes
//         escreve os bytes do fd no buffer até encontrar um final de linha ou o size acabar
//         retorna um numero do tipo ssize_t que se refere ao numero de bytes lidos na linha. MAX Return Number = size
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
