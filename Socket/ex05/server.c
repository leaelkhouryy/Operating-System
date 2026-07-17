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
    int socket_listening_fd, socket_connection_fd, n;
    struct sockaddr_in server_addr, client_addr;
    char buf[BUFSIZE];

    if (argc != 3){
        perror("Usage error");
        exit(1);
    }

    if ((socket_listening_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket creation failed");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if ((bind(socket_listening_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0){
        perror("Binding error");
        exit(1);
    }

    if ((listen(socket_listening_fd, 5)) < 0){
        perror("Failed to listen");
        exit(1);
    }

    socklen_t client_addr_len = sizeof(client_addr);
    socket_connection_fd = accept(socket_listening_fd, (struct sockaddr*)& client_addr, &client_addr_len);
    if (socket_connection_fd < 0){
        perror("Accepting connection failure");
        exit(1);
    }

    n = recv(socket_connection_fd, buf, sizeof(buf), 0);
    if(n < 0){
        perror("Reading failure");
        exit(1);
    }

    buf[n] = '\0';
    send(socket_connection_fd, buf, n, 0);

    printf("Program terminated gracefully..");

    close(socket_connection_fd);
    close(socket_listening_fd);

    return 0;
}