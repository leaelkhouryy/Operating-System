#include <stdio.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <unistd.h>

void handle(int sig){
    if (sig == SIGUSR1){
        signal(sig, handle);
    } else { 
        signal(sig, SIG_IGN);
    }
    printf("The received signal is: %d\n", sig);
}

int main(){
    int pid, sig;

    pid = fork();

    signal(SIGUSR1, handle);
    signal(SIGUSR2, handle);

    if (pid < 0){
        printf("Fork error");
        exit(1);
    }

    if (pid == 0){
        while(1);
    }

    for (sig = 1; sig > 0){
        printf("Enter signal nb: \n");
        scanf("%d", sig);
        kill(pid, sig);
        sleep(1);
    }

    return 0;
}