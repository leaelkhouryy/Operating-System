// Write a program that:
// 1. Installs a handler for SIGUSR1.
// 2. Blocks SIGUSR1.
// 3. Another process sends the signal five times while it is blocked.
// 4. Unblock the signal after 10 seconds.
// Question: How many times is the handler executed?

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int s){
    if(s == SIGUSR1){
        printf("Received SIGUSR1\n");
    }
}

int main(){
    sigset_t set;

    signal(SIGUSR1, handler);

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    sigprocmask(SIG_BLOCK, &set, NULL);

    printf("PID: %d\n", getpid());

    sleep(10);

    sigprocmask(SIG_UNBLOCK, &set, NULL);

    printf("Bye bye..");

    return 0;
}