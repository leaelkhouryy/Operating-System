// Write two C programs, writer.c and reader.c, that communicate through a named pipe (FIFO) instead of an anonymous pipe.
// writer.c:
// Creates a FIFO at path "/tmp/myfifo" using mkfifo() if it doesn't already exist (check the return value — ignore the specific error if it already exists, but handle/report any other error).
// 1. Opens the FIFO for writing only.
// 2. Reads lines of text from stdin (loop until EOF) and writes each line into the FIFO as-is.
// 3. Closes the FIFO and exits when stdin hits EOF.

// reader.c:
// 1. Opens the same FIFO path for reading only.
// 2. Reads from the FIFO in a loop until EOF (i.e., until the writer closes its end) and prints everything it receives to stdout.
// 3. Closes the FIFO and exits.

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#define BUFSIZE 256

int main(){
    int fdwrite, nbRead;
    char buf[BUFSIZE];

    if((mkfifo("/tmp/myfifo", 0644)) < 0 && errno != EEXIST){
        perror("fifo");
        exit(1);
    }

    if((fdwrite = open("/tmp/myfifo", O_WRONLY)) < 0){
        perror("write");
        exit(1);
    }

    while((nbRead = read(STDIN_FILENO, buf, BUFSIZE)) > 0){
        write(fdwrite, buf, nbRead);
    }
    close(fdwrite);
}