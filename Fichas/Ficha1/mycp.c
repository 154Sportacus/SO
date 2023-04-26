#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdio.h> /* cus printf and fprintf */



#define BUFFER_SIZE 1024 //Tamanho do buffer size 

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Error:Wrong number of arguments\n");
        printf("Sintaxe: mycp file_origin_directory file_destination_directory\n");
    }

    //Opening the origin file for reading
    int fd_origin = open(argv[1], O_RDONLY);
    if (fd_origin == -1){
        printf("Error: The file wasn't successfully opened\n");
        return 1;
    }
    
    //Opening the destination file for writing
    int fd_destination = open(argv[2], O_WRONLY | O_CREAT, 0666);
    if (fd_destination == -1){
        printf("Error: The file wasn't successfully opened\n");
        close(fd_origin);
        return 1;
    }

    ssize_t  bytes_read, bytes_written;
    off_t pos = 0;
    char buffer[BUFFER_SIZE];


    while((bytes_read = read(fd_origin, buffer, BUFFER_SIZE)) > 0){
        bytes_written = write(fd_destination, buffer, bytes_read);
        if(bytes_written != bytes_read){
            printf("Error: It was not possible to write all the content from the origin file to the destination file\n");
            close(fd_origin);
            close(fd_destination);
            return 1;
        }
    }

    close(fd_origin);
    close(fd_destination);

    return 0;

}