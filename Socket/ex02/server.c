#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include <arpa/inet.h>

void error(char *);

int main(int argc, char** argv){
    struct sockaddr_in server_addr, client_addr;
    int socket_listener_fd, socket_connection_fd, client_addr_len, n;
    char msg[80];

    if(argc != 3){
        printf("Usage error");
        exit(1);
    }

    if((socket_listener_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        error("Socket creation failed");
    }

    //bzero(&server_addr, sizeof(server_addr));
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(bind(socket_listener_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        error("Bind");
    }

    if((listen(socket_listener_fd, 5)) < 0){
        error("Listening");
    }

    client_addr_len = sizeof(client_addr);
    socket_connection_fd = accept(socket_listener_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(socket_connection_fd < 0){
        error("Accepting");
    }
    
    n = recv(socket_connection_fd, msg, sizeof(msg) - 1, 0);
    if(n < 0){
        error("Receive failed");
    }

    printf("Connection established...");

    msg[n] = '\0';
    write(1, msg, n);

    send(socket_connection_fd, "I got your messge", 19, 0);

    close(socket_connection_fd);
    close(socket_listener_fd);
    return 0;
}

void error(char *msg){ 
    perror(msg); 
    exit(1); 
} 