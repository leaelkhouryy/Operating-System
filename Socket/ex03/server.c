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
    int socket_listener_fd, n;
    socklen_t server_addr_len, client_addr_len;
    char msg[80];

    if(argc != 2){
        printf("Usage error\n");
        exit(1);
    }

    if((socket_listener_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        error("Socket creation failed");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr_len = sizeof(server_addr);
    client_addr_len = sizeof(client_addr);

    if((bind(socket_listener_fd, (struct sockaddr*)&server_addr, server_addr_len)) < 0){
        error("Binding failed");
    }

    while(1){
        n = recvfrom(socket_listener_fd, msg, sizeof(msg) - 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if(n < 0){
            error("Receiving msg has failed");
        }

        msg[n] = '\0';
        printf("The received message is: %s\n", msg);

        memset(msg, 0, sizeof(msg));
        strcpy(msg, "I got your message");
        sendto(socket_listener_fd, msg, strlen(msg), 0, (struct sockaddr*)&client_addr, &client_addr_len);
    }

    return 0;
}

void error(char*msg){
    perror(msg);
    exit(1);
}