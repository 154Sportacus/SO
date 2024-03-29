#include "utils.h"


int myreadln(int fd, char *line, int size) {
    int i = 0, n;
    char character;



    while (i<size-1) {  
        if ((n = read(fd, &character, 1)) == 1) {
            if (character == '\n' || character == '\0' || character == EOF)
                break;
            line[i++] = character;
        } else if (n == 0) {
            continue;
        } else {
            return -1;
        }
    }

    if(i>0)
    line[i] = '\0';


    return i;
}

long get_long_timestamp(){
    struct timeval time_value;

    if (gettimeofday(&time_value, NULL) == -1) {
            perror("gettimeofday() error");
            exit(EXIT_FAILURE);
        }
    
    return (long)(time_value.tv_sec * 1000LL + time_value.tv_usec / 1000LL);
}

long return_timestamp(char *time_stamp){
    long time_ms;
    time_ms = get_long_timestamp();
    sprintf(time_stamp, "%ld\n", time_ms);
    return time_ms;
}


void get_timestamp(char* time_stamp){
    long time_ms;
    time_ms = get_long_timestamp();
    sprintf(time_stamp, "%ld\n", time_ms);
}




