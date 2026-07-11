// 1. Install a signal handler for SIGUSR1 using sigaction().
// 2. When SIGUSR1 is received:
//      - Print: "SIGUSR1 received"
//      - Block SIGINT while the SIGUSR1 handler is executing
//      - Simulate some work inside the handler by sleeping for 5 seconds
// 3. The program should terminate gracefully after handling SIGUSR1.
// 4. The program must wait for signals without using busy-waiting.
// 5. Use sigaction() only (do not use signal()).

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

struct sigaction action;

void handler(int sig){
    if(sig == SIGUSR1){
        printf("SIGUSR1 received");
        sleep(5);
    }
}

int main(){
    action.sa_handler = handler;

    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGINT);

    action.sa_flags = 0;

    if (sigaction(SIGUSR1, &action, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("Waiting for SIGUSR1...");

    pause();

    printf("Program terminated gracefully!");

    return 0;
}