// Write a C program that contains a producer process (reads from the stdin and 
// write to a pipe) and a consumer process that makes capital letter the small letter (reads 
// from the pipe and writes to the stdout). 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#define BUFFER_SIZE 256

int main(){
    int fd[2];
    pid_t pid;
    char buf[BUFFER_SIZE];
    ssize_t n;

    if (pipe(fd) != 0) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid != 0) {
        close(fd[0]);
        while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
            if (write(fd[1], buf, n) == -1) {
                perror("write to pipe failed");
                exit(EXIT_FAILURE);
            }        
        }
        close(fd[1]);
        wait(NULL);
    } else {
        close(fd[1]);
        while ((n = read(fd[0], buf, sizeof(buf))) > 0) {
            for (ssize_t i = 0; i < n; i++) {
                buf[i] = tolower((unsigned char)buf[i]);
            }
            if (write(STDOUT_FILENO, buf, n) == -1) {
                perror("write to stdout failed");
                exit(EXIT_FAILURE);
            }
        }
        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}