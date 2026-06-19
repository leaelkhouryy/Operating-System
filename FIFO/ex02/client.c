// Create 2 programs: one a server and the other one a client. A client sends its 
// pid and the pathname of a file to the server. The server can accept the request of many 
// clients. It opens the requested file and sends its content to the correspondent client. If 
// the server cannot open the file, it sends an error message to the client. 

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
    char filename [20];
};

void Copying(int fdin, int fdout, char*buf){ 
    int nbRead; 
    while((nbRead = read(fdin,buf,BUFSIZE)) > 0){ 
        write(fdout, buf, nbRead); 
    } 
} 

int main(int argc, char** argv){
    char buf[BUFSIZE];
    int fdread, fdwrite;
    pid_t pid;
    char fifoname[20], filename[20];
    struct Message msg;

    if(argc != 2){
        perror("Usage error");
        exit(EXIT_FAILURE);
    }

    strcpy(filename, argv[1]);
    sprintf(fifoname, "/tmp/fifo%d", getpid());
    if(mkfifo(fifoname, 0644) < 0 && errno != EEXIST){
        perror("FIFO Creation");
        exit(EXIT_FAILURE);
    }
    printf("My fifoname is: %s\n", fifoname);

    //Open fifo1 and write the pid and filename
    fdwrite = open("/tmp/fifo.serv", O_WRONLY);
    if(fdwrite < 0){
        perror("Opening");
        exit(EXIT_FAILURE);
    }
    msg.pid = getpid();
    strcpy(msg.filename, filename);
    write(fdwrite, &msg, sizeof(msg));
    close(fdwrite);

    //Open fifo2 to copy the server's answer to STDOUT
    fdread = open(fifoname, O_RDONLY);
    if(fdread < 0){
        perror("Opening");
        exit(EXIT_FAILURE);
    }
    Copying(fdread, STDOUT_FILENO, buf);
    
    close(fdread);
    unlink(fifoname);
    return 0;
}