// Write a C program where:
// The parent process creates a FIFO named "numbers_fifo".
// The parent sends a series of 5 integers (10, 20, 30, 40, 50) to the child process through the FIFO.
// The child process reads each number and:
//      computes its square
//      prints the result

#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <fcntl.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>
#define BUFSIZE 256

int main(){
    int fdread, fdwrite, Nr;
    pid_t pid;
    int nb [5] = {10, 20, 30, 40, 50};

    if(mkfifo("numbers_fifo", 0644) < 0 && errno != EEXIST){
        perror("FIFO Creation");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid < 0){
        perror("Fork");
        exit(EXIT_FAILURE);
    }
    
    if(pid != 0){
        fdwrite = open("numbers_fifo", O_WRONLY);
        if(fdwrite < 0){
            perror("Opening");
            exit(EXIT_FAILURE);
        }
        for(int i = 0; i < 5; i++){
            write(fdwrite, &nb[i], sizeof(int));
        }
        close(fdwrite);
        wait(NULL);
        unlink("numbers_fifo");
    } 
    else{
        fdread = open("numbers_fifo", O_RDONLY);
        if(fdread < 0){
            perror("Opening");
            exit(EXIT_FAILURE);
        }
        while((Nr = read(fdread, &nb[0], sizeof(int))) > 0){
            int val = nb[0];
            printf("%d^2 = %d\n", val, val * val);
        }
        close(fdread);
    }
    return 0;
}