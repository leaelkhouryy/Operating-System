// Write two programs ‐ a client and a server ‐ that communicate using a socket. The client 
// sends a character string entered from the keyboard to the server via the socket created. The 
// server reads the message received from the client and displays it on the screen. The server 
// then sends back to the client the message "I got your message", which will be displayed by 
// the client. You are asked to write this program using a remote sockets in connected mode

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
    char msg[80];

    if(argc != 3){
        printf("Usage error");
        exit(1);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        error("Socket creation failed");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if((connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0){
        error("Connecting");
    }

    printf("Please enter your message: ");
    fgets(msg, sizeof(msg), stdin);
    send(sockfd, msg, strlen(msg), 0);

    n = recv(sockfd, msg, sizeof(msg) - 1, 0);
    if(n < 0){
        error("Receive failed");
    }
    msg[n] = '\0';
    write(1, msg, n);

    close(sockfd);
    return 0;
}

void error(char *msg){
    perror(msg);
    exit(1);
}