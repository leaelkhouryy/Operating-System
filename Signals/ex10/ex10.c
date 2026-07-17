// Write a C program that satisfies the following requirements:
// 1. Install a signal handler for SIGINT using sigaction()
// 2. When SIGINT is received: 
//     . Print the message: SIGINT received
//     . Block SIGTERM while the SIGINT handler is executing
// 3. The program must terminate gracefully after handling SIGINT
// 4. The program must wait for signal without busy-waiting

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

struct sigaction action;

void sigint_handler(int sig){
    if(sig == SIGINT){
        printf("SIGINT received");
        sleep(1);
    }
}

int main(){
    action.sa_handler = sigint_handler;

    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGTERM);

    action.sa_flags = 0;

    sigaction(SIGINT, &action, 0);

    printf("Waiting for SIGINT...");

    pause();

    printf("Program terminated gracefully!");

    return 0;
}