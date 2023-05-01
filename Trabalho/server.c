#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "myreadln.c"

#define SERVER_PIPE "ClientServerConnection"

int main()
{
    // create the named pipe
    int fd = mkfifo(SERVER_PIPE, 0600);

    // open the pipe for reading
    int pipe_fd = open(SERVER_PIPE, O_RDONLY);

    //Main loop
    while (1)
    {
        //Create buffers to hold the names of client created "named pipes"
        char buffer[256];
        int bytes_read;

        while(bytes_read=myreadln(pipe_fd, buffer, 256)){
            if(buffer[0] == 'S'){
               //falta isto
            }  
            else if(buffer[0] == 'C'){
                //falta isto
            }

        }



        //Handle errors and empty messages
        if (bytes_read <= 0){
            perror("read");
            continue;
        }

        //Print the message to stdout
        printf("Received message: %s\n", buffer);

        //Fork a new process to handle the client
        pid_t pid = fork();
        if (pid == -1)
        {
            exit(1);
        }
        else if(pid == 0)// child process
        {
            //Open a new pipe for writing to the client
            open(buf_RD, O_RDONLY);

            //Send a message to the client
            

            //Close the pipe to the client
            

            //Exit the child process
           
        }

    // close the pipe from the server
    

    // remove the named pipe
    

    return 0;
}
