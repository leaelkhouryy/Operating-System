#include<stdio.h> 
#include<sys/types.h>
#include<unistd.h> 
#include<fcntl.h>
#include<string.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#define BUFSIZE 256

int main(){
    int fdread, fdwrite;
    char buf[BUFSIZE];
    mode_t mode = 0644;
    int Nr, N, n, result;

    if(mkfifo("/tmp/fifo2", mode) < 0 && errno != EEXIST){
        perror("FIFO Creation");
        exit(EXIT_FAILURE);
    }

    printf("Server listening for client connection...\n");
    fdread = open("/tmp/fifo1", O_RDONLY);
    fdwrite = open("/tmp/fifo2", O_WRONLY);

    if(fdread < 0 || fdwrite < 0){
        perror("Opening");
        exit(EXIT_FAILURE);
    }

    //Initialize result, and start reading from buffer to perform calculations
    result = 0;
    Nr = read(fdread, buf, sizeof(buf));
    if (Nr < 0){
        perror("Reading");
        exit(EXIT_FAILURE);
    }
    N = atoi(buf);
    for(int i = 0; i < N; i++){
        Nr = read(fdread, buf, sizeof(buf));
        if(Nr < 0){
            perror("Reading");
            exit(EXIT_FAILURE);
        }
        n = atoi(buf);
        result += n;
    }

    //Send back result
    sprintf(buf, "%d", result);
    write(fdwrite, buf, strlen(buf) + 1);

    close(fdread);
    close(fdwrite);
    unlink("/tmp/fifo2");
    return 0;
}