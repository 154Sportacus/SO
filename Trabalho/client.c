#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myreadln.c"

#define pidBuffSize 64
#define SERVER_PIPE "ClientServerConnection"

int server_fd;


char** split_string(char* input_string, int* num_strings) {
    // Allocate memory for the array of strings
    char** string_array = (char**)malloc(sizeof(char*) * strlen(input_string));
    if (string_array == NULL) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    // Split the input string by spaces and store each substring in the array
    char* token = strtok(input_string, " ");
    int i = 0;
    while (token != NULL) {
        string_array[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    //Add NULL argument at the end of the array
    string_array[i] = NULL;
    
    // Set the number of strings in the array and return it
    *num_strings = i;
    return string_array;
}

void executeUnique(char** array){
    int ret,status;
    pid_t pid;

    //Alocate temporary buffers
    char buffer[pidBuffSize]; //pidBuffSize == 64
    char pid_SC[pidBuffSize+4]; //e.g: SC_1053 -> 1053 pid do filho
    char pid_CS[pidBuffSize+4];

    //Child process
    if((pid=fork())==0){
        int pidValue = getpid();
        sprintf(buffer, "Running PID: %d\n", pidValue);
        
        //Name of the named pipes about to be used
        sprintf(pid_CS, "CS_%d\n",pidValue); //client to server [WRITE]
        sprintf(pid_SC, "SC_%d\n",pidValue); //server to client [READ]
        
        //Create the named pipes
        int fd_Write = makeFIFO(pid_CS,0600);
        int fd_Read = makeFIFO(pid_SC,0600);

        //Tell the server the "client named pipes" names 
        write(server_fd, pid_CS, strlen(pid_CS));
        write(server_fd, pid_SC, strlen(pid_SC));
        
        /*
            open(fd_Write, O_WRONLY);
            open(fd_Read, O_RDONLY);
        */

        //Inform the user which PID is running
        write(0,buffer, strlen(buffer));
        
        //Proceed to exec:
        if((ret=execvp(array[0],array))==-1){
            perror("Falha ao executar o comando \"ls\"\n");
            exit(EXIT_FAILURE);
        }
        //_exit(0); O GROOT DISSE QUE O EXEC NO IF ACIMA JÁ FAZ UM EXIT, THEREFORE THIS LINE IS SHIET!
    }
    else{ //Parent process
        waitpid(pid, &status, 0);
        printf("Jogas-te bem meu filho\n");

    }
}


int main(int argc, char** argv) {
    char input_string[] = "This is a test string.";
    int num_strings;

    server_fd = open(SERVER_PIPE, O_WRONLY);

    if(argc < 3){
        return -1;
    }

    if(argc >= 3 && !strcmp(argv[2], "-u")){

    char** string_array = split_string(argv[3], &num_strings);

    executeUnique(string_array);

    // Print the array of strings
    printf("Number of strings: %d\n", num_strings);
    for (int i = 0; i < num_strings; i++) {
        printf("String %d: %s\n", i, string_array[i]);
    }

    // Free the memory allocated for the array of strings
    free(string_array);
    }
    return 0;
}
