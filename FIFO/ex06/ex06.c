// Write a C program where:
// 1. The parent process creates a FIFO named "myfifo".
// 2. The parent process sends a series of integers (5–10) to the child process through the FIFO.
// 3. The child process reads each integer, computes its square, and prints the result

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#define BUFSIZE 256

int square(int n){
    return  n * n;
}

int main(){
    pid_t pid;
    int nbRead, fdwrite, fdread, result;

    if ((mkfifo("myfifo", 0644)) < 0){
        perror("fifo");
        exit(1);
    }

    if ((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }

    if (pid != 0){    
        if ((fdwrite = open("myfifo", O_WRONLY)) < 0){
            perror("write");
            exit(1);
        }

        for (int i = 5; i <= 10; i++){
            write(fdwrite, &i, sizeof(int));
        }

        close(fdwrite);
    } else {
        fdread = open("myfifo", O_RDONLY);

        if(fdread < 0){
            perror("open");
            exit(1);
        }

        while (read(fdread, &nbRead, sizeof(nbRead)) > 0){
            result = square(nbRead);
            printf("Number: %d Square: %d\n", nbRead, result);
        }
        close(fdread);
    }
    unlink("myfifo");
    return 0;
}