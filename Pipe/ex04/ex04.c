// Write a C program that is equivalent to  the shell command 
// pc aux | grep root | wc -l

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#define BUFFER_SIZE 256

int main (){
    pid_t pid1, pid2;
    int fd1[2], fd2[2];

    if ((pipe(fd1) < 0) || (pipe(fd2) < 0)){
        perror("Pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 < 0){
        perror("Fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 != 0){
        close(fd1[0]);
        dup2(fd[1], 1);
        close(fd1[1]);
        execlp("ps", "ps", "aux", NULL);
        perror("Error");
    } 
    else {
        pid2 = fork();
        if (pid2 != 0){
            perror("Fork");
            exit(EXIT_FAILURE);
        }

        if(pid2 != 0){
            close(fd1[1]);
            close(fd2[0]);
            dup2(fd1[0], 0);
            dup2(fd2[1], 1);
            close(fd1[0]);
            close(fd2[1]);
            execlp("grep", "grep", "root", NULL);
            perror("Error");
        }
        else {
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[1]);
            dup2(fd2[0], 0);
            close(fd2[1]);
            execlp("wc", "wc", "-l", NULL);
            perror("Error");
        }
    }
    return 0;
}