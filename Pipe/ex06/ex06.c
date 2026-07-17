// Write a C program that simulates the following shell command using system
// calls only (no shell execution):
//  cat input.txt | grep error > result.txt
// The program should take into consideration the below:
// 1. The parent process executes the cat command and writes its output to the pipe.
// 2. The child process executes the grep command to search for the word "error" and
// writes the result into result.txt.
// 3. Use dup2() to redirect standard input and output appropriately.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#define BUFFER_SIZE 256

int main(){
    pid_t pid;
    char buf[BUFFER_SIZE];
    int fd[2], fd;

    if (pipe(fd) < 0){
        perror("Pipe creation error..");
        exit(1);
    }

    if (pid = fork() < 0){
        perror("Fork error..");
        exit(1);
    }

    if (pid != 0){
        close (fd[0]);
        dup2(fd[1], 1);
        close (fd[1]);
        execlp("cat", "cat", "input.txt", NULL);
        perror("Error executing cat"); //should not execute unless there's an error
    }
    else {
        close (fd[1]);
        fd = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0){
            perror("Opening file error..");
            exit(1);
        }
        dup2(fd[0], 0);
        dup2(fd, "result.txt");
        close (fd);
        close (fd[0]);
        execlp("grep", "grep", "error", NULL);
    }

    return 0;
}