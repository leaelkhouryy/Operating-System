// Write a C program where:
// 1. Parent creates a child.
// 2. Parent sends:
//     . SIGUSR1 → increase a shared counter by 1.
//     . SIGUSR2 → decrease the counter by 1.
// 3. Child handles signals and prints the counter value.
// 4. Program ends when counter reaches 10.

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

int count = 0;

void handler(int sig){
    if(sig == SIGUSR1){
        count ++;
    } else if (sig == SIGUSR2){
        count --;
    }
    printf("Current count: %d\n", count);
    if (count == 10){
        printf("Bye bye..\n");
        exit(0);
    }
}

int main(){
    pid_t pid;

    if ((pid = fork()) < 0){
        perror("Fork error");
        exit(1);
    }

    if (pid == 0){
        signal(SIGUSR1, handler);
        signal(SIGUSR2, handler);
        while(1){ pause();}
    } else {
        sleep(1);
        kill(pid, SIGUSR2);  
        sleep(1);
        for (int i = 0; i < 10; i++){
            kill(pid, SIGUSR1);
            sleep(1);
        }
        wait(NULL);
    }
    return 0;
}