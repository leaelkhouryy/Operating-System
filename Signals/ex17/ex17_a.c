// Write a C program that intercepts two signals:
// 1. SIGINT (CTRL-C): normally used to terminate the program.
// 2. SIGUSR2: sent from another terminal to increase the program's execution time.
// The program has a global integer variable called counter, initialized to 10.
//     . Each time the user presses CTRL-C, the value of counter is decreased by 2.
//     . Each time the program receives SIGUSR2, the value of counter is increased by 5.
//     . The main function contains an infinite loop that continues running as long as counter > 0.
//     . When counter reaches 0, the program prints "Program finished" and exits normally.

// Tasks:
//     a. Implement the program using the signal() function.
//     b. Rewrite the same program using sigaction().

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/signal.h>
#include <stdio.h>

int counter = 10;

void handler(int s){
    printf("The counter is currently at: %d\n", counter);
    if(s == SIGINT){
        counter -= 2;
    } else if (s == SIGUSR2){
        counter += 5;
    }
    printf("The counter is currently at: %d\n", counter);
}

int main(){
    
    signal(SIGINT, handler);
    signal(SIGUSR2, handler);

    while(counter > 0){
        pause();
    }

    printf("Program finished!");

    return 0;
}