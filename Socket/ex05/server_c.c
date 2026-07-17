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
    int socketfd, n, result;
    struct sockaddr_in server_addr, client_addr;
    char buf[BUFSIZE];

    if(argc != 2){
        perror("Usage error");
        exit(1);
    }

    if((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Socket creation");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t server_addr_len = sizeof(server_addr);
    socklen_t client_addr_len = sizeof(client_addr);

    if((bind(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0){
        perror("Binding error");
        exit(1);
    }
    
    while(1){
        n = recvfrom(socketfd, &buf, sizeof(buf), 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if(n < 0){
            perror("Receiving");
            exit(1);
        }

        result = computeFactorial(n);

        sendto(socketfd, &result, sizeof(result), 0, (struct sockaddr*)&client_addr, &client_addr+len);
    }

    close(socketfd);
    return 0;
}