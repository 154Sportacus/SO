#ifndef UTILS_H
#define UTILS_H

//header files
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

//macros
#define SERVER_PIPE "ClientServerConnection"
#define NEWLINE "\n"
#define BUFF_SIZE 256
#define WR_NEWLINE(fd) write(fd,"\n",2);

//function declarations
int myreadln(int fd, char *line, int size);
void get_timestamp(char* time_stamp);
long get_long_timestamp();

#endif 
