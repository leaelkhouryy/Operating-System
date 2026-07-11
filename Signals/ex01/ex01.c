#include <stdio.h>
#include <signal.h>

int main(){
    int sig;
    sigset_t set1, set2;

    sigemtyset(&set1);
    sigaddset(&set1, SIGINT);
    sigaddset(&set1, SIGQUIT);
    sigaddset(&set1, SIGUSR1);

    siprocmask(SIG_SETMASK, &set1, NULL);
    
    sleep(15);

    sigpending(&set2);

    printf("The remaining signals are: \n");
    for(sig = 0; sig < NSIG; sig++){
        if(sigismember(&set2, sig)){
            printf("%d", sig);
        }
    }

    sigprocmask(SIG_UNBLOCK, &set1, NULL);
    sigemptyset(&set1);

    printf("End of process");

    return 0;
}