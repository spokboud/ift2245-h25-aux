//
// Created by guib on 2021-07-20.
//

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void gere(int sig) {
    for(int i=0; i<5; i++) {
        printf("Itération %d pour la gestion du signal %d\n", i+1, sig);
    }
    exit(1);
}

int main(void) {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = gere;
    sigaction(SIGUSR1, &action, NULL);
    pause();
    return 0 ;
}