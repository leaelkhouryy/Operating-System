// Write a program that:
//     . Installs a handler for SIGINT using signal().
//     . Blocks SIGINT using sigprocmask().
//     . Sleeps for 10 seconds.
//     . During those 10 seconds, pressing CTRL+C should have no immediate effect.
//     . After 10 seconds, unblock SIGINT.
//     . The pending SIGINT (if any) should then execute the handler.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig){
    printf("SIGINT received!\n");
}

int main(){
    signal(SIGINT, handler);
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    sigprocmask(SIG_BLOCK, &set, NULL);

    sleep(10);        

    sigprocmask(SIG_UNBLOCK, &set, NULL);

    while (1){
        pause();
    }
}