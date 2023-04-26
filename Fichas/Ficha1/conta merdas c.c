/*
Red Wacky League Antlez Broke the Stereo Neon Tide Bring Back Honesty Coalition Feedback Hand of Aces Keep Going Captain Let’s Pretend Lost State of Dance Paper Taxis Lunar Road Up Down Strange All and I Neon Sheep Eve Hornby Faye Bradley AJ Wilde Michael Rice Dion Watts Matthew Appleyard John Ashurst Lauren Swales Zoe Angus Jaspreet Singh Emma Matthews Nicola Brown Leanne Pickering Victoria Davies Rachel Burnside Gil Parker Freya Watson Alisha Watts James Pearson Jacob Sotheran Darley Beth Lowery Jasmine Hewitt Chloe Gibson Molly Farquhar Lewis Murphy Abbie Coulson Nick Davies Harvey Parker Kyran Williamson Michael Anderson Bethany Murray Sophie Hamilton Amy Wilkins Emma Simpson Liam Wales Jacob Bartram Alex Hooks Rebecca Miller Caitlin Miller Sean McCloskey Dominic Parker Abbey Sharpe Elena Larkin Rebecca Simpson Nick Dixon Abbie Farrelly Liam Grieves Casey Smith Liam Downing Ben Wignall Elizabeth Hann Danielle Walker Lauren Glen James Johnson Ben Ervine Kate Burton James Hudson Daniel Mayes Matthew Kitching Josh Bennett Evolution Dreams.

*/
#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdio.h> /* cus printf and fprintf */



#define BUFFER_SIZE 1024 //Tamanho do buffer size 



int main(int argc, char **argv){
     int result=0;
     
     ssize_t bytes_read;
     char buffer[BUFFER_SIZE];

    //open origin file
    ssize_t fd_origin = open(argv[1], O_RDONLY);

    if(fd_origin == -1){
        perror("Deu merda a abrir");
        return -1;
    }
    else{
        char *filename = argv[1];  // Get the filename from the command line argument
        char *dot = strrchr(filename, '.');  // Find the last occurrence of the '.' character
         if (dot) {
             *dot = '\0';  // Replace the '.' character with a null terminator
        }
        printf("Ficheiro \"%s\" aberto corretamente!\n", filename);
    }

    while((bytes_read = read(fd_origin,buffer,BUFFER_SIZE)) > 0){
        if(bytes_read > 0){
            result += 0 + (bytes_read);
        }
        else if(bytes_read == 0){ 
            for(int i = 0;buffer[i]!='\0'; i++){
            result++;
            }
        }
        else{
            result = -1;
            break;
        }    
    }
    
    printf("Answer: %d\n", result);


    return 0;
}