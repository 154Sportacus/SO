#include "utils.h"


//Readline recebe um fd -> file descriptor; *line -> buffer; size -> buffer size in bytes
//         escreve os bytes do fd no buffer até encontrar um final de linha ou o size acabar
//         retorna um numero do tipo ssize_t que se refere ao numero de bytes lidos na linha. MAX Return Number = size
int myreadln(int fd, char *line, int size) {
    int i = 0, n, r = 0;
    char character;


// testList = [1,2,3] = 1:2:3:[]
//escreve os bytes do fd no buffer até encontrar um final de linha ou o size acabar;

while (i<size-1) {  // i < size -1 : o size é referente ao buffer.  size-1 é a ultima posicao do buffer que contem o \'0'.
                        //Como este spot está resrvado, percorremos o ciclo no máximno até ao penultimo spot do buffer.
        if ((n = read(fd, &character, 1)) == 1) {
            if (character == '\n') // se encontrarmos o ultimo caracter paramos tudo. 
                break;
            r += n; //contar quantos merdas foram escritos no buffer (line)
            line[i++] = character; //actually escrever as merdas no buffer. escrita é feita caracter a caracter.
        } else if (n == 0) { // se o read chegar ao final do ficheiro retorna 0 e paramos.
            break;
        } else {
            return -1;  // O n é um ssize_t therefore se retornar um numero negativo encontramo-nos numa situaçcao de erro de leitura, retornamos entao um -1;
        }
    }
// escreve '\0' no final do buffer para fechar o mesmo e.g. x:[] no haskell [] == '\0'
    line[i] = '\0';


    return i;
}


void get_timestamp(char* time_stamp){
    long time_ms;
    struct timeval time_value;

    if (gettimeofday(&time_value, NULL) == -1) {
            perror("gettimeofday() error");
            exit(EXIT_FAILURE);
        }

    //Write the time stamp.
    time_ms = (long)(time_value.tv_sec * 1000LL + time_value.tv_usec / 1000LL);
    sprintf(time_stamp, "%ld\n", time_ms);
}

long get_long_timestamp(){
    struct timeval time_value;

    if (gettimeofday(&time_value, NULL) == -1) {
            perror("gettimeofday() error");
            exit(EXIT_FAILURE);
        }
    
    return (long)(time_value.tv_sec * 1000LL + time_value.tv_usec / 1000LL);
}

