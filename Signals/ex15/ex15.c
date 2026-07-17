// Write a C program where:
// 1. Parent creates a child.
// 2. Parent sends SIGUSR1 to the child 5 times.
// 3. The child handler counts how many signals were received.
// 4. After receiving 5 signals:
//     . Child prints "Received 5 signals".
//     . Sends SIGUSR2 to parent.
// 5. Parent terminates after receiving SIGUSR2.

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

int count = 0;

void parent_handler(int sig){
    if(sig == SIGUSR2){
        printf("Terminating\n");
        exit(0);
    }
}

void child_handler(int sig){
    if(sig == SIGUSR1){
        count++;
        if (count == 5){
            printf("Received 5 signals\n");
            kill(getppid(), SIGUSR2);
            exit(0);
        }
    }
}

int main(){
    pid_t pid;

    if ((pid = fork()) < 0){
        perror("Fork error\n");
        exit(1);
    }

    if (pid == 0){
        signal(SIGUSR1, child_handler);
        while(1){ pause(); }
    } else {
        signal(SIGUSR2, parent_handler);
        sleep(1);
        for (int i = 0; i < 5; i++){
            kill(pid, SIGUSR1);
            sleep(1);
        }
        pause();
        wait(NULL);
    }
    return 0;
}