#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define BUFSIZE 256

int main(int argc, char**argv){
    int socketfd, n, result;
    struct sockaddr_in server_addr;
    char buf[BUFSIZE];

    if(argc != 3){
        perror("Usage error");
        exit(1);
    }

    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket creation");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if((connect(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0){
        perror("Connection");
        exit(1);
    }

    printf("Enter an integer to compute its factorial: \n");
    scanf("%d", &n);
    write(socketfd, &n, sizeof(n));

    read(socketfd, &result, sizeof(result));
    printf("The factorial of %d is: %d", n, result);
}
