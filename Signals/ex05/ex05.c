// Write a program that creates 2 processes using the fork system call.
// The "parent process" will display even integers between 1 and 100.
// The "child process" will display odd integers in the same interval.
// Synchronize the two processes using signals to display: 1 2 3 ... 100.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig){
    fprintf(stderr, "Handler %d\n", getpid());
}

int main(){
    int pid, i;

    if (signal(SIGUSR1, handler) == SIG_ERR){ 
        perror("signal"); exit(1);
    }

    if ((pid = fork()) < 0){
        perror("Fork error..");
        exit(1);
    }
    
    if (pid == 0){
        pid = getppid();
        printf("%d child: \n", getpid());
        for (i = 1; i <= 100; i+=2){
            printf("%d\n", i);
            kill(pid, SIGUSR1);
            pause();
        }
    } else {
        for (i = 2; i <= 100; i +=2){
            pause();
            printf("%d\n
                ", i);
            kill(pid, SIGUSR1);
        }
    }

    return 0;
}