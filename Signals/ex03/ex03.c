// Write a program that counts the signals it receives and displays this counter
// and the received signal number.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <unistd.h>

int nsig[NSIG];

void handler(int sig){
    nsig[sig]++;
    printf("Received %d signal %d times..", s, nsig[sig]);
}

inr main(){
    int sig;

    for (sig = 1; sig < NSIG; sig++){
        if (signal(sig, handler) == SIG_ERR){
            printf(stdout, "Cannot catch signals #%d\n", sig);
        }
        nsig[sig] = 0;
    }

    while(1){
        pause();
    }

    return 0;
}