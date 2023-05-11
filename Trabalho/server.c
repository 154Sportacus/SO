#include "utils.h"

#define SERVER_PIPE "ClientServerConnection"
#define TEMPORARY "tmp/"
#define TERMINATED "term/"
#define NEWLINE "\n"
#define BUFF_SIZE 1024

int writefd, readfd, server_fd;

void answer_status(){
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    long time_ms;
    long time_stamp_inicial;
    char execution_time[2*BUFF_SIZE];
    int bytesRead;
    char file_path[BUFF_SIZE];
    char buffer[2*BUFF_SIZE];
    char program_name[2*BUFF_SIZE];
    char *endptr;
    char* error_message;
    
    if ((dir = opendir(TEMPORARY)) == NULL) {
        error_message = "Failed to open directory\n";
        write(2, error_message, strlen(error_message));
        write(writefd, error_message, strlen(error_message));
        return;
    }


    while ((ent = readdir(dir)) != NULL) {
        sprintf(file_path, "%s/%s", TEMPORARY, ent->d_name);
        printf("%s\n", file_path);
        
        //S_ISREG(st.st_mode) -> Macro to check if a file is a regular file
        if (stat(file_path, &st) == 0 && S_ISREG(st.st_mode)) {
            int file = open(file_path, O_RDONLY);
            if (file<0) {
                error_message = "Failed to open file\n";
                write(2,error_message,strlen(error_message));
                
            }
            while((bytesRead = myreadln(file, buffer, 2*BUFF_SIZE)) > 0) {
                if(buffer[0] == 'I'){
                    time_ms = get_long_timestamp();
                    time_stamp_inicial = strtol(buffer+1, &endptr, 10);
                    sprintf(execution_time, "%ld\n", time_ms - time_stamp_inicial);
                    write(writefd,execution_time,strlen(execution_time));
                }else if(buffer[0] == 'N'){
                    sprintf(program_name,"Program name: %s\n",buffer+1);
                    write(writefd,program_name,strlen(program_name));
                }
            }
            close(file);
        }
    }
}


void message_protocol(char* pid){
    char buffer[1024];
    int bytesRead;
    long time_stamp_inicial;
    long time_stamp_final;
    char executiontime_str[2*BUFF_SIZE];
    char program_name[2*BUFF_SIZE];
    char *endptr;
    int file_fp=0;
    char file_path_tmp[64];
    char file_path_term[64];
    sprintf(file_path_tmp,"%s%s",TEMPORARY,pid);
    sprintf(file_path_term,"%s%s",TERMINATED,pid);

    while((bytesRead = myreadln(readfd,buffer,1024)) > 0){
        if(strcmp(buffer,"status")==0){
            answer_status();
        }else if(buffer[0] == 'I'){
            time_stamp_inicial = strtol(buffer+1, &endptr, 10);
            printf("%s\n",file_path_tmp);
            file_fp = open(file_path_tmp, O_WRONLY | O_CREAT | O_APPEND, 0666);
            write(file_fp,buffer,bytesRead);
            write(file_fp,NEWLINE,1);
            close(file_fp);
        }else if(buffer[0] == 'N'){
            sprintf(program_name,"%s",buffer+1);
            file_fp = open(file_path_tmp, O_WRONLY | O_CREAT | O_APPEND, 0666);
            write(file_fp,buffer,bytesRead);
            write(file_fp,NEWLINE,1);
            close(file_fp);
        }else if(buffer[0] == 'F'){
            time_stamp_final = strtol(buffer+1, &endptr, 10);
            sprintf(executiontime_str,"tempo de execução: %ld\n",time_stamp_final-time_stamp_inicial);
            file_fp = open(file_path_term, O_WRONLY | O_CREAT | O_APPEND, 0666);
            write(file_fp, program_name,strlen(executiontime_str));
            write(file_fp,NEWLINE,1);
            write(file_fp,executiontime_str,strlen(executiontime_str));
            write(file_fp,NEWLINE,1);
            unlink(file_path_tmp);
        }
    }
} 
 
void client_handler(char* pid){
    char pid_CS[BUFF_SIZE];
    char pid_SC[BUFF_SIZE];

    sprintf(pid_CS, "CS_%s", pid); //Read
    sprintf(pid_SC, "SC_%s", pid); //Write
    printf("1 \n");
    writefd = open(pid_SC, O_WRONLY);
    printf("2 \n");
    readfd = open(pid_CS, O_RDONLY);
    printf("3 \n");
    message_protocol(pid);

    close(writefd);
    close(readfd);
}

int main()
{
    // create the named pipe
    server_fd = mkfifo(SERVER_PIPE, 0600);

    // open the pipe for reading
    int pipe_fd = open(SERVER_PIPE, O_RDONLY);

    //Main loop
    while (1)
    {
        //Create buffers to hold the names of client created "named pipes"
        char buffer[256];
        int bytes_read;

        while((bytes_read=myreadln(pipe_fd, buffer, 256))>0){
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork\n");
            }
            else if(pid == 0)// child process
            {
                printf("Client %s connected\n", buffer);
                client_handler(buffer);
            }
            else{
                break;
            }
        }
        continue;



        //Handle errors and empty messages
        if (bytes_read <= 0){
            perror("read");
            continue;
        }
    }
    return 0;
}
