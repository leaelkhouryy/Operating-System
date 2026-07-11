// Write a C program where:
// 1. The parent creates a child.
// 2. The child waits using pause().
// 3. The parent sends:
//     . SIGUSR1 → child prints "Starting work" and sleeps for 3 seconds.
//     . SIGUSR2 → child prints "Stopping work" and terminates.
// 4. The parent waits for the child to finish.

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>

void handler(int sig){
    if (sig == SIGUSR1){
        printf("Starting work...\n");
        sleep(3);
    } else if (sig == SIGUSR2){
        printf("Stopping work...\n");
        exit(0);
    }
}

int main(){
    pid_t pid;

    if((pid = fork()) < 0){
        perror("Fork error");
        exit(1);
    }

    if(pid == 0){
        signal(SIGUSR1, handler);
        signal(SIGUSR2, handler);
        while(1){ pause();}
    } else {
        sleep(1);
        kill(pid, SIGUSR1);
        sleep(4);
        kill(pid, SIGUSR2);
        wait(NULL);
    }

    return 0;
}