Write a C program where:
1. A parent process creates a child process
2. The child waits for a signal from the parent (using signal())
3. The parent sends SIGUSR1 to the child
4. Upon receiving SIGUSR1, the child:
    . Prints "Child received SIGUSR1"
    . Sends SIGUSR2 back to the parent
5. The parent waits for SIGUSR2 and prints "Parent received SIGUSR2" before terminating

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

void parent_handler(int sig){
    if (sig == SIGUSR2){
        printf("Parent received SIGUSR2\n");
    }
}

void child_handler(int sig){
    if (sig == SIGUSR1){
        printf("Child received SIGUSR1\n");
        kill(getppid(), SIGUSR2);
    }
}

int main(){
    pid_t pid;

    if((pid = fork()) < 0){
        perror("Forking error");
        exit(1);
    }

    if (pid == 0){
        signal(SIGUSR1, child_handler);
        while(1){ pause(); }
    } else {
        signal(SIGUSR2, parent_handler);
        sleep(1);
        kill(pid, SIGUSR1);
        wait(NULL);
    }
    return 0;
}