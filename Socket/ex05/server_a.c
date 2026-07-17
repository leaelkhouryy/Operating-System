#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <fcntl.h>
#define BUFSIZE 256

int computeFactorial(int n){
    if (n <= 1){ return 1; }
    return n * computeFactorial(n - 1);
}

int main(int argc, char**argv){
    int socket_listening_fd, socket_connection_fd, n;
    struct sockaddr_un server_addr, client_addr;
    char buf[BUFSIZE];

    if(argc != 2){
        perror("Usage error");
        exit(1);
    }

    if((socket_listening_fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0){
        perror("Socket creation");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, argv[1]);

    unlink(argv[1]);

    if((bind(socket_listening_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0){
        perror("Binding error");
        exit(1);
    }

    if((listen(socket_listening_fd, 5)) < 0){
        perror("Listening");
        exit(1);
    }

    socklen_t client_len = sizeof(client_addr);
    if((socket_connection_fd = accept(socket_listening_fd, (struct sockaddr*)&client_addr, client_len)) < 0){
        perror("Accepting failed");
        exit(1);
    }

    n = read(socket_connection_fd, &buf, sizeof(buf));
    if(n < 0){
        perror("Reading error");
        exit(1);
    }

    result = computeFactorial(n);
    printf("The factorial of %d is: %d\n", n, result);
    write(socket_connection_fd, &result, sizeof(result));

    close(socket_listening_fd);
    close(socket_connection_fd);
    return 0;
}