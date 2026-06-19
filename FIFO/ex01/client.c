// Write 2 programs in C, one is a client and the other is
// a server. Both communicate using named pipes. The
// client sends many integers to the server, and the
// server calculates their sum and returns the result to
// the client.

#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <fcntl.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#define BUFSIZE 256

int main(){
    int fdwrite, fdread;
    char buf[BUFSIZE];
    mode_t mode = 0644;
    int N, n, result;

    if (mkfifo("/tmp/fifo1", mode) < 0 && errno != EEXIST){
        perror("FIFO Creation");
        exit(EXIT_FAILURE);
    }

    fdwrite = open("/tmp/fifo1", O_WRONLY);
    fdread = open("/tmp/fifo2", O_RDONLY);
    
    if ((fdwrite < 0) || (fdread < 0)){
        perror("Opening");
        exit(EXIT_FAILURE);
    }

    //User inputs the digits
    printf("Please enter the nb of digits you will enter: \n");
    scanf("%i", &N);
    sprintf(buf, "%d", N);
    write(fdwrite, buf, strlen(buf) + 1);

    printf("Enter %d digits: \n", N);
    for (int i = 0; i < N; i++){
        scanf("%i", &n);
        sprintf(buf, "%d", n);
        write (fdwrite, buf, strlen(buf) + 1);
    }

    //Display result
    read(fdread, buf, sizeof(buf));
    result = atoi(buf);
    printf("The sum of the entered integers is: %i\n", result);
    
    close(fdread);
    close(fdwrite);
    unlink("/tmp/fifo1");
    return 0;
}