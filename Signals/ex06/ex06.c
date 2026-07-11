// Write a program in which a process opens a file on which it works (for
// example: writes 5 times the string "abcdefghijkl" in the file once each second).
// In case of receipt of SIGINT signal, the process closes the file before exiting.

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

void handler(iny sig){
    printf("Handler..\n");
    close(fd);
    exit(1);
}

int main(){
    int fd;
    int i =0;

    signal(SIGINT, handle);

    fd = open("Signal_ex06.txt", O_RDWR | O_APPEND, 0);

    for (i < 5){
        write(fd, "abcdefghijkl", 12);
        i++;
        sleep(1);
    } 
    
    close(fd);
    printf("Process ended gracefully!");
    return 0;
}

