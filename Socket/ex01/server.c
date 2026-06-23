#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>

void error(char *); 

int main(int argc, char** argv){
    struct sockaddr_un server_addr, client_addr;
    int socket_listener_fd, socket_connection_fd, server_addr_len, client_addr_len, n;
    char msg[80];

    if(argc != 2){
        printf("Usage: %s socket_name\n", argv[0]);
        exit(1);
    }

    //Create socket
    if((socket_listener_fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0){
        error("Creating Socket");
    }
    
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, argv[1]);
    server_addr_len = sizeof(server_addr.sun_family) + strlen(server_addr.sun_path);

    //Bind
    if((bind(socket_listener_fd, (struct sockaddr *)&server_addr, server_addr_len)) < 0){
        error("Binding");
    }
    //Listen
    if((listen(socket_listener_fd, 5) < 0)){
        error("Listening");
    }

    //Accept connection
    client_addr_len = sizeof(client_addr);
    socket_connection_fd = accept(socket_listener_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if(socket_connection_fd < 0){
        error("Accepting");
    }

    //Read received msg
    n = read(socket_connection_fd, msg, sizeof(msg));
    if(n < 0){
        error("Reading");
    }
    printf("A new connection has been established");
    write(1, msg, n);

    write(socket_connection_fd, "I got your message\n", 19);

    close(socket_connection_fd);
    close(socket_listener_fd);
    return 0;
}

void error(char *msg) { 
  perror(msg); 
  exit(0); 
} 