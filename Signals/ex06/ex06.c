// Write a program in which a process opens a file on which it works (for
// example: writes 5 times the string "abcdefghijkl" in the file once each second).
// In case of receipt of SIGINT signal, the process closes the file before exiting.

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int fd;

void handler(int sig){
    printf("Handler..\n");
    close(fd);
    exit(1);
}

int main(){
    int i =0;

    signal(SIGINT, handler);

    fd = open("Signal_ex06.txt", O_RDWR | O_APPEND, 0);

    for (i = 0; i < 5; i++){
        write(fd, "abcdefghijkl", 12);
        sleep(1);
    } 
    
    close(fd);
    printf("Process ended gracefully!");
    return 0;
}

