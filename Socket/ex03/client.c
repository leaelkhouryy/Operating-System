// Write two programs ‐ a client and a server ‐ that communicate using a socket. The client
// sends a character string entered from the keyboard to the server via the socket created. The
// server reads the message received from the client and displays it on the screen. The server
// then sends back to the client the message "I got your message", which will be displayed by
// the client. You are asked to write this program using remote sockets in non‐connected mode.

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
    struct sockaddr_in server_addr;
    int sockfd, n;
    socklen_t server_addr_len;
    char msg[80];

    if(argc != 3){
        printf("Usage error\n");
        exit(1);
    }

    //Create socket
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        error("Socket creation failed");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr_len = sizeof(server_addr);

    //Send msg to server
    printf("Please enter your message: ");
    fgets(msg, sizeof(msg), stdin);
    sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&server_addr, server_addr_len);

    //Receive server response and print it
    n = recvfrom(sockfd, msg, sizeof(msg) - 1, 0, (struct sockaddr*)&server_addr, server_addr_len);
    if(n < 0){
        error("Reading message");
    }
    msg[n] = '\0';
    printf("Server reply: %s\n", msg);

    close(sockfd);
    return 0;
}

void error(char* msg){
    perror(msg);
    exit(1);
}