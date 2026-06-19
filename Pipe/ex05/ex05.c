// Write a program that creates a ring of 3 processes connected by pipes:
// The first process should ask the user for a string and then send it to the second process
// The second process should reverse the string and then send it to the third process
// The third process should convert the string to uppercase and send it back to the first process
// When the first process gets the processed string, it should display it to the terminal.
// When this is done, all 3 processes should terminate.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#define BUFFER_SIZE 256

void reverseLine(char *buffer,int n){
    int i;
    char tmp;
    for(i = 0; i < n/2; i++){
        tmp = buffer[i];
        buffer[i] = buffer[n - i - 1];
        buffer[n - i - 1] = tmp;
    }
}

int main(){
    pid_t pid1, pid2;
    int fd1[2], fd2[2], fd3[2];
    char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE], buf3[BUFFER_SIZE], buf4[BUFFER_SIZE];
    ssize_t n;

    if ((pipe(fd1) < 0) || (pipe(fd2) < 0) || (pipe(fd3) < 0)){
        perror("Pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if(pid1 < 0){
        perror("Fork 1");
        exit(EXIT_FAILURE);
    }

    if (pid1 != 0){
        close(fd1[0]);
        close(fd3[1]);
        while((n = read(STDIN_FILENO, buf1, BUFFER_SIZE)) > 0){
            if (write(fd1[1], buf1, n) != n){
                perror("Writing to pipe1");
                exit(EXIT_FAILURE);
            }
        }
        close(fd1[1]);
        while((n = read(fd3[0], buf4, BUFFER_SIZE)) > 0){
            if (write(STDOUT_FILENO, buf4, n) != n){
                perror("Writing to STDOUT");
                exit(EXIT_FAILURE);
            }
        }
        close(fd3[0]);
    }
    else {
        pid2 = fork();
        if(pid2 < 0){
           perror("Fork 2");
           exit(EXIT_FAILURE);
        }
        if (pid2 != 0){
            close(fd1[1]);
            close(fd2[0]);
            close(fd3[0]);
            close(fd3[1]);
            while((n = read(fd1[0], buf2, BUFFER_SIZE)) > 0){
                reverseLine(buf2, n);
                if(write(fd2[1], buf2, n) != n){
                    perror("Writing to pipe2");
                    exit(EXIT_FAILURE);
                }
            }
            close(fd1[0]);
            close(fd2[1]);
        }
        else {
            close(fd2[1]);
            close(fd3[0]);
            close(fd1[0]);
            close(fd1[1]);
            while((n = read(fd2[0], buf3, BUFFER_SIZE)) > 0){
                for(int i = 0; i < n; i++){
                    buf3[i] = toupper(buf3[i]);
                }
                if(write(fd3[1], buf3, n) != n){
                    perror("Writing to pipe3");
                    exit(EXIT_FAILURE);
                }
            }
            close(fd2[0]);
            close(fd3[1]);
        }
    }
    return 0;
}
