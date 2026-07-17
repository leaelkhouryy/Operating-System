#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/signal.h>
#include <stdio.h>

int counter = 10;

void handler(int s){
    printf("The counter is currently at: %d\n", counter);
    if(s == SIGINT){
        counter -= 2;
    } else if (s == SIGUSR2){
        counter += 5;
    }
    printf("The counter is currently at: %d\n", counter);
}

int main(){
    struct sigaction action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGINT, &action, NULL);

    sigaction(SIGUSR2, &action, NULL);

    while(counter > 0){ pause();}

    printf("Program finished!");

    return 0;
}