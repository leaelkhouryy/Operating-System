// Write two programs ‐ a client and a server ‐ that communicate using a socket. The client 
// sends a character string entered from the keyboard to the server via the socket created. The 
// server reads the message received from the client and displays it on the screen. The server 
// then sends back to the client the message "I got your message", which will be displayed by 
// the client. You are asked to write this prgram using a local socket in connected mode with up to 5 simultaneous connections. The 
// socket name is taken as a parameter when the programs are run. 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>

void error(char *); 

int main(int argc, char** argv){
    struct sockaddr_un server_addr;
    int sockfd, server_addr_len, n;
    char msg[80];

    //Check if the user enters args
    if(argc != 2){
    printf("Usage: %s socket_name\n", argv[0]);
    exit(1);
    }

    bzero(&server_addr, sizeof(server_addr)); //bzero() erases the data in the sizeof(server_addr) bytes of the memory starting at the location pointed to by server_addr, by writing zeros (bytes containing '\0') to that area
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, argv[1]);
    server_addr_len = sizeof(server_addr.sun_family) + strlen(server_addr.sun_path); //server_addr_len = sizeof(server_addr);
    
    //Create socket
    if((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0){
        error("Creating Socket");
    }
    
    //Connect to socket
    if(connect(sockfd, (struct sockaddr *)&server_addr, server_addr_len) < 0){
        error("Connecting");
    }
    
    //Get user input and write it to socket
    printf("Please enter your message: ");
    bzero(msg, 80);
    fgets(msg, sizeof(msg), stdin);
    write(sockfd, msg, strlen(msg));
    
    //Read server's msg
    n = read(sockfd, msg, 78);
    printf("The returned msg is: ");
    write(1, msg, n);

    close(sockfd);
    return 0;
}

void error(char *msg){ 
  perror(msg); 
  exit(1); 
} 