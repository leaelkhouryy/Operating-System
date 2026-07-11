// Write a C program where:
// 1. A parent creates a child.
// 2. Parent sends SIGUSR1 to child.
// 3. Child:
//     . Prints "Child received request".
//     . Performs a calculation (example: factorial of 5).
//     . Sends SIGUSR2 back.
// 4. Parent:
//     . Waits for SIGUSR2.
//     . Prints "Child completed calculation".

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

int factorial(int n){
    if (n <= 1){ return 1;}
    return n * factorial(n - 1); 
}

void parent_handler(int sig){
    if(sig == SIGUSR2){
        printf("Child completed calculation\n");
    }
}

void child_handler(int sig){
    if (sig == SIGUSR1){
        printf("Child received request\n");
        printf("Factorial = %d\n", factorial(5));
        kill(getppid(), SIGUSR2);
        exit(0);
    }
}

int main(){
    pid_t pid;

    if((pid = fork()) < 0){
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0){
        signal(SIGUSR1, child_handler);
        while(1){ pause();}
    } else {
        signal(SIGUSR2, parent_handler);
        sleep(1);
        kill(pid, SIGUSR1);
        pause();
        wait(NULL);
    }
    return 0;
}