// Write a program in which a process creates a child process and then waits for
// the end of his child. The child process runs a code that loop. After 10 seconds, if the child
// process does not finish, the parent kills his child process.

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

int pid;

void timeout_handler(int sig){
    printf("Timeout! Killing child\n");
    kill(pid, SIGKILL);
    exit(1);
}

int main(){
    if ((pid = fork()) < 0){
        perror("Fork error");
        exit(1);
    }

    if (pid == 0){
        while(1){
            printf("Child running..\n");
            sleep(1);
        }
    } else {
        signal(SIGALRM, timeout_handler);
        alarm(10);
        wait(NULL);
    }

    return 0;
}