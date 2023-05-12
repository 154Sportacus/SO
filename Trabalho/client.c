#include "utils.h"


int fd_write, fd_read, server_fd;


void generate_named_pipes(char* pid_SC, char* pid_CS){
    
    //Create the named pipes
    printf("Creatingdasf  \n");
    if(mkfifo(pid_CS,0666)<0)
        perror("Error: Could not create the client -> server named pipe");
    if(mkfifo(pid_SC,0666)<0)
        perror("Error: Could not create the server -> client named pipe");

    //Guarded opens
    printf("Creatingdasf sadasdas \n");
    
    if((fd_write = open(pid_CS, O_WRONLY))==-1){
        perror("Error: Could not open write pipe for client -> server");
        return;
    }
    if((fd_read = open(pid_SC, O_RDONLY))==-1){
        perror("Error: Could not open read pipe for client -> server");
        sleep(1);
    } 
    printf("Creatingdasf sadasdas3 \n");

    

}
 
/*
void open_named_pipes(char* pid_SC, char* pid_CS){
    if((fd_read = open(pid_SC, O_RDONLY))==-1){
            perror("Error: Could not open read pipe for client -> server");
            return;
    } 

    if((fd_write = open(pid_CS, O_WRONLY))==-1){
            perror("Error: Could not open write pipe for client -> server");
            return;
    }
}
*/

void close_named_pipes(){
    close(fd_read);
    close(fd_write);
}




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
    char buffer[BUFF_SIZE];
    char pid_buff[BUFF_SIZE+1];
    char time_stamp[2*BUFF_SIZE];
    char initial_time_stamp[2*BUFF_SIZE];
    char final_time_stamp[2*BUFF_SIZE];
    char program_name[BUFF_SIZE];
    char error_message[BUFF_SIZE];

    //Child process:
    if((pid=fork())==0){ 
        close(server_fd);
        if((ret=execvp(array[0],array))==-1){
            perror("Falha ao executar o comando.\n");
            exit(EXIT_FAILURE);
        }
    }
    //Parent process:
    else{ 
        char pid_SC[32];
        char pid_CS[32];
        
        //Name of the named pipes about to be used
        sprintf(pid_CS, "CS_%d",pid); //client to server [WRITE]
        sprintf(pid_SC, "SC_%d",pid); //server to client [READ]
        printf("Parent process: %d\n",getpid());
        get_timestamp(time_stamp);
        sprintf(buffer, "Running PID: %d\n", pid);
        
        //Send the PID of the child process to the server
        sprintf(pid_buff, "%d\n", pid);
        
        printf("_95 generate_named_pipes(pid);\n");                    /*             /!\     /!\     /!\ */
        generate_named_pipes(pid_SC, pid_CS);
        
        //Writes to serverhe connection request with child process's pid.
        printf("Server connection request by pid %d: \" \"\n", pid);
        write(server_fd, pid_buff, strlen(pid_buff));
        WR_NEWLINE(server_fd);
        close(server_fd);
        sleep(10);

        //open_named_pipes(pid_SC, pid_CS);

        //Informs the user which PID is running.
        write(0,buffer, strlen(buffer));
        
        //Initial time stamp protocol is prepended.
        sprintf(initial_time_stamp, "I");
        strcat(initial_time_stamp, time_stamp);
        strcat(initial_time_stamp, "\n");

        //Informs the user of the initial timestamp.
        write(0,time_stamp,strlen(time_stamp));
       
        //Program name protocol is prepended.
        sprintf(program_name, "N");
        strcat(program_name,array[0]);
        strcat(program_name, "\n");

        //Informs the server of the program name.
        write(fd_write, program_name, strlen(program_name));

        //Informs the server of the initial timestamp.
        write(fd_write,initial_time_stamp,strlen(initial_time_stamp));

        //Waits for the child process to finish.
        waitpid(pid, &status, 0);

        if(WEXITSTATUS(status)){ 
            sprintf(error_message,"Child process exited with status: %d\n", status);
            perror(error_message);
        }
        //Gets the final time stamp.
        get_timestamp(time_stamp);
        sprintf(final_time_stamp, "F");
        strcat(final_time_stamp, time_stamp);
        strcat(final_time_stamp, "\n");
    
        //Informs the server of the final timestamp.
        write(fd_write,final_time_stamp,strlen(final_time_stamp));
        WR_NEWLINE(fd_write);

        close_named_pipes();
    }
}





void status_request(){
    char *status = "status\n";
    int bytes_read;
    char buffer[BUFF_SIZE];
    char pid_SC[32];
    char pid_CS[32];
    int pid = getpid();
        
    //Name of the named pipes about to be used
    sprintf(pid_CS, "CS_%d",pid); //client to server [WRITE]
    sprintf(pid_SC, "SC_%d",pid); //server to client [READ]

    generate_named_pipes(pid_SC,pid_CS);
    //open_named_pipes(pid_SC,pid_CS);
    write(fd_write, status, strlen(status));
    WR_NEWLINE(fd_write);
    
    while((bytes_read = myreadln(fd_read, buffer, BUFF_SIZE))>=0){
        if(bytes_read==0)
            break;
        write(0, buffer, bytes_read);
    }

    close_named_pipes();
}






int main(int argc, char** argv) {
    int num_strings;

    server_fd = open(SERVER_PIPE, O_WRONLY,0);

    if (server_fd < 0){
        write(2,"could not open server pipe",strlen("could not open server pipe"));
        return 1;
    }


    if(argc >= 3 && !strcmp(argv[2], "-u")){
        char** string_array = split_string(argv[3], &num_strings);

        executeUnique(string_array);
        printf("acabou de correr executeUnique\n");
        // Print the array of strings
        printf("Number of strings: %d\n", num_strings);
        for (int i = 0; i < num_strings; i++) {
            printf("String %d: %s\n", i, string_array[i]);
        }

        // Free the memory allocated for the array of strings
        free(string_array);
    }
    else if(argc == 2 && !strcmp(argv[1], "status")){
        status_request();
    }

    return 0;
}
