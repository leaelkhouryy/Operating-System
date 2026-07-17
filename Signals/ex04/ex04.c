// Write a program that executes a command with its arguments after protecting
// the process of SIGHUP signal (sent by the kernel when the terminal to which the process
// is attached, disconnects).

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char**argv){
    signal(SIGHUP, SIG_IGN);
    printf("Sending SIGHUP signal...");
    kill(getpid(), SIGHUP);
    execvp(argv[1], &argv[1]);
    return 0;
}