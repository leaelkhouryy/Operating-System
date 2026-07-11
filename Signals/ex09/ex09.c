// Write a program that intercepts 2 signals:
// 1- the CTR-C (that tries to stop the program)
// &
// 2- the SIGUSR1 (from another terminal) which prolongs the "life" of the program
// (The default value of the global parameter "life" is 5).
// For each call to CTR-C the counter "life" is decremented.
// For each receiving a SIGUSR1 the counter "life" is added 10.
// The main function loops as the value of the counter "life" is not zero.
// Write this program with sigaction function

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int life = 5;

void handler (int sig){
    printf("Received signal is: %d\n", sig);
    if (sig == SIGINT){
        life --;
        if (life == 0){
            printf("Bye bye...\n");
            exit(1);
        }
    }
    else if (sig == SIGUSR1){
        life += 10;
    }
    printf("Life: %d\n", life);
}

int main(){
    struct sigaction action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGINT, &action, 0);
    sigaction(SIGUSR1, &action, 0);

    while (life > 0){
        sleep(1);
    }

    return 0;
}