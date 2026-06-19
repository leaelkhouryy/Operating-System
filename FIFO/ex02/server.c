#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <fcntl.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#define BUFSIZE 256

struct Message{
    pid_t pid;
    char filename[20];
};

void Copying(int fdin, int fdout, char*buf){ 
    int nbRead; 
    while((nbRead = read(fdin,buf,BUFSIZE)) > 0){ 
        write(fdout, buf, nbRead); 
    } 
}

int main(int argc, char** argv){
    char buf[BUFSIZE];
    int fdread, fdwrite, Nr, fd;
    pid_t pid;
    char fifoname[20], filename[20];
    struct Message msg;
    
    if(mkfifo("/tmp/fifo.serv", 0644) < 0 && errno != EEXIST){
        perror("FIFO Creation");
        exit(EXIT_FAILURE);
    }
    fdread = open("/tmp/fifo.serv", O_RDONLY);
    if(fdread < 0){
        perror("Opening");
        exit(EXIT_FAILURE);
    }
    while(1){
        Nr = read(fdread, &msg, sizeof(msg));

        if(Nr <= 0 || Nr != sizeof(msg)){
            continue;
        }
        pid = msg.pid;
        strcpy(filename, msg.filename);
        sprintf(fifoname, "/tmp/fifo.%d", pid);
        printf("Opening %s...\n", fifoname);

        fdwrite = open(fifoname, O_WRONLY);
        if(fdwrite < 0){
            perror("Opening");
            continue;
        }
        fd = open(filename, O_RDONLY);
        if(fd < 0){
            write(fdwrite, "File not found\n", 15);
            close(fdwrite);
            continue;
        }
        Copying(fd, fdwrite, buf);
        close(fd);
        close(fdwrite);
    }
    
    close(fdread);
    unlink("/tmp/fifo.serv");
    return 0;
}