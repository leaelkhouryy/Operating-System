// Write a program that:
// 1. Installs handlers for SIGINT and SIGUSR1 using sigaction().
// 2. Initially blocks SIGUSR1 using sigprocmask().
// 3. The program prints "Working..." every second for 15 seconds.
// 4. If another terminal sends SIGUSR1 while it is blocked, the signal should not be handled immediately.
// 5. After 15 seconds, unblock SIGUSR1 so the pending signal is delivered.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int s){
    printf("Received signal is: %d\n", s);
}

int main(){
    struct sigaction action;
    sigset_ set;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGUSR1, &action, NULL);

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    
    sigprocmask(SIG_BLOCK, &set, NULL);
    
    for(int i = 0; i < 15; i++){
        printf("Working...\n");
        sleep(1);
    }
    
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    return 0;
}