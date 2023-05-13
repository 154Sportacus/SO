#include "utils.h"

#define TEMPORARY "tmp/"
#define TERMINATED "term/"


int fd_write, fd_read;

void answer_status(char* pid){
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    long time_ms;
    long time_stamp_inicial;
    char execution_time[2*BUFF_SIZE];
    int bytesRead;
    char file_path[32];
    char buffer[2*BUFF_SIZE];
    char program_name[2*BUFF_SIZE];
    char *endptr;
    char* error_message;
    char pid_SC[BUFF_SIZE];
   
    close(fd_read);

    sprintf(pid_SC, "SC_%s", pid); //Write

    fd_write = open(pid_SC, O_WRONLY);
    
    if ((dir = opendir(TEMPORARY)) == NULL) {
        error_message = "Failed to open directory\n";
        write(2, error_message, strlen(error_message));
        write(fd_write, error_message, strlen(error_message));
        return;
    }

    while ((ent = readdir(dir)) != NULL) {

        sprintf(file_path,"%s%s",TEMPORARY,ent->d_name);
        //S_ISREG(st.st_mode) -> Macro to check if a file is a regular file
        if (stat(file_path, &st) == 0 && S_ISREG(st.st_mode)) {
            printf("%s\n", file_path);
            int file = open(file_path, O_RDONLY);
            if (file<0) {
                error_message = "Failed to open file\n";
                write(2,error_message,strlen(error_message));
                
            }
            while((bytesRead = myreadln(file, buffer, 2*BUFF_SIZE)) > 0) {
                if(buffer[0] == 'I'){
                    time_ms = get_long_timestamp();
                    time_stamp_inicial = strtol(buffer+1, &endptr, 10);
                    sprintf(execution_time, "Execution time: %ld\n", time_ms - time_stamp_inicial);
                    write(fd_write,execution_time, strlen(execution_time));
                    close(file);
                }else if(buffer[0] == 'N'){
                    sprintf(program_name,"Program name: %s\n",buffer+1);
                    write(fd_write,program_name, strlen(program_name));
                }else 
                    break;
            }
        }
    }
    printf("about to send new line ro clien \n\n");
    WR_NEWLINE(fd_write);
    close(fd_write);
}







void message_protocol(char* pid){
    char buffer[1024];
    int bytesRead;
    long time_stamp_inicial;
    long time_stamp_final;
    char executiontime_str[2*BUFF_SIZE];
    char program_name[2*BUFF_SIZE];
    char *endptr;
    int file_fp;
    char file_path_tmp[64];
    char file_path_term[64];
    sprintf(file_path_tmp,"%s%s",TEMPORARY,pid);
    sprintf(file_path_term,"%s%s",TERMINATED,pid);
    int i=0;
    int on=1;

    while((bytesRead = myreadln(fd_read,buffer,1024)) >= 0 && on){
        printf("Entrei pela %dª vez no while\n",i++);
        printf("Recebi o buffer: %s\n\n",buffer);
        if(bytesRead==0)
            continue;
        else if(strcmp(buffer,"status")==0){
            printf("Status is about to run\n");
            answer_status(pid);
            printf("Status finished\n");
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
            close(fd_read);
            printf("Recebeu F%s\n",buffer);
            time_stamp_final = strtol(buffer+1, &endptr, 10);
            sprintf(executiontime_str,"tempo de execução: %ld\n",time_stamp_final-time_stamp_inicial);
            file_fp = open(file_path_term, O_WRONLY | O_CREAT | O_APPEND, 0666);
            write(file_fp, program_name,strlen(program_name));
            write(file_fp,NEWLINE,1);
            write(file_fp,executiontime_str,strlen(executiontime_str));
            write(file_fp,NEWLINE,1);
            close(file_fp);
            unlink(file_path_tmp);
            on=0;
        }
    }
} 
 





void client_handler(char* pid){
    printf("1.buffer -> pid= %s\n",pid); 
    char pid_CS[BUFF_SIZE];

    sprintf(pid_CS, "CS_%s", pid); // 
    fd_read = open(pid_CS, O_RDONLY);
    message_protocol(pid);
    printf("2.buffer -> pid= %s\n",pid); 

}







int main()
{
    // create the named pipe
    if(mkfifo(SERVER_PIPE, 0666) < 0){
        printf("Error creating '%s'\n",SERVER_PIPE);
        return 1;
    }
    printf("Server pipe created\n");

    // open the pipe for reading
    int pipe_fd;
    if((pipe_fd=open(SERVER_PIPE, O_RDONLY,0))==-1){
        perror("Error: Could not open read pipe for server -> client");
        return -1;
    }
    printf("server pipe opened successfully\n");
    char buffer[256];
    int bytes_read;
    pid_t pid;
 
    //Main loop
    while (1){
        
        while((bytes_read=myreadln(pipe_fd, buffer, 256))>0){
            if (bytes_read==1){
                continue;
            }
            if ((pid=fork()) == -1){
                perror("fork\n");
            }
            else if(pid == 0)// child process
            {
                printf("Client %s connected\n", buffer);
                client_handler(buffer);
                printf("Client_handler processed the following buffer:/!\\%s\n", buffer);
            }
            else{
                break;
            }
        }
        continue;
    
    
    }
    return 0;
}
