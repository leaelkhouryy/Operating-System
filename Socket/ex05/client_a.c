// Write two programs — a client and a server — that communicate using sockets.
// The client reads an integer value entered by the user and sends it to the server through the socket. The server receives the integer, calculates its factorial, and sends the result back to the client. The client then displays the factorial received from the server.
// You are asked to implement the programs in three different ways:
// a) Using a local socket (UNIX domain socket) in connected mode with up to 5 simultaneous client connections.
// The socket file name is given as a parameter when running the programs.
// b) Using remote sockets (TCP/IP sockets) in connected mode with the same characteristics as part (a).
// The server should accept multiple client connections.
// c) Using remote sockets (UDP sockets) in non-connected mode.
// The client sends the integer to the server, and the server sends the factorial result back to the client without establishing a connection.

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <fcntl.h>
#define BUFSIZE 256

int main(int argc, char** argv){
    int socketfd, n, result;
    struct sockaddr_un server_addr;

    if (argc != 2){
        perror("Usage error");
        exit(1);
    }

    if ((socketfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0){
        perror("Socket creation");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, argv[1]);

    if ((connect(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0){
        perror("Connection error");
        exit(1);
    }

    printf("Enter an integer to calculate its factorial: \n");
    scanf("%d", &n);
    write(socketfd, &n, sizeof(n));

    bzero(&buf, BUFSIZE);
    n = read(socketfd, &result, sizeof(buf));
    if(n < 0){
        perror("Reading");
        exit(1);
    }
    printf("The received answer is: %d", result);

    close(socketfd);
    return 0;
}