#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFSIZE 256

int computeFactorial(int n){
    if (n <= 1){ return 1;}
    return n * computeFactorial(n - 1);
}

int main(int argc, char**argv){
    int socket_listening_fd, socket_connection_fd, n, result;
    struct sockaddr_in server_addr, client_addr;
    char buf[BUFSIZE];

    if(argc != 2){
        perror("Usage error");
        exit(1);
    }

    if((socket_listening_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket creation");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if((bind(socket_listening_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0){
        perror("Binding error");
        exit(1);
    }

    listen(socket_listening_fd, 5);

    socklen_t client_len = sizeof(client_addr);
    if((socket_connection_fd = accept(socket_listening_fd, (struct sockaddr*)&client_addr, &client_len)) < 0){
        perror("Accepting");
        exit(1);
    }

    recv(socket_connection_fd, &buf, sizeof(buf), 0);

    result = computeFactorial(n);
    send(socket_connection_fd, &result, sizeof(result), 0);

    close(socket_connection_fd);
    close(socket_listening_fd);
    return 0;
}