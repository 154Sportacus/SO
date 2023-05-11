#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>


int myreadln(int fd, char *line, int size);
void get_timestamp(char* time_stamp);
long get_long_timestamp();

#endif 
