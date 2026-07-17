// Write a TCP server where:
// 1. The server creates a socket.
// 2. Binds it to a port.
// 3. Listens for client connections.
// 4. Accepts one client.
// 5. Receives a message from the client.
// 6. Sends the same message back (echo).
// 7. Closes sockets properly.

// Client:
// 1. Connects to the server.
// 2. Sends a message.
// 3. Receives the echoed message.
// 4. Prints it.

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFSIZE 1024

int main(int argc, char**argv){
    int socketfd, n;
    struct sockaddr_in server_addr;
    char buf[BUFSIZE];

    if (argc != 3){
        perror("Usage error");
        exit(1);
    }

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket creation failed");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if ((connect(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0){
        perror("Connecting error");
        exit(1);
    }

    strcpy(buf, "Hello");
    send(socketfd, buf, strlen(buf), 0);

    n = recv(socketfd, buf, sizeof(buf), 0);
    if (n < 0){
        perror("Error reading");
        exit(1);
    }

    buf[n] = '\0';
    write(1, buf, n);

    close(socketfd);

    return 0;
}