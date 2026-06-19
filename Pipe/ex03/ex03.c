// Write a C program that creates 2 processes A and B that can communicate via
// a pipe. The process "A" open a file given by argument to the program and transfers the
// content of the file to process B via the pipe. The process "B" write the content of the pipe
// to a second file given also by argument. Verify if the first file have the default permissions
// 0644.  
 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#define BUFFER_SIZE 256
 
int main(int argc, char** argv){
    int fd[2];
    pid_t pid;
    char buf1[BUFFER_SIZE];
    char buf2[BUFFER_SIZE];
    int fd1, fd2;
    char* fsrc;
    char* fdest;
    ssize_t n;
 
    if (argc != 3) {
        fprintf(stderr, "Usage error");
        exit(EXIT_FAILURE);
    }
 
    fsrc = argv[1];
    fdest = argv[2];
 
    if (pipe(fd) < 0){
        perror("Pipe error");
        exit(EXIT_FAILURE);
    }
 
    pid = fork();
    if (pid < 0){
        perror("Fork error");
        exit(EXIT_FAILURE);
    }

    if (pid != 0){
        close (fd[0]);
 
        // printf("DEBUG: trying to open '%s'\n", fsrc);
        // printf("PARENT: reading file\n");
        fd1 = open (fsrc, O_RDONLY);
        if (fd1 < 0) {
                perror (fsrc); //perror("Opening file");
                exit(EXIT_FAILURE);
        }
 
        while((n = read(fd1, buf1, BUFFER_SIZE)) > 0){
            if (write(fd[1], buf1, n) != n) {
                perror("write to pipe");
                exit(EXIT_FAILURE);
            }
        }
 
        //copy(fd1, fd[1], buf1);
 
        close(fd[1]);
        close(fd1);
        wait (NULL);
 
    } else {
        close(fd[1]);
 
        // printf("DEBUG: trying to open '%s'\n", fdest);
        // printf("CHILD: writing file\n");
        fd2 = open(fdest, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd2 < 0){
            perror(fdest); //perror("Opening file");
            exit(EXIT_FAILURE);
        }
 
        while((n = read(fd[0], buf2, BUFFER_SIZE)) > 0){
            if (write(fd2, buf2, n) != n) {
                perror("write to file");
                exit(EXIT_FAILURE);
            }
        }
 
        close(fd[0]);
        close(fd2);
        exit (0);
    }
    return 0;
}