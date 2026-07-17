// Write a program that maintains a global variable:
// int score = 20;
// Requirements:
//     . Install handlers using sigaction().
//     . SIGINT subtracts 2 from score.
//     . SIGUSR1 adds 5 to score.
//     . Before entering the main loop, block SIGINT using sigprocmask().
//     . The program sleeps for 8 seconds.
//     . During these 8 seconds, the user may press CTRL+C several times.
//     . After 8 seconds, unblock SIGINT.
//     . Continue running until score <= 0.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int score = 20; 

void handler(int s){
    if(s == SIGINT){
        score -= 2;
    } else if (s == SIGUSR1){
        score +=5;
    }
    printf("Score = %d\n", score);
}

int main(){
    struct sigaction action;
    sigset_t set;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGINT, &action, NULL);
    sigaction(SIGUSR1, &action, NULL);

    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    sigprocmask(SIG_BLOCK, &set, NULL);

    sleep(8);

    sigprocmask(SIG_UNBLOCK, &set, NULL);
    
    while(score > 0){ pause();}

    printf("Finished!");
    
    return 0;
}