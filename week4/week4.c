#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h> 

const int SIZE = 64;

void sigIntHandler (int sigNum, siginfo_t* infoPtr, void* dataPtr) {
    if(infoPtr->si_pid == getppid()) {
        printf("Correct PID");
    } else {
        printf("Incorrect PID");
    }
}



int main() {

    pid_t childId = fork();

    if (childId == 0) {
        struct sigaction sa;
        memset(&sa,'\0',sizeof(struct sigaction));
        sa.sa_sigaction = sigIntHandler;
        sa.sa_flags = SA_SIGINFO;
        sigaction(SIGINT,&sa,NULL);

        while(1) {
            sleep(1);
        }

        exit(EXIT_SUCCESS);

    }

    int i;
    char line[SIZE];

    for(int i = 0; i < 4; i++) {
        printf("Press enter..");

        fgets(line,SIZE,stdin);

        kill(childId, SIGINT);

    }

    kill(childId, SIGTERM);
    wait(NULL);

    return 0;
}


//send kill -2 [pid]
