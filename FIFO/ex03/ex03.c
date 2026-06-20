// Write a C program where:
// 1. The parent process creates a FIFO named "myfifo".
// 2. The parent sends a series of numbers (1–5) to the child process through the
// FIFO.
// 3. The child process reads each number, computes its factorial, and prints the
// result.

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

int computeFactorial(int n){
    if (n <= 1){
        return 1;
    }
    return n * computeFactorial(n - 1);
}

int main(){
    int i, fdread, fdwrite, Nr;
    pid_t pid;

    if(mkfifo("myfifo", 0644) < 0 && errno != EEXIST){
        perror("FIFO Creation");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid < 0){
        perror("Fork");
        exit(EXIT_FAILURE);
    }

    if(pid != 0){
        fdwrite = open("myfifo", O_WRONLY);
        if(fdwrite < 0){
            perror("Opening");
            exit(EXIT_FAILURE);
        }
        
        for(i = 1; i <= 5; i++){
            write(fdwrite, &i, sizeof(int));
        }   
        close(fdwrite);
        wait(NULL);
        unlink("myfifo");
    }
    else {
        fdread = open("myfifo", O_RDONLY);
        if(fdread < 0){
            perror("Opening");
            exit(EXIT_FAILURE);
        }
        while ((Nr = read(fdread, &i, sizeof(int))) > 0){
            printf("%d! = %d\n", i, computeFactorial(i));        
        }
        close(fdread);
    }
    return 0;
}