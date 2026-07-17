#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#define BUFSIZE 256

int main(){
    int nbRead, file;
    char buf[BUFSIZE];

    if((file = open("/tmp/myfifo", O_RDONLY)) < 0){
        perror("read");
        exit(1);
    }

    while((nbRead = read(file, buf, sizeof(buf))) > 0){
        write(STDOUT_FILENO, buf, nbRead);
    }

    close(file);
    return 0;
}